#ifndef MATRIXREF_H
#define MATRIXREF_H


#include "Matrix.h"

class MatrixRef
{
public:
	MatrixRef(const Matrix& matrix);
	MatrixRef(const Matrix& matrix, int rowOffset, int columnOffset, int rowCount, int columnCount);
	MatrixRef(const MatrixRef& other);

	MatrixRef& operator=(const MatrixRef& other);

	double value(int row, int column) const;

	double convolve(const Matrix& other) const;
	double convolve(const MatrixRef& other) const;

	inline int rowCount() const
	{
		return _rowCount;
	}

	inline int columnCount() const
	{
		return _columnCount;
	}

private:
	const Matrix& _matrix;
	int _rowOffset;
	int _columnOffset;
	int _rowCount;
	int _columnCount;
};


#endif // MATRIXREF_H
