#ifndef TESTSCORESTORE_H
#define TESTSCORESTORE_H

#include <QObject>

class TestScoreStore : public QObject
{
    Q_OBJECT
private slots:
    void testSaveAndLoad();
    void testHasPlayer();
    void testPlayerNamesExcludesGuest();
    void testScoresFor();
    void testBackupIsWritten();
    void testRestoreFromBackup();

private:
    QString m_dir;
};

#endif // TESTSCORESTORE_H
