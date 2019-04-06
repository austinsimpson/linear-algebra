#ifndef COMPLEXNUMBER_H
#define COMPLEXNUMBER_H

class ComplexNumber
{
public:
	ComplexNumber();
	ComplexNumber(const ComplexNumber& other);
	ComplexNumber(double real, double imaginary);
	ComplexNumber(double real);

	ComplexNumber& operator=(const ComplexNumber& other);
	ComplexNumber& operator=(double real);

	double real() const;
	double imaginary() const;

	void setReal(double real);
	void setImaginary(double imaginary);

	double modulus() const;
	double argument() const;

	ComplexNumber conjugate() const;
	ComplexNumber inverse() const;

	ComplexNumber operator+(const ComplexNumber& other) const;
	ComplexNumber operator+(double real) const;

	ComplexNumber& operator+=(const ComplexNumber& other);

	ComplexNumber operator-(const ComplexNumber& other) const;

	ComplexNumber& operator-=(const ComplexNumber& other);
	ComplexNumber operator-(double realNumber) const;

	ComplexNumber operator*(const ComplexNumber& other) const;
	ComplexNumber operator*(double scale) const;

	ComplexNumber& operator*=(const ComplexNumber& other);
	ComplexNumber& operator*=(double scale);

	ComplexNumber operator/(const ComplexNumber& other) const;
	ComplexNumber operator/(double scale) const;

	ComplexNumber& operator/=(const ComplexNumber& other);
	ComplexNumber& operator/=(double scalar);

	bool operator==(const ComplexNumber& other) const;

	const static ComplexNumber& i();

private:
	double _real;
	double _imaginary;
};

ComplexNumber operator+(double realNumber, const ComplexNumber& complexNumber);

ComplexNumber operator*(double scale, const ComplexNumber& complexNumber);

#endif // COMPLEXNUMBER_H
