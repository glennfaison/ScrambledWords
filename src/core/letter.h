#ifndef LETTER_H
#define LETTER_H

#include <QChar>

class Letter
{
public:
    Letter();
    explicit Letter(unsigned seed);

    QChar character() const;
    unsigned value() const;

    static int valueFor(QChar letter);
    static QChar randomLetter();
    static QChar randomLetter(unsigned seed);

private:
    QChar m_character;
    unsigned m_value;
};

#endif // LETTER_H
