#include "Loader.h"
#include "Loader.h"
#include <QDir>
#include <QFontDatabase>

Loader::Loader()
{
    QDir fonts("Fonts");

    if (fonts.exists()) {
        QStringList filters;
        filters << "*.ttf" <<"*.otf";
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
    bool check = QDir("Fonts").exists() and  QDir("Images").exists() and QDir("Results").exists() ;

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
    return  fontList;
}

QFileInfoList Loader::getImages()
{

    return imageList;
}

int Loader::loadFonts()
{
    for(auto const & font: fontList ){
        QFontDatabase::addApplicationFont(font.filePath());

    }
}
