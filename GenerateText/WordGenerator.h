#ifndef WORDGENERATOR_H
#define WORDGENERATOR_H
#include <QString>
#include <random>

class WordGenerator
{
public:
    WordGenerator():gen(std::random_device()()),  disLetterType(0,3), disWordLen( 3, 10), disEnglishLetter(0, 25), disRussianLetter(0, 32){}
    QString getWord();
private:
    QString getLetter(int letterType);
    QString getRusLowLetter();
    QString getRusUpLetter();

private:
     std::mt19937 gen;
     std::uniform_int_distribution<> disLetterType;
     std::uniform_int_distribution<> disWordLen;
     std::uniform_int_distribution<> disEnglishLetter;
     std::uniform_int_distribution<> disRussianLetter;


};

#endif // WORDGENERATOR_H
