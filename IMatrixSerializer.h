#ifndef IMATRIXSERIALIZER_H
#define IMATRIXSERIALIZER_H

#endif // IMATRIXSERIALIZER_H

#include <QIODevice>
#include "Matrix.h"

class IMatrixSerializer
{
public:
	virtual void readMatrix(QIODevice* device) = 0;
	virtual bool saveMatrix(QIODevice* device, Matrix saveMe) = 0;
};
