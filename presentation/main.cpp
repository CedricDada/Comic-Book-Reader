#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "../model/Page.h"

int main(int argc, char *argv[]) {
    qRegisterMetaType<Page>("Page");
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "BookReaderView_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    qSetMessagePattern("[%{type}] %{function}:%{line} - %{message}");
    //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    MainWindow w;
    w.show();
    return a.exec();
}
