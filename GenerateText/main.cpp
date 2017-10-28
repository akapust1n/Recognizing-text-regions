#include <Generator.h>
#include <QApplication>
#include <QCoreApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    // QCoreApplication a(argc, argv);
    QApplication app(argc, argv, false);
    bool onlyNoise = false;
    std::cout << "ARGC " << argc << std::endl;
    if (argc > 1) {
        if (std::string(argv[1]) == "noise") {
            onlyNoise = true;
            std::cout << "Only noise!" << std::endl;
        }
    }

    Generator generator(&app);
    int output = generator.generateImages(1, onlyNoise);
    std::cout << "Agregated " << output << " images!\n";
    return 0;
}
