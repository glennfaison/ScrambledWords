#ifndef TESTPLAYER_H
#define TESTPLAYER_H

#include <QObject>

class TestPlayer : public QObject
{
    Q_OBJECT
private slots:
    void testDefaultNameIsGuest();
    void testAddScoreInsertsDescending();
    void testBestScore();

private:
    QString m_dir;
};

#endif // TESTPLAYER_H
