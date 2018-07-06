#include "TransformationWindow.h"

#include "MatrixTableViewModel.h"
#include "Matrix.h"

#include <QFileDialog>

TransformationWindow::TransformationWindow
(
    QWidget* parent
):
    QMainWindow(parent),
    Ui::TransformationWindow()
{
    setupUi(this);

	_filters.insert("horizontalSobel", ConvolutionalImageFilter::horizontalSobel());
	_filters.insert("verticalSobel", ConvolutionalImageFilter::verticalSobel());
	_filters.insert("normalEdge", ConvolutionalImageFilter::edgeDetector());
	_filters.insert("gaussian", ConvolutionalImageFilter::gaussian());

	Matrix matrix(2, 2);
	for (int i = 0; i < matrix.rowCount(); i++)
	{
		for (int j = 0; j < matrix.columnCount(); j++)
		{
			matrix.setEntry(i, j, 3 * (i * i) + 2 * (j + 1));
		}
	}


	MatrixTableViewModel* inputModel = new MatrixTableViewModel(new Matrix(matrix));
	_inputMatrixView->setModel(inputModel);

	Matrix output = matrix.rowEchelonForm();
	MatrixTableViewModel* outputModel = new MatrixTableViewModel(new Matrix(output));
	_outputMatrixView->setModel(outputModel);

	for (QString key : _filters.keys())
	{
		_selectedFilterComboBox->addItem(key);
	}

	//_transformationView->setTransformationMatrix(matrix);
}

TransformationWindow::~TransformationWindow()
{

}

void TransformationWindow::on__browseImageButton_clicked()
{
	QString imageName = QFileDialog::getOpenFileName(this, "Select an image to convolve");
	if (!imageName.isEmpty() && !imageName.isNull())
	{
		QImage original(imageName);
		_currentImage = original;
		_originalImageDisplayLabel->setPixmap(QPixmap::fromImage(original));
		QImage convolved = _filters[_selectedFilterComboBox->currentText()].apply(original);
		_convolutionDisplayLabel->setPixmap(QPixmap::fromImage(convolved));
	}
}

void TransformationWindow::on__selectedFilterComboBox_currentIndexChanged
(
	const QString& selectedIndex
)
{
	if (_currentImage.isNull() == false)
	{
		_convolutionDisplayLabel->setPixmap(QPixmap::fromImage(_filters[selectedIndex].apply(_currentImage)));
	}
}
