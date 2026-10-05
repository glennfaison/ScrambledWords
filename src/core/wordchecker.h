#ifndef WORDCHECKER_H
#define WORDCHECKER_H

#include <QObject>
#include <QSet>
#include <QThread>

class WordChecker : public QObject
{
    Q_OBJECT
public:
    explicit WordChecker(const QString &word, QObject *parent = nullptr);

    bool isValid() const;
    unsigned points() const;

    // Performs the check and emits finished(). Intended to be
    // invoked via QMetaObject::invokeMethod on a queued connection.
    void run();

    static bool isWordInDictionary(const QString &word);
    static unsigned pointsFor(const QString &word);

signals:
    void finished(const QString &word, unsigned points);

private:
    QString m_word;
};

#endif // WORDCHECKER_H
