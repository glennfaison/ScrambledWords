#include "testletter.h"

#include <QTest>

#include "../src/core/letter.h"

#include <QSet>

void TestLetter::testCharacterIsInAlphabet()
{
    const QSet<QChar> alphabet = QSet<QChar>()
        << 'A' << 'B' << 'C' << 'D' << 'E' << 'F' << 'G' << 'H'
        << 'I' << 'J' << 'K' << 'L' << 'M' << 'N' << 'O' << 'P'
        << 'Q' << 'R' << 'S' << 'T' << 'U' << 'V' << 'W' << 'X'
        << 'Y' << 'Z';

    for (int i = 0; i < 100; ++i) {
        Letter letter;
        QVERIFY2(alphabet.contains(letter.character()),
                 qPrintable(QString("unexpected letter: %1").arg(letter.character())));
        QVERIFY(letter.value() > 0);
    }
}

void TestLetter::testSeededConstructorIsDeterministic()
{
    Letter a(42);
    Letter b(42);
    QCOMPARE(a.character(), b.character());
    QCOMPARE(a.value(), b.value());
}

void TestLetter::testValuesMatchScrabble()
{
    QCOMPARE(Letter::valueFor('A'), 1);
    QCOMPARE(Letter::valueFor('B'), 3);
    QCOMPARE(Letter::valueFor('C'), 3);
    QCOMPARE(Letter::valueFor('D'), 2);
    QCOMPARE(Letter::valueFor('E'), 1);
    QCOMPARE(Letter::valueFor('F'), 4);
    QCOMPARE(Letter::valueFor('G'), 2);
    QCOMPARE(Letter::valueFor('H'), 4);
    QCOMPARE(Letter::valueFor('I'), 1);
    QCOMPARE(Letter::valueFor('J'), 8);
    QCOMPARE(Letter::valueFor('K'), 2);
    QCOMPARE(Letter::valueFor('M'), 1);
    QCOMPARE(Letter::valueFor('O'), 1);
    QCOMPARE(Letter::valueFor('P'), 3);
    QCOMPARE(Letter::valueFor('Q'), 8);
    QCOMPARE(Letter::valueFor('S'), 1);
    QCOMPARE(Letter::valueFor('V'), 4);
    QCOMPARE(Letter::valueFor('W'), 4);
    QCOMPARE(Letter::valueFor('Z'), 10);
    // lowercase is handled
    QCOMPARE(Letter::valueFor('q'), 8);
}

void TestLetter::testValueForRejectsNonLetters()
{
    QCOMPARE(Letter::valueFor('0'), 0);
    QCOMPARE(Letter::valueFor(' '), 0);
    QCOMPARE(Letter::valueFor(QChar()), 0);
}
