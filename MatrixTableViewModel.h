#ifndef MATRIXTABLEVIEWMODEL_H
#define MATRIXTABLEVIEWMODEL_H


#include <QAbstractTableModel>

#include "Matrix.h"

class MatrixTableViewModel : public QAbstractTableModel
{
public:
	explicit MatrixTableViewModel(const Matrix& matrix, QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent) const;
	int columnCount(const QModelIndex& parent) const;
	QVariant data(const QModelIndex& index, int role) const;

	void setMatrix(const Matrix& matrix);
	const Matrix& matrix() const;

private:
	Matrix _matrix;
};

#endif // MATRIXTABLEVIEWMODEL_H
