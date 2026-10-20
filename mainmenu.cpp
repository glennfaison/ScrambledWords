#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    setUpTabWidget();
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_logInButton_clicked()
{
    logIn = new LogInDialog(this);
    logIn->show();
}

void MainMenu::on_signUpButton_clicked()
{
    signUp = new SignUpDialog(this);
    connect(signUp, SIGNAL(rejected()), this, SLOT(setUpTabWidget()));
    signUp->show();
}

void MainMenu::on_guestButton_clicked()
{
    logInWithName("GUEST");
}

void MainMenu::onSessionClosed(Player player)
{
    player.saveToFile();
    setUpTabWidget();
}

void MainMenu::onUserLoggedIn(QString name)
{
    Player p(name);
    if(!Player::exists(name))
    {
        // create the account if it doesn't already exist or is faulty
        p.saveToFile();
    }
    logIn->setVisible(false);
    logInWithName(name);
}

void MainMenu::logInWithName(QString name)
{
    QString s = name;
    session = new Session(s, this);
    connect(session, SIGNAL(sessionClosed(Player)), this, SLOT(onSessionClosed(Player)));
    session->setGeometry(this->geometry());
    session->show();
    this->setVisible(false);
}

void MainMenu::setUpTabWidget()
{
    // clear tab
    ui->tabWidget->clear();
    // set up the first tab
    QString icon(":/icons/notes.png");
    ui->tabWidget->insertTab(0, ui->listWidget,
                             QIcon(icon),
                             "Current Accounts");
    // clear listWidget
    ui->listWidget->clear();
    ui->listWidget->setSortingEnabled(true);
    // load game players
    QStringList playerList = Player::loadAllPlayerNames();
    playerList.sort();  // sort the list
    // add each player to the listWidget, making sure there are no repetitions
    QString check;
    foreach(QString i, playerList)
    {
        if(i != check)
        {
            ui->listWidget->addItem(i);
        }
        check = i;
    }

    // set up the second tab
    treeWidget = new QTreeWidget(this);
    QString icon2(":/icons/notes.png");
    ui->tabWidget->insertTab(1, treeWidget, QIcon(icon2), "High Scores");
    QList<Player::HiScoreEntry> hs = Player::loadGeneralHiScores();
    treeWidget->clear();
    treeWidget->setHeaderLabels(QStringList() << "NAME" << "HI-SCORE");
    foreach(Player::HiScoreEntry i, hs)
    {
        QTreeWidgetItem *twItm = new QTreeWidgetItem(QStringList() << i.first << QString::number(i.second));
        treeWidget->addTopLevelItem(twItm);
    }
    treeWidget->sortItems(1, Qt::DescendingOrder);
}

void MainMenu::on_actionPlay_as_Guest_triggered()
{
    on_guestButton_clicked();
}

void MainMenu::on_actionSign_Up_triggered()
{
    on_signUpButton_clicked();
}

void MainMenu::on_actionLog_In_triggered()
{
    on_logInButton_clicked();
}

void MainMenu::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    logInWithName(item->text());
}

void MainMenu::on_actionExit_triggered()
{
    QString title("Quit Game?!");
    QString text("Why would anybody want to do that?!\n\n"
                 "Are you sure?");
    int ans = QMessageBox::question(this, title, text);
    std::cout << ans << std::endl;
    if(ans == QMessageBox::Yes)
    {
        this->close();
    }
}

void MainMenu::on_actionRules_of_the_Game_triggered()
{
    QString title("Rules of the Game!");
    QString text("\tThe objective of this game is for the player to\n"
            "get as many points as possible in the allocated time.\n\n"
            "\tPoints are obtained by entering words which are valid\n"
            "according to the SCRABBLE dictionary.");

    QMessageBox::information(this, title, text);
}

void MainMenu::on_actionControls_triggered()
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

void MainMenu::on_actionAuthor_triggered()
{
    QString title("Author");
    QString text("\u00A9Faison\n"
                 "glennfaison@gmail.com");
    QMessageBox::information(this, title, text);
}
