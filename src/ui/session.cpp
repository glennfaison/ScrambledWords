#include "session.h"
#include "ui_session.h"

#include <QDateTime>
#include <QMessageBox>

namespace {
constexpr unsigned kAllocatedTime = 100; // seconds
constexpr int kNumberOfButtons = 10;
}

Session::Session(const QString &user, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Session)
    , m_currentUser(user)
    , m_sessionScore(0)
{
    m_currentLetters.reserve(kNumberOfButtons);
    for (int i = 0; i < kNumberOfButtons; ++i)
        m_currentLetters.append(QChar());

    ui->setupUi(this);

    m_timer = new Timer(this, kAllocatedTime);
    connect(m_timer, &Timer::countDown, this, &Session::onCountDown);

    linkLetterButtonsAndValues();
    setGameActive(false);
    getNewLetterButtons();

    ui->sessionLabel->setText(QStringLiteral("Jumble <br/><b style='color:blue;'>%1</b>")
                                   .arg(user));
    ui->highScoreLabel->setText(highScoreHtml());
}

Session::~Session()
{
    m_timer->wait();
    delete ui;
}

void Session::setGameActive(bool active)
{
    for (QPushButton *button : m_letterButtons)
        button->setEnabled(active);
    ui->submitButton->setEnabled(active);
    ui->clearAllButton->setEnabled(active);
    ui->clearOneButton->setEnabled(active);
    ui->lineEdit->setEnabled(active);
    ui->treeWidget->setEnabled(active);
}

void Session::getNewLetterButtons()
{
    unsigned seed = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    for (int i = 0; i < kNumberOfButtons; ++i) {
        seed = seed * 13 + 7;
        Letter letter(seed);
        m_currentLetters[i] = letter.character();
        m_letterButtons[i]->setText(QString(letter.character()));
        m_letterScores[i]->setText(QString::number(letter.value()));
    }
}

void Session::linkLetterButtonsAndValues()
{
    m_letterButtons = {
        ui->letterButton_1,  ui->letterButton_2,  ui->letterButton_3,
        ui->letterButton_4,  ui->letterButton_5,  ui->letterButton_6,
        ui->letterButton_7,  ui->letterButton_8,  ui->letterButton_9,
        ui->letterButton_10
    };
    m_letterScores = {
        ui->letterScore_1,  ui->letterScore_2,  ui->letterScore_3,
        ui->letterScore_4,  ui->letterScore_5,  ui->letterScore_6,
        ui->letterScore_7,  ui->letterScore_8,  ui->letterScore_9,
        ui->letterScore_10
    };
    for (int i = 0; i < kNumberOfButtons; ++i)
        connect(m_letterButtons[i], &QPushButton::clicked,
                this, [this, i]() { appendLetter(m_letterButtons[i]->text()); });
}

void Session::appendLetter(const QString &letter)
{
    ui->lineEdit->setText(ui->lineEdit->text() + letter);
    ui->lineEdit->setFocus();
}

bool Session::wordHasBeenEntered(const QString &entry)
{
    if (m_previouslyEnteredWords.contains(entry)) {
        m_rejectedEntries.append(entry);
        return true;
    }
    m_previouslyEnteredWords.insert(entry);
    return false;
}

bool Session::wordIsValid(const QString &entry) const
{
    QChar available[kNumberOfButtons];
    for (int i = 0; i < kNumberOfButtons; ++i)
        available[i] = m_currentLetters[i];

    for (const QChar &ch : entry.toUpper()) {
        int index = -1;
        for (int i = 0; i < kNumberOfButtons; ++i) {
            if (available[i] == ch) {
                index = i;
                break;
            }
        }
        if (index == -1)
            return false;
        available[index] = QChar();
    }
    return true;
}

void Session::on_newGameButton_clicked()
{
    setGameActive(true);
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabels(QStringList() << tr("WORDS") << tr("POINTS"));
    ui->correctnessLabel->clear();
    ui->sessionScoreLabel->setText(QStringLiteral("0"));

    m_sessionScore = 0;
    m_previouslyEnteredWords.clear();
    m_rejectedEntries.clear();

    getNewLetterButtons();

    ui->lineEdit->clear();
    ui->lineEdit->setFocus();

    m_timer->start();
}

