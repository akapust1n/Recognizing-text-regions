#include "Generator.h"
#include <QDir>
#include <QFontDatabase>
#include <QImage>
#include <QPainter>
#include <iostream>
#include <random>
#include <stdlib.h>

Generator::Generator(QObject* parent)
    : QObject(parent)
    , gen(std::random_device()())
    , dis(0, 9)
    , disColor(0, 9)
{
    fontNum = loader.countFonts();
    disFont = std::uniform_int_distribution<>(0, fontNum - 1);
    loader.loadFonts();
}

int Generator::generateImages(int count)
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
        auto image = imageList.at(i);
        addText(IMAGESPATH + image.fileName(), fontCounter);
        //  std::cout << "font counter " << fontCounter << std::endl;
        fontCounter++;
        if (!(fontCounter % tenPercents)) {
            static int counter = 10;
            std::cout << counter << " percents!\n";
            counter += 10;
        }
    }
    return fontCounter;
}

bool Generator::addText(QString imagename, const int fontCounter)
{
    QImage image(imagename);
    int numWords = dis(gen) + 3;
    QPainter p;

    for (int i = 0; i < numWords; i++) {
        int fontsize = getFontSize(image.rect());
        if (!p.begin(&image))
            return false;
        QFont font = getFont(imagesPerFont, fontCounter);
        font.setPixelSize(getFontSize(image.rect()));
        p.setFont(font);
        p.setPen(QPen(getColor(), 100));

        QRect coords1 = getCoords(image.rect(), fontsize);
        p.drawText(coords1, Qt::AlignCenter, wordGenerator.getWord());

        p.end();
    }
    bool result = image.save(RESULTSPATH + QString::number(fontCounter) + ".png");
    return result;
}

QFont Generator::getFont(const int imagesPerFont, const int countImages)
{
    static QFileInfoList fontList = loader.getFonts();
    int index = countImages / imagesPerFont - 1;
    if (index < 0 or index == fontList.size()) {
        index = 0;
    }

    int id = disFont(gen);

    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family, 12);
    return font;
}

QColor Generator::getColor()
{
    int temp = disColor(gen);
    switch (temp) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return Qt::black;
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

QRect Generator::getCoords(QRect imageCoords, int fontSize)
{

    std::uniform_int_distribution<> disX(0, imageCoords.bottomRight().x());
    std::uniform_int_distribution<> disY(0, imageCoords.bottomRight().y());
    QPoint start, finish;
    do {
        start.setX(disX(gen));
        start.setY(disY(gen));
        finish.setX(disX(gen));
        finish.setY(disY(gen));
        if (start.x() > finish.x()) {
            int temp = start.x();
            start.setX(finish.x());
            finish.setX(temp);
        }
        if (start.y() > finish.y()) {
            int temp = start.y();
            start.setY(finish.y());
            finish.setY(temp);
        }
    } while ((finish.y() - start.y()) < fontSize);
    QRect result(start, finish);
    return result;
}

QString Generator::getWord()
{
    return wordGenerator.getWord();
}

int Generator::getFontSize(QRect rect)
{
    auto borders = getBorders(rect);
    std::uniform_int_distribution<> dis(borders.low, borders.high);
    return dis(gen);
}

Generator::fontBorders Generator::getBorders(QRect rect)
{
    int height = rect.size().height();
    int start = height / 20;
    int finish = height / 5; // потому что
    return { start, finish };
}
