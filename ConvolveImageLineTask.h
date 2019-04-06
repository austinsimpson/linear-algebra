#ifndef CONVOLVEIMAGELINETASK_H
#define CONVOLVEIMAGELINETASK_H

#include <QImage>
#include <QRunnable>
#include <QRgb>

#include "Matrix.h"

class ConvolveImageLineTask : public QRunnable
{
public:
	ConvolveImageLineTask(int line, const Matrix& convolutionMatrix, const Matrix& redMatrix, const Matrix& greenMatrix, const Matrix& blueMatrix, QImage& result);

	void run();
private:
	int _line;
	const Matrix& _convolutionMatrix;
	const Matrix& _redMatrix;
	const Matrix& _greenMatrix;
	const Matrix& _blueMatrix;
	QImage& _result;

};

#endif // CONVOLVEIMAGELINETASK_H
