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
	_filters.insert("gaussian", ConvolutionalImageFilter::gaussianFilter(.82999));
	_filters.insert("identity", ConvolutionalImageFilter::identity());
	_filters.insert("sharpen", ConvolutionalImageFilter::sharpen());

	MatrixTableViewModel* inputModel = new MatrixTableViewModel(_filters["horizontalSobel"].convolutionMatrix());
	_inputMatrixView->setModel(inputModel);

	Matrix output = Matrix(inputModel->matrix().rowEchelonForm());
	MatrixTableViewModel* outputModel = new MatrixTableViewModel(output);
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
