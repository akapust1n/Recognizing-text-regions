#ifndef GENERATOR_H
#define GENERATOR_H
#include <Loader.h>
#include <QObject>
#include <QVector>
#include <WordGenerator.h>
#include <QFont>

class Generator : public QObject {
    Q_OBJECT
public:
    Generator(QObject* parent = 0);
    int generateImages(int count);

private:
    struct fontBorders {
        int low;
        int high;
    };

private:
    Loader loader;
    bool addText(QString imagename, int& fontCounter);
    QFont getFont(const int imagesPerFont, const int countImages);
    QColor getColor();
    QRect getCoords(QRect imageCoords, int fontSize, QVector<QRect>& rects, QRect wordSize);
    QString getWord(int maxWidth, QFont &font);
    int getFontSize(QRect rect);
    fontBorders getBorders(QRect rect);
    bool contains(QVector<QRect>& rects, QRect coords) const;

private:
    int imagesPerFont;
    int fontNum;
    std::mt19937 gen;
    WordGenerator wordGenerator;
    std::uniform_int_distribution<> dis;
    std::uniform_int_distribution<> disColor;
    std::uniform_int_distribution<> disFont;
};

#endif // GENERATOR_H
