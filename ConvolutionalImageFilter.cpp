#include "ConvolutionalImageFilter.h"

#include "Algebra.h"

#include <QDebug>
#include <math.h>

#include "MatrixRef.h"
#include "ConvolveImageLineTask.h"

#include <QThreadPool>

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
				norm += _convolutionMatrix.value(i, j);
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
		result = QImage(image.size(), QImage::Format_RGB32);
	}
	else
	{
		result = QImage(image.size(), QImage::Format_Grayscale8);
	}

	Matrix redMatrix(image.height(), image.width());
	Matrix greenMatrix(image.height(), image.width());
	Matrix blueMatrix(image.height(), image.width());

	const QRgb* imageData = (QRgb*) image.constBits();
	int imageWidth = image.width();
	int imageHeight = image.height();
	for (int i = 0; i < imageWidth; i++)
	{
		for (int j = 0; j < imageHeight; j++)
		{
			redMatrix.setValue(j, i, qRed(imageData[i + (j * imageWidth)]));
			greenMatrix.setValue(j, i, qGreen(imageData[i + (j * imageWidth)]));
			blueMatrix.setValue(j, i, qBlue(imageData[i + (j * imageWidth)]));
		}
	}

	for (int line = 0; line < redMatrix.rowCount(); line++)
	{
		QThreadPool::globalInstance()->start(new ConvolveImageLineTask(line, _convolutionMatrix, redMatrix, greenMatrix, blueMatrix, result));
	}

	QThreadPool::globalInstance()->waitForDone();

	result.save("/Users/AustinSimpson/Desktop/convolution.bmp");

	return result;
}

QRgb ConvolutionalImageFilter::convolveImagePixel
(
	const Matrix& redMatrix,
	const Matrix& greenMatrix,
	const Matrix& blueMatrix,
	int pixelX,
	int pixelY
)
{
	QRgb result;

	int halfWidth = ((_convolutionMatrix.columnCount() - 1) / 2);
	int halfHeight = ((_convolutionMatrix.rowCount() - 1) / 2);

	int lowerColumnBound = pixelX - halfWidth;
	int lowerRowBound = pixelY - halfHeight;

	MatrixRef redSubmatrix(redMatrix, lowerRowBound, lowerColumnBound, _convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());
	MatrixRef greenSubmatrix(greenMatrix, lowerRowBound, lowerColumnBound, _convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());
	MatrixRef blueSubmatrix(blueMatrix, lowerRowBound, lowerColumnBound, _convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());

	double redConvolution = fabs(redSubmatrix.convolve(_convolutionMatrix));
	double greenConvolution = fabs(greenSubmatrix.convolve(_convolutionMatrix));
	double blueConvolution = fabs(blueSubmatrix.convolve(_convolutionMatrix));

	clamp<double>(redConvolution, 0.0, 255.0);
	clamp<double>(greenConvolution, 0.0, 255.0);
	clamp<double>(blueConvolution, 0.0, 255.0);

	result = qRgb((int)(redConvolution), (int)(greenConvolution), (int)(blueConvolution));
	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::horizontalSobel()
{
	ConvolutionalImageFilter result;
	result.setShouldApplyGrayscale(true);
	Matrix convolutionalMatrix(3, 3);
	convolutionalMatrix.setValue(0, 0, 1);
	convolutionalMatrix.setValue(0, 1, 2);
	convolutionalMatrix.setValue(0, 2, 1);

	convolutionalMatrix.setValue(2, 0, -1);
	convolutionalMatrix.setValue(2, 1, -2);
	convolutionalMatrix.setValue(2, 2, -1);
	result.setConvolutionMatrix(convolutionalMatrix);

	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::verticalSobel()
{
	ConvolutionalImageFilter result;
	result.setShouldApplyGrayscale(true);
	Matrix convolutionalMatrix(3, 3);
	convolutionalMatrix.setValue(0, 0, -1);
	convolutionalMatrix.setValue(1, 0, -2);
	convolutionalMatrix.setValue(2, 0, -1);

	convolutionalMatrix.setValue(0, 2, 1);
	convolutionalMatrix.setValue(1, 2, 2);
	convolutionalMatrix.setValue(2, 2, 1);
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
			convolutionalMatrix.setValue(i, j, gaussian(sigma, i, j));
		}
	}

	result.setConvolutionMatrix(convolutionalMatrix);
	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::boxBlur()
{
	ConvolutionalImageFilter result;

	Matrix convolutionalMatrix(3,3);
	convolutionalMatrix.setValue(0, 0, 1.0);
	convolutionalMatrix.setValue(0, 1, 1.0);
	convolutionalMatrix.setValue(0, 2, 1.0);
	convolutionalMatrix.setValue(1, 0, 1.0);
	convolutionalMatrix.setValue(1, 1, 1.0);
	convolutionalMatrix.setValue(1, 2, 1.0);
	convolutionalMatrix.setValue(2, 0, 1.0);
	convolutionalMatrix.setValue(2, 1, 1.0);
	convolutionalMatrix.setValue(2, 2, 1.0);

	result.setConvolutionMatrix(convolutionalMatrix);
	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::edgeDetector()
{
	ConvolutionalImageFilter result;
	result.setShouldApplyGrayscale(true);

	Matrix convolutionalMatrix(3, 3);
	convolutionalMatrix.setValue(0, 0, -1);
	convolutionalMatrix.setValue(0, 1, -1);
	convolutionalMatrix.setValue(0, 2, -1);

	convolutionalMatrix.setValue(1, 0, -1);
	convolutionalMatrix.setValue(1, 1, 8);
	convolutionalMatrix.setValue(1, 2, -1);

	convolutionalMatrix.setValue(2, 0, -1);
	convolutionalMatrix.setValue(2, 1, -1);
	convolutionalMatrix.setValue(2, 2, -1);
	result.setConvolutionMatrix(convolutionalMatrix);
	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::sharpen()
{
	ConvolutionalImageFilter result;
	Matrix convolutionalMatrx(3,3);

	convolutionalMatrx.setValue(0, 1, -1);

	convolutionalMatrx.setValue(1, 0, -1);
	convolutionalMatrx.setValue(1, 1, 5);
	convolutionalMatrx.setValue(1, 2, -1);

	convolutionalMatrx.setValue(2, 1, -1);

	result.setConvolutionMatrix(convolutionalMatrx);
	return result;
}

ConvolutionalImageFilter ConvolutionalImageFilter::identity()
{
	return ConvolutionalImageFilter();
}

