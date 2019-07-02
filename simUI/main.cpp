#include "simui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    simUI w;
    w.show();

    return a.exec();
}
