#include "TransformationWindow.h"

#include "LatexMatrixSerializer.h"
#include "MatrixTableViewModel.h"
#include "Matrix.h"

#include <QFileDialog>

#include "ComplexNumber.h"
#include "ElementaryComplexFunctions.h"

#include "CudaMatrixOperations.h"

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
	_filters.insert("gaussian", ConvolutionalImageFilter::gaussianFilter(10));
	_filters.insert("boxBlur", ConvolutionalImageFilter::boxBlur());
	_filters.insert("identity", ConvolutionalImageFilter::identity());
	_filters.insert("sharpen", ConvolutionalImageFilter::sharpen());

	Matrix a(512, 512, [](int row, int column) {return (row + 1) + (column + 1); });
	Matrix b(512, 512, [](int row, int column) {return row == column ? 1 : 0; });

	CudaMatrix aCopy;
	aCopy._data = a.getData();
	aCopy._dimension = 512;
	CudaMatrix bCopy;
	bCopy._data = b.getData();
	bCopy._dimension = 512;

	CudaMatrix rawResult = multiplyMatricesWithCuda(aCopy, bCopy);
	Matrix result(rawResult._dimension, rawResult._dimension, rawResult._data);
	if (result == a * b)
	{
		int i = 0;
	}
	else
	{
		int j = 1;
	}

	MatrixTableViewModel* inputModel = new MatrixTableViewModel(result);
	_inputMatrixView->setModel(inputModel);

	Matrix output = Matrix(inputModel->matrix().rowEchelonForm());
	MatrixTableViewModel* outputModel = new MatrixTableViewModel(output);
	_outputMatrixView->setModel(outputModel);

	for (QString key : _filters.keys())
	{
		_selectedFilterComboBox->addItem(key);
	}

	connect(_alphaSlider, &QSlider::valueChanged, _complexGraphWidget, &ColorWheelGraphWidget::setAlpha);
	connect(_radiusAffectsBrightnessCheckBox, &QCheckBox::toggled, _complexGraphWidget, &ColorWheelGraphWidget::setRadiusAffectsBrightness);
	connect(_radiusAffectsBrightnessCheckBox, &QCheckBox::toggled,[this](bool b){ _alphaSlider->setEnabled(b);});

	_complexGraphWidget->setFunction([](const ComplexNumber& z) { return ((complexExp(ComplexNumber::i() * 3 * z) - complexExp(ComplexNumber::i() * 5 * z))*((z*z).inverse())).real(); });
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

void TransformationWindow::on__actionSave_Matrix_triggered()
{
	QString location = QFileDialog::getSaveFileName(nullptr, "Choose a place to save this matrix.", QDir::homePath());
	QFile file(location);
	if (file.open(QFile::WriteOnly))
	{
		LatexMatrixSerializer serializer;
		serializer.saveMatrix(&file, ((MatrixTableViewModel*)_outputMatrixView->model())->matrix());
		file.close();
	}
}