void Session::onCountDown(unsigned secondsLeft)
{
    ui->timeLabel->setText(QString::number(secondsLeft));
    if (secondsLeft == 0) {
        setGameActive(false);
        endSession();
    }
}

void Session::endSession()
{
    m_currentUser.addScore(m_sessionScore);
    ScoreStore store;
    store.saveScores(m_currentUser.name(), m_currentUser.scores());
}

void Session::onPointsReceived(const QString &word, unsigned points)
{
    if (points == 0) {
        ui->correctnessLabel->setText(QStringLiteral("<b style='color:red;'>%1</b>")
                                          .arg(word));
        return;
    }

    ui->correctnessLabel->setText(QStringLiteral("<b style='color:green;'>%1</b>")
                                      .arg(word));

    auto *item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, word);
    item->setText(1, QString::number(points));
    item->setTextAlignment(0, Qt::AlignRight);
    item->setTextAlignment(1, Qt::AlignLeft);
    ui->treeWidget->addTopLevelItem(item);

    m_sessionScore += points;
    ui->sessionScoreLabel->setText(QString::number(m_sessionScore));
}

void Session::on_submitButton_clicked()
{
    const QString text = ui->lineEdit->text();
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();

    QString display = text;
    if (wordHasBeenEntered(text)) {
        display = tr("PREVIOUSLY ENTERED");
        onPointsReceived(display, 0);
        return;
    }
    if (!wordIsValid(text)) {
        display = tr("INVALID WORD!");
        onPointsReceived(display, 0);
        return;
    }

    auto *checker = new WordChecker(display, this);
    m_checkers.append(checker);
    connect(checker, &WordChecker::finished,
            this, &Session::onPointsReceived, Qt::UniqueConnection);
    connect(checker, &QObject::destroyed,
            [this, checker]() { m_checkers.removeAll(checker); });
    QMetaObject::invokeMethod(checker, &WordChecker::run, Qt::QueuedConnection);
}

void Session::on_clearOneButton_clicked()
{
    QString text = ui->lineEdit->text();
    if (!text.isEmpty())
        text.remove(text.length() - 1, 1);
    ui->lineEdit->setText(text);
    ui->lineEdit->setFocus();
}

void Session::on_clearAllButton_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
}

void Session::on_lineEdit_returnPressed()
{
    emit ui->submitButton->clicked();
}

void Session::on_lineEdit_textChanged(const QString &text)
{
    ui->lineEdit->setText(text.toUpper());
}

void Session::on_actionNew_Game_triggered()
{
    emit ui->newGameButton->clicked();
}

void Session::on_actionRules_of_the_Game_triggered()
{
    QMessageBox::information(this, tr("Rules of the Game!"),
                             tr("The objective of this game is for the player to\n"
                                "get as many points as possible in the allocated time.\n\n"
                                "Points are obtained by entering words which are valid\n"
                                "according to the SCRABBLE dictionary."));
}

void Session::on_actionControls_triggered()
{
    QMessageBox::information(this, tr("Controls"),
                             tr("Enter a letter by either:\n"
                                "\ttyping the letter from the keyboard or\n"
                                "\tclicking the buttons which correspond to the desired letter\n\n"
                                "The button which says 'Clear All' empties the input.\n\n"
                                "The button which says 'Clear One' deletes the last letter in the input.\n\n"
                                "You can submit a word by doing one of the following:\n"
                                "\tclicking the button which reads 'Submit'\n"
                                "\tpressing the <ENTER> button on your keyboard\n\n"
                                "Enjoy!"));
}

void Session::on_backLink_clicked()
{
    endSession();
    if (qobject_cast<QWidget *>(parent()))
        parentWidget()->setGeometry(geometry());
    parentWidget()->setVisible(true);
    setVisible(false);
    emit sessionClosed(m_currentUser);
}

void Session::on_actionBack_triggered()
{
    on_backLink_clicked();
}

void Session::on_actionAuthor_triggered()
{
    QMessageBox::information(this, tr("Author"),
                             tr("\u00A9Faison\nglennfaison@gmail.com"));
}

QString Session::highScoreHtml() const
{
    return QStringLiteral("<i style='color:blue;'>Your High Score - </i>"
                          "<b style='font-weight:bolder;'>%1</b>")
        .arg(m_currentUser.bestScore());
}
