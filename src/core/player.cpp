#include "player.h"

#include "scorestore.h"

#include <algorithm>

Player::Player(const QString &name)
    : m_name(name)
{
    m_scores = ScoreStore().scoresFor(m_name);
}

QString Player::name() const
{
    return m_name;
}

void Player::addScore(unsigned points)
{
    m_sessionScore = static_cast<int>(points);
    m_scores.prepend(m_sessionScore);
    std::sort(m_scores.begin(), m_scores.end(), std::greater<int>());
}

QList<int> Player::scores() const
{
    return m_scores;
}

int Player::bestScore() const
{
    return m_scores.isEmpty() ? 0 : m_scores.first();
}

void Player::save(ScoreStore *store) const
{
    if (store)
        store->saveScores(m_name, m_scores);
}

void Player::loadFrom(const QList<int> &storedScores)
{
    m_scores = storedScores;
}
