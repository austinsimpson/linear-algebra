#include "MatrixRef.h"

MatrixRef::MatrixRef
(
	Matrix& matrix
):
	_matrix(matrix)
{
}

MatrixRef::MatrixRef
(
	const MatrixRef& other
):
	_matrix(other._matrix)
{

}
