#ifndef GENERATOR_H
#define GENERATOR_H
#include <Loader.h>
#include <QFont>
#include <QObject>
#include <QVector>
#include <WordGenerator.h>
const int FONTLOWBORDER = 27;
const int FONTHIGHBORDER = 10;
const int LINELOWBORDER = 9;
const int LINEHIGHBORDER = 2;

const int MAXNUMWORDS = 7;

class Generator : public QObject {
    Q_OBJECT
public:
    Generator(QObject* parent = 0);
    int generateImages(int count, bool onlyNoise = false);

private:
    struct fontBorders {
        int low;
        int high;
    };

private:
    Loader loader;
    const QVector<QRect> addText(QImage& image, int& fontCounter, bool save = false);
    const QVector<QRect>& addLines(QImage& image, int& fontCounter, QVector<QRect>& rects, bool save = true);
    QFont getFont(const int imagesPerFont, const int countImages);
    QColor getColor(bool isWord = true);
    QRect getCoords(QRect imageCoords, QVector<QRect>& rects, QRect wordSize);
    QString getWord(int maxWidth, QFont& font);
    int getFontSize(QRect rect);
    fontBorders getBorders(QRect rect);
    bool contains(QVector<QRect>& rects, QRect coords) const;
    QRect getLineSize(QSize size, int lineWidth);

private:
    int imagesPerFont;
    int fontNum;
    std::mt19937 gen;
    WordGenerator wordGenerator;
    std::uniform_int_distribution<> dis;
    std::uniform_int_distribution<> disColor;
    std::uniform_int_distribution<> disFont;

    std::uniform_int_distribution<> disLines;
    std::uniform_int_distribution<> disLineWidth;
    std::uniform_int_distribution<> disLinesPosition;
    std::uniform_int_distribution<> disAngleGenerator;
};

#endif // GENERATOR_H
