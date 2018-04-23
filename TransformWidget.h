#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QWidget>
#include <QTimer>

class Matrix;
class TransformWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TransformWidget(QWidget *parent = nullptr);

	void setTransformationMatrix(Matrix* transformationMatrix);
	Matrix* transformationMatrix() const;

	void paintEvent(QPaintEvent* event);

signals:

public slots:

private slots:
	void UpdateTransformation();

private:
	Matrix* _transformationMatrix = nullptr;
	Matrix* _geometryMatrix = nullptr;

	QTimer _updateTimer;
};

#endif // TRANSFORMWIDGET_H
