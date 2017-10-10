#include "WordGenerator.h"
#include <QFontMetrics>
#include <iostream>

QString WordGenerator::getWord(int maxWidth, QFont& font)
{
start:
    int wordLen = 0, len = 0;
    QFontMetrics fm(font);
    int widthOneLetter = fm.width("a");

    int letterType = disLetterType(gen);
    do {
        len = disWordLen(gen);
        wordLen = len * widthOneLetter;

    } while ((maxWidth -10) < len);
    //std::cout << "\n\n Image width and word width" << maxWidth << "__" << len << std::endl;
    QString word;
    for (int i = 0; i < len; i++) {
        word += getLetter(letterType);
    }
    if (fm.width(word) > (maxWidth-10)){
        goto start;                //потому что я могу
    }
   // std::cout<<fm.width(word)<<"____-"<<maxWidth<<"\n";

    return word;
}

QRect WordGenerator::wordSize(QString word, QFont& font)
{
    QFontMetrics fm(font);
    int width = fm.width(word);
    int height = fm.height();
    return { QPoint{ 0, 0 }, QPoint{ width, height } };
}

QString WordGenerator::getLetter(int letterType)
{
    switch (letterType) {
    case 0: //eng lowerCase
        return QChar('a' + disEnglishLetter(gen));
    case 1: //eng upp
        return QChar('A' + disEnglishLetter(gen));
    case 2: //rus lowerCase
        return getRusLowLetter();
    case 3: //rus up
        return getRusUpLetter();
    default:
        return QChar('a' + disEnglishLetter(gen));
    }
}

QString WordGenerator::getRusLowLetter()
{
    int letter = disRussianLetter(gen);
    switch (letter) {
    case 0:
        return QString::fromUtf8(u8"а");
    case 1:
        return QString::fromUtf8(u8"б");
    case 2:
        return QString::fromUtf8(u8"в");
    case 3:
        return QString::fromUtf8(u8"г");
    case 4:
        return QString::fromUtf8(u8"д");
    case 5:
        return QString::fromUtf8(u8"е");
    case 6:
        return QString::fromUtf8(u8"ё");
    case 7:
        return QString::fromUtf8(u8"ж");
    case 8:
        return QString::fromUtf8(u8"з");
    case 9:
        return QString::fromUtf8(u8"и");
    case 10:
        return QString::fromUtf8(u8"й");
    case 11:
        return QString::fromUtf8(u8"к");
    case 12:
        return QString::fromUtf8(u8"л");
    case 13:
        return QString::fromUtf8(u8"м");
    case 14:
        return QString::fromUtf8(u8"н");
    case 15:
        return QString::fromUtf8(u8"о");
    case 16:
        return QString::fromUtf8(u8"п");
    case 17:
        return QString::fromUtf8(u8"р");
    case 18:
        return QString::fromUtf8(u8"с");
    case 19:
        return QString::fromUtf8(u8"т");
    case 20:
        return QString::fromUtf8(u8"у");
    case 21:
        return QString::fromUtf8(u8"ф");
    case 22:
        return QString::fromUtf8(u8"х");
    case 23:
        return QString::fromUtf8(u8"ц");
    case 24:
        return QString::fromUtf8(u8"ч");
    case 25:
        return QString::fromUtf8(u8"ш");
    case 26:
        return QString::fromUtf8(u8"щ");
    case 27:
        return QString::fromUtf8(u8"ъ");
    case 28:
        return QString::fromUtf8(u8"ы");
    case 29:
        return QString::fromUtf8(u8"ь");
    case 30:
        return QString::fromUtf8(u8"э");
    case 31:
        return QString::fromUtf8(u8"ю");
    case 32:
        return QString::fromUtf8(u8"я");
    }
}

QString WordGenerator::getRusUpLetter()
{
    int letter = disRussianLetter(gen);
    switch (letter) {
    case 0:
        return QString::fromUtf8(u8"А");
    case 1:
        return QString::fromUtf8(u8"Б");
    case 2:
        return QString::fromUtf8(u8"В");
    case 3:
        return QString::fromUtf8(u8"Г");
    case 4:
        return QString::fromUtf8(u8"Д");
    case 5:
        return QString::fromUtf8(u8"Е");
    case 6:
        return QString::fromUtf8(u8"Ё");
    case 7:
        return QString::fromUtf8(u8"Ж");
    case 8:
        return QString::fromUtf8(u8"З");
    case 9:
        return QString::fromUtf8(u8"И");
    case 10:
        return QString::fromUtf8(u8"Й");
    case 11:
        return QString::fromUtf8(u8"К");
    case 12:
        return QString::fromUtf8(u8"Л");
    case 13:
        return QString::fromUtf8(u8"М");
    case 14:
        return QString::fromUtf8(u8"Н");
    case 15:
        return QString::fromUtf8(u8"О");
    case 16:
        return QString::fromUtf8(u8"П");
    case 17:
        return QString::fromUtf8(u8"Р");
    case 18:
        return QString::fromUtf8(u8"С");
    case 19:
        return QString::fromUtf8(u8"Т");
    case 20:
        return QString::fromUtf8(u8"У");
    case 21:
        return QString::fromUtf8(u8"Ф");
    case 22:
        return QString::fromUtf8(u8"Х");
    case 23:
        return QString::fromUtf8(u8"Ц");
    case 24:
        return QString::fromUtf8(u8"Ч");
    case 25:
        return QString::fromUtf8(u8"Ш");
    case 26:
        return QString::fromUtf8(u8"Щ");
    case 27:
        return QString::fromUtf8(u8"Ъ");
    case 28:
        return QString::fromUtf8(u8"Ы");
    case 29:
        return QString::fromUtf8(u8"Ь");
    case 30:
        return QString::fromUtf8(u8"Э");
    case 31:
        return QString::fromUtf8(u8"Ю");
    case 32:
        return QString::fromUtf8(u8"Я");
    }
}
