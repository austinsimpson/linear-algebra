#include "Matrix.h"

#include <QStringBuilder>
#include <math.h>

#ifdef USE_CUDA
#include "CudaMatrixOperations.h"
#endif

Matrix::Matrix()
{
	_matrixData = new MatrixData();
}

Matrix::Matrix
(
	int rowCount,
    int columnCount
)
{
	_matrixData = nullptr;

	if (rowCount > 0 && columnCount > 0)
	{
		_matrixData = new MatrixData(rowCount, columnCount);
	}
}

Matrix::Matrix(const Matrix& other)
{
	_matrixData = other._matrixData;
}

Matrix::Matrix
(
	int rowCount,
	int columnCount,
	double* data
)
{
	_matrixData = new MatrixData(rowCount, columnCount, data);
}

Matrix::Matrix
(
	int rowCount, 
	int columnCount, 
	std::function<double(int, int)> function
): 
	Matrix(rowCount, columnCount)
{
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < columnCount; j++)
		{
			setValue(i, j, function(i, j));
		}
	}
}


Matrix::~Matrix()
{
}

Matrix& Matrix::operator=
(
	const Matrix& other
)
{
	_matrixData = other._matrixData;

	return *this;
}

bool Matrix::operator ==
(
	const Matrix& other
)
{
	bool result = this->rowCount() == other.rowCount() && this->columnCount() == other.columnCount();

	if (result)
	{
		for (int i = 0; i < this->rowCount() && result; i++)
		{
			for (int j = 0; j < this->columnCount() && result; j++)
			{
				result = value(i, j) == other.value(i, j);
			}
		}
	}
	return result;
}

bool Matrix::operator !=(const Matrix& other)
{
	return !(*this == other);
}
 
Matrix Matrix::operator+(const Matrix& other)
{
	Matrix result;
	if (this->rowCount() == other.rowCount() && this->columnCount() == other.columnCount())
	{
#ifdef USE_CUDA
		CudaMatrix firstMatrix = matrixToCudaMatrix(*this);
		CudaMatrix secondMatrix = matrixToCudaMatrix(other);
		CudaMatrix cudaResult = addMatricesWithCuda(firstMatrix, secondMatrix);
		result = Matrix(cudaResult.rowCount, cudaResult.columnCount, cudaResult.data);
#else
		result = Matrix(rowCount(), columnCount(), [this, other](int row, int column) { return value(row, column) + other.value(row, column); });
#endif
	}

	return result;
}

Matrix Matrix::operator-(const Matrix& other)
{
	Matrix result;
	if (this->rowCount() == other.rowCount() && this->columnCount() == other.columnCount())
	{
#ifdef USE_CUDA
		CudaMatrix firstMatrix = matrixToCudaMatrix(*this);
		CudaMatrix secondMatrix = matrixToCudaMatrix(other);
		CudaMatrix cudaResult = subtractMatricesWithCuda(firstMatrix, secondMatrix);
		result = Matrix(cudaResult.rowCount, cudaResult.columnCount, cudaResult.data);
#else
		result = Matrix(rowCount(), columnCount(), [this, other](int row, int column) { return value(row, column) - other.value(row, column); });
#endif 
	}
	return result; 
}

Matrix Matrix::operator*(const Matrix& other)
{
	return multiply(other);
}

double Matrix::value(int row, int column) const
{
	return _matrixData->value(row, column);
}

void Matrix::setValue(int row, int column, double value)
{
	_matrixData->setValue(row, column, value);
}

bool Matrix::isReducedRowEchelonForm() const
{
	return false;
}

Matrix Matrix::multiply(const Matrix& otherMatrix)
{
	Matrix result = Matrix();
    if (columnCount() == otherMatrix.rowCount())
    {
#ifdef USE_CUDA
		CudaMatrix first = matrixToCudaMatrix(*this);
		CudaMatrix second = matrixToCudaMatrix(otherMatrix);
		CudaMatrix cudaResult = multiplyMatricesWithCuda(first, second);
		result = Matrix(cudaResult.rowCount, cudaResult.columnCount, cudaResult.data);
#else
		result = Matrix(rowCount(), otherMatrix.columnCount());
        for (int row = 0; row < rowCount(); row++)
        {
            for (int column = 0; column < otherMatrix.columnCount(); column++)
            {
                double sum = 0.0;
                for (int r = 0; r < columnCount(); r++)
                {
					sum += (value(row, r) * otherMatrix.value(r, column));
                }
				result.setValue(row, column, sum);
            }
        }
#endif
    }
    return result;
}

