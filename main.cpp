#include "TransformationWindow.h"
#include <QApplication>
#include <QDebug>

#include <QTableView>

#include "Matrix.h"
#include "MatrixTableViewModel.h"

#include <QtMacExtras>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	TransformationWindow w;
	w.setUnifiedTitleAndToolBarOnMac(true);
	w.show();

	return app.exec();
}
