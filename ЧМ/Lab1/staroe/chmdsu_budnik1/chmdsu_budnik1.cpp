#include <iostream>
#include "Matrix.h"

void ProfileMatrix::Input(FILE* matrixFile, FILE* vectorFile) {
   fscanf_s(matrixFile, "%d", &n);
   ia = new int[n + 1];

   for (int i = 0; i <= n; i++)
	  fscanf_s(matrixFile, "%d", &ia[i]);
   nProfile = ia[n] - ia[0];
   AllocateMemory();
   if (ia[0]) {
	  for (int i = 0; i <= n; i++)
		 ia[i]--;
   }

   for (int i = 0; i < nProfile; i++)
	  fscanf_s(matrixFile, REALIN, &al[i]);

   for (int i = 0; i < nProfile; i++)
	  fscanf_s(matrixFile, REALIN, &au[i]);

   for (int i = 0; i < n; i++)
	  fscanf_s(matrixFile, REALIN, &di[i]);

   for (int i = 0; i < n; i++)
	  fscanf_s(vectorFile, REALIN, &b[i]);
}

void ProfileMatrix::AllocateMemory() {
   al = new real[nProfile];
   au = new real[nProfile];
   di = new real[n];
   b = new real[n];
   x = b;
   y = b;
}

void ProfileMatrix::CalculateLU()
{
   for (int i = 0; i < n; i++)
   {
	  int i0 = ia[i];
	  int i1 = ia[i + 1];
	  realSum sumD = 0;
	  int j = i - (i1 - i0);
	  for (int k = i0; k < i1; j++, k++)
	  {
		 realSum sumL = 0, sumU = 0;

		 int j0 = ia[j], j1 = ia[j + 1];

		 int difI = i1 - i0 - (i - j);
		 int difJ = j1 - j0;
		 int difIJ = difI - difJ;

		 int ki = i0, kj = j0;

		 if (difIJ < 0)
			kj -= difIJ;
		 else
			ki += difIJ;

		 for (; ki < k; ki++, kj++)
		 {
			sumL += al[ki] * au[kj];
			sumU += al[kj] * au[ki];
		 }

		 al[k] = al[k] - sumL;
		 au[k] = (au[k] - sumU) / di[j];
		 sumD += al[k] * au[k];
	  }
	  di[i] -= sumD;
   }
}

void ProfileMatrix::CalculateY() {
   for (int i = 0; i < n; i++)
   {
	  realSum sum = 0;
	  int i0 = ia[i], i1 = ia[i + 1];
	  int j = i - (i1 - i0);
	  for (int k = i0; k < i1; k++, j++)
		 sum += y[j] * al[k];
	  y[i] = (b[i] - sum) / di[i];
   }
}

void ProfileMatrix::CalculateX() {
   for (int i = n - 1; i >= 0; i--)
   {
	  int i0 = ia[i], i1 = ia[i + 1];
	  int j = i - (i1 - i0);
	  real xi = y[i];
	  for (int ij = i0; ij < i1; ij++, j++)
		 y[j] -= au[ij] * xi;
	  x[i] = xi;
   }
}

void ProfileMatrix::GenerateHilbertMatrix(int size)
{
   n = size;
   for (int i = 0; i < size; i++)
   {
	  nProfile += i;
   }
   ia = new int[n + 1];

   AllocateMemory();
   ia[0] = 0;
   for (int i = 1, k = 0; i <= n; i++)
   {
	  ia[i] = ia[i - 1] + (i - 1);
	  di[i - 1] = (double)1 / (2 * i - 1);
	  for (int j = 1; j < i; j++, k++)
	  {
		 al[k] = (double)1 / (i + j - 1);
		 au[k] = (double)1 / (i + j - 1);
	  }
   }

   for (int i = 0; i < n; i++)
   {
	  double sum = 0;
	  for (int xk = 1; xk <= n; xk++)
	  {
		 sum += (double)1 / (i + xk) * xk;
	  }
	  b[i] = sum;
   }
}

