/**
 * @file main.cpp
 *
 */

#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QPushButton>
#include <QInputDialog>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    #endif
    QApplication a(argc, argv);
    QSettings settings("CSE 661", "Homework2");

    MainWindow w{};

    w.show();
    return a.exec();
}
