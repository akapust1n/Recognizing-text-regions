#ifndef GENERATOR_H
#define GENERATOR_H
#include <Loader.h>
#include <QObject>
#include <WordGenerator.h>

class Generator : public QObject {
    Q_OBJECT
public:
    Generator(QObject* parent = 0)
        : QObject(parent)
        , gen(std::random_device()())
    {
    }
    int generateImages(int count);

private:
    struct fontBorders {
        int low;
        int high;
    };

private:
    Loader loader;
    bool addText(QString imagename, const int fontCounter);
    QFont getFont(const int imagesPerFont, const int countImages);
    QColor getColor();
    QRect getCoords(QRect imageCoords, int fontSize);
    QString getWord();
    QString getLetter();
    int getFontSize(QRect rect);
    fontBorders getBorders(QRect rect);

private:
    int imagesPerFont;
    std::mt19937 gen;
    WordGenerator wordGenerator;
};

#endif // GENERATOR_H