#include "allpagelistwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AllPageListWidget w;
    w.show();

    return a.exec();
}
