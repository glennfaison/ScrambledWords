#ifndef SCORESTORE_H
#define SCORESTORE_H

#include <QList>
#include <QMap>
#include <QString>

// Stores player high-score data persistently.
//
// Data lives in a single JSON file under the user's per-application
// data location (QStandardPaths::AppDataLocation),
// e.g. ~/.local/share/ScrambledWords/highscores.json on Linux.
// A second copy is kept as a backup next to the main file.
class ScoreStore
{
public:
    ScoreStore();

    void saveScores(const QString &name, const QList<int> &scores);
    QList<int> scoresFor(const QString &name) const;
    bool hasPlayer(const QString &name) const;
    QStringList playerNames() const;
    QMap<QString, QList<int>> byName() const;
    QList<int> topScores(int limit) const;

    // Loads data from the given directory instead of the
    // per-user application data location (used by tests).
    static ScoreStore inDirectory(const QString &storageDir);

private:
    explicit ScoreStore(const QString &storageDir, bool loadFromDisk);

    QString m_storageDir;
    QString m_mainFile;
    QString m_backupFile;
    QMap<QString, QList<int>> m_scores;

    void load();
    void writeToFile() const;
    void backUp() const;
};

#endif // SCORESTORE_H
