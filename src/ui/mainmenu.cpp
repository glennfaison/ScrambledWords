#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    setUpTabs();
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_logInButton_clicked()
{
    auto *dialog = new LogInDialog(this);
    connect(dialog, &LogInDialog::userLoggedIn,
            this, &MainMenu::onUserLoggedIn);
    dialog->show();
}

void MainMenu::on_signUpButton_clicked()
{
    auto *dialog = new SignUpDialog(this);
    connect(dialog, &SignUpDialog::successfulSignUp,
            this, [this]() { setUpTabs(); });
    dialog->show();
}

void MainMenu::on_guestButton_clicked()
{
    logInWithName(QStringLiteral("GUEST"));
}

void MainMenu::onSessionClosed(const Player &player)
{
    player.save(&m_store);
    setUpTabs();
}

void MainMenu::onUserLoggedIn(const QString &name)
{
    logInWithName(name);
}

void MainMenu::logInWithName(const QString &name)
{
    m_session = new Session(name, this);
    connect(m_session, &Session::sessionClosed,
            this, &MainMenu::onSessionClosed);
    m_session->setGeometry(geometry());
    m_session->show();
    setVisible(false);
}

void MainMenu::setUpTabs()
{
    ui->tabWidget->clear();

    ui->listWidget->clear();
    ui->listWidget->setSortingEnabled(true);
    const QStringList names = m_store.playerNames();
    for (const QString &name : names)
        ui->listWidget->addItem(new QListWidgetItem(name));
    ui->tabWidget->addTab(ui->listWidget, tr("Current Accounts"));

    auto *treeWidget = new QTreeWidget;
    const QMap<QString, QList<int>> byName = m_store.byName();
    treeWidget->setHeaderLabels(QStringList() << tr("NAME") << tr("HI-SCORE"));
    for (auto it = byName.constBegin(); it != byName.constEnd(); ++it) {
        if (it.key() == QStringLiteral("GUEST"))
            continue;
        for (int score : it.value())
            treeWidget->addTopLevelItem(
                new QTreeWidgetItem(QStringList() << it.key() << QString::number(score)));
    }
    treeWidget->sortItems(1, Qt::DescendingOrder);
    ui->tabWidget->addTab(treeWidget, tr("High Scores"));
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
    const int ans = QMessageBox::question(
        this, tr("Quit Game?!"),
        tr("Why would anybody want to do that?!\n\nAre you sure?"));
    if (ans == QMessageBox::Yes)
        close();
}

void MainMenu::on_actionRules_of_the_Game_triggered()
{
    QMessageBox::information(this, tr("Rules of the Game!"),
                             tr("The objective of this game is for the player to\n"
                                "get as many points as possible in the allocated time.\n\n"
                                "Points are obtained by entering words which are valid\n"
                                "according to the SCRABBLE dictionary."));
}

void MainMenu::on_actionControls_triggered()
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

void MainMenu::on_actionAuthor_triggered()
{
    QMessageBox::information(this, tr("Author"),
                             tr("\u00A9Faison\nglennfaison@gmail.com"));
}
