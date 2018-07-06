#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <QList>

QList<int> Factorize(int number);

template <typename T>
void clamp(T& toClamp, T lowerBound, T upperBound)
{
	if (toClamp < lowerBound)
		toClamp = lowerBound;
	if (toClamp > upperBound)
		toClamp = upperBound;
}


#endif // ALGEBRA_H
