#include <QtTest/QtTest>
#include "tst_scrambledwords.h"
#include "wordchecker.h"
#include "letter.h"
#include <QFile>
#include <QSignalSpy>

// Fixture dictionary: 6 words of 31 bytes each = 186 bytes total
// Sorted for binary search: A < ALPHA < BETA < DELTA < EPSILON < GAMMA
static QByteArray buildFixtureDictionary()
{
    QByteArray fixture;
    const char *words[] = {"A", "ALPHA", "BETA", "DELTA", "EPSILON", "GAMMA"};
    for (const char *w : words) {
        char buf[31] = {0};           // zero-initialized to all NUL
        strncpy(buf, w, 30);          // copy up to 30 chars (leave room for terminator)
        fixture.append(buf, 31);      // append exactly 31 bytes
    }
    return fixture;
}

static void ensureFixtureDictionary()
{
    QFile f("dictionary.txt");
    if (f.open(QFile::WriteOnly | QFile::Truncate))
    {
        f.write(buildFixtureDictionary());
        f.close();
    }
}

static void backupAndRemoveHighScoreFiles()
{
    if (QFile::exists("highScores.dat"))
        QFile::rename("highScores.dat", "highScores.dat.backup");
    if (QFile::exists("highScores.backup"))
        QFile::rename("highScores.backup", "highScores.backup.backup");
}

static void restoreHighScoreFiles()
{
    if (QFile::exists("highScores.dat.backup"))
        QFile::rename("highScores.dat.backup", "highScores.dat");
    if (QFile::exists("highScores.backup.backup"))
        QFile::rename("highScores.backup.backup", "highScores.backup");
}

void tst_scrambledwords::initTestCase()
{
    backupAndRemoveHighScoreFiles();
    ensureFixtureDictionary();
}

void tst_scrambledwords::cleanupTestCase()
{
    restoreHighScoreFiles();
    ensureFixtureDictionary();  // ensure we leave fixture in place for consistency
}

void tst_scrambledwords::findWordInDictionary_presentWord()
{
    WordChecker checker("ALPHA");
    QVERIFY(checker.findWordInDictionary());
}

void tst_scrambledwords::findWordInDictionary_absentWord()
{
    WordChecker checker("NOTHERE");
    QVERIFY(!checker.findWordInDictionary());
}

void tst_scrambledwords::findWordInDictionary_firstRecord()
{
    WordChecker checker("A");
    QVERIFY(checker.findWordInDictionary());
}

void tst_scrambledwords::findWordInDictionary_lastRecord()
{
    WordChecker checker("GAMMA");
    QVERIFY(checker.findWordInDictionary());
}

void tst_scrambledwords::findWordInDictionary_emptyFile()
{
    // Save current dictionary, truncate, test, restore
    QFile::remove("dictionary.txt");
    QFile f("dictionary.txt");
    f.open(QFile::WriteOnly);  // creates empty file
    f.close();

    WordChecker checker("ALPHA");
    QVERIFY(!checker.findWordInDictionary());

    // Restore fixture
    ensureFixtureDictionary();
}

void tst_scrambledwords::sendPoints_validWord()
{
    WordChecker checker("ALPHA");
    QSignalSpy spy(&checker, SIGNAL(sendPoints(QString, unsigned)));
    checker.run();
    QCOMPARE(spy.count(), 1);
    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.at(0).toString(), QString("ALPHA"));
    QCOMPARE(args.at(1).toUInt(), 10u);
}

void tst_scrambledwords::sendPoints_invalidWord()
{
    WordChecker checker("NOTHERE");
    QSignalSpy spy(&checker, SIGNAL(sendPoints(QString, unsigned)));
    checker.run();
    QCOMPARE(spy.count(), 1);
    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.at(0).toString(), QString("NOTHERE"));
    QCOMPARE(args.at(1).toUInt(), 0u);
}

void tst_scrambledwords::sendPoints_singleLetter()
{
    WordChecker checker("A");
    QSignalSpy spy(&checker, SIGNAL(sendPoints(QString, unsigned)));
    checker.run();
    QCOMPARE(spy.count(), 1);
    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.at(0).toString(), QString("A"));
    QCOMPARE(args.at(1).toUInt(), 1u);
}

void tst_scrambledwords::sendPoints_pointsMatchValuesTable()
{
    // Values array from WordChecker::run():
    // {1, 2, 3, 1, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1, 1, 3, 8, 1, 1, 1, 1, 1, 10, 10, 10, 10}
    // Index by letter - 'A': A=1, B=2, C=3, D=1, E=1, F=4, G=2, H=4, I=1, J=8, K=10, L=1, M=2, N=1, O=1, P=3, Q=8, R=1, S=1, T=1, U=1, V=1, W=10, X=10, Y=10, Z=10
    
    // Test known combinations from fixture dictionary
    struct TestCase {
        QString word;
        uint expectedPoints;
    };
    TestCase cases[] = {
        {"A", 1},          // A
        {"ALPHA", 10},     // A=1, L=1, P=3, H=4, A=1
        {"BETA", 5},       // B=2, E=1, T=1, A=1
        {"GAMMA", 8},      // G=2, A=1, M=2, M=2, A=1
    };

    for (const TestCase &tc : cases) {
        WordChecker checker(tc.word);
        QSignalSpy spy(&checker, SIGNAL(sendPoints(QString, unsigned)));
        checker.run();
        QCOMPARE(spy.count(), 1);
        QList<QVariant> args = spy.takeFirst();
        QCOMPARE(args.at(0).toString(), tc.word);
        QCOMPARE(args.at(1).toUInt(), tc.expectedPoints);
    }
}

void tst_scrambledwords::letterCharacterAndValueFromSeed()
{
    // Values array from Letter.cpp (indexed by letter - 'A')
    const unsigned charValues[26] = {
        1, 2, 3, 1, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1,
        1, 3, 8, 1, 1, 1, 1, 1, 10, 10, 10, 10
    };

    // Test that Letter constructor produces consistent character/value
    Letter l1(42);
    Letter l2(42);
    QCOMPARE(l1.character(), l2.character());
    QCOMPARE(l1.value(), l2.value());

    // Test that the value matches the character
    QChar ch = l1.character();
    QVERIFY(ch >= 'A' && ch <= 'Z');
    uint expectedValue = charValues[ch.toLatin1() - 'A'];
    QCOMPARE(l1.value(), expectedValue);

    // Test that different seeds can give different results
    bool differentSeen = false;
    QChar firstCh;
    for (int i = 0; i < 20; ++i) {
        Letter l(1000 + i * 100);
        if (i == 0) {
            firstCh = l.character();
        } else if (l.character() != firstCh) {
            differentSeen = true;
            break;
        }
    }
    QVERIFY(differentSeen);  // with different seeds, we should get varied results
}
