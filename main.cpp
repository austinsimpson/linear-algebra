#include "TransformationWindow.h"
#include <QApplication>
#include <QDebug>

#include <QtMacExtras>

#include "ComplexNumber.h"
#include "ElementaryComplexFunctions.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	TransformationWindow w;
	w.setUnifiedTitleAndToolBarOnMac(true);
	w.show();

	return app.exec();
}
