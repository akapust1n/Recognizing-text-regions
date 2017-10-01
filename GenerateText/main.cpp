#include <iostream>
#include <Generator.h>
#include <QCoreApplication>
#include <QApplication>

int main(int argc, char *argv[])
{
   // QCoreApplication a(argc, argv);
    QApplication app( argc, argv, false );


    Generator generator(&app);
    int output = generator.generateImages(1);
    std::cout<<"Agregated "<< output<< " images!\n";
    return 0;
}
