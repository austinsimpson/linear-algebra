#ifndef LATEXMATRIXSERIALIZER_H
#define LATEXMATRIXSERIALIZER_H

#include "IMatrixSerializer.h"

class LatexMatrixSerializer : public IMatrixSerializer
{
public:
	enum Enclosure
	{
		eBrackets = 0,
		eParenthesis = 1
	};

	LatexMatrixSerializer();
	void readMatrix(QIODevice* device);
	bool saveMatrix(QIODevice* device, Matrix matrix);

	void setEnclosure(Enclosure enclosure);
	Enclosure enclosure() const;

private:
	void initializeEnclosureMap();

	Enclosure _enclosure;
};

#endif // LATEXMATRIXSERIALIZER_H
