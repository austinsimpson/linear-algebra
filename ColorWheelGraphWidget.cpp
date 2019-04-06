#include "ColorWheelGraphWidget.h"

#include "ElementaryComplexFunctions.h"

#include <QColor>
#include <QPainter>
#include <QDebug>

ColorWheelGraphWidget::ColorWheelGraphWidget
(
	QWidget* parent
):
	QWidget(parent),
	_alpha(30),
	_radiusAffectsBrightness(true),
	_numberOfRealSamples(500),
	_numberOfImaginarySamples(500),
	_windowLeft(-M_PI),
	_windowRight(M_PI),
	_windowBottom(-M_PI),
	_windowTop(M_PI)
{
	setFunction([](const ComplexNumber& z) { return z; }); //We use the identity function by default
}

void ColorWheelGraphWidget::paintEvent
(
	QPaintEvent* paintEvent
)
{
	QPainter painter;
	painter.begin(this);
	if (_graphicalImage.isNull() == false)
		painter.drawImage(geometry().translated(-x(), -y()), _graphicalImage);

	painter.end();
}

void ColorWheelGraphWidget::buildDomain(double left, double right, double bottom, double top)
{
	_domain.clear();

	double deltaReal = (right - left) / _numberOfRealSamples;
	double deltaImaginary = (top - bottom) / _numberOfImaginarySamples;
	for (int i = 0; i < _numberOfImaginarySamples; i++)
	{
		for (int j = 0; j < _numberOfRealSamples; j++)
		{
			_domain.append(ComplexNumber(left + deltaReal*(j), top - deltaImaginary*(i))); //We go top down because we want to properly generate the image
		}
	}
}

void ColorWheelGraphWidget::buildFunctionImage()
{
	_functionImage.clear();
	if (_domain.count() > 0 && _numberOfRealSamples * _numberOfImaginarySamples == _domain.count())
	{
		for (int i = 0; i < _numberOfImaginarySamples; i++)
		{
			for (int j = 0; j < _numberOfRealSamples; j++)
			{
				ComplexNumber fz = _function(_domain[(i * _numberOfRealSamples) + j]);
				_functionImage.append(fz);
			}
		}
	}
}

void ColorWheelGraphWidget::buildGraphicalImage()
{
	if (_domain.count() == _functionImage.count())
	{
		QImage result(_numberOfRealSamples, _numberOfImaginarySamples, QImage::Format_RGB888);
		for (int i = 0; i < _numberOfImaginarySamples; i++)
		{
			for (int j = 0; j < _numberOfRealSamples; j++)
			{
				QColor val = getColorForComplex(_functionImage[(i * _numberOfRealSamples) + j]);
				result.setPixel(j, i, val.rgb());
			}
		}
		_graphicalImage = result;
		//_graphicalImage.save("/Users/AustinSimpson/Desktop/blah.png");
	}
}

QColor ColorWheelGraphWidget::getColorForComplex
(
	const ComplexNumber& number
)	const
{
	QColor result;
	double hue = (number.argument()) / (2*M_PI);
	if (hue < 0) hue += 1;
	double brightness;
	if (_radiusAffectsBrightness)
		brightness = 255 * (1.0 - pow((double)_alpha/100, number.modulus()));
	else
		brightness = 126;

	result.setHsl((360 * (1 - hue)), 255, brightness);
	return result;
}

void ColorWheelGraphWidget::setFunction
(
	std::function<ComplexNumber(ComplexNumber)> function
)
{
	_function = function;
	buildDomain(_windowLeft, _windowRight, _windowBottom, _windowTop);
	buildFunctionImage();
	buildGraphicalImage();
}

void ColorWheelGraphWidget::setAlpha(int alpha)
{
	_alpha = alpha;
	buildGraphicalImage();
	update();
}

void ColorWheelGraphWidget::setRadiusAffectsBrightness
(
	bool b
)
{
	_radiusAffectsBrightness = b;
	buildGraphicalImage();
	update();
}
