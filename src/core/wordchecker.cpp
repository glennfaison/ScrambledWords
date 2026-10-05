#include "wordchecker.h"

#include "letter.h"

#include <QFile>

WordChecker::WordChecker(const QString &word, QObject *parent)
    : QObject(parent)
    , m_word(word)
{
}

void WordChecker::run()
{
    const bool valid = isWordInDictionary(m_word);
    const unsigned points = valid ? pointsFor(m_word) : 0;
    emit finished(m_word, points);
}

bool WordChecker::isValid() const
{
    return isWordInDictionary(m_word);
}

unsigned WordChecker::points() const
{
    return isValid() ? pointsFor(m_word) : 0;
}

bool WordChecker::isWordInDictionary(const QString &word)
{
    if (word.isEmpty())
        return false;

    QFile file(QStringLiteral("dictionary.txt"));
    if (!file.exists())
        return false;

    // The dictionary is stored as fixed 30-byte null-padded records
    // sorted by word. Load it and look the word up in memory.
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const int recordSize = 30;
    const QByteArray data = file.readAll();
    const int count = data.size() / recordSize;

    QSet<QString> words;
    words.reserve(count);
    for (int i = 0; i < count; ++i) {
        QByteArray record(data.constData() + i * recordSize, recordSize);
        words.insert(QString::fromLatin1(record.trimmed()));
    }

    return words.contains(word);
}

unsigned WordChecker::pointsFor(const QString &word)
{
    unsigned points = 0;
    for (const QChar &ch : word)
        points += Letter::valueFor(ch);
    return points;
}
