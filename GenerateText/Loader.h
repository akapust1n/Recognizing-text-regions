#ifndef LOADER_H
#define LOADER_H
#include <QFileInfoList>
#include <QFont>
#include <QFontMetrics>
#include <QVector>
#include <vector>
const QString FONTSPATH = "Fonts/";
const QString IMAGESPATH = "Images/";
const QString RESULTSPATH = "Results/";

class Loader {
public:
    Loader();
    bool checkFolders();
    int countFonts();
    int countImages();
    QFileInfoList getFonts();
    QFont* getFont(int id);
    QSize fontWordSize(QString word, int id);
    QFileInfoList getImages();
    int loadFonts();

private:
    QFileInfoList fontList;
    QFileInfoList imageList;
    QVector<QFont> fonts;
    std::vector<QFontMetrics> fontMetrics;
};

#endif // LOADER_H
