#include "allpagelistwidget.h"
#include <QApplication>
#include "globaldate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GlobalDate::initCatalog("Download");
    GlobalDate::initCatalog("IM");
    GlobalDate::initCatalog("IM/file");
    GlobalDate::initCatalog("IM/data");

    AllPageListWidget w;
    w.show();

    return a.exec();
}
