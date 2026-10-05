#include "scorestore.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <algorithm>

namespace {
constexpr int kMaxScores = 10;

QString storageDirectory()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dir.isEmpty())
        dir = QDir::tempPath() + QStringLiteral("/ScrambledWords");
    QDir().mkpath(dir);
    return dir;
}

QList<int> topEntries(const QMap<QString, QList<int>> &scores, int limit)
{
    struct Entry { QString name; int score; };
    QList<Entry> entries;
    for (auto it = scores.constBegin(); it != scores.constEnd(); ++it) {
        if (it.key() == QStringLiteral("GUEST"))
            continue;
        for (int score : it.value())
            entries.append({it.key(), score});
    }
    std::sort(entries.begin(), entries.end(),
              [](const Entry &a, const Entry &b) { return a.score > b.score; });

    QList<int> result;
    for (int i = 0; i < limit && i < entries.size(); ++i)
        result.append(entries.at(i).score);
    return result;
}
} // namespace

ScoreStore::ScoreStore()
    : ScoreStore(storageDirectory(), true)
{
}

ScoreStore ScoreStore::inDirectory(const QString &storageDir)
{
    return ScoreStore(storageDir, true);
}

ScoreStore::ScoreStore(const QString &storageDir, bool loadFromDisk)
    : m_storageDir(storageDir)
    , m_mainFile(storageDir + QStringLiteral("/highscores.json"))
    , m_backupFile(storageDir + QStringLiteral("/highscores.json.bak"))
{
    if (loadFromDisk)
        load();
}

void ScoreStore::load()
{
    QFile file(m_mainFile);
    if (!file.exists() && QFile(m_backupFile).exists()) {
        QFile::remove(m_mainFile);
        QFile::copy(m_backupFile, m_mainFile);
    }

    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError || !document.isObject())
        return;

    const QJsonObject object = document.object();
    for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
        QList<int> scores;
        for (const QJsonValue &value : it.value().toArray())
            scores.append(value.toInt());
        m_scores.insert(it.key(), scores);
    }
}

void ScoreStore::saveScores(const QString &name, const QList<int> &scores)
{
    m_scores[name] = scores;
    writeToFile();
}

QList<int> ScoreStore::scoresFor(const QString &name) const
{
    return m_scores.value(name);
}

bool ScoreStore::hasPlayer(const QString &name) const
{
    return m_scores.contains(name);
}

QStringList ScoreStore::playerNames() const
{
    QStringList names = m_scores.keys();
    names.removeOne(QStringLiteral("GUEST"));
    names.sort();
    return names;
}

QMap<QString, QList<int>> ScoreStore::byName() const
{
    return m_scores;
}

QList<int> ScoreStore::topScores(int limit) const
{
    return topEntries(m_scores, limit);
}

void ScoreStore::writeToFile() const
{
    QJsonObject object;
    for (auto it = m_scores.constBegin(); it != m_scores.constEnd(); ++it) {
        QJsonArray array;
        for (int score : it.value())
            array.append(score);
        object[it.key()] = array;
    }

    QFile file(m_mainFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    file.write(QJsonDocument(object).toJson());
    file.close();

    backUp();
}

void ScoreStore::backUp() const
{
    QFile::remove(m_backupFile);
    QFile::copy(m_mainFile, m_backupFile);
}
