#include "ConvolveImageLineTask.h"

#include "Algebra.h"
#include "MatrixRef.h"

ConvolveImageLineTask::ConvolveImageLineTask
(
	int line,
	const Matrix& convolutionMatrix,
	const Matrix& redMatrix,
	const Matrix& greenMatrix,
	const Matrix& blueMatrix,
	QImage& result
):
	_line(line),
	_convolutionMatrix(convolutionMatrix),
	_redMatrix(redMatrix),
	_greenMatrix(greenMatrix),
	_blueMatrix(blueMatrix),
	_result(result)
{

}

void ConvolveImageLineTask::run()
{	
	int halfWidth = ((_convolutionMatrix.columnCount() - 1) / 2);
	int halfHeight = ((_convolutionMatrix.rowCount() - 1) / 2);

	int pixelY = _line;
	for (int pixelX = 0; pixelX < _redMatrix.columnCount(); pixelX++)
	{
		int lowerColumnBound = pixelX - halfWidth;
		int lowerRowBound = pixelY - halfHeight;

		MatrixRef redSubmatrix(_redMatrix, lowerRowBound, lowerColumnBound, _convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());
		MatrixRef greenSubmatrix(_greenMatrix, lowerRowBound, lowerColumnBound, _convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());
		MatrixRef blueSubmatrix(_blueMatrix, lowerRowBound, lowerColumnBound, _convolutionMatrix.rowCount(), _convolutionMatrix.columnCount());

		double redConvolution = fabs(redSubmatrix.convolve(_convolutionMatrix));
		double greenConvolution = fabs(greenSubmatrix.convolve(_convolutionMatrix));
		double blueConvolution = fabs(blueSubmatrix.convolve(_convolutionMatrix));

		clamp<double>(redConvolution, 0.0, 255.0);
		clamp<double>(greenConvolution, 0.0, 255.0);
		clamp<double>(blueConvolution, 0.0, 255.0);

		_result.setPixel(pixelX, pixelY, qRgb((int)(redConvolution), (int)(greenConvolution), (int)(blueConvolution)));
	}
}
