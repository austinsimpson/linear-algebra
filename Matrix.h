#ifndef MATRIX_H
#define MATRIX_H

#include <QObject>

class Matrix : public QObject
{
    Q_OBJECT
public:
    explicit Matrix(QObject *parent = nullptr, int rowCount = 3, int columnCount = 3);
	explicit Matrix(int rowCount, int columnCount);
	Matrix* clone();
	~Matrix();

    double getEntry(int row, int column) const;
    void setEntry(int row, int column, double value);

    int rowCount() const;
    int columnCount() const;

	bool isRref() const;

	Matrix* multiply(const Matrix& other);
	void scale(double factor);

	Matrix* transpose();

    static Matrix* Identity(int size);
    static Matrix* Zeroes(int rowCount, int columnCount);

	Matrix* ref();
	Matrix* rref();

    QString toString();

signals:

public slots:

private:
	void scaleRow(int row, double factor);
	void swapRows(int first, int second);
	void pivot(int pivotRow, int targetRow, double scalar);

    int _rowCount;
    int _columnCount;

    double* _values = nullptr;
};

#endif // MATRIX_H
