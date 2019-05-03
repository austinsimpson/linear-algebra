#include "TransformationWindow.h"
#include <QApplication>
#include <QDebug>

#ifdef QT_OS_MAC
#include <QtMacExtras>
#endif

#include "ComplexNumber.h"
#include "ElementaryComplexFunctions.h"

#include "CudaMatrixOperations.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	ComplexNumber number = -1 + ComplexNumber::i();
	auto result = complexSin(number);

	TransformationWindow w;
	w.setUnifiedTitleAndToolBarOnMac(true);
	w.show();

	return app.exec();
}
