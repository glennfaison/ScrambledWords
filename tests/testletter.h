#ifndef TESTLETTER_H
#define TESTLETTER_H

#include <QObject>

class TestLetter : public QObject
{
    Q_OBJECT
private slots:
    void testCharacterIsInAlphabet();
    void testSeededConstructorIsDeterministic();
    void testValuesMatchScrabble();
    void testValueForRejectsNonLetters();
};

#endif // TESTLETTER_H
