#include "testsession.h"

#include "../src/ui/session.h"

#include <QTest>
#include <QLineEdit>
#include <QLabel>

void TestSession::testConstructorSetsUserLabel()
{
    Session session(QStringLiteral("alice"));
    session.show();

    QLabel *sessionLabel = session.findChild<QLabel *>(QStringLiteral("sessionLabel"));
    QVERIFY2(sessionLabel, "sessionLabel should exist in the UI");
    QVERIFY2(sessionLabel->text().contains(QStringLiteral("alice")),
              qPrintable(QStringLiteral("sessionLabel should contain user name: %1")
                             .arg(sessionLabel->text())));
}

void TestSession::testNewGameResetsState()
{
    Session session(QStringLiteral("bob"));
    session.show();

    QLineEdit *lineEdit = session.findChild<QLineEdit *>(QStringLiteral("lineEdit"));
    QVERIFY2(lineEdit, "lineEdit should exist in the UI");

    lineEdit->setText(QStringLiteral("TEST"));
    session.on_newGameButton_clicked();

    QCOMPARE(lineEdit->text(), QStringLiteral(""));
}

void TestSession::testLineEditForcesUppercase()
{
    Session session(QStringLiteral("carol"));
    session.show();

    QLineEdit *lineEdit = session.findChild<QLineEdit *>(QStringLiteral("lineEdit"));
    QVERIFY2(lineEdit, "lineEdit should exist in the UI");

    QTest::keyClicks(lineEdit, QStringLiteral("abc"));
    QCOMPARE(lineEdit->text(), QStringLiteral("ABC"));
}

void TestSession::testClearOneButtonRemovesLastChar()
{
    Session session(QStringLiteral("dave"));
    session.show();

    QLineEdit *lineEdit = session.findChild<QLineEdit *>(QStringLiteral("lineEdit"));
    QVERIFY2(lineEdit, "lineEdit should exist in the UI");

    lineEdit->setText(QStringLiteral("WORD"));
    session.on_clearOneButton_clicked();
    QCOMPARE(lineEdit->text(), QStringLiteral("WOR"));
}

void TestSession::testClearAllButtonEmptiesInput()
{
    Session session(QStringLiteral("eve"));
    session.show();

    QLineEdit *lineEdit = session.findChild<QLineEdit *>(QStringLiteral("lineEdit"));
    QVERIFY2(lineEdit, "lineEdit should exist in the UI");

    lineEdit->setText(QStringLiteral("WORD"));
    session.on_clearAllButton_clicked();
    QCOMPARE(lineEdit->text(), QStringLiteral(""));
}

void TestSession::testSubmitButtonClearsLineEdit()
{
    Session session(QStringLiteral("frank"));
    session.show();

    QLineEdit *lineEdit = session.findChild<QLineEdit *>(QStringLiteral("lineEdit"));
    QVERIFY2(lineEdit, "lineEdit should exist in the UI");

    lineEdit->setText(QStringLiteral("HELLO"));
    session.on_submitButton_clicked();
    QCOMPARE(lineEdit->text(), QStringLiteral(""));
}
