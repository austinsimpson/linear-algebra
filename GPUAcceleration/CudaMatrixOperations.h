
#ifndef CUDAMATRIXOPERATIONS_H
#define CUDAMATRIXOPERATIONS_H

//In designing the GPU acceleration library, I didn't want to have a circular dependency to my main LinearTransformation project. So, I create a lightweight struct that keeps track of our CUDA operations. CudaMatrix might be a bit of a misnomer, but I wanted to differentiate it from the main Matrix class.
//CudaMatrix is not meant to be used by the user. All occurrences of CudaMatrix should be restricted to the Matrix.h class in other other project

typedef struct CudaMatrix
{
	int rowCount;
	int columnCount;
	double* data;

	CudaMatrix() {}
	CudaMatrix(int rows, int columns)
	{
		rowCount = rows;
		columnCount = columns;
		data = new double[rowCount * columnCount * sizeof(double)];
	}
} CudaMatrix;

extern __declspec(dllexport) CudaMatrix multiplyMatricesWithCuda(const CudaMatrix& firstMatrix, const CudaMatrix& secondMatrix);
extern __declspec(dllexport) CudaMatrix addMatricesWithCuda(const CudaMatrix& first, const CudaMatrix& second);
extern __declspec(dllexport) CudaMatrix subtractMatricesWithCuda(const CudaMatrix& first, const CudaMatrix& second);

#endif //CUDAMATRIXOPERATIONS_H