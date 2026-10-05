#include "letter.h"

#include <QTime>
#include <cstdlib>

namespace {

// Letter distribution weighted like a Scrabble tile bag.
static const QChar kAlphabet[] = {
    'A', 'A', 'B', 'C', 'D', 'E', 'E', 'F', 'G', 'H',
    'I', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'U', 'V', 'W', 'X', 'Y',
    'Y', 'Z'
};

// Letter values exactly as used by the original game (SCRABBLE-style,
// with K=2 and Q=8 as in the original code).
static const int kLetterValues[26] = {
    /* A */ 1, /* B */ 3, /* C */ 3, /* D */ 2, /* E */ 1,
    /* F */ 4, /* G */ 2, /* H */ 4, /* I */ 1, /* J */ 8,
    /* K */ 2, /* L */ 1, /* M */ 1, /* N */ 1, /* O */ 1,
    /* P */ 3, /* Q */ 8, /* R */ 1, /* S */ 1, /* T */ 1,
    /* U */ 1, /* V */ 4, /* W */ 4, /* X */ 10, /* Y */ 10,
    /* Z */ 10
};

constexpr int kAlphabetSize = 32;
constexpr unsigned kInvalidSeed = 0xFFFFFFFFu;

} // namespace

Letter::Letter()
{
    m_character = randomLetter(kInvalidSeed);
    m_value = valueFor(m_character);
}

Letter::Letter(unsigned seed)
{
    m_character = randomLetter(seed);
    m_value = valueFor(m_character);
}

QChar Letter::character() const
{
    return m_character;
}

unsigned Letter::value() const
{
    return m_value;
}

int Letter::valueFor(QChar letter)
{
    const int index = letter.toUpper().unicode() - 'A';
    if (index < 0 || index > 25)
        return 0;
    return kLetterValues[index];
}

QChar Letter::randomLetter()
{
    return randomLetter(kInvalidSeed);
}

QChar Letter::randomLetter(unsigned seed)
{
    // rand_r() is deterministic per seed and thread-safe.
    // The sentinel value means "seed from the clock" so the same
    // code path serves both unseeded and seeded randomness.
    unsigned state = (seed == kInvalidSeed)
        ? static_cast<unsigned>(QTime::currentTime().msec())
        : seed;
    const int pick = rand_r(&state) % kAlphabetSize;
    return kAlphabet[pick];
}
