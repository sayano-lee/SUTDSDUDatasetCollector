#include "collectorController.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    collectorController w;
    w.show();
    return a.exec();
}
