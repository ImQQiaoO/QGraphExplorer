#include <spdlog/spdlog.h>
#include "MainWindow.h"
#include <QApplication>

//#ifdef _WIN32
//#include <Windows.h>
//#endif  // _WIN32


int main(int argc, char *argv[]) {
//#ifdef _WIN32
//    SetConsoleOutputCP(CP_UTF8);
//    SetConsoleCP(CP_UTF8);
//#endif  // _WIN32

    spdlog::set_level(spdlog::level::debug);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//#ifdef _WIN32
//    SetConsoleOutputCP(CP_ACP);
//    SetConsoleCP(CP_ACP);
//#endif  // _WIN32
    return QApplication::exec();
}
