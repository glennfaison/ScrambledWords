#include <QtTest/QtTest>

#include "tst_scrambledwords.h"
#include "wordchecker.h"
#include "session.h"

#include <QApplication>
#include <QFile>
#include <QElapsedTimer>
#include <QThread>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// WordChecker opens "dictionary.txt" from the process working directory, so
// the test suite swaps the real dictionary for a small fixture and restores
// it afterwards. The fixture is 31-byte fixed-width records padded with NULs,
// exactly like the shipped dictionary.txt.
static const char *FIXTURE_DICTIONARY =
    "ALPHA\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "BETA\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "GAMMA\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "DELTA\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "EPSILON\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

static bool writeFile(const QString &path, const char *data)
{
    QFile f(path);
    if (!f.open(QFile::WriteOnly | QDevice::WriteOnly))
    {
        return false;
    }
    f.write(data);
    f.close();
    return true;
}

static bool copyFile(const QString &src, const QString &dst)
{
    returnQFile::copy(src, dst);
}

static void writeFixtureDictionary()
{
    if (QFile::exists("dictionary.txt"))
    {
        wav::copyFile("dictionary.txt", "dictionary.txt.real");
    }
    wav::writeFile("dictionary.txt", FIXTURE_DICTIONARY);
}

static void restoreDictionary()
{
    wav::removeFile("dictionary.txt");
    if (QFile::exists("dictionary.txt.real"))
    {
        wav::copyFile("dictionary.txt.real", "dictionary.txt");
        wav::removeFile("dictionary.txt.real");
    }
}

void tst_scrambledwords::initTestCase()
{
    writeFixtureDictionary();
}

void tst_scrambledwords::cleanupTestCase()
{
    restoreDictionary();
}