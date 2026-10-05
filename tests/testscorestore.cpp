#include "testscorestore.h"

#include "../src/core/scorestore.h"

#include <QFile>
#include <QTemporaryDir>
#include <QTest>

void TestScoreStore::testSaveAndLoad()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    ScoreStore store = ScoreStore::inDirectory(dir.path());
    store.saveScores(QStringLiteral("alice"), {100, 50, 200});

    ScoreStore reloaded = ScoreStore::inDirectory(dir.path());
    QCOMPARE(reloaded.scoresFor(QStringLiteral("alice")), QList<int>({100, 50, 200}));
}

void TestScoreStore::testHasPlayer()
{
    QTemporaryDir dir;
    ScoreStore store = ScoreStore::inDirectory(dir.path());
    store.saveScores(QStringLiteral("bob"), {10});

    ScoreStore reloaded = ScoreStore::inDirectory(dir.path());
    QVERIFY(reloaded.hasPlayer(QStringLiteral("bob")));
    QVERIFY(!reloaded.hasPlayer(QStringLiteral("carol")));
}

void TestScoreStore::testPlayerNamesExcludesGuest()
{
    QTemporaryDir dir;
    ScoreStore store = ScoreStore::inDirectory(dir.path());
    store.saveScores(QStringLiteral("GUEST"), {10});
    store.saveScores(QStringLiteral("dave"), {20});

    ScoreStore reloaded = ScoreStore::inDirectory(dir.path());
    const QStringList names = reloaded.playerNames();
    QVERIFY2(names.contains(QStringLiteral("dave")),
             qPrintable(QString("names: %1").arg(names.join(','))));
    QVERIFY(!names.contains(QStringLiteral("GUEST")));
}

void TestScoreStore::testScoresFor()
{
    QTemporaryDir dir;
    ScoreStore store = ScoreStore::inDirectory(dir.path());
    store.saveScores(QStringLiteral("erin"), {30, 40});

    ScoreStore reloaded = ScoreStore::inDirectory(dir.path());
    QCOMPARE(reloaded.scoresFor(QStringLiteral("erin")), QList<int>({30, 40}));
    QCOMPARE(reloaded.scoresFor(QStringLiteral("nobody")), QList<int>());
}

void TestScoreStore::testBackupIsWritten()
{
    QTemporaryDir dir;
    ScoreStore store = ScoreStore::inDirectory(dir.path());
    store.saveScores(QStringLiteral("frank"), {5});

    const QString backupPath = dir.path() + QStringLiteral("/highscores.json.bak");
    QVERIFY2(QFile(backupPath).exists(), qPrintable(backupPath));
    QVERIFY2(QFile(backupPath).size() > 0,
             qPrintable(QStringLiteral("backup file should not be empty")));
}

void TestScoreStore::testRestoreFromBackup()
{
    QTemporaryDir dir;
    ScoreStore store = ScoreStore::inDirectory(dir.path());
    store.saveScores(QStringLiteral("gina"), {70});

    QFile mainFile(dir.path() + QStringLiteral("/highscores.json"));
    mainFile.remove();

    ScoreStore reloaded = ScoreStore::inDirectory(dir.path());
    QCOMPARE(reloaded.scoresFor(QStringLiteral("gina")), QList<int>({70}));
}
