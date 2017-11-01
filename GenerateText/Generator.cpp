#include "Generator.h"
#include <QDir>
#include <QFontDatabase>
#include <QImage>
#include <QMutex>
#include <QPainter>
#include <cmath>
#include <iostream>
#include <omp.h>
#include <random>
#include <stdlib.h>

Generator::Generator(QObject* parent)
    : QObject(parent)
    , gen(std::random_device()())
    , dis(2, MAXNUMWORDS)
    , disColor(0, 9)
    , disLines(0, 15)
    , disLineWidth(1, 5)
    , disLinesPosition(0, 4)
    , disAngleGenerator(10, 80)
{
    fontNum = loader.countFonts();
    disFont = std::uniform_int_distribution<>(0, fontNum - 1);
    loader.loadFonts();
    //    for (int i = 0; i < fontNum; i++) {
    //        fonts.append(QFontDatabase::applicationFontFamilies(i).at(0));
    //    }
}

int Generator::generateImages(int count, bool prMode)
{
    int countFonts = loader.countFonts();
    int countImages = loader.countImages();

    if (!loader.checkFolders() or !countFonts or !countImages)
        return -1;
    imagesPerFont = countImages / countFonts;
    int fontCounter = 0;
    auto imageList = loader.getImages();
    int fullSize = imageList.size();
    int tenPercents = fullSize * 1.0 / 10.0;
    if (tenPercents == 0) {
        tenPercents = 1;
    }

#pragma omp parallel for
    // for (const auto& image : imageList) {
    for (int i = 0; i < imageList.size(); i++) {

        auto imageName = imageList.at(i).fileName();
        QImage image(QString(IMAGESPATH + imageName));
        if (image.isNull()) {
            continue;
        }
        switch (prMode) {
        case OnlyNoise: {
            auto rects = QVector<QRect>(); // :))))))))
            addLines(image, fontCounter, rects);
            break;
        }
        case OnlyWords: {
            auto rects = addText(image, fontCounter, true);
            break;
        }
        case NoiseWords: {
            auto rects = addText(image, fontCounter);
            if (rects.size()) {
                addLines(image, fontCounter, rects);
            };
        }
        }

        if (!(fontCounter % tenPercents)) {
            static int counter = 10;
            std::cout << counter << " percents!\n";
            counter += 10;
        }
    }

    return fontCounter;
}

QVector<QRect> Generator::addText(QImage& image, int& fontCounter, bool save)
{
    int numWords = dis(gen) + 1;
    QPainter p;
    QVector<QRect> rects;
    //std::cout << "BEFORE CYCLE" << std::endl;

    for (int i = 0; i < numWords; i++) {
        if (!p.begin(&image))
            return QVector<QRect>{};

        int fontsize = getFontSize(image.rect());

        Font font = getFont(imagesPerFont, fontCounter);
        font.font->setPixelSize(fontsize);
        p.setFont(*font.font); //:)))))
        p.setPen(QPen(getColor(), 100));
        QString word = wordGenerator.getWord(image.rect().width(), &loader, font.id);
        QRect wordSize = wordGenerator.wordSize(word, loader.fontWordSize(word, font.id));
        QRect coords1 = getCoords(image.rect(), rects, wordSize);
        if (coords1.size().width() == 0) {
            //std::cout << "CANT INSERT new word to image" << std::endl;
            p.end();
            break; //can't insert new word to image
        }
        rects.append(coords1);

        p.drawText(coords1, Qt::AlignCenter, word);

        p.end();
    }
    //std::cout << "END CYCLE" << std::endl;
    // //std::cout<<"rects size_"<<rects.size()<<std::endl;

    if (save) {
        int counter;
#pragma omp critical
        {
            counter = fontCounter;
            fontCounter++;
        }

        image.save(RESULTSPATH + QString::number(counter) + ".jpg");
    }
    //std::cout << result << std::endl;
    return rects;
}

QVector<QRect> Generator::addLines(QImage& image, int& fontCounter, QVector<QRect>& rects, bool save)
{
    int numLines = disLines(gen) + 1;
    QPainter p;

    for (int i = 0; i < numLines; i++) {
        if (!p.begin(&image))
            return QVector<QRect>{};
        p.setRenderHint(QPainter::Antialiasing);
        int lineWidth = disLineWidth(gen);
        p.setPen(QPen(getColor(false), lineWidth));
        QRect lineSize = getLineSize(image.size(), lineWidth);
        QRect coords1 = getCoords(image.rect(), rects, lineSize);

        if (coords1.size().width() == 0) {
            p.end();
            break; //can't insert new word to image
        }
        rects.append(coords1);
        int lineDirection = disLinesPosition(gen);
        if (lineDirection > 1) {
            p.drawLine(coords1.topLeft(), coords1.bottomRight());
        } else {
            p.drawLine(coords1.bottomLeft(), coords1.topRight());
        }
        p.end();
    }

    if (save) {
        int counter;
#pragma omp critical
        {
            counter = fontCounter;
            fontCounter++;
        }

        image.save(RESULTSPATH + QString::number(counter) + ".jpg");
    }
    //std::cout << result << std::endl;
    return rects;
}

