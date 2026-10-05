#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QListWidget>
#include <QTreeWidget>

#include "../core/scorestore.h"
#include "logindialog.h"
#include "session.h"
#include "signupdialog.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu() override;

public slots:
    void logInWithName(const QString &name);
    void onUserLoggedIn(const QString &name);
    void onSessionClosed(const Player &player);

private slots:
    void on_logInButton_clicked();
    void on_signUpButton_clicked();
    void on_guestButton_clicked();
    void on_actionPlay_as_Guest_triggered();
    void on_actionSign_Up_triggered();
    void on_actionLog_In_triggered();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_actionExit_triggered();
    void on_actionRules_of_the_Game_triggered();
    void on_actionControls_triggered();
    void on_actionAuthor_triggered();

private:
    void setUpTabs();

    Ui::MainMenu *ui;
    ScoreStore m_store;
    Session *m_session = nullptr;
};

#endif // MAINMENU_H
