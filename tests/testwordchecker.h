#ifndef TESTWORDCHECKER_H
#define TESTWORDCHECKER_H

#include <QObject>

class TestWordChecker : public QObject
{
    Q_OBJECT
private slots:
    void testPointsFor();
    void testIsWordInDictionaryUsesWorkingDirectory();
    void testWordCheckerSignals();

private:
    void setWorkingDictionary(const QString &path);
    QString m_originalWorkingDirectory;
};

#endif // TESTWORDCHECKER_H
