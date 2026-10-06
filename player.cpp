#include "player.h"
#include <cstring>

Player::Player(QString name)
{
    std::cout << name.toStdString() << " constructor" << std::endl;
    this->userName = name;
    // check for existence of backup file
    QFile fp("highScores.backup");
    if(!fp.exists())
    {
        fp.open(QFile::ReadWrite);
        fp.close();
    }
    // read high scores from file
    loadHiScores(name);
    updatePlayerData();
}

void Player::loadHiScores(QString name)
{
    // first restore save file
    restoreSaveFile();
    // attempt to create the file if it doesn't exist
    QFile file("highScores.dat");
    try
    {
        file.open(QFile::ReadWrite);
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        restoreSaveFile();
        return loadHiScores(name);
    }

    qint32 first = 0;
    qint32 last = file.size();
    while(last >= first)
    {
        Record record;
        qint32 blocks = (last - first) / sizeof(record);
        qint32 mid = first + (blocks / 2) * sizeof(record);
        file.seek(mid);
        file.read((char*)&record, sizeof(record));
        if(name == QString(record.userName))
        {
            copyFromRecord(record);
            file.close();
            return;
        }
        if(name > QString(record.userName))
        {
            first = mid + sizeof(record);
        }
        else
        {
            last = mid - sizeof(record);
        }
    }
    userName = name;
    for(unsigned i = 0; i <= highScoreLength; i++)
    {
        high_score[i] = 0;
    }
    file.close();
}

void Player::saveToFile()
{
    Record record;
    backUpSaveFile();
    QFile file("highScores.dat");
    try
    {
        file.open(QFile::ReadWrite);
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        restoreSaveFile();
        return saveToFile();
    }
    qint32 first = 0;
    qint32 last = file.size();
    while(last >= first)
    {
        // get the number of blocks in search space
        qint32 blocks = (last - first) / sizeof(record);
        // get the position of the middle block
        qint32 mid = first + (blocks / 2) * sizeof(record);
        // go to the position of the middle block
        file.seek(mid);
        // read that block
        file.read((char*)&record, sizeof(record));

        // if the record is found, return
        if(userName == QString(record.userName))
        {
            // write relevant info to the record
            copyToRecord(record);
            // write the record to the file
            file.seek(mid);
            file.write((char*)&record, sizeof(record));
            file.close();
            return;
        }
        if(userName > QString(record.userName))
        {
            first = mid + (sizeof(record));
        }
        else    // if(userName < QString(record.userName))
        {
            last = mid - (sizeof(record));
        }
    }
    file.close();
    // go to the end of the file

    try
    {
        file.open(QFile::ReadWrite|QFile::Append);
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        restoreSaveFile();
        return saveToFile();
    }
    // write relevant info to the record
    copyToRecord(record);
    // write the record to the file
    file.write((char*)&record, sizeof(record));
    file.close();
    backUpSaveFile();
    return;
}

bool Player::exists(QString entry)
{
    QFile file("highScores.dat");
    try
    {
        file.open(QFile::ReadWrite);
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        restoreSaveFile();
        return exists(entry);
    }
    qint32 first = 0;
    qint32 last = file.size();
    while(last >= first)
    {
        Record record;
        // get the number of blocks in search space
        qint32 blocks = (last - first) / sizeof(record);
        // get the position of the middle block
        qint32 mid = first + (blocks / 2) * sizeof(record);
        // go to the position of the middle block
        file.seek(mid);
        // read that block
        file.read((char*)&record, sizeof(record));

        // if the word is found, return true
        if(entry == QString(record.userName))
        {
            return true;
        }
        if(entry > QString(record.userName))
        {
            first = mid + (sizeof(record));
        }
        else    // if(entry < QString(record.userName))
        {
            last = mid - (sizeof(record));
        }
    }
    return false;
}

