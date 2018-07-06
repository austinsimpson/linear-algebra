#ifndef MATRIX_H
#define MATRIX_H

#include <QObject>
#include <QSharedPointer>

class Matrix
{
public:
	Matrix();
	Matrix(int rowCount, int columnCount);
	Matrix(const Matrix& other);

	~Matrix();

	Matrix& operator= (Matrix other);

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

	Matrix rowEchelonForm();
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
