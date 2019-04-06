#ifndef COLORWHEELGRAPHWIDGET_H
#define COLORWHEELGRAPHWIDGET_H

#include <functional>

#include <QWidget>
#include <QList>

#include "ComplexNumber.h"

class IComplexFunction;
class ColorWheelGraphWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ColorWheelGraphWidget(QWidget* parent = nullptr);

	void paintEvent(QPaintEvent* paintEvent);

signals:

public slots:
	void setAlpha(int alpha);
	void setFunction(std::function<ComplexNumber(ComplexNumber)> function);
	void setRadiusAffectsBrightness(bool b);

private:
	ComplexNumber f(const ComplexNumber& number);
	std::function<ComplexNumber(ComplexNumber)> _function;

	void buildDomain(double left, double right, double top, double bottom);
	void buildFunctionImage();
	void buildGraphicalImage();

	QColor getColorForComplex(const ComplexNumber& number) const;

	QList<ComplexNumber> _domain;
	QList<ComplexNumber> _functionImage;
	QImage _graphicalImage;

	double _windowLeft;
	double _windowRight;
	double _windowTop;
	double _windowBottom;

	int _numberOfRealSamples;
	int _numberOfImaginarySamples;
	int _alpha;

	bool _radiusAffectsBrightness;
};

#endif // COLORWHEELGRAPHWIDGET_H
