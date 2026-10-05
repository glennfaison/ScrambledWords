#ifndef SESSION_H
#define SESSION_H

#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QPushButton>
#include <QSet>

#include "../core/letter.h"
#include "../core/player.h"
#include "../core/scorestore.h"
#include "../core/timer.h"
#include "../core/wordchecker.h"

namespace Ui {
class Session;
}

class Session : public QMainWindow
{
    Q_OBJECT
public:
    explicit Session(const QString &user, QWidget *parent = nullptr);
    ~Session() override;

    QString name() const { return m_currentUser.name(); }
    unsigned sessionScore() const { return m_sessionScore; }

signals:
    void sessionClosed(const Player &player);

public slots:
    void on_newGameButton_clicked();
    void on_submitButton_clicked();
    void on_clearOneButton_clicked();
    void on_clearAllButton_clicked();
    void on_lineEdit_returnPressed();
    void on_lineEdit_textChanged(const QString &text);
    void on_actionNew_Game_triggered();
    void on_actionRules_of_the_Game_triggered();
    void on_actionControls_triggered();
    void on_backLink_clicked();
    void on_actionBack_triggered();
    void on_actionAuthor_triggered();

private:
    void setGameActive(bool active);
    void getNewLetterButtons();
    void linkLetterButtonsAndValues();
    void appendLetter(const QString &letter);
    bool wordHasBeenEntered(const QString &entry);
    bool wordIsValid(const QString &entry) const;
    void onCountDown(unsigned secondsLeft);
    void onPointsReceived(const QString &word, unsigned points);
    void endSession();
    QString highScoreHtml() const;

    Ui::Session *ui;
    Player m_currentUser;
    Timer *m_timer;

    QList<WordChecker *> m_checkers;
    QList<QPushButton *> m_letterButtons;
    QList<QLabel *> m_letterScores;
    QList<QChar> m_currentLetters;

    unsigned m_sessionScore;
    QSet<QString> m_previouslyEnteredWords;
    QStringList m_rejectedEntries;
};

#endif // SESSION_H
