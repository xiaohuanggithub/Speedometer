#include "speedometer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Speedometer w;
    w.show();
    return a.exec();
}
