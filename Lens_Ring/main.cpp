#include "mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("Lens-Ring/green-circle-md.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();
    MainWindow w;
    w.setAttribute(Qt::WA_QuitOnClose,true);
    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    w.show();

    splash.finish(&w);
    return a.exec();
}
