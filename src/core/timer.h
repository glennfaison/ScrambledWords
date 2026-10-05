#ifndef TIMER_H
#define TIMER_H

#include <QThread>

// Counts down from a starting value to zero, emitting
// countDown once per second. Intended to be started with start()
// and deleted (or waited on) when no longer needed.
class Timer : public QThread
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = nullptr, unsigned seconds = 100);

    void addSeconds(unsigned seconds);

protected:
    void run() override;

signals:
    void countDown(unsigned secondsLeft);

private:
    unsigned m_timeLeft;
};

#endif // TIMER_H
