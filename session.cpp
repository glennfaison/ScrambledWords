#include "session.h"
#include "ui_session.h"


Session::Session(QString user, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Session)
{
    // first load user data
    currentUser = new Player(user);
    ui->setupUi(this);
    this->sessionScore = 0;
    timer = new Timer();
    checker = new WordChecker();

    linkLetterButtonsAndValues();

    // start all buttons and the lineEdit inactive
    setGameActive(false);
    // set the sessionLabel
    ui->sessionLabel->setText("Jumble <br/>" + QString("<b style='color:blue;'>")
                              + user + QString("</b>"));

    ui->highScoreLabel->setText("<i style='color:blue;'>Your High Score - </i><b style='font-weight:bolder;'>"
                                + QString::number(currentUser->high_score[0]) + "</b>");
}

Session::~Session()
{
    delete ui;
}

void Session::setGameActive(bool x)
{
    // deactivate all buttons
    for(int i = 0; i < 10; i++)
    {
        letterButton[i]->setEnabled(x);
    }
    ui->submitButton->setEnabled(x);
    ui->clearAllButton->setEnabled(x);
    ui->clearOneButton->setEnabled(x);
    ui->lineEdit->setEnabled(x);
    ui->treeWidget->setEnabled(x);
}

void Session::getNewLetterButtons()
{
    // select letters for each button
    int j = QTime::currentTime().msec();  // get the current time in seconds
    for(uint i = 0; i < 10; i++)
    {
        uint k = (1+j)*(3+j)*(i+(13+j));
        j += 6;
        letter = new Letter(k);
        currentLetters[i] = letter->character();
        letterButton[i]->setText(QString("") + letter->character());
        letterScore[i]->setText(QString("") + QString::number(letter->value()));
        delete letter;
    }
}

void Session::linkLetterButtonsAndValues()
{
    letterButton[0] = ui->letterButton_1;
    letterButton[1] = ui->letterButton_2;
    letterButton[2] = ui->letterButton_3;
    letterButton[3] = ui->letterButton_4;
    letterButton[4] = ui->letterButton_5;
    letterButton[5] = ui->letterButton_6;
    letterButton[6] = ui->letterButton_7;
    letterButton[7] = ui->letterButton_8;
    letterButton[8] = ui->letterButton_9;
    letterButton[9] = ui->letterButton_10;
    letterScore[0] = ui->letterScore_1;
    letterScore[1] = ui->letterScore_2;
    letterScore[2] = ui->letterScore_3;
    letterScore[3] = ui->letterScore_4;
    letterScore[4] = ui->letterScore_5;
    letterScore[5] = ui->letterScore_6;
    letterScore[6] = ui->letterScore_7;
    letterScore[7] = ui->letterScore_8;
    letterScore[8] = ui->letterScore_9;
    letterScore[9] = ui->letterScore_10;
}

bool Session::wordHasBeenEntered(const QString &entry)
{
    if(previouslyEnteredWords.empty())
    {
        // do nothing
    }
    else
    {
        foreach(QString i, previouslyEnteredWords)
        {
            if(i == entry)
            {
                return true;
            }
        }
    }
    previouslyEnteredWords << entry;
    return false;
}

bool Session::wordIsValid(const QString &entry)
{
    /* copy the ltrs array into a temporary one
     * This is to make sure no letter is used more than once */
    const unsigned numberOfButtons = 10;
    char temp[numberOfButtons];
    for(unsigned i = 0; i < numberOfButtons; i++)
    {
        temp[i] = currentLetters[i];
    }
    unsigned n = entry.size();
    for(unsigned j = 0; j < n; j++)
    {
        bool found = false;
        for(unsigned i = 0; i < numberOfButtons; i++)
        {
            if(entry.toStdString().at(j) == temp[i])
            {
                found = true;
                temp[i] = '\0';
                break;
            }
        }
        if(!found) return false;
    }
    return true;
}

void Session::on_newGameButton_clicked()
{
    // activate the game
    setGameActive(true);
    // clear treeWidget
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabels(QStringList() << "WORDS" << "POINTS");
    // reset labels
    ui->correctnessLabel->clear();
    ui->sessionScoreLabel->setText(QString::number(0));
    // update high scores
    currentUser->updatePlayerData(this->sessionScore);
    currentUser->saveToFile();
    ui->highScoreLabel->setText("<i style='color:blue;'>Your High Score - </i><b style='font-weight:bolder;'>"
                                + QString::number(currentUser->high_score[0]) + "</b>");
    // reset the session score
    this->sessionScore = 0;
    // clear lineEdit and set focus
    emit ui->clearAllButton->clicked();
    // get new letters
    getNewLetterButtons();
    // delete trees of previously entered words
    previouslyEnteredWords.clear();

    // block signals from previous timers
    timer->blockSignals(true);
    // start a new timer
    timer = new Timer(0, allocatedTime);
    connect(timer, SIGNAL(countDown(uint)), this, SLOT(onCountDown(uint)));
    timer->start();
}

