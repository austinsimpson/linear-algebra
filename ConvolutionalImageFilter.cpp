#include "ConvolutionalImageFilter.h"

#include "Algebra.h"

#include <QDebug>
#include <math.h>

ConvolutionalImageFilter::ConvolutionalImageFilter() : _shouldApplyGrayscale(false)
{
	_convolutionMatrix = Matrix::Identity(1);
	//The 1x1 identity matrix yields an identical image after the convolution operation
}

Matrix& ConvolutionalImageFilter::convolutionMatrix()
{
	return _convolutionMatrix;
}

bool ConvolutionalImageFilter::setConvolutionMatrix(const Matrix& matrix)
{
	bool result = false;
	if (matrix.rowCount() % 2 != 0 && matrix.columnCount() % 2)
	{
		_convolutionMatrix = matrix;
		double norm = 0;
		for (int i = 0; i < _convolutionMatrix.rowCount(); i++)
		{
			for (int j = 0; j < _convolutionMatrix.columnCount(); j++)
			{
				norm += _convolutionMatrix.getEntry(i, j);
			}
		}

		if (norm == 0)
			norm = 1;

		_convolutionMatrix.scale(1.0 / norm);
	}
	return result;
}

bool ConvolutionalImageFilter::shouldApplyGrayscale()
{
	return _shouldApplyGrayscale;
}

void ConvolutionalImageFilter::setShouldApplyGrayscale(bool isGrayscale)
{
	_shouldApplyGrayscale = isGrayscale;
}

QImage ConvolutionalImageFilter::apply(const QImage& image)
{
	QImage result;
	if (_shouldApplyGrayscale == false)
	{
		result = QImage(image.size(), QImage::Format_RGB888);
	}
	else
	{
		result = QImage(image.size(), QImage::Format_Grayscale8);
	}

	for (int row = 0; row < image.height(); row++)
	{
		for (int column = 0; column < image.width(); column++)
		{
			result.setPixel(column, row, convolveImage(image, column, row));
		}
	}

	result.save("/Users/AustinSimpson/Desktop/convolution.bmp");

	return result;
}

QRgb ConvolutionalImageFilter::convolveImage
(
	const QImage& image,
	int pixelX,
	int pixelY
)
{
	QRgb result;

	int lowerColumnBound = (pixelX - ((_convolutionMatrix.columnCount() - 1) / 2));
	int upperColumnBound = (pixelX + ((_convolutionMatrix.columnCount() - 1) / 2));

	int lowerRowBound = (pixelY - (_convolutionMatrix.rowCount() - 1) / 2);
	int upperRowBound = (pixelY + (_convolutionMatrix.rowCount() - 1) / 2);

	Matrix redMatrix(_convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());
	Matrix greenMatrix(_convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());
	Matrix blueMatrix(_convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());

	for (int x = lowerColumnBound; x <= upperColumnBound; x++)
	{
		for (int y = lowerRowBound; y <= upperRowBound; y++)
		{
			int usedX = x;
			int usedY = y;
			if (usedX < 0) usedX = 0;
			if (usedX >= image.width()) usedX = image.width() - 1;
			if (usedY < 0) usedY = 0;
			if (usedY >= image.height()) usedY = image.height() - 1;

			QRgb currentPixel = image.pixel(usedX, usedY);

			int matrixRowIndex = y - lowerRowBound;
			int matrixColumnIndex = x - lowerColumnBound;

			redMatrix.setEntry(matrixRowIndex, matrixColumnIndex, (double)qRed(currentPixel));
			greenMatrix.setEntry(matrixRowIndex, matrixColumnIndex, (double)qGreen(currentPixel));
			blueMatrix.setEntry(matrixRowIndex, matrixColumnIndex, (double)qBlue(currentPixel));
		}
	}

	double redConvolution = fabs(_convolutionMatrix.convolve(redMatrix));
	double greenConvolution = fabs(_convolutionMatrix.convolve(greenMatrix));
	double blueConvolution = fabs(_convolutionMatrix.convolve(blueMatrix));

	result = qRgb((int)(redConvolution), (int)(greenConvolution), (int)(blueConvolution));
	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::horizontalSobel()
{
	ConvolutionalImageFilter result;
	result.setShouldApplyGrayscale(true);
	Matrix convolutionalMatrix(3, 3);
	convolutionalMatrix.setEntry(0, 0, 1);
	convolutionalMatrix.setEntry(0, 1, 2);
	convolutionalMatrix.setEntry(0, 2, 1);

	convolutionalMatrix.setEntry(2, 0, -1);
	convolutionalMatrix.setEntry(2, 1, -2);
	convolutionalMatrix.setEntry(2, 2, -1);
	result.setConvolutionMatrix(convolutionalMatrix);

	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::verticalSobel()
{
	ConvolutionalImageFilter result;
	result.setShouldApplyGrayscale(true);
	Matrix convolutionalMatrix(3, 3);
	convolutionalMatrix.setEntry(0, 0, -1);
	convolutionalMatrix.setEntry(1, 0, -2);
	convolutionalMatrix.setEntry(2, 0, -1);

	convolutionalMatrix.setEntry(0, 2, 1);
	convolutionalMatrix.setEntry(1, 2, 2);
	convolutionalMatrix.setEntry(2, 2, 1);
	result.setConvolutionMatrix(convolutionalMatrix);
	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::gaussianFilter(double sigma)
{
	ConvolutionalImageFilter result;
	int matDim = (int) ceil(6 * sigma);
	if (matDim % 2 == 0) matDim++;
	Matrix convolutionalMatrix(matDim, matDim);

	for (int i = 0; i < convolutionalMatrix.rowCount(); i++)
	{
		for (int j = 0; j < convolutionalMatrix.columnCount(); j++)
		{
			convolutionalMatrix.setEntry(i, j, gaussian(sigma, i, j));
		}
	}

	result.setConvolutionMatrix(convolutionalMatrix);
	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::edgeDetector()
{
	ConvolutionalImageFilter result;
	result.setShouldApplyGrayscale(true);

	Matrix convolutionalMatrix(3, 3);
	convolutionalMatrix.setEntry(0, 0, -1);
	convolutionalMatrix.setEntry(0, 1, -1);
	convolutionalMatrix.setEntry(0, 2, -1);

	convolutionalMatrix.setEntry(1, 0, -1);
	convolutionalMatrix.setEntry(1, 1, 8);
	convolutionalMatrix.setEntry(1, 2, -1);

	convolutionalMatrix.setEntry(2, 0, -1);
	convolutionalMatrix.setEntry(2, 1, -1);
	convolutionalMatrix.setEntry(2, 2, -1);
	result.setConvolutionMatrix(convolutionalMatrix);
	return result;
}

