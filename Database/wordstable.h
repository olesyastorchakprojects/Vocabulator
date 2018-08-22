#ifndef WORDSTABLE_H
#define WORDSTABLE_H

#include "../Objects/word.h"

class WordsTable
{
public:
    static QList<Word> words(const int projectId );
    static int insertWord(const QString& word, const int projectId);
};

#endif // WORDSTABLE_H
