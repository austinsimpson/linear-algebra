#include "MatrixTableViewModel.h"
#include "Matrix.h"

#include <QVariant>

MatrixTableViewModel::MatrixTableViewModel
(
	Matrix* matrix,
	QObject* parent
):
	QAbstractTableModel(parent)
{
	_matrix = matrix;
}

int MatrixTableViewModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED (parent);
	return _matrix->rowCount();
}

int MatrixTableViewModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED (parent);
	return _matrix->columnCount();
}

QVariant MatrixTableViewModel::data(const QModelIndex& index, int role) const
{
	QVariant result;
	switch (role)
	{
	case Qt::DisplayRole:
		result = _matrix->getEntry(index.row(), index.column());
		break;
	}

	return result;
}

void MatrixTableViewModel::setMatrix(Matrix* matrix)
{
	_matrix = matrix;
}

Matrix* MatrixTableViewModel::matrix() const
{
	return _matrix;
}
