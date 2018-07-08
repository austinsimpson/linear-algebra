#ifndef MATRIX_H
#define MATRIX_H

#include <QObject>
#include <QSharedPointer>

class MatrixData : public QSharedData
{
public:
	MatrixData()
	{
		_rowCount = 0;
		_columnCount = 0;
	}

	MatrixData(int rowCount, int columnCount)
	{
		int elementCount = rowCount * columnCount;
		_matrixValues = new double[elementCount];
		memset(_matrixValues, 0, sizeof(double) * elementCount);
	}

	~MatrixData()
	{
		if (_matrixValues != nullptr)
		{
			delete[] _matrixValues;
		}
	}

private:
	double* _matrixValues;
	int _rowCount;
	int _columnCount;
};

class Matrix
{
public:
	Matrix();
	Matrix(int rowCount, int columnCount);
	Matrix(const Matrix& other);

	~Matrix();

	Matrix& operator= (Matrix other);
	bool operator == (const Matrix& other);
	bool operator != (const Matrix& other);

	Matrix operator+(const Matrix& other);
	Matrix operator-(const Matrix& other);
	Matrix operator*(const Matrix& other);

    double getEntry(int row, int column) const;
    void setEntry(int row, int column, double value);

    int rowCount() const;
    int columnCount() const;

	bool isReducedRowEchelonForm() const;

	Matrix multiply(const Matrix& other);
	void scale(double factor);

	double convolve(const Matrix& other);

	Matrix transpose();

	static Matrix Identity(int size);

	Matrix rowEchelonForm() const;
	Matrix reducedRowEchelonForm();

    QString toString();

signals:

public slots:

private:
	void scaleRow(int row, double factor);
	void swapRows(int first, int second);
	void pivot(int pivotRow, int targetRow, double scalar);

    int _rowCount;
    int _columnCount;

	QSharedPointer<double> _values;
};

#endif // MATRIX_H
