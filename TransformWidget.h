#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QWidget>
#include <QTimer>

#include "Matrix.h"

class Matrix;
class TransformWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TransformWidget(QWidget *parent = nullptr);
	~TransformWidget();

	void setTransformationMatrix(const Matrix& transformationMatrix);
	Matrix& transformationMatrix();

	void paintEvent(QPaintEvent* event);

signals:

public slots:

private slots:
	void UpdateTransformation();

private:
	Matrix _transformationMatrix;
	Matrix _geometryMatrix;

	QTimer _updateTimer;
};

#endif // TRANSFORMWIDGET_H
