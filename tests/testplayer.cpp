#include "testplayer.h"

#include <QTest>

#include "../src/core/player.h"

#include <QTemporaryDir>

void TestPlayer::testDefaultNameIsGuest()
{
    Player player;
    QCOMPARE(player.name(), QString(QStringLiteral("GUEST")));
}

void TestPlayer::testAddScoreInsertsDescending()
{
    Player player(QStringLiteral("alice"));
    player.addScore(50);
    player.addScore(120);
    player.addScore(80);
    QCOMPARE(player.scores(), QList<int>({120, 80, 50}));
}

void TestPlayer::testBestScore()
{
    Player player(QStringLiteral("bob"));
    QCOMPARE(player.bestScore(), 0);
    player.addScore(42);
    QCOMPARE(player.bestScore(), 42);
}
