#include <iostream>
#include "Matrix.h"
#include <math.h>

int main()
{
   setlocale(LC_ALL, "Russian");

   FILE* inputMatrix, * inputVector;
   int menuInput;

   printf_s("1 - ������� ���������� ������� �� ����� � ������ �� � ������� LU ����������\n2 - ������� ������� ������� �� ����� � ������ �� � ������� ������ ������\n3 - ������������� �������� ��������� � ������ �� � ������� LU ����������\n");
   scanf_s("%d", &menuInput);

   if (menuInput == 1) {
	  ProfileMatrix matrix;
	  int numOfCondition;
	  fopen_s(&inputMatrix, "matrix.txt", "r");
	  fopen_s(&inputVector, "vector.txt", "r");

	  matrix.Input(inputMatrix, inputVector);
	  fclose(inputMatrix);
	  fclose(inputVector);

	  printf_s("����������� ������� � ������������ ������ ���������������?\n");
	  scanf_s("%d", &numOfCondition);
	  if (numOfCondition)
	  {
		 int k;
		 printf_s("����� k ��� a(11)+10^(-k)?\n");
		 scanf_s("%d", &k);
		 real tmp = pow(10.0f, -k);
		 matrix.di[0] += tmp;
		 matrix.b[0] += tmp;
	  }
	  matrix.OutputDense();
	  matrix.SolveSLAE();
	  printf("\n");
	  //matrix.OutputDense();

	  FILE* out;
	  fopen_s(&out, "out.txt", "w");
	  matrix.OutputSolutionVector(out);

	  fclose(out);
   }
   else if (menuInput == 2) {
	  DenseMatrix matrix;
	  int numOfCondition;
	  fopen_s(&inputMatrix, "matrixDense.txt", "r");
	  fopen_s(&inputVector, "vectorDA.txt", "r");

	  matrix.Input(inputMatrix, inputVector);
	  fclose(inputMatrix);
	  fclose(inputVector);

	  printf_s("����������� ������� � ������������ ������ ���������������?\n");
	  scanf_s("%d", &numOfCondition);

	  if (numOfCondition)
	  {
		 int k;
		 printf_s("����� k ��� a(11)+10^(-k)?\n");
		 scanf_s("%d", &k);
		 real tmp = pow(10.0f, -k);
		 matrix.matrix[0][0] += tmp;
		 matrix.b[0] += tmp;
	  }
	  matrix.OutputDense();
	  matrix.SolveSLAE();

	  FILE* out;
	  fopen_s(&out, "out.txt", "w");
	  matrix.OutputSolutionVector(out);
	  fclose(out);
   }
   else if (menuInput == 3) {
	  ProfileMatrix matrix;
	  int m;
	  printf_s("������� �����������: ");
	  scanf_s("%d", &m);
	  matrix.GenerateHilbertMatrix(m);
	  //matrix.OutputDense();
	  matrix.SolveSLAE();
	  printf("\n");
	  //matrix.OutputDense();

	  FILE* out2;
	  fopen_s(&out2, "out2.txt", "w");
	  matrix.OutputSolutionVector(out2);

	  fclose(out2);
   }
}
