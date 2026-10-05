#include "testwordchecker.h"

#include "../src/core/wordchecker.h"

#include <QDir>
#include <QFile>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>

namespace {
// Write a small dictionary in the same format as the real one:
// fixed 30-byte null-padded records.
void writeDictionary(const QString &path, const QStringList &words)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    for (const QString &word : words) {
        QByteArray record(30, '\0');
        const QByteArray raw = word.toLatin1();
        record.replace(0, qMin<int>(raw.size(), 30), raw);
        file.write(record);
    }
}
} // namespace

void TestWordChecker::testPointsFor()
{
    // A=1 B=3 C=3 -> "ABC" = 7
    QCOMPARE(WordChecker::pointsFor(QStringLiteral("ABC")), 7u);
    QCOMPARE(WordChecker::pointsFor(QStringLiteral("AB")), 4u);
    QCOMPARE(WordChecker::pointsFor(QString()), 0u);
    // Q=8 -> "QQ" = 16 (same table as the original code)
    QCOMPARE(WordChecker::pointsFor(QStringLiteral("QQ")), 16u);
}

void TestWordChecker::testIsWordInDictionaryUsesWorkingDirectory()
{
    const QString original = QDir::currentPath();

    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString dictPath = dir.path() + QStringLiteral("/dictionary.txt");
    writeDictionary(dictPath, {QStringLiteral("AB"), QStringLiteral("XYZ")});

    const bool ok = QDir::setCurrent(dir.path());
    QVERIFY2(ok, qPrintable(QStringLiteral("could not chdir to %1").arg(dir.path())));

    QVERIFY(WordChecker::isWordInDictionary(QStringLiteral("AB")));
    QVERIFY(WordChecker::isWordInDictionary(QStringLiteral("XYZ")));
    QVERIFY(!WordChecker::isWordInDictionary(QStringLiteral("QQ")));
    QVERIFY(!WordChecker::isWordInDictionary(QString()));

    QDir::setCurrent(original);
}

void TestWordChecker::testWordCheckerSignals()
{
    const QString original = QDir::currentPath();
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString dictPath = dir.path() + QStringLiteral("/dictionary.txt");
    writeDictionary(dictPath, {QStringLiteral("CAT")});
    QVERIFY(QDir::setCurrent(dir.path()));

    WordChecker checker(QStringLiteral("CAT"));
    QSignalSpy spy(&checker, &WordChecker::finished);
    QVERIFY(spy.isValid());

    checker.run();

    QCOMPARE(spy.count(), 1);
    const QList<QVariant> args = spy.at(0);
    QCOMPARE(args.at(0).toString(), QString(QStringLiteral("CAT")));
    QCOMPARE(args.at(1).toUInt(), 5u); // C=3 A=1 T=1

    QDir::setCurrent(original);
}
