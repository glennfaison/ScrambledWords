#include "testmainmenu.h"

#include "../src/ui/mainmenu.h"
#include "../src/ui/logindialog.h"
#include "../src/ui/signupdialog.h"
#include "../src/ui/session.h"

#include <QTest>
#include <QPushButton>
#include <QDialog>

void TestMainMenu::testPlayAsGuestActionOpensSession()
{
    MainMenu menu;
    menu.show();

    QAction *playAsGuest = menu.findChild<QAction *>(QStringLiteral("actionPlay_as_Guest"));
    QVERIFY2(playAsGuest, "actionPlay_as_Guest should exist");

    QSignalSpy spy(&menu, &MainMenu::sessionClosed);
    playAsGuest->trigger();

    QVERIFY(!menu.isVisible());
    QCOMPARE(spy.count(), 0);
}

void TestMainMenu::testLogInButtonOpensDialog()
{
    MainMenu menu;
    menu.show();

    QPushButton *logInButton = menu.findChild<QPushButton *>(QStringLiteral("logInButton"));
    QVERIFY2(logInButton, "logInButton should exist in the UI");

    QTest::mouseClick(logInButton, Qt::LeftButton);

    QVERIFY(!menu.isVisible());
}

void TestMainMenu::testSignUpButtonOpensDialog()
{
    MainMenu menu;
    menu.show();

    QPushButton *signUpButton = menu.findChild<QPushButton *>(QStringLiteral("signUpButton"));
    QVERIFY2(signUpButton, "signUpButton should exist in the UI");

    QTest::mouseClick(signUpButton, Qt::LeftButton);

    QVERIFY(!menu.isVisible());
}

void TestMainMenu::testMenuActionsTriggerSlots()
{
    MainMenu menu;
    menu.show();

    QAction *playAsGuest = menu.findChild<QAction *>(QStringLiteral("actionPlay_as_Guest"));
    QVERIFY2(playAsGuest, "actionPlay_as_Guest should exist");
    QSignalSpy guestSpy(&menu, &MainMenu::sessionClosed);
    playAsGuest->trigger();
    QVERIFY(!menu.isVisible());

    menu.setVisible(true);
    QAction *logInAction = menu.findChild<QAction *>(QStringLiteral("actionLog_In"));
    QVERIFY2(logInAction, "actionLog_In should exist");
    logInAction->trigger();
    QVERIFY(!menu.isVisible());
}
