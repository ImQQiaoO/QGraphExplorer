#include <spdlog/spdlog.h>
#include "MainWindow.h"
#include <QApplication>


int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return QApplication::exec();
}
