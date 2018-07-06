#include "TransformationWindow.h"
#include <QApplication>
#include <QDebug>

#include <QtMacExtras>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TransformationWindow w;
	w.setUnifiedTitleAndToolBarOnMac(true);
    w.show();

    return a.exec();
}
