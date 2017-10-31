#include <Generator.h>
#include <QApplication>
#include <QCoreApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    // QCoreApplication a(argc, argv);
    QApplication app(argc, argv, false);
    bool modes = NoiseWords;
    std::cout << "ARGC " << argc << std::endl;
    if (argc > 1) {
        if (std::string(argv[1]) == "noise") {
            modes = OnlyNoise;
            std::cout << "Only noise!" << std::endl;
        } else if (std::string(argv[1]) == "words") {
            modes = OnlyWords;
            std::cout << "Only words!" << std::endl;
        }
    }

    Generator generator(&app);
    int output = generator.generateImages(1, modes);
    std::cout << "Agregated " << output << " images!\n";
    return 0;
}
