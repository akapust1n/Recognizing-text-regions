#include "Loader.h"
#include "Loader.h"
#include <QDir>
#include <QFontDatabase>

Loader::Loader()
{
    QDir fonts("Fonts");

    if (fonts.exists()) {
        QStringList filters;
        filters << "*.ttf"
                << "*.otf";
        fonts.setNameFilters(filters);
        fontList = fonts.entryInfoList(QDir::Files);
    };
    QDir images("Images");
    if (images.exists()) {
        QStringList filters;
        filters << "*.jpg"
                << "*.jpeg"
                << "*.png"
                << "*.bmp"
                << "*.gif";
        images.setNameFilters(filters);

        imageList = images.entryInfoList(QDir::Files);
    }
}

bool Loader::checkFolders()
{
    bool check = QDir("Fonts").exists() and QDir("Images").exists() and QDir("Results").exists();

    return check;
}

int Loader::countFonts()
{
    return fontList.size();
}

int Loader::countImages()
{

    return imageList.size();
}

QFileInfoList Loader::getFonts()
{
    return fontList;
}

QFont* Loader::getFont(int id)
{
    return &fonts[id];
}

QSize Loader::fontWordSize(QString word, int id)
{
    return { fontMetrics[id].width(word), fontMetrics[id].height() };
}

QFileInfoList Loader::getImages()
{

    return imageList;
}

int Loader::loadFonts()
{
    int id = 0;
    QString family;
    for (auto const& font : fontList) {
        id = QFontDatabase::addApplicationFont(font.filePath());
        family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont temp(family, 12);
        fonts.append(temp);
        QFontMetrics metrics(temp);
        fontMetrics.push_back(metrics); //предположим, что шрифты моноширинные
    }
}
