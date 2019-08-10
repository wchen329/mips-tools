#include "simui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon mainIco = QIcon(":/icons/art/priscas_icon_24.ico");
    QApplication::setWindowIcon(mainIco);
    simUI w;
    w.show();

    return a.exec();
}
