#include "TransformationWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TransformationWindow w;
    w.show();

    return a.exec();
}
