#include "svbwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SvbWindow w;
    w.show();

    return a.exec();
}