void Session::onCountDown(unsigned time)
{
    // if time is spent, deactivate game
    if(time == 0)
    {
        setGameActive(false);
        // update high scores
        currentUser->updatePlayerData(this->sessionScore);
        currentUser->saveToFile();
        std::cout << sessionScore << "\t" << currentUser->high_score[0] << std::endl;
    }
    ui->timeLabel->setText(QString::number(time));
}

void Session::onPointsReceived(QString word, unsigned pts)
{
    if(pts == 0)
    {
        // write the word on the correctnessLabel
        ui->correctnessLabel->setText(QString("<b style='color:red;'>") +
                                      word + QString("</b>"));
    }
    else
    {
        // write the word on the correctnessLabel
        ui->correctnessLabel->setText(QString("<b style='color:green;'>") +
                                      word + QString("</b>"));

        // add treeWidgetItem to treeWidget
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, word);
        item->setText(1, QString::number(pts));
        item->setTextAlignment(0, Qt::AlignRight);
        item->setTextAlignment(1, Qt::AlignLeft);
        ui->treeWidget->addTopLevelItem(item);
        this->sessionScore += pts;
        ui->sessionScoreLabel->setText(QString::number(sessionScore));
    }
}

void Session::on_submitButton_clicked()
{
    QString text = ui->lineEdit->text();
    // clear lineEdit
    emit ui->clearAllButton->clicked();
    /* check the word against previously entered words.
     * If it's been entered before, indicate, else,
     * add to previously entered words */
    if(wordHasBeenEntered(text))
    {
        // on the premise that all dictinary contents are single words
        text = "PREVIOUSLY ENTERED";
    }
    else if(!wordIsValid(text))
    {
        // on the premise that all dictinary contents are single words
        text = "INVALID WORD!";
    }
    // check the word against the database
    checker = new WordChecker(text);
    connect(checker, SIGNAL(sendPoints(QString,uint)), this, SLOT(onPointsReceived(QString,uint)));
    checker->start();
}

void Session::on_clearOneButton_clicked()
{
    // erase one element of the lineEdit text
    // make the string one character shorter
    QString text = ui->lineEdit->text();
    text.resize(text.size() - 2);
    ui->lineEdit->setText(QString(text));
    // return focus to the lineEdit
    ui->lineEdit->setFocus();
}

void Session::on_clearAllButton_clicked()
{
    // clear lineEdit
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();   // return focus to the lineEdit
}

void Session::on_lineEdit_returnPressed()
{
    emit ui->submitButton->clicked();
}

void Session::on_lineEdit_textChanged(const QString &arg1)
{
    // capitalize
    ui->lineEdit->setText(arg1.toUpper());
}

void Session::on_letterButton_1_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_1->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_2_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_2->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_3_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_3->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_4_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_4->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_5_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_5->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_6_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_6->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_7_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_7->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_8_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_8->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_9_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_9->text());
    ui->lineEdit->setFocus();
}

void Session::on_letterButton_10_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text() + ui->letterButton_10->text());
    ui->lineEdit->setFocus();
}

void Session::on_actionNew_Game_triggered()
{
    emit ui->newGameButton->clicked();
}

void Session::on_actionRules_of_the_Game_triggered()
{
    QString title("Rules of the Game!");
    QString text("\tThe objective of this game is for the player to\n"
            "get as many points as possible in the allocated time.\n\n"
            "\tPoints are obtained by entering words which are valid\n"
            "according to the SCRABBLE dictionary.");

    QMessageBox::information(this, title, text);
}

void Session::on_actionControls_triggered()
{
    QString title("Controls");
    QString text("Enter a letter by either:\n"
            "\ttyping the letter from the keyboard or\n"
            "\tclicking the buttons which correspond to the desired letter\n\n"
            "The button which says 'Clear All' empties the input.\n\n"
            "The button which says 'Clear One' deletes the last letter in the input.\n\n"
            "You can submit a word by doing one of the following:\n"
            "\tclicking the button which reads 'Submit'\n"
            "\tpressing the <ENTER> button on your keyboard\n\n"
            "Enjoy!");

    QMessageBox::information(this, title, text);
}

void Session::on_backLink_clicked()
{
    // update high scores and emit current user's stats
    currentUser->updatePlayerData();
    this->parentWidget()->setGeometry(this->geometry());
    this->parentWidget()->setVisible(true);
    this->setVisible(false);
    emit sessionClosed(*currentUser);
}

void Session::on_actionBack_triggered()// EXIT PROBLEM LIES HERE
{
    std::cout << "back link clicked" << std::endl;
    try
    {
        on_backLink_clicked();
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        on_backLink_clicked();
    }
}

void Session::on_actionAuthor_triggered()
{
    QString title("Author");
    QString text("\u00A9Faison\n"
                 "glennfaison@gmail.com");
    QMessageBox::information(this, title, text);
}
