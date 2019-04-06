#include "ComplexNumber.h"

#include <math.h>

const double kCompareThreshold = 0.0001;

ComplexNumber::ComplexNumber()
{
	_real = 0;
	_imaginary = 0;
}

ComplexNumber::ComplexNumber
(
	const ComplexNumber& other
)
{
	_real = other.real();
	_imaginary = other.imaginary();
}

ComplexNumber::ComplexNumber
(
	double real,
	double imaginary
)
{
	_real = real;
	_imaginary = imaginary;
}

ComplexNumber::ComplexNumber
(
	double real
):
	_imaginary(0)
{
	_real = real;
}

ComplexNumber& ComplexNumber::operator=
(
	const ComplexNumber& other
)
{
	_real = other.real();
	_imaginary = other.imaginary();
	return *this;
}

ComplexNumber& ComplexNumber::operator=
(
	double real
)
{
	_real = real;
	_imaginary = 0.0;
	return *this;
}

double ComplexNumber::real() const
{
	return _real;
}

double ComplexNumber::imaginary() const
{
	return _imaginary;
}

void ComplexNumber::setReal
(
	double real
)
{
	_real = real;
}

void ComplexNumber::setImaginary
(
	double imaginary
)
{
	_imaginary = imaginary;
}

double ComplexNumber::modulus() const
{
	return sqrt((_real*_real) + (_imaginary * _imaginary));
}

double ComplexNumber::argument() const
{
	return atan2(_imaginary, _real);
}

ComplexNumber ComplexNumber::conjugate() const
{
	return ComplexNumber(_real, -_imaginary);
}

ComplexNumber ComplexNumber::inverse() const
{
	return conjugate() / modulus();
}

ComplexNumber ComplexNumber::operator+
(
	const ComplexNumber& other
)	const
{
	return ComplexNumber(_real + other.real(), imaginary() + other.imaginary());
}

ComplexNumber ComplexNumber::operator+
(
	double real
)	const
{
	return ComplexNumber(_real + real, _imaginary);
}

ComplexNumber operator+
(
	double realNumber, const ComplexNumber& complexNumber
)
{
	return ComplexNumber(complexNumber.real() + realNumber, complexNumber.imaginary());
}

ComplexNumber& ComplexNumber::operator+=
(
	const ComplexNumber& other
)
{
	_real += other.real();
	_imaginary += other.imaginary();
	return *this;
}

ComplexNumber ComplexNumber::operator-
(
	const ComplexNumber& other
)	const
{
	return ComplexNumber(_real - other.real(), _imaginary - other.imaginary());
}

ComplexNumber ComplexNumber::operator-
(
	double real
)	const
{
	return ComplexNumber(_real - real, _imaginary);
}

ComplexNumber& ComplexNumber::operator-=
(
	const ComplexNumber& other
)
{
	_real -= other.real();
	_imaginary -= other.imaginary();
	return *this;
}

ComplexNumber ComplexNumber::operator*
(
	const ComplexNumber& other
)	const
{
	double real = (_real * other.real()) - (_imaginary * other.imaginary());
	double imaginary = ((_imaginary * other.real()) + (_real * other.imaginary()));
	return ComplexNumber(real, imaginary);
}

ComplexNumber ComplexNumber::operator*
(
	double scale
)	const
{
	return ComplexNumber(_real * scale, _imaginary * scale);
}

ComplexNumber operator*
(
	double scale, const ComplexNumber& complexNumber
)
{
	return ComplexNumber(complexNumber.real() * scale, complexNumber.imaginary() * scale);
}

ComplexNumber& ComplexNumber::operator*=
(
	const ComplexNumber& other
)
{
	double real = _real;
	double imaginary = _imaginary;

	_real = (real * other.real()) - (imaginary * other.imaginary());
	_imaginary = ((imaginary * other.real()) + (real * other.imaginary()));
	return *this;
}

ComplexNumber& ComplexNumber::operator*=
(
	double scale
)
{
	_real *= scale;
	_imaginary *= scale;
	return *this;
}

ComplexNumber ComplexNumber::operator/
(
	const ComplexNumber& other
)	const
{
	ComplexNumber otherInverse = other.conjugate() / other.modulus();
	return (*this) * otherInverse;
}

ComplexNumber ComplexNumber::operator/
(
	double scale
)	const
{
	return ComplexNumber(_real / scale, _imaginary / scale);
}

ComplexNumber& ComplexNumber::operator/=
(
	const ComplexNumber& other
)
{
	return (*this) *= other.inverse();
}

ComplexNumber& ComplexNumber::operator/=
(
	double scale
)
{
	return ((*this) *= 1/scale);
}

bool ComplexNumber::operator==
(
	const ComplexNumber& other
)	const
{
	double realDifference = _real - other.real();
	double imaginaryDifference = _imaginary - other.imaginary();
	return (realDifference < kCompareThreshold) && (-realDifference < kCompareThreshold) && (imaginaryDifference < kCompareThreshold) && (-imaginaryDifference < kCompareThreshold);
}

const static ComplexNumber _i = ComplexNumber(0, 1);
const ComplexNumber& ComplexNumber::i()
{
	return _i;
}
