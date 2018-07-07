#include "Algebra.h"

#include <math.h>

QList<int> Factorize(int number)
{
	QList<int> result;

	int currentFactor = 2;

	if (number < 0)
	{
		result.append(-1);
		number = abs(number);
	}

	if (number == 0)
	{
		result.append(0);
	}
	else if (number == 1)
	{
		result.append(1);
	}
	else
	{
		while (number != 1)
		{
			while ((number % currentFactor) == 0)
			{
				result.append(currentFactor);
				number /= currentFactor;
			}
			currentFactor++;
		}
	}

	return result;
}


double gaussian
(
	double sigma,
	int x,
	int y
)
{
	double sigmaSquared = sigma * sigma;
	double exponent = - ((x*x + y*y) / (2 * sigmaSquared));
	double coefficient = 1.0 / (2 * M_PI * sigmaSquared);
	return coefficient * pow(M_E, exponent);
}
