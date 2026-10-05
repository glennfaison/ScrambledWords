#include "timer.h"

Timer::Timer(QObject *parent, unsigned seconds)
    : QThread(parent)
    , m_timeLeft(seconds)
{
}

void Timer::addSeconds(unsigned seconds)
{
    m_timeLeft += seconds;
}

void Timer::run()
{
    for (unsigned i = 0; i <= m_timeLeft; ++i) {
        emit countDown(m_timeLeft - i);
        sleep(1);
    }
}
