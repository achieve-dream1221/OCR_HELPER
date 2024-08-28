#include <QApplication>
#include <qscreen.h>
#include <QTranslator>
#include "mainwindow.h"

int main(int args, char *argv[]) {
    QApplication app(args, argv);
    // QTranslator translator;
    // if (translator.load(":/i18n/en_US.qm")) {
    //     QApplication::installTranslator(&translator);
    // }
    MainWindow window;
    const auto geometry = QGuiApplication::primaryScreen()->geometry();
    window.move(geometry.center() - window.rect().center());
    window.show();
    return QApplication::exec();
}
