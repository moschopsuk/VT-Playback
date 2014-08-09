#include "vtcontrolwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mlt::Factory::init(NULL);
    VTControlWindow w;
    w.show();

    return a.exec();
}
