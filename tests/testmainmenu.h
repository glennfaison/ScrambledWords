#ifndef TESTMAINMENU_H
#define TESTMAINMENU_H

#include <QObject>
#include <QSignalSpy>

class TestMainMenu : public QObject
{
    Q_OBJECT
private slots:
    void testPlayAsGuestActionOpensSession();
    void testLogInButtonOpensDialog();
    void testSignUpButtonOpensDialog();
    void testMenuActionsTriggerSlots();
};

#endif // TESTMAINMENU_H