void Matrix::scale(double scalar)
{
	for (int i = 0; i < _matrixData->rowCount(); i++)
	{
		for (int j = 0; j < _matrixData->columnCount(); j++)
		{
			_matrixData->setValue(i, j, _matrixData->value(i, j) * scalar);
		}
	}
}

Matrix Matrix::transpose()
{
	Matrix result = Matrix(columnCount(), rowCount());
	for (int i = 0; i < rowCount(); i++)
	{
		for (int j = 0; j < columnCount(); j++)
		{
			result.setValue(j, i, value(i, j));
		}
	}
	return result;
}

Matrix Matrix::Identity(int size)
{
	Matrix matrix(size, size);
    for (int i = 0; i < size; i++)
    {
		matrix.setValue(i, i, 1);
    }
    return matrix;
}

Matrix Matrix::rowEchelonForm() const
{
	Matrix result = Matrix(*this);
	int pivotI = 0;
	int pivotJ = 0;

	while (pivotI < result.rowCount() && pivotJ < result.columnCount())
	{
		//First thing we do is find the pivot row of the matrix.
		int rowIndex = pivotI;
		while (result.value(rowIndex, pivotJ) == 0 && rowIndex < result.rowCount())
		{
			rowIndex++;
		}

		if (rowIndex < result.rowCount())
		{
			result.swapRows(pivotI, rowIndex);
			result.scaleRow(pivotI, (1.0 / result.value(pivotI, pivotJ)));

			for (rowIndex = pivotI + 1; rowIndex < result.rowCount(); rowIndex++)
			{
				if (result.value(rowIndex, pivotJ) != 0)
				{
					result.pivot(pivotI, rowIndex, -(result.value(rowIndex, pivotJ)));
				}
			}

			pivotI++;
		}

		pivotJ++;
	}
	return result;
}

Matrix Matrix::reducedRowEchelonForm()
{
	Matrix result = rowEchelonForm(); //We use ref as a starting point because rref is basically just more ref action
	return result;
}

QString Matrix::toString()
{
	QString result = QString();
	for (int i = 0; i < rowCount(); i++)
    {
		for (int j = 0; j < columnCount(); j++)
		{
			result = result % QString::number(value(i, j)) % ", ";
		}
		result = result % '\n';
    }
	return result;
}

void Matrix::scaleRow
(
	int row,
	double factor
)
{
	if (row >= 0 && row < rowCount())
	{
		for (int i = 0; i < columnCount(); i++)
		{
			setValue(row, i, value(row, i) * factor);
		}
	}
}

double Matrix::convolve(const Matrix& other) const
{
	double result = 0;

	for (int i = 0; i < rowCount(); i++)
	{
		for (int j = 0; j < columnCount(); j++)
		{
			result += this->value(i, j) * other.value(other.rowCount() - i - 1, other.columnCount() - j - 1);
		}
	}

	return result;
}


void Matrix::swapRows
(
	int first,
	int second
)
{
	if (first >= 0 && first < rowCount() && second >= 0 && second < rowCount() && first != second)
	{
		for (int i = 0; i < rowCount(); i++)
		{
			setValue(first, i, value(first, i) + value(second, i));
			setValue(second, i, value(first, i) - value(second, i));
			setValue(first, i, value(first, i) - value(second, i));
		}
	}
}

void Matrix::pivot
(
	int pivotRow,
	int targetRow,
	double scalar
)
{
	if (pivotRow >= 0 && pivotRow < rowCount() && targetRow >= 0 && targetRow < rowCount())
	{
		for (int i = 0; i < rowCount(); i++)
		{
			setValue(targetRow, i, value(pivotRow, i) * scalar + value(targetRow, i));
		}
	}
}


#ifdef USE_CUDA
CudaMatrix matrixToCudaMatrix(const Matrix& matrix) 
{
	CudaMatrix result;
	result.data = const_cast<double*>(matrix.getData());
	result.rowCount = matrix.rowCount();
	result.columnCount = matrix.columnCount();
	return result;
}
#endif