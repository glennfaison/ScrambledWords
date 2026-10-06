#ifndef TST_SCRAMBLEDWORDS_H
#define TST_SCRAMBLEDWORDS_H

#include <QObject>
#include <QtTest/QtTest>

class tst_scrambledwords : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // --- WordChecker: dictionary lookup ---
    // Plain English description:
    // Verifies the binary-search dictionary lookup correctly reports
    // whether a given word exists in dictionary.txt.
    void findWordInDictionary_presentWord();     // a word known to be in the dictionary → found
    void findWordInDictionary_absentWord();      // a word not in the dictionary → not found
    void findWordInDictionary_firstRecord();     // the first (sorted) entry in the file
    void findWordInDictionary_lastRecord();      // the last (sorted) entry in the file
    void findWordInDictionary_emptyFile();       // empty dictionary file → always not found

    // --- WordChecker: scoring ---
    // Plain English description:
    // Verifies that points are only awarded for dictionary-valid words
    // and that the points equal the sum of per-letter Scrabble values
    // defined in WordChecker::run().
    void sendPoints_validWord();                // valid dict word → correct point total
    void sendPoints_invalidWord();              // invalid word → 0 points
    void sendPoints_singleLetter();             // single-letter word → correct value
    void sendPoints_pointsMatchValuesTable();   // every letter's value matches the code table

    // --- Letter ---
    // Verifies that Letter(seed) deterministically maps a seed to a
    // letter and that the letter's Scrabble value matches the code's
    // values table.
    void letterCharacterAndValueFromSeed();

private:
    void writeFixtureDictionary();
};

#endif // TST_SCRAMBLEDWORDS_H
