#include "ConvolutionalImageFilter.h"

#include "Algebra.h"

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
		result = QImage(image.size(), image.format());
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
			if (0 <= x && x < image.width() && 0 <= y && y < image.height())
			{
				QRgb currentPixel = image.pixel(x, y);

				int matrixRowIndex = y - lowerRowBound;
				int matrixColumnIndex = x - lowerColumnBound;

				redMatrix.setEntry(matrixRowIndex, matrixColumnIndex, qRed(currentPixel));
				greenMatrix.setEntry(matrixRowIndex, matrixColumnIndex, qGreen(currentPixel));
				blueMatrix.setEntry(matrixRowIndex, matrixColumnIndex, qBlue(currentPixel));
			}
		}
	}

	result = qRgb((quint8)_convolutionMatrix.convolve(redMatrix), (quint8)_convolutionMatrix.convolve(greenMatrix), (quint8)_convolutionMatrix.convolve(blueMatrix));

	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::horizontalSobel()
{
	ConvolutionalImageFilter result;
	result.setShouldApplyGrayscale(true);
	Matrix convolutionalMatrix(3, 3);
	convolutionalMatrix.setEntry(0, 0, -1);
	convolutionalMatrix.setEntry(0, 1, -2);
	convolutionalMatrix.setEntry(0, 2, -1);

	convolutionalMatrix.setEntry(2, 0, 1);
	convolutionalMatrix.setEntry(2, 1, 2);
	convolutionalMatrix.setEntry(2, 2, 1);

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

ConvolutionalImageFilter ConvolutionalImageFilter::gaussian()
{
	ConvolutionalImageFilter result;
	Matrix convolutionalMatrix(5, 5);
	convolutionalMatrix.setEntry(0, 0, 1.0);
	convolutionalMatrix.setEntry(0, 1, 4.0);
	convolutionalMatrix.setEntry(0, 2, 6.0);
	convolutionalMatrix.setEntry(0, 3, 4.0);
	convolutionalMatrix.setEntry(0, 4, 1.0);

	convolutionalMatrix.setEntry(1, 0, 4.0);
	convolutionalMatrix.setEntry(1, 1, 16.0);
	convolutionalMatrix.setEntry(1, 2, 24.0);
	convolutionalMatrix.setEntry(1, 3, 16.0);
	convolutionalMatrix.setEntry(1, 4, 4.0);

	convolutionalMatrix.setEntry(2, 0, 6.0);
	convolutionalMatrix.setEntry(2, 1, 24.0);
	convolutionalMatrix.setEntry(2, 2, 36.0);
	convolutionalMatrix.setEntry(2, 3, 24.0);
	convolutionalMatrix.setEntry(2, 4, 6.0);

	convolutionalMatrix.setEntry(3, 0, 4.0);
	convolutionalMatrix.setEntry(3, 1, 16.0);
	convolutionalMatrix.setEntry(3, 2, 24.0);
	convolutionalMatrix.setEntry(3, 3, 16.0);
	convolutionalMatrix.setEntry(3, 4, 4.0);

	convolutionalMatrix.setEntry(4, 0, 1.0);
	convolutionalMatrix.setEntry(4, 1, 4.0);
	convolutionalMatrix.setEntry(4, 2, 6.0);
	convolutionalMatrix.setEntry(4, 3, 4.0);
	convolutionalMatrix.setEntry(4, 4, 1.0);
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

