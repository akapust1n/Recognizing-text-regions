#include "Generator.h"
#include <QDir>
#include <QFontDatabase>
#include <QImage>
#include <QMutex>
#include <QPainter>
#include <iostream>
#include <omp.h>
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
        //  //std::cout << "font counter " << fontCounter << std::endl;

        if (!(fontCounter % tenPercents)) {
            static int counter = 10;
            std::cout << counter << " percents!\n";
            counter += 10;
        }
    }

    return fontCounter;
}

bool Generator::addText(QString imagename, int& fontCounter)
{
    QImage image(imagename);
    int numWords = dis(gen) + 1;
    QPainter p;
    QVector<QRect> rects;
    //std::cout << "BEFORE CYCLE" << std::endl;

    for (int i = 0; i < numWords; i++) {
        int fontsize = getFontSize(image.rect());
        if (!p.begin(&image))
            return false;

        QFont font = getFont(imagesPerFont, fontCounter);
        font.setPixelSize(fontsize);
        p.setFont(font);
        p.setPen(QPen(getColor(), 100));

        QString word = wordGenerator.getWord(image.rect().width(), font);
        QRect wordSize = wordGenerator.wordSize(word, font);
        //std::cout<<"WORD "<<word.toStdString()<<"__"<<font.pointSize()<<std::endl;
        QRect coords1 = getCoords(image.rect(), fontsize, rects, wordSize);
        if (coords1.size().width() == 0) {
            //std::cout << "CANT INSERT new word to image" << std::endl;
            p.end();
            break; //can't insert new word to image
        }
        rects.append(coords1);
        p.drawText(coords1, Qt::AlignCenter, word);
        //        p.fillRect(coords1,Qt::red);
        //        QPainterPath path;
        //        path.addRoundedRect(coords1, 10, 10);
        //        QPen pen(Qt::black, 10);
        //        p.setPen(pen);
        //        p.fillPath(path, Qt::red);
        //        p.drawPath(path);
        p.end();
    }
    //std::cout << "END CYCLE" << std::endl;
    // //std::cout<<"rects size_"<<rects.size()<<std::endl;
    int counter;
#pragma omp critical
    {
        counter = fontCounter;
        fontCounter++;
    }

    bool result = image.save(RESULTSPATH + QString::number(counter) + ".png");
    //std::cout << result << std::endl;
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

QRect Generator::getCoords(QRect imageCoords, int fontSize, QVector<QRect>& rects, QRect wordSize)
{
   if(imageCoords.bottomRight().x()<wordSize.width())

       std::cout<<"error1";
   if(imageCoords.bottomRight().y()<wordSize.height())
       std::cout<<"error2";
    std::uniform_int_distribution<> disX(0, imageCoords.bottomRight().x() - wordSize.width());
    std::uniform_int_distribution<> disY(0, imageCoords.bottomRight().y() - wordSize.height());
    //std::cout << "after uniform_int_distribution\n ";
    QPoint start, finish;
    QRect result{ QPoint{ 0, 0 }, QPoint{ 0, 0 } };
    int counter = 0;
    bool isContains = true;
    do {
        counter++;
        do {
            //std::cout << "before generation \n ";
            //std::cout << (imageCoords.bottomRight().x())<<"_"<<(wordSize.width())
                      //<< std::endl
                    //  << (imageCoords.bottomRight().y() - wordSize.height()) << std::endl;

            start.setX(disX(gen));
            start.setY(disY(gen));

            finish.setX(start.x() + wordSize.width());
            finish.setY(start.y() + wordSize.height());
            //std::cout << "intro cycle1 " << start.x() << " " << start.y() << std::endl;
            //std::cout << "FINISH: " << finish.x() << " " << finish.y() << std::endl;

            //std::cout << "IMAGE: " << imageCoords.height() << " " << imageCoords.width() << std::endl;
            //std::cout << "WORD: " << wordSize.width() << " " << wordSize.height() << std::endl;

        } while (finish.y() > imageCoords.height() or finish.x() > imageCoords.width());
        //std::cout << "intro cycle2 \n";

        isContains = contains(rects, { start, finish });
    } while (counter < 10 and (isContains));
    if (!isContains) {
        result.setCoords(start.x(), start.y(), start.x() + wordSize.width(), start.y() + wordSize.height());
    } //QRect result(start, finish);
    return result;
}

QString Generator::getWord(int maxWidth, QFont &font)
{
    return wordGenerator.getWord(maxWidth, font);
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
    int finish = height / 7; // потому что
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
