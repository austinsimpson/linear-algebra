#include "TransformationWindow.h"

#include "MatrixTableViewModel.h"
#include "Matrix.h"

TransformationWindow::TransformationWindow
(
    QWidget* parent
):
    QMainWindow(parent),
    Ui::TransformationWindow()
{
    setupUi(this);

	Matrix* matrix = Matrix::Zeroes(2, 2);
	for (int i = 0; i < matrix->rowCount(); i++)
	{
		for (int j = 0; j < matrix->columnCount(); j++)
		{
			//if (j != 1)
				matrix->setEntry(i, j, 3 * (i * i) + 2 * (j + 1));
		}
	}


	MatrixTableViewModel* inputModel = new MatrixTableViewModel(matrix);
	_inputMatrixView->setModel(inputModel);

	MatrixTableViewModel* outputModel = new MatrixTableViewModel(matrix->ref());
	_outputMatrixView->setModel(outputModel);

	//_transformationView->setTransformationMatrix(matrix);
}

TransformationWindow::~TransformationWindow()
{

}
