#ifndef TST_SCRAMBLEDWORDS_H
#define TST_SCRAMBLEDWORDS_H

#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QApplication>
#include <QTemporaryDir>
#include <QFile>
#include <QTextStream>

// A small fixture dictionary that exercises the binary-search edge cases
// (first record, last record, empty search space) without depending on the
// 10 MB shipped dictionary. Written as 31-byte fixed-width records, padded
// with NULs, exactly like dictionary.txt.
static const char *FIXTURE_DICTIONARY =
    "ALPHA\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "BETA\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "GAMMA\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "DELTA\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "EPSILON\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

class tst_scrambledwords : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // --- WordChecker: dictionary lookup ---
    void findWordInDictionary_presentWord();
    void findWordInDictionary_absentWord();
    void findWordInDictionary_firstRecord();
    void findWordInDictionary_lastRecord();
    void findWordInDictionary_emptyFile();

    // --- WordChecker: scoring ---
    void sendPoints_validWord();
    void sendPoints_invalidWord();
    void sendPoints_singleLetter();
    void sendPoints_pointsMatchScrabbleTable();

    // --- Session: word validity ---
    void wordIsValid_usesEachLetterOnce();
    void wordIsValid_rejectsMissingLetter();
    void wordIsValid_rejectsOveruseOfRepeatedLetter();
    void wordIsValid_emptyString();
    void wordIsValid_singleLetter();

    // --- Session: duplicate tracking ---
    void wordHasBeenEntered_firstSubmission();
    void wordHasBeenEntered_repeatedSubmission();
    void wordHasBeenEntered_caseSensitive();
    void previouslyEnteredWordsResetOnNewGame();

    // --- Session: score accumulation ---
    void sessionScoreAccumulatesCorrectWords();
    void sessionScoreIgnoresInvalidAndDuplicateWords();

private:
    void writeFixture();
    void restoreWorkingDirectory();
    quint64 originalCwd_;
    QTemporaryDir *fixtureDir_;
};

#endif // TST_SCRAMBLEDWORDS_H