#include "TransformWidget.h"
#include "Matrix.h"

#include <QPainter>

#include <math.h>

TransformWidget::TransformWidget
(
	QWidget* parent
):
	QWidget(parent),
	_transformationMatrix(2, 3),
	_geometryMatrix(3, 3)

{
	_transformationMatrix.setEntry(0, 0, 2);
	_transformationMatrix.setEntry(1, 1, 10);
	_transformationMatrix.setEntry(0, 2, 100);
	_transformationMatrix.setEntry(1, 2, 40);

	_geometryMatrix.setEntry(1, 1, 10);
	_geometryMatrix.setEntry(0, 2, 10);
	_geometryMatrix.setEntry(1, 2, 5);

	for (int i = 0 ; i < 3; i++)
	{
		_geometryMatrix.setEntry(2, i, 1);
	}

	_updateTimer.setInterval(1000/60);
	connect(&_updateTimer, &QTimer::timeout, this, &TransformWidget::UpdateTransformation);
	//_updateTimer.start();
}

TransformWidget::~TransformWidget()
{
	_updateTimer.stop();
}

void TransformWidget::paintEvent
(
	QPaintEvent* event
)
{
	Q_UNUSED (event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillRect(geometry().translated(-geometry().x(), -geometry().y()), QColor("Cornflower Blue"));

	QPen pen;
	pen.setColor(QColor("Black"));
	pen.setCapStyle(Qt::RoundCap);
	pen.setWidth(5);
	painter.setPen(pen);

	Matrix transformedGeometry = _transformationMatrix.multiply(_geometryMatrix);
	for (int i = 0; i < transformedGeometry.columnCount(); i++)
	{
		painter.drawPoint(transformedGeometry.getEntry(0, i), transformedGeometry.getEntry(1, i));
	}

	pen.setColor(QColor("Blue"));
	painter.setPen(pen);

	for (int i = 0; i < _geometryMatrix.columnCount(); i++)
	{
		painter.drawPoint(_geometryMatrix.getEntry(0, i), _geometryMatrix.getEntry(1, i));
	}
}

void TransformWidget::setTransformationMatrix
(
	const Matrix& transformationMatrix
)
{
	_transformationMatrix = transformationMatrix;
}

Matrix& TransformWidget::transformationMatrix()
{
	return _transformationMatrix;
}

void TransformWidget::UpdateTransformation()
{
	static double theta = 0.0;
	static double dTheta = 0.1;
	double sinTheta = sin(theta); //Precompute trig values cause it's expensive
	double cosTheta = cos(theta); //Precompute trig values cause it's expensive

	_transformationMatrix.setEntry(0, 0, cosTheta);
	_transformationMatrix.setEntry(1, 0, -sinTheta);
	_transformationMatrix.setEntry(0, 1, sinTheta);
	_transformationMatrix.setEntry(1, 1, cosTheta);

	static double xVelocity = 1.4 * cosTheta;
	static double yVelocity = 1.4 * sinTheta;

	double translationX = _transformationMatrix.getEntry(0, 2);
	double translationY = _transformationMatrix.getEntry(1, 2);

	if (10 > translationX || translationX > 500)
	{
		_transformationMatrix.setEntry(0, 0, (-1 * _transformationMatrix.getEntry(0, 0)));
		xVelocity *= -1;
	}

	_transformationMatrix.setEntry(0, 2, translationX + xVelocity);
	_transformationMatrix.setEntry(1, 2, translationY + yVelocity);
	theta += dTheta;
	update();
}
