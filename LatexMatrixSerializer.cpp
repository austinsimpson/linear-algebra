#include "LatexMatrixSerializer.h"

#include <QTextStream>
#include <QMap>

static QMap<LatexMatrixSerializer::Enclosure, QString> gEnclosureStringMap;

LatexMatrixSerializer::LatexMatrixSerializer(): _enclosure(LatexMatrixSerializer::eBrackets)
{
	if (gEnclosureStringMap.isEmpty())
	{
		initializeEnclosureMap();
	}
}

void LatexMatrixSerializer::readMatrix
(
	QIODevice* device
)
{

}

bool LatexMatrixSerializer::saveMatrix
(
	QIODevice* device,
	Matrix matrix
)
{
	bool result = false;
	QTextStream stream(device);
	if (stream.status() == QTextStream::Ok)
	{
		stream << QString("\\begin{%1}").arg(gEnclosureStringMap[_enclosure]);
		for (int i = 0; i < matrix.rowCount(); i++)
		{
			for (int j = 0; j < matrix.columnCount(); j++)
			{
				QString terminator = j == (matrix.columnCount() - 1) ? "\\\\" : "&";
				stream << QString(" %1 %2").arg(matrix.value(i, j)).arg(terminator);
			}
		}
		stream << QString("\\end{%1}").arg(gEnclosureStringMap[_enclosure]);
	}
	return result;
}

void LatexMatrixSerializer::setEnclosure(Enclosure enclosure)
{
	_enclosure = enclosure;
}

LatexMatrixSerializer::Enclosure LatexMatrixSerializer::enclosure() const
{
	return _enclosure;
}

void LatexMatrixSerializer::initializeEnclosureMap()
{
	gEnclosureStringMap[LatexMatrixSerializer::Enclosure::eBrackets] = "bmatrix";
	gEnclosureStringMap[LatexMatrixSerializer::Enclosure::eParenthesis] = "pmatrix";
}
