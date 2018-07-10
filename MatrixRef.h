#ifndef MATRIXREF_H
#define MATRIXREF_H


#include "Matrix.h"

class MatrixRef
{
public:
	MatrixRef(Matrix& matrix);
	MatrixRef(const MatrixRef& other);

	MatrixRef& operator=(const MatrixRef& other);


private:
	Matrix& _matrix;
};

#endif // MATRIXREF_H
