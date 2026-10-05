#include "testletter.h"
#include "testplayer.h"
#include "testscorestore.h"
#include "testwordchecker.h"

#include <QCoreApplication>
#include <QTest>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);

    int failures = 0;

    TestLetter letter;
    failures += QTest::qExec(&letter, argc, argv);

    TestPlayer player;
    failures += QTest::qExec(&player, argc, argv);

    TestScoreStore scoreStore;
    failures += QTest::qExec(&scoreStore, argc, argv);

    TestWordChecker wordChecker;
    failures += QTest::qExec(&wordChecker, argc, argv);

    return failures;
}
