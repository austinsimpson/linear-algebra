#include "CudaMatrixOperations.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

const int kThreadsPerBlockDimension = 32;

//So as far as I can tell, CUDAs memcpy doesnt support deep copying, so as a work around, I copy all of the data (at this point only row and column count) into the device memory and then create a second variable for the matrix data itself
//Input is the Cuda matrix from the cpu, device matrix data is an out parameter that is the pointer to the memory on the device. Sorry if this comment isn't helpful

CudaMatrix* putMatrixIntoDeviceMemory(const CudaMatrix& input, double** deviceMatrixData)
{
	CudaMatrix* result; 
	cudaMalloc((void**)&result , sizeof(CudaMatrix));
	cudaMemcpy((void*)result, (void*)&input, sizeof(CudaMatrix), cudaMemcpyHostToDevice);

	cudaMalloc((void**)deviceMatrixData, input.rowCount * input.columnCount * sizeof(double));
	cudaMemcpy(*deviceMatrixData, input.data, input.rowCount * input.columnCount * sizeof(double), cudaMemcpyHostToDevice);
	return result;
}

__global__ void matrixMultiplicationKernel(double* result, const double* firstMatrixData, const double* secondMatrixData, const CudaMatrix* firstMatrixDimensions, const CudaMatrix* secondMatrixDimensions)
{
	int row = (blockIdx.y * blockDim.y) + threadIdx.y;
	int column = (blockIdx.x * blockDim.x) + threadIdx.x;

	if (row < firstMatrixDimensions->rowCount && column < secondMatrixDimensions->columnCount)
	{
		for (int i = 0; i < firstMatrixDimensions->columnCount; i++)
		{
			result[(row * secondMatrixDimensions->columnCount) + column] += firstMatrixData[(row * firstMatrixDimensions->columnCount) + i] * secondMatrixData[(i * secondMatrixDimensions->columnCount) + column];
		}
	}
}

__declspec(dllexport) CudaMatrix multiplyMatricesWithCuda(const CudaMatrix& firstMatrix, const CudaMatrix& secondMatrix)
{
	CudaMatrix result(firstMatrix.rowCount, secondMatrix.columnCount);
	
	double* deviceFirstMatrixData; 
	CudaMatrix* deviceFirstMatrixDimensions = putMatrixIntoDeviceMemory(firstMatrix, &deviceFirstMatrixData);

	double* deviceSecondMatrixData;
	CudaMatrix* deviceSecondMatrixDimensions = putMatrixIntoDeviceMemory(secondMatrix, &deviceSecondMatrixData);

	double* deviceResultMatrixData;
	cudaMalloc((void**) &deviceResultMatrixData, result.rowCount * result.columnCount * sizeof(double));
	cudaMemset(deviceResultMatrixData, 0.0, result.rowCount * result.columnCount * sizeof(double));

	dim3 blockCounts((result.columnCount + (kThreadsPerBlockDimension - 1)) / kThreadsPerBlockDimension, ((result.rowCount + (kThreadsPerBlockDimension - 1)) / kThreadsPerBlockDimension));
	matrixMultiplicationKernel<<<blockCounts, dim3(kThreadsPerBlockDimension, kThreadsPerBlockDimension)>>>(deviceResultMatrixData, deviceFirstMatrixData, deviceSecondMatrixData, deviceFirstMatrixDimensions, deviceSecondMatrixDimensions);

	cudaMemcpy(result.data, deviceResultMatrixData, result.rowCount * result.columnCount * sizeof(double), cudaMemcpyDeviceToHost);

	cudaFree(deviceFirstMatrixData);
	cudaFree(deviceSecondMatrixData);
	cudaFree(deviceResultMatrixData);
	cudaFree(deviceFirstMatrixDimensions);
	cudaFree(deviceSecondMatrixDimensions);

	return result;
}

__global__ void matrixAdditionKernel(double* result, const double* firstMatrixData, const double* secondMatrixData, const CudaMatrix* firstMatrixDimensions, const CudaMatrix* secondMatrixDimensions)
{
	int row = (blockIdx.y * blockDim.y) + threadIdx.y;
	int column = (blockIdx.x * blockDim.x) + threadIdx.x;

	if (row < firstMatrixDimensions->rowCount && column < firstMatrixDimensions->columnCount)
	{
		result[(row * firstMatrixDimensions->columnCount) + column] = firstMatrixData[(row * firstMatrixDimensions->columnCount) + column] + secondMatrixData[(row * secondMatrixDimensions->columnCount) + column];
	}
}

