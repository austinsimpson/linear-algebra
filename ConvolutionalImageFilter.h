#ifndef CONVOLUTIONALIMAGEFILTER_H
#define CONVOLUTIONALIMAGEFILTER_H

#include "Matrix.h"

#include <QImage>

class ConvolutionalImageFilter
{
public:
	ConvolutionalImageFilter();

	QImage apply(const QImage& image);

	Matrix& convolutionMatrix();
	bool setConvolutionMatrix(const Matrix& matrix);

	void setShouldApplyGrayscale(bool isGrayscale);
	bool shouldApplyGrayscale();

	static ConvolutionalImageFilter horizontalSobel();
	static ConvolutionalImageFilter verticalSobel();
	static ConvolutionalImageFilter edgeDetector();

	static ConvolutionalImageFilter gaussianFilter(double sigma);

private:
	QRgb convolveImage(const QImage& image, int pixelX, int pixelY);

	Matrix _convolutionMatrix;
	bool _shouldApplyGrayscale;
};

#endif // CONVOLUTIONALIMAGEFILTER_H
