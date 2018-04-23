#ifndef MATRIXTABLEVIEWMODEL_H
#define MATRIXTABLEVIEWMODEL_H


#include <QAbstractTableModel>

class Matrix;
class MatrixTableViewModel : public QAbstractTableModel
{
public:
	explicit MatrixTableViewModel(Matrix* matrix, QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent) const;
	int columnCount(const QModelIndex& parent) const;
	QVariant data(const QModelIndex& index, int role) const;

	void setMatrix(Matrix* matrix);
	Matrix* matrix() const;

private:
	Matrix* _matrix = nullptr;
};

#endif // MATRIXTABLEVIEWMODEL_H
