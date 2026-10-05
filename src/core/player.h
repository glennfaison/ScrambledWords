#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include <QPair>
#include <QString>

class ScoreStore;

class Player
{
public:
    struct HiScoreEntry {
        QString name;
        int score;
    };

    explicit Player(const QString &name = QStringLiteral("GUEST"));

    QString name() const;
    void addScore(unsigned points);
    QList<int> scores() const;
    int bestScore() const;

    void save(ScoreStore *store) const;

private:
    void loadFrom(const QList<int> &storedScores);

    QString m_name;
    QList<int> m_scores;
    int m_sessionScore = 0;
};

#endif // PLAYER_H
