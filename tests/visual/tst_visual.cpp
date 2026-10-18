// Visual regression tests for the Jumble UI.
//
// Each test renders a window off-screen with a pinned style and bundled fonts,
// then compares the result pixel-by-pixel against a PNG baseline.
//
//   Run:               QT_QPA_PLATFORM=offscreen ./tst_visual
//   Refresh baselines: UPDATE_BASELINES=1 QT_QPA_PLATFORM=offscreen ./tst_visual
//
// On a mismatch the actual and diff images are written to $VISUAL_OUTPUT_DIR
// (default: ./visual-output) so they can be inspected or uploaded by CI.

#include <QtTest>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QStyleFactory>
#include <QTemporaryDir>

#include "mainmenu.h"
#include "logindialog.h"
#include "signupdialog.h"
#include "session.h"

namespace {

const char *kSourceDir = VISUAL_TESTS_SOURCE_DIR;

// A pixel counts as different when any channel differs by more than this.
const int kChannelTolerance = 48;
// The test fails when more than this fraction of pixels differ.
const double kMaxDiffRatio = 0.005;

QString baselineDir() { return QString(kSourceDir) + "/baselines"; }

QString outputDir()
{
    const QString env = qEnvironmentVariable("VISUAL_OUTPUT_DIR");
    return env.isEmpty() ? QDir::current().absoluteFilePath("visual-output") : env;
}

// Point fontconfig at the bundled fonts only, so rendering does not depend on
// whatever fonts the host machine happens to have installed.
void pinFonts(const QString &cacheDir)
{
    const QString conf = cacheDir + "/fonts.conf";
    QFile f(conf);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    const QString xml = QString(
        "<?xml version=\"1.0\"?>\n"
        "<!DOCTYPE fontconfig SYSTEM \"fonts.dtd\">\n"
        "<fontconfig>\n"
        "  <dir>%1/fonts</dir>\n"
        "  <cachedir>%2/fc-cache</cachedir>\n"
        "  <alias><family>Consolas</family><prefer><family>DejaVu Sans Mono</family></prefer></alias>\n"
        "  <alias><family>sans-serif</family><prefer><family>DejaVu Sans</family></prefer></alias>\n"
        "  <alias><family>monospace</family><prefer><family>DejaVu Sans Mono</family></prefer></alias>\n"
        "  <match target=\"font\">\n"
        "    <edit name=\"antialias\" mode=\"assign\"><bool>true</bool></edit>\n"
        "    <edit name=\"hinting\" mode=\"assign\"><bool>false</bool></edit>\n"
        "    <edit name=\"rgba\" mode=\"assign\"><const>none</const></edit>\n"
        "  </match>\n"
        "</fontconfig>\n").arg(kSourceDir, cacheDir);
    f.write(xml.toUtf8());
    f.close();
    qputenv("FONTCONFIG_FILE", conf.toLocal8Bit());
}

} // namespace

class VisualTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void mainMenu();
    void logInDialog();
    void signUpDialog();
    void sessionIdle();

private:
    void compare(QWidget *widget, const QString &name);

    QTemporaryDir m_workDir;
    QString m_originalDir;
};

void VisualTest::initTestCase()
{
    QVERIFY2(QFile::exists(baselineDir()) || qEnvironmentVariableIsSet("UPDATE_BASELINES"),
             "Baseline directory missing; run with UPDATE_BASELINES=1 to create it.");
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication::setPalette(QApplication::style()->standardPalette());
    QApplication::setFont(QFont("DejaVu Sans", 9));
}

void VisualTest::init()
{
    // The app reads/writes highScores.* in the working directory. Run every
    // test in a fresh, empty directory so the windows always start blank.
    m_originalDir = QDir::currentPath();
    QVERIFY(m_workDir.isValid());
    QDir work(m_workDir.path());
    for (const QString &f : work.entryList(QDir::Files))
        work.remove(f);
    QDir::setCurrent(m_workDir.path());
}