__declspec(dllexport) CudaMatrix addMatricesWithCuda(const CudaMatrix& firstMatrix, const CudaMatrix& secondMatrix)
{
	CudaMatrix result(firstMatrix.rowCount, firstMatrix.columnCount);

	double* deviceFirstMatrixData;
	CudaMatrix* deviceFirstMatrixDimensions = putMatrixIntoDeviceMemory(firstMatrix, &deviceFirstMatrixData);

	double* deviceSecondMatrixData;
	CudaMatrix* deviceSecondMatrixDimensions = putMatrixIntoDeviceMemory(secondMatrix, &deviceSecondMatrixData);

	double* deviceResultMatrixData;
	cudaMalloc((void**)&deviceResultMatrixData, result.rowCount * result.columnCount * sizeof(double));
	cudaMemset(deviceResultMatrixData, 0.0, result.rowCount * result.columnCount * sizeof(double));

	dim3 blockCounts((result.columnCount + (kThreadsPerBlockDimension - 1)) / kThreadsPerBlockDimension, ((result.rowCount + (kThreadsPerBlockDimension - 1)) / kThreadsPerBlockDimension));
	matrixAdditionKernel<<<blockCounts, dim3(kThreadsPerBlockDimension, kThreadsPerBlockDimension)>>> (deviceResultMatrixData, deviceFirstMatrixData, deviceSecondMatrixData, deviceFirstMatrixDimensions, deviceSecondMatrixDimensions);

	cudaMemcpy(result.data, deviceResultMatrixData, result.rowCount * result.columnCount * sizeof(double), cudaMemcpyDeviceToHost);

	cudaFree(deviceFirstMatrixData);
	cudaFree(deviceSecondMatrixData);
	cudaFree(deviceResultMatrixData);
	cudaFree(deviceFirstMatrixDimensions);
	cudaFree(deviceSecondMatrixDimensions);

	return result;
}

__global__ void matrixSubtractionKernel(double* result, const double* firstMatrixData, const double* secondMatrixData, const CudaMatrix* firstMatrixDimensions, const CudaMatrix* secondMatrixDimensions)
{
	int row = (blockIdx.y * blockDim.y) + threadIdx.y;
	int column = (blockIdx.x * blockDim.x) + threadIdx.x;

	if (row < firstMatrixDimensions->rowCount && column < firstMatrixDimensions->columnCount)
	{
		result[(row * firstMatrixDimensions->columnCount) + column] = firstMatrixData[(row * firstMatrixDimensions->columnCount) + column] - secondMatrixData[(row * secondMatrixDimensions->columnCount) + column];
	}
}

__declspec(dllexport) CudaMatrix subtractMatricesWithCuda(const CudaMatrix& firstMatrix, const CudaMatrix& secondMatrix)
{
	CudaMatrix result(firstMatrix.rowCount, firstMatrix.columnCount);

	double* deviceFirstMatrixData;
	CudaMatrix* deviceFirstMatrixDimensions = putMatrixIntoDeviceMemory(firstMatrix, &deviceFirstMatrixData);

	double* deviceSecondMatrixData;
	CudaMatrix* deviceSecondMatrixDimensions = putMatrixIntoDeviceMemory(secondMatrix, &deviceSecondMatrixData);

	double* deviceResultMatrixData;
	cudaMalloc((void**)&deviceResultMatrixData, result.rowCount * result.columnCount * sizeof(double));
	cudaMemset(deviceResultMatrixData, 0.0, result.rowCount * result.columnCount * sizeof(double));

	dim3 blockCounts((result.columnCount + (kThreadsPerBlockDimension - 1)) / kThreadsPerBlockDimension, ((result.rowCount + (kThreadsPerBlockDimension - 1)) / kThreadsPerBlockDimension));
	matrixSubtractionKernel<<<blockCounts, dim3(kThreadsPerBlockDimension, kThreadsPerBlockDimension)>>> (deviceResultMatrixData, deviceFirstMatrixData, deviceSecondMatrixData, deviceFirstMatrixDimensions, deviceSecondMatrixDimensions);

	cudaMemcpy(result.data, deviceResultMatrixData, result.rowCount * result.columnCount * sizeof(double), cudaMemcpyDeviceToHost);

	cudaFree(deviceFirstMatrixData);
	cudaFree(deviceSecondMatrixData);
	cudaFree(deviceResultMatrixData);
	cudaFree(deviceFirstMatrixDimensions);
	cudaFree(deviceSecondMatrixDimensions);

	return result;
}