void Player::backUpSaveFile()
{
    // first remove the old backup file
    try
    {
        QFile::remove("highScores.backup");
        std::cout << "removing old backup..." << std::endl;
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        return backUpSaveFile();
    }

    // back up the save file
    try
    {
        QFile::copy("highScores.dat", "highScores.backup");
        std::cout << "backing up save file..." << std::endl;
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        return backUpSaveFile();
    }
}

void Player::restoreSaveFile()
{
    // first delete the save file,
    try
    {
        QFile::remove("highScores.dat");
        std::cout << "removing old save file..." << std::endl;
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        return restoreSaveFile();
    }

    // then replace it with a copy of the backup file
    try
    {
        QFile::copy("highScores.backup", "highScores.dat");
        std::cout << "restoring save file..." << std::endl;
    }
    catch(const QException &e)
    {
        std::cout << e.what() << std::endl;
        return restoreSaveFile();
    }
}

QStringList Player::loadAllPlayerNames()
{
    QFile file("highScores.dat");
    try
    {
        file.open(QFile::ReadWrite);
    }
    catch(const QException &e)
    {
        std::cerr << e.what() << std::endl;
        restoreSaveFile();
        return loadAllPlayerNames();
    }
    Record record;
    QStringList ret;
    while(file.read((char*)&record, sizeof(record)))
    {
        if(QString(record.userName) != "GUEST")
        {
            ret << QString(record.userName);
        }
    }
    file.close();
    return ret;
}

QList<Player::HiScoreEntry> Player::loadGeneralHiScores()
{
    QFile file("highScores.dat");
    try
    {
        file.open(QFile::ReadWrite);
    }
    catch(const QException &e)
    {
        std::cerr << e.what() << std::endl;
        return loadGeneralHiScores();
    }
    QList<HiScoreEntry> ret;
    HiScoreEntry hs;
    hs.first = "GUEST";
    hs.second = 0;
    // add empty HiScoreEntrys
    for(unsigned i = 0; i < highScoreLength; i++) ret.push_back(hs);
    Record record;
    while(file.read((char*)&record, sizeof(record)))
    {
        // add each high score in a sorted manner to the list
        for(unsigned i = 0; i < highScoreLength; i++)
        {
            hs.first = record.userName;
            hs.second = record.high_score[i];
            for(unsigned j = 0; j < highScoreLength; j++)
            {
                if(hs.second > ret.at(j).second)
                {
                    // insert at the right position
                    ret.insert(j, hs);
                }
            }
        }
    }
    file.close();
    return ret;
}

bool Player::less(const Player::HiScoreEntry &first, const Player::HiScoreEntry &second)
{
    return (first.second < second.second);
}

void Player::updatePlayerData(unsigned points)
{
    high_score[highScoreLength] = points;
    sortHiScores(high_score, 0, highScoreLength);
}

void Player::sortHiScores(unsigned *A, unsigned left, unsigned right)
{
    if(left < right)
        {
            unsigned m = split(A, left, right);
            sortHiScores(A, left, m - 1);
            sortHiScores(A, m + 1, right);
        }
}

unsigned Player::split(unsigned *A, unsigned left, unsigned right)
{
    unsigned key = A[left];
    unsigned i = left + 1;
    unsigned j = right + 1;

    while(i < j)
    {
        // find first smaller or equal element
        while(A[i] > key)
        {
            i++;
        }
        // find last bigger or equal element
        while(A[--j] < key);

        if(i < j)
        {
            swap(A, i, j);
        }
    }

    swap(A, left, j);
    return j;
}

void Player::swap(unsigned *A, unsigned left, unsigned right)
{
    A[left] ^= A[right], A[right] ^= A[left], A[left] ^= A[right];
}

void Player::copyToRecord(Record &record)
{
    // write relevant info to the record
    std::strcpy(record.userName, userName.toStdString().c_str());
    for(unsigned i = 0; i <= highScoreLength; i++)
    {
        record.high_score[i] = high_score[i];
    }
}

void Player::copyFromRecord(Record &record)
{
    userName = record.userName;
    for(unsigned i = 0; i <= highScoreLength; i++)
    {
        high_score[i] = record.high_score[i];
    }
}
