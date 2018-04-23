#include "Matrix.h"

#include <QStringBuilder>
#include <math.h>

Matrix::Matrix
(
    QObject* parent,
    int rowCount,
    int columnCount
):
    QObject(parent)
{
    _rowCount = rowCount;
    _columnCount = columnCount;

    _values = new double[rowCount * columnCount];
}

Matrix::Matrix
(
	int rowCount,
	int columnCount
):
	QObject(nullptr)
{
	_rowCount = rowCount;
	_columnCount = columnCount;

	_values = new double[rowCount * columnCount];
}

Matrix::~Matrix()
{
	delete _values;
}

Matrix* Matrix::clone()
{
	Matrix* result = new Matrix(rowCount(), columnCount());
	for (int i = 0; i < rowCount(); i++)
	{
		for (int j = 0; j < columnCount(); j++)
		{
			result->setEntry(i, j, getEntry(i, j));
		}
	}
	return result;
}

double Matrix::getEntry(int row, int column) const
{
	double result = nan("");
	if (row < rowCount() && column < columnCount())
	{
		result = _values[row * _columnCount + column];
	}
	return result;
}

void Matrix::setEntry(int row, int column, double value)
{
	if (row < rowCount() && column < columnCount())
	{
		_values[row * _columnCount + column] = value;
	}
}

int Matrix::rowCount() const
{
    return _rowCount;
}

int Matrix::columnCount() const
{
    return _columnCount;
}

bool Matrix::isRref() const
{
	return false;
}

Matrix* Matrix::multiply(const Matrix& otherMatrix)
{
    Matrix* result = nullptr;
    if (columnCount() == otherMatrix.rowCount())
    {
		result = new Matrix(rowCount(), otherMatrix.columnCount());
        for (int row = 0; row < rowCount(); row++)
        {
            for (int column = 0; column < otherMatrix.columnCount(); column++)
            {
                double sum = 0.0;
                for (int r = 0; r < columnCount(); r++)
                {
                    sum += (getEntry(row, r) * otherMatrix.getEntry(r, column));
                }
                result->setEntry(row, column, sum);
            }
        }
    }
    return result;
}

void Matrix::scale(double scalar)
{
	for (int i = 0; i < rowCount() * columnCount(); i++)
	{
		_values[i] *= scalar;
	}
}

Matrix* Matrix::transpose()
{
	Matrix* result = new Matrix(nullptr, columnCount(), rowCount());
	for (int i = 0; i < rowCount(); i++)
	{
		for (int j = 0; j < columnCount(); j++)
		{
			result->setEntry(j, i, getEntry(i, j));
		}
	}
	return result;
}

Matrix* Matrix::Identity(int size)
{
    Matrix* matrix = Matrix::Zeroes(size, size);
    for (int i = 0; i < size; i++)
    {
        matrix->setEntry(i, i, 1);
    }
    return matrix;
}

Matrix* Matrix::Zeroes(int rowCount, int columnCount)
{
    Matrix* matrix = new Matrix(nullptr, rowCount, columnCount);
    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < columnCount; j++)
        {
            matrix->setEntry(i, j, 0);
        }
    }
    return matrix;
}

Matrix* Matrix::ref()
{
	Matrix* result = clone();
	int pivotI = 0;
	int pivotJ = 0;
	while (pivotI < result->rowCount() && pivotJ < result->columnCount())
	{
		//First thing we do is find the pivot row of the matrix.
		int rowIndex = pivotI;
		while (result->getEntry(rowIndex, pivotJ) == 0 && rowIndex < result->rowCount())
		{
			rowIndex++;
		}

		if (rowIndex < result->rowCount())
		{
			result->swapRows(pivotI, rowIndex);
			result->scaleRow(pivotI, (1.0 / result->getEntry(pivotI, pivotJ)));

			for (rowIndex = pivotI + 1; rowIndex < result->rowCount(); rowIndex++)
			{
				if (result->getEntry(rowIndex, pivotJ) != 0)
				{
					result->pivot(pivotI, rowIndex, -(result->getEntry(rowIndex, pivotJ)));
				}
			}

			pivotI++;
		}

		pivotJ++;
	}
	return result;
}

Matrix* Matrix::rref()
{
	Matrix* result = ref(); //We use ref as a starting point because rref is basically just more ref action
	return result;
}

QString Matrix::toString()
{
	QString result = QString();
	for (int i = 0; i < rowCount(); i++)
    {
		for (int j = 0; j < columnCount(); j++)
		{
			result = result % QString::number(getEntry(i, j)) % ", ";
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
	if (row >= 0 && row < _rowCount)
	{
		for (int i = 0; i < _columnCount; i++)
		{
			setEntry(row, i, getEntry(row, i) * factor);
		}
	}
}

void Matrix::swapRows
(
	int first,
	int second
)
{
	if (first >= 0 && first < _rowCount && second >= 0 && second < _rowCount && first != second)
	{
		for (int i = 0; i < _rowCount; i++)
		{
			setEntry(first, i, getEntry(first, i) + getEntry(second, i));
			setEntry(second, i, getEntry(first, i) - getEntry(second, i));
			setEntry(first, i, getEntry(first, i) - getEntry(second, i));
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
	if (pivotRow >= 0 && pivotRow < _rowCount && targetRow >= 0 && targetRow < _rowCount)
	{
		for (int i = 0; i < _rowCount; i++)
		{
			setEntry(targetRow, i, getEntry(pivotRow, i) * scalar + getEntry(targetRow, i));
		}
	}
}
