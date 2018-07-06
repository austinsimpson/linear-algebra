#include "Algebra.h"

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