Generator::Font Generator::getFont(const int imagesPerFont, const int countImages)
{
    static QFileInfoList fontList = loader.getFonts();
    int index = countImages / imagesPerFont - 1;
    if (index < 0 or index == fontList.size()) {
        index = 0;
    }

    int id = disFont(gen);
    QFont* font = loader.getFont(id);
    //    QString family = fonts[id];
    //    QFont font(family, 12);
    return { font, id };
}

QColor Generator::getColor(bool isWord)
{
    int temp = disColor(gen);
    if (isWord) {
        switch (temp) {
        case 0:
        case 1:
            return Qt::black;
        case 2:
        case 3:
        case 4:
        case 5:
            return Qt::white;
        case 6:
            return Qt::red;
        case 7:
            return Qt::green;
        case 8:
            return Qt::gray;
        case 9:
            return Qt::darkCyan;
        }
    } else {
        switch (temp) {
        case 0:
        case 1:
            return Qt::black;
        case 2:
        case 3:
            return Qt::gray; //да, так задумно

        case 4:
        case 5:
            return Qt::white;
        case 6:
            return Qt::red;
        case 7:
            return Qt::green;
        case 8:
            return Qt::gray;
        case 9:
            return Qt::darkCyan;
        }
    }
}

QRect Generator::getCoords(QRect imageCoords, QVector<QRect>& rects, QRect wordSize)
{
    if ((imageCoords.bottomRight().x() - wordSize.width()) <= 0)
        return QRect(0, 0, 0, 0);

    if ((imageCoords.bottomRight().y() - wordSize.height()) <= 0)
        return QRect(0, 0, 0, 0);

    std::uniform_int_distribution<> disX(0, imageCoords.bottomRight().x() - wordSize.width());
    std::uniform_int_distribution<> disY(0, imageCoords.bottomRight().y() - wordSize.height());
    QPoint start, finish;
    QRect result{ QPoint{ 0, 0 }, QPoint{ 0, 0 } };
    int counter = 0;
    bool isContains = true;
    do {
        counter++;
        do {
            start.setX(disX(gen));
            start.setY(disY(gen));

            finish.setX(start.x() + wordSize.width());
            finish.setY(start.y() + wordSize.height());
        } while (finish.y() > imageCoords.height() or finish.x() > imageCoords.width());
        //std::cout << "intro cycle2 \n";

        isContains = contains(rects, { start, finish });
    } while (counter < 10 and (isContains));
    if (!isContains) {
        result.setCoords(start.x(), start.y(), start.x() + wordSize.width(), start.y() + wordSize.height());
    } //QRect result(start, finish);
    return result;
}

//QString Generator::getWord(int maxWidth, QFont& font)
//{
//    return wordGenerator.getWord(maxWidth, font);
//}

int Generator::getFontSize(QRect rect)
{
    auto borders = getBorders(rect);
    std::uniform_int_distribution<> dis(borders.low, borders.high);
    return dis(gen);
}

Generator::fontBorders Generator::getBorders(QRect rect)
{
    int height = rect.size().height();
    int start = height / FONTLOWBORDER;
    int finish = height / FONTHIGHBORDER; // потому что
    return { start, finish };
}

bool Generator::contains(QVector<QRect>& rects, QRect coords) const
{
    for (auto const& rect : rects) {

        if (rect.intersects(coords)) {
            // //std::cout<<"INTERSECTS!"<<std::endl;
            return true;
        }
    }
    return false;
}

QRect Generator::getLineSize(QSize size, int lineWidth)
{
    int height = size.height();
    int width = size.width();
    int start = height / LINELOWBORDER;
    int finish = height / LINEHIGHBORDER; // потому что
    if (finish > width)
        finish = width;

    std::uniform_int_distribution<> disLocal(start, finish);
    int lineLength = disLocal(gen);
    int lineType = disLinesPosition(gen);
    int angle = disAngleGenerator(gen);
    enum {
        Vertical = 0,
        Horizonal,
        lineAtAnAngleFromTop,
        lineAtAnAngleFromTop1,
        lineAtAnAngleFromTop2,
    };
    switch (lineType) {
    case Vertical:
        return QRect(0, 0, lineWidth, lineLength);

    case Horizonal:
        return QRect(0, 0, lineLength, lineWidth);
    case lineAtAnAngleFromTop:
    case lineAtAnAngleFromTop1:
    case lineAtAnAngleFromTop2: {
        return QRect(0, 0, width * cos(angle * M_PI / 180), height * sin(angle * M_PI / 180));
    }
    default:
        return QRect(0, 0, lineWidth, lineLength);
    }
}
