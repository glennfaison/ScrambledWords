#ifndef TESTSESSION_H
#define TESTSESSION_H

#include <QObject>
#include <QSignalSpy>
#include <QLineEdit>
#include <QLabel>

class TestSession : public QObject
{
    Q_OBJECT
private slots:
    void testConstructorSetsUserLabel();
    void testNewGameResetsState();
    void testLineEditForcesUppercase();
    void testClearOneButtonRemovesLastChar();
    void testClearAllButtonEmptiesInput();
    void testSubmitButtonClearsLineEdit();
};

#endif // TESTSESSION_H
