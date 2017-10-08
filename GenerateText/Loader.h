#ifndef LOADER_H
#define LOADER_H
#include <QFont>
#include <QFileInfoList>
const QString FONTSPATH = "Fonts/";
const QString IMAGESPATH = "Images/";
const QString RESULTSPATH = "Results/";

class Loader
{
public:
    Loader();
    bool checkFolders();
    int countFonts();
    int countImages();
    QFileInfoList getFonts();
    QFileInfoList getImages();
    int loadFonts();
private:
    QFileInfoList fontList;
    QFileInfoList  imageList;
};

#endif // LOADER_H