void VisualTest::cleanup()
{
    QDir::setCurrent(m_originalDir);
}

void VisualTest::compare(QWidget *widget, const QString &name)
{
    widget->show();
    QVERIFY(QTest::qWaitForWindowExposed(widget));
    QApplication::processEvents();

    const QImage actual = widget->grab().toImage().convertToFormat(QImage::Format_RGB32);
    QVERIFY2(!actual.isNull(), "Failed to render widget");

    const QString baselinePath = baselineDir() + "/" + name + ".png";

    if (qEnvironmentVariableIsSet("UPDATE_BASELINES")) {
        QDir().mkpath(baselineDir());
        QVERIFY(actual.save(baselinePath));
        qInfo("Updated baseline %s", qPrintable(baselinePath));
        return;
    }

    QImage expected(baselinePath);
    QVERIFY2(!expected.isNull(),
             qPrintable("Missing baseline " + baselinePath + " (run with UPDATE_BASELINES=1)"));
    expected = expected.convertToFormat(QImage::Format_RGB32);

    QDir().mkpath(outputDir());
    const QString actualPath = outputDir() + "/" + name + ".actual.png";

    if (expected.size() != actual.size()) {
        actual.save(actualPath);
        QFAIL(qPrintable(QString("%1: size %2x%3 differs from baseline %4x%5")
                         .arg(name)
                         .arg(actual.width()).arg(actual.height())
                         .arg(expected.width()).arg(expected.height())));
    }

    QImage diff(actual.size(), QImage::Format_RGB32);
    diff.fill(Qt::white);
    qint64 differing = 0;
    for (int y = 0; y < actual.height(); ++y) {
        const QRgb *a = reinterpret_cast<const QRgb *>(actual.constScanLine(y));
        const QRgb *e = reinterpret_cast<const QRgb *>(expected.constScanLine(y));
        QRgb *d = reinterpret_cast<QRgb *>(diff.scanLine(y));
        for (int x = 0; x < actual.width(); ++x) {
            const int delta = qMax(qMax(qAbs(qRed(a[x]) - qRed(e[x])),
                                        qAbs(qGreen(a[x]) - qGreen(e[x]))),
                                   qAbs(qBlue(a[x]) - qBlue(e[x])));
            if (delta > kChannelTolerance) {
                ++differing;
                d[x] = qRgb(255, 0, 0);
            } else {
                const int g = qGray(e[x]) / 2 + 128;
                d[x] = qRgb(g, g, g);
            }
        }
    }

    const double ratio = double(differing) / (actual.width() * actual.height());
    if (ratio > kMaxDiffRatio) {
        actual.save(actualPath);
        diff.save(outputDir() + "/" + name + ".diff.png");
        QFAIL(qPrintable(QString("%1: %2% of pixels differ from baseline (limit %3%). "
                                 "See %4")
                         .arg(name)
                         .arg(ratio * 100, 0, 'f', 2)
                         .arg(kMaxDiffRatio * 100, 0, 'f', 2)
                         .arg(outputDir())));
    }
}

void VisualTest::mainMenu()
{
    MainMenu w;
    compare(&w, "main_menu");
}

void VisualTest::logInDialog()
{
    LogInDialog w;
    compare(&w, "login_dialog");
}

void VisualTest::signUpDialog()
{
    SignUpDialog w;
    compare(&w, "signup_dialog");
}

void VisualTest::sessionIdle()
{
    // Before "New Game" is pressed the board is empty and deterministic.
    Session w("GUEST");
    compare(&w, "session_idle");
}

int main(int argc, char **argv)
{
    if (!qEnvironmentVariableIsSet("QT_QPA_PLATFORM"))
        qputenv("QT_QPA_PLATFORM", "offscreen");

    QTemporaryDir fontCache;
    pinFonts(fontCache.path());

    QCoreApplication::setAttribute(Qt::AA_Use96Dpi, true);
    QApplication app(argc, argv);
    VisualTest tc;
    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_visual.moc"