void ProfileMatrix::SolveSLAE() {
   CalculateLU();
   CalculateY();
   CalculateX();
}

void ProfileMatrix::OutputDense()
{
   for (int i = 0; i < n; i++)
   {
	  for (int j = 0; j < (i - (ia[i + 1] - ia[i])); j++)
	  {
		 printf(REALOUTD, 0.0);
	  }

	  for (int j = ia[i]; j < ia[i + 1]; j++)
	  {
		 printf(REALOUTD, al[j]);
	  }


	  printf(REALOUTD, di[i]);

	  for (int j = i + 1; j < n; j++)
	  {
		 int k = ia[j + 1] - ia[j];
		 if ((j - i) <= k) {
			printf(REALOUTD, au[ia[j] + k + i - j]);
		 }
		 else
		 {
			printf(REALOUTD, 0.0);
		 }
	  }

	  printf("\n");
   }

}

void BaseMatrix::OutputSolutionVector(FILE* out) {
   for (int i = 0; i < n; i++)
	  fprintf_s(out, REALOUT, x[i]);
}

void DenseMatrix::Input(FILE* matrixFile, FILE* vectorFile)
{
   fscanf_s(matrixFile, "%d", &n);
   AllocateMemory();

   for (int i = 0; i < n; i++)
   {
	  for (int j = 0; j < n; j++)
		 fscanf_s(matrixFile, REALIN, &matrix[i][j]);
   }

   for (int i = 0; i < n; i++)
	  fscanf_s(vectorFile, REALIN, &b[i]);
}

void DenseMatrix::SolveSLAE()
{
   GaussForwardElimination();
   GaussBackSubstitution();
}

void DenseMatrix::OutputDense()
{
   for (int i = 0; i < n; i++)
   {
	  for (int j = 0; j < n; j++)
		 printf_s(REALOUTD, matrix[i][j]);
	  printf_s("\n");
   }
}

void DenseMatrix::AllocateMemory()
{
   matrix = new real * [n];
   for (int i = 0; i < n; ++i) {
	  matrix[i] = new real[n];
   }
   b = new real[n];
   x = b;
}

void DenseMatrix::GaussForwardElimination()
{
   real max;
   int indexMax;
   for (int k = 0; k < n; k++)
   {
	  // Поиск строки с максимальным a[i][k]
	  max = fabsf(matrix[k][k]);
	  indexMax = k;
	  for (int i = k + 1; i < n; i++)
	  {
		 if (fabsf(matrix[i][k]) > max)
		 {
			max = fabsf(matrix[i][k]);
			indexMax = i;
		 }
	  }
	  // Перестановка строк
	  for (int j = 0; j < n; j++)
	  {
		 real temp = matrix[k][j];
		 matrix[k][j] = matrix[indexMax][j];
		 matrix[indexMax][j] = temp;
	  }
	  real temp = b[k];
	  b[k] = b[indexMax];
	  b[indexMax] = temp;
	  // Нормализация уравнений
	  for (int i = k; i < n; i++)
	  {
		 real temp = matrix[i][k];
		 if (fabsf(temp) < EPS)
			continue; // для нулевого коэффициента пропустить
		 for (int j = 0; j < n; j++)
			matrix[i][j] = matrix[i][j] / temp;
		 b[i] /= temp;
		 if (i == k)
			continue; // уравнение не вычитать само из себя
		 for (int j = 0; j < n; j++)
			matrix[i][j] -= matrix[k][j];
		 b[i] -= b[k];
	  }
   }
}

void DenseMatrix::GaussBackSubstitution()
{
   for (int k = n - 1; k >= 0; k--)
   {
	  x[k] = b[k];
	  for (int i = 0; i < k; i++)
		 b[i] -= matrix[i][k] * x[k];
   }
}
