#ifndef FRACTION_H
#define FRACTION_H

class Fraction
{
public:
	Fraction();
	Fraction(int numerator, int denominator);
	Fraction(double decimal);

	void setNumerator(int numerator);
	void setDenomintator(int denominator);

	int numerator() const;
	int denominator() const;

	Fraction reciprocal() const;

	void reduce();

	double toDecimal() const;

	Fraction operator=(const Fraction& other);
	Fraction operator=(double number);

	bool operator==(const Fraction& other);
	bool operator==(double number);
	bool operator!=(const Fraction& other);
	bool operator!=(double number);

	Fraction operator+(const Fraction& other);
	Fraction operator+(int number);
	Fraction operator+(double number);

	Fraction operator-(const Fraction& other);
	Fraction operator-(int number);
	Fraction operator-(double number);

	Fraction operator*(const Fraction& other);
	Fraction operator*(int number);
	Fraction operator*(double number);

	Fraction operator/(const Fraction& other);
	Fraction operator/(int number);
	Fraction operator/(double number);


	void operator+=(const Fraction& other);
	void operator+=(int number);
	void operator+=(double number);

	void operator-=(const Fraction& other);
	void operator-=(int number);
	void operator-=(double number);

	void operator*=(const Fraction& other);
	void operator*=(int number);

	void operator/=(const Fraction& other);

private:
	int _numerator;
	int _denominator;
};

#endif // FRACTION_H
