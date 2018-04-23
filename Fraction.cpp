#include "Fraction.h"
#include "Algebra.h"

#include "math.h"

#include <QList>

Fraction::Fraction()
{

}

Fraction::Fraction
(
	int numerator,
	int denominator
)
{
	setNumerator(numerator);
	setDenomintator(denominator);
}

Fraction::Fraction
(
	double decimal
)
{
	int denominator = 1;
	double watch = floor(decimal + 0.1);
	while (qFuzzyCompare(watch, decimal) == false)
	{
		decimal *= 10;
		denominator *= 10;
		watch = floor(decimal + 0.1);
	}

	setNumerator((int) watch);
	setDenomintator(denominator);
	reduce();
}

void Fraction::setNumerator
(
	int numerator
)
{
	_numerator = numerator;
}

void Fraction::setDenomintator
(
	int denominator
)
{
	Q_ASSERT(denominator != 0);
	_denominator = denominator;
}

int Fraction::numerator() const
{
	return _numerator;
}

int Fraction::denominator() const
{
	return _denominator;
}

Fraction Fraction::reciprocal() const
{
	return Fraction(denominator(), numerator());
}

void Fraction::reduce()
{
	QList<int> numeratorFactors = Factorize(numerator());
	QList<int> denominatorFactors = Factorize(denominator());

	QSet<int> commonFactors = numeratorFactors.toSet().intersect(denominatorFactors.toSet());
	QSet<int>::iterator factor = commonFactors.begin();
	while (factor != commonFactors.end())
	{
		int numeratorCount = numeratorFactors.count(*factor);
		int denominatorCount = denominatorFactors.count(*factor);

		int minCount = numeratorCount < denominatorCount ? numeratorCount : denominatorCount;

		_numerator = _numerator / pow(*factor, minCount);
		_denominator = _denominator / pow(*factor, minCount);
		factor++;
	}
}

double Fraction::toDecimal() const
{
	return (double) numerator() / (double) denominator();
}

Fraction Fraction::operator=
(
	const Fraction& other
)
{
	setNumerator(other.numerator());
	setDenomintator(other.denominator());
	return *this;
}

Fraction Fraction::operator=
(
	double number
)
{
	*this = Fraction(number);
	return *this;
}

bool Fraction::operator ==
(
	const Fraction& other
)
{
	return denominator() == other.denominator() && numerator() == other.numerator();
}

bool Fraction::operator ==(double number)
{
	return toDecimal() == number;
}

bool Fraction::operator !=
(
	const Fraction& other
)
{
	return !(*this == other);
}

bool Fraction::operator != (double number)
{
	return !(*this == number);
}

Fraction Fraction::operator+
(
	const Fraction& other
)
{
	Fraction result = Fraction();
	result.setDenomintator(this->denominator() * other.denominator());
	result.setNumerator((this->numerator() * other.denominator()) + (other.numerator() * this->denominator()));
	result.reduce();
	return result;
}

void Fraction::operator +=
(
	const Fraction& other
)
{
	setDenomintator(denominator() * other.denominator());
	setNumerator((numerator() * other.denominator()) + (other.numerator() * denominator()));
	reduce();
}

Fraction Fraction::operator-
(
	const Fraction& other
)
{
	Fraction result = Fraction();
	result.setDenomintator(this->denominator() * other.denominator());
	result.setNumerator((this->numerator() * other.denominator()) - (other.numerator() * this->denominator()));
	result.reduce();
	return result;
}

void Fraction::operator-=
(
	const Fraction& other
)
{
	setDenomintator(this->denominator() * other.denominator());
	setNumerator((this->numerator() * other.denominator()) - (other.numerator() * this->denominator()));
	reduce();
}

Fraction Fraction::operator *
(
	const Fraction& other
)
{
	Fraction result = Fraction();
	result.setNumerator(this->numerator() * other.numerator());
	result.setDenomintator(this->denominator() * other.denominator());
	result.reduce();
	return result;
}

void Fraction::operator *=
(
	const Fraction& other
)
{
	setNumerator(numerator() * other.numerator());
	setDenomintator(denominator() * other.denominator());
	reduce();
}

Fraction Fraction::operator /
(
	const Fraction& other
)
{
	Fraction result = Fraction();
	result.setNumerator(this->numerator() * other.denominator());
	result.setDenomintator(this->denominator() * other.numerator());
	result.reduce();
	return result;
}

void Fraction::operator/=
(
	const Fraction& other
)
{
	setNumerator(this->numerator() * other.denominator());
	setDenomintator(this->denominator() * other.numerator());
	reduce();
}
