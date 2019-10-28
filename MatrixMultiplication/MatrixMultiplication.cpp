// MatrixMultiplication.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>

using std::size_t;

#define SIZE 4
const size_t BLOCK_SIZES[] = { 1, 6, 10, 15, 20, 24, 30, 36, 40, 60, 72, 80, 96, 120, 144, 160, 180, 240, 360, 480, 720 };
#define BLOCK_SIZE_IND 0
const size_t BLOCK_SIZE = BLOCK_SIZES[BLOCK_SIZE_IND];
const size_t BLOCK_COUNT = SIZE / BLOCK_SIZE;
const size_t BLOCK_SIZE2 = BLOCK_SIZE * BLOCK_SIZE;

const size_t NO_ELEM = (size_t)0 - 1;
double *MatrixA, *MatrixB, *MatrixC;

struct Task {
public:
	size_t i;
	size_t j;

	Task(size_t _i,size_t _j){
		i = _i;
		j = _j;
	}
};

std::vector<Task> tasks;


size_t ** offsetA, ** offsetB, **offsetC;
size_t arraySize;
size_t calcSize() {
	size_t res = 0;
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		for (size_t j = 0; j <= i; j++)
		{
			res += BLOCK_SIZE2;
		}
	}
	return res;
}

size_t ** generateOffsetA() {
	
	size_t ** result = new size_t*[BLOCK_COUNT];
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		result[i] = new size_t[BLOCK_COUNT]{ 0 };
	}

	// columns first
	size_t offset = 0;
	for (size_t j = 0; j < BLOCK_COUNT; j++)
	{
		for (size_t i = 0; i < BLOCK_COUNT; i++)
		{
			if (i < j) {
				result[i][j] = NO_ELEM;
			}
			else {
				result[i][j] = offset;
				offset += BLOCK_SIZE2;
			}
			
		}
	}
	return result;
	
}


size_t ** generateOffsetB() {

	size_t ** result = new size_t*[BLOCK_COUNT];
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		result[i] = new size_t[BLOCK_COUNT]{ 0 };
	}

	// columns first
	size_t offset = 0;
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		for (size_t j = 0; j < BLOCK_COUNT; j++)
		{
			if (i < j) {
				result[i][j] = NO_ELEM;
			}
			else {
				result[i][j] = offset;
				offset += BLOCK_SIZE2;
			}

		}
	}
	return result;

}

size_t ** generateOffsetC() {

	size_t ** result = new size_t*[BLOCK_COUNT];
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		result[i] = new size_t[BLOCK_COUNT]{ 0 };
	}

	// columns first
	size_t offset = 0;
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		for (size_t j = 0; j < BLOCK_COUNT; j++)
		{
			
				result[i][j] = offset;
				offset += BLOCK_SIZE2;
			

		}
	}
	return result;

}


inline double * addresA(const size_t row, const size_t col){
	// shouldn't be needed if proper permutations are generated
	// i guess
	if (col > row)
		return nullptr;
	return MatrixA + offsetA[row][col];
}

inline double * addresB(const size_t row, const size_t col) {
	if (col > row)
		return MatrixB + offsetB[col][row];
	return MatrixB + offsetB[row][col];
}

inline double * addresC(const size_t row, const size_t col) {
	return MatrixC + offsetC[row][col];
}

void genereateTasks() {
	for (size_t i = 0; i < BLOCK_COUNT; i++)
		for (size_t j = 0; j < BLOCK_COUNT; j++)
			tasks.push_back(Task(i, j));
}


void printOffset(size_t ** off) {
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		for (size_t j = 0; j < BLOCK_COUNT; j++)
		{
			int pr = off[i][j] == NO_ELEM ? -1 : (int)off[i][j];
			std::cout << pr << "\t";
		}
		std::cout << std::endl;
	}
}

inline void multBlocks(const double* const A, const double* const B,  double* const C) {
	for (size_t i = 0; i < BLOCK_SIZE; i++)
		for (size_t j = 0; j < BLOCK_SIZE; j++)
			for (size_t k = 0; k < BLOCK_SIZE; k++)
				C[i*BLOCK_SIZE + j] += A[i*BLOCK_SIZE + k] * B[k*BLOCK_SIZE + j];
			
}

void multMatrixes() {
	for (size_t i = 0; i < tasks.size; i++)
	{
		Task t = tasks[i];
		for (size_t k = 0; k < BLOCK_COUNT; k++)
		{
			double * A = addresA(t.i, k);
			if (A == nullptr)
				continue;
			double * B = addresB(k, t.j);
			double * C = addresC(t.i, t.j);
			multBlocks(A, B, C);
		}
	}
}


void generateMatrixA() {
	//TODO
}

void generateMatrixB() {
	//TODO
}


int main()
{
	arraySize = calcSize();
	MatrixA = new double[arraySize] {0};
	MatrixB = new double[arraySize] {0};
	MatrixC = new double[SIZE*SIZE] {0};
	offsetA = generateOffsetA();
	offsetB = generateOffsetB();
	offsetC = generateOffsetC();
	multMatrixes();
	std::cout << std::endl;
}
