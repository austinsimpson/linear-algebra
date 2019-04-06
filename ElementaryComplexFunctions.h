#ifndef ELEMENTARYCOMPLEXFUNCTIONS_H
#define ELEMENTARYCOMPLEXFUNCTIONS_H

#include <math.h>

#include <QSet>
#include <QHash>
#include <ComplexNumber.h>

inline ComplexNumber complexExp(const ComplexNumber& number)
{
	return exp(number.real()) * ComplexNumber(cos(number.imaginary()), sin(number.imaginary()));
}

inline ComplexNumber complexLog(const ComplexNumber& number)
{
	return log(number.modulus()) + ComplexNumber::i() * number.argument();
}

inline ComplexNumber complexExponent
(
	const ComplexNumber& base,
	const ComplexNumber& exponent
)
{
	return complexExp(exponent * complexLog(base));
}

inline ComplexNumber complexExponent
(
	const ComplexNumber& base,
	double exponent
)
{
	return complexExponent(base, ComplexNumber(exponent));
}

inline ComplexNumber complexSin(const ComplexNumber& number)
{
	return (complexExp(number) - complexExp(-1 * number))/(2*ComplexNumber::i());
}

inline uint qHash(const ComplexNumber& key)
{
	return qHash(key.real()) + qHash(key.imaginary());
}

inline QHash<ComplexNumber, ComplexNumber> applyElementaryFunctionToSet(QSet<ComplexNumber> numbers, ComplexNumber (*function)(const ComplexNumber&))
{
	QHash<ComplexNumber, ComplexNumber> result;
	QSet<ComplexNumber>::iterator number = numbers.begin();
	while (number != numbers.end())
	{
		result[*number] = function(*number);
		number++;
	}
	return result;
}

inline ComplexNumber operator^(const ComplexNumber& base, const ComplexNumber& exponent)
{
	return complexExponent(base, exponent);
}

#endif // ELEMENTARYCOMPLEXFUNCTIONS_H
