﻿#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;
typedef double real;
#define realin "%lf"
#define EPS 1e-13

class matrix {
public:
   int n, m;
   real** al, ** au, ** A;
   //real** l, ** u;
   real* di, * b, * x, * y;
   real** mat;
   //matrix() {};


   void allocateMemory();
   void calcLU();
   void input(FILE* inmat, FILE* invec);
   void output(ofstream& res);
   void getY();
   void getX();
   void Gilbert();
   void Gauss();
   void gauss();
   void gausss();
};/*
void matrix::calcLU() {

   for (int i = 0; i < n; i++) {
      real sumL = 0;
      real sumU = 0;
      real sumD = 0;
      for (int j = 0; j < i; j++) {



         for (int ki = 0; ki <= i - 1; ki++)
         {
            sumU += al[i][ki + m - i] * au[j][ki + m - j];
            //printf("%d sumU: %f %d %d\n", ki, sumU, i, j);
            printf("%d sumL: %f %d %d al: %f au: %f\n", ki, sumL, i, j, al[i][ki + m - j], au[j][ki + m - j]);

         }
         printf("%d %d sumU: %f \n\n", i, j, sumU);



         for (int kj = 0; kj <= j - 1; kj++)
         {
            sumL += al[i][kj + m - i] * au[j][kj + m - j];
            printf("%d sumL: %f %d %d al: %f au: %f\n", kj, sumL, i, j, al[i][kj + m - i], au[j][kj + m - j]);
         }

         printf("%d %d sumL: %f \n\n", i, j, sumL);


         au[i][j] = au[i][j + m - i] - sumU;
         al[j][i] = (al[j][i + m - j] - sumL) / di[j];
         //sumD += al[i][j] * au[k];
      }
   }

}
*/

void matrix::input(FILE* inmat, FILE* invec) {

   fscanf(inmat, "%d %d", &n, &m);
   allocateMemory();

   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
         fscanf(inmat, realin, &al[i][j]);
      }
   }

   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
         fscanf(inmat, realin, &au[i][j]);
      }
   }

   for (int i = 0; i < n; i++)
      fscanf(inmat, realin, &di[i]);
   for (int i = 0; i < n; i++)
      fscanf(invec, realin, &b[i]);
   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
         fscanf(inmat, realin, &mat[i][j]);
      }
   }
   fclose(inmat);
   fclose(invec);

}
/*
void matrix::calcLU() {

   for (int i = 0; i < n; i++) {

      for (int j = 0; j < i; j++) {
         real sumL = 0;
         real sumU = 0;
         real sumD = 0;



         for (int kj = 0; kj <= j - 1; kj++)
         {
            sumL += al[i][kj + m - i] * au[j][kj + m - j];
            printf("%d sumL: %f %d %d al: %f au: %f\n", kj, sumL, i, j, al[i][kj + m - i], au[j][kj + m - j]);
         }

         printf("%d %d sumL: %f \n\n", i, j, sumL);




         al[i][j] = (al[j][i + m - j] - sumL) / di[j];


         //sumD += al[i][j] * au[k];
      }
   }

   printf("------------------------------------------\n");

   for (int i = 0; i < n; i++) {

      for (int j = i + 1; j < n; j++) {
         real sumL = 0;
         real sumU = 0;
         real sumD = 0;



         for (int ki = 0; ki <= i - 1; ki++)
         {
            sumU += al[i][ki + m - i] * au[j][ki + m - j];
            //printf("%d sumU: %f %d %d\n", ki, sumU, i, j);
            printf("%d sumU: %f %d %d al: %f au: %f\n", ki, sumU, i, j, al[i][ki + m - i], au[j][ki + m - j]);

         }
         printf("%d %d sumU: %f \n\n", i, j, sumU);

         au[i][j] = au[i][j + m - i] - sumU;


         //sumD += al[i][j] * au[k];
      }
   }

}*/

void matrix::calcLU() {
   A = new real * [n];

   for (int i = 0; i < n; i++) {
      A[i] = new real[m];
      for (int j = 0; j < n; j++) {
         //A[i] = new real[m];

         if (j > i) {
            A[i][j] = au[j][i + m - j];
            au[j][i + m - j] = 0.0;
         }
         else {
            A[i][j] = al[i][j + m - i];
            al[i][j + m - i] = 0.0;
         }
         if (i == j) al[i][j] = 1.0;
         A[i][i] = di[i];
         //al[i][i] = 1.0;
         printf("%f   ", A[i][j]);
      }
      printf("\n");
   }

   for (int j = 0; j < n; j++) {
      // Заполнение верхнетреугольной матрицы au
      for (int i = 0; i <= j; i++) {
         au[i][j] = A[i][j];

         for (int k = 0; k < i; k++) {
            au[i][j] -= al[i][k] * au[k][j];
         }

      }
      // Заполнение нижнетреугольной матрицы al
      for (int i = j + 1; i < n; i++) {
         al[i][j] = A[i][j];
         for (int k = 0; k < j; k++) {
            al[i][j] -= al[i][k] * au[k][j];
         }

         al[i][j] /= au[j][j];
      }
   }
}

void matrix::allocateMemory() {
   al = new real * [n];
   for (int i = 0; i < n; ++i) {
      al[i] = new real[m];
   }
   au = new real * [n];
   for (int i = 0; i < n; ++i) {
      au[i] = new real[m];
   }
   mat = new real * [n];
   for (int i = 0; i < n; ++i) {
      mat[i] = new real[n];
   }
   di = new real[n];
   b = new real[n];
   x = b;
   y = b;
}


void matrix::gausss()
{
   real max;
   int indexMax;
   for (int k = 0; k < n; k++)
   {
      //Поиск максимального элемента в столбце
      max = fabs(mat[k][k]);
      indexMax = k;
      for (int i = k + 1; i < n; i++)
      {
         if (fabs(max < fabs(mat[i][k])))
         {
            max = fabs(mat[i][k]);
            indexMax = i;
         }

      }
      //Перестановка строк
      for (int j = 0; j < n; j++)
      {
         real temp = mat[k][j];
         mat[k][j] = mat[indexMax][j];
         mat[indexMax][j] = temp;
      }

      real temp = b[k];
      b[k] = b[indexMax];
      b[indexMax] = temp;
      // Нормализация уравнений
      for (int i = k; i < n; i++)
      {
         real temp = mat[i][k];
         if (fabsf(temp) < EPS)
            continue; // для нулевого коэффициента пропустить
         for (int j = 0; j < n; j++)
            mat[i][j] = mat[i][j] / temp;
         b[i] /= temp;
         if (i == k)
            continue; // уравнение не вычитать само из себя
         for (int j = 0; j < n; j++)
            mat[i][j] -= mat[k][j];
         b[i] -= b[k];
      }

      /*for (int j = 0; j < n; j++)
      {
         mat[]
      }
*/
   }
}



void matrix::Gauss()
{
   real max;
   int indexMax;
   for (int k = 0; k < n; k++)
   {
      // Поиск строки с максимальным a[i][k]
      max = fabsf(mat[k][k]);
      indexMax = k;
      for (int i = k + 1; i < n; i++)
      {
         if (fabsf(mat[i][k]) > max)
         {
            max = fabsf(mat[i][k]);
            indexMax = i;
         }
      }
      // Перестановка строк
      for (int j = 0; j < n; j++)
      {
         real temp = mat[k][j];
         mat[k][j] = mat[indexMax][j];
         mat[indexMax][j] = temp;
      }
      real temp = b[k];
      b[k] = b[indexMax];
      b[indexMax] = temp;
      // Нормализация уравнений
      for (int i = k; i < n; i++)
      {
         real temp = mat[i][k];
         if (fabsf(temp) < EPS)
            continue; // для нулевого коэффициента пропустить
         for (int j = 0; j < n; j++)
            mat[i][j] = mat[i][j] / temp;
         b[i] /= temp;
         if (i == k)
            continue; // уравнение не вычитать само из себя
         for (int j = 0; j < n; j++)
            mat[i][j] -= mat[k][j];
         b[i] -= b[k];
      }
   }
}


void comp()
{

   int nn = 4;
   double A[4][4] = {
   { 1, 7, 6, 4 },
   { 5, 8, 2, 3 },
   { 9, 3, 4, 1 },
   { 8, 2, 7, 6 }
   };
   double B[4] = { 11, 7, 8, 1 };

   double X[4] = { 0 };
   double U[4][4] = { 0 };

   //U[4][4] = A[4][4];

   // Печать матрицы
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++) {
         //A[i][j] = 1.0;
         printf("%f  ", A[i][j]);
      }
      printf("   %f", B[i]);
      printf("\n");
   }
   printf("--------------------------------------\n");


   // Прямой ход алгоритма
   for (int k = 0; k < nn; k++)
   {
      printf("k  %d   k\n", k);
      int i = k;
      //for (int i = k; i < 1; i++)
      {
         double amax = A[i][k];
         int jmax = k;
         // Поиск главного элемента
         for (int j = k + 1; j < nn; j++)
         {
            if (abs(A[i][j]) > abs(A[i][jmax])) { jmax = j; };
            //printf("%f  ", A[i][j]);
            //printf("%d %d  ", i, j);
            printf("[  %f  %d  ]", A[i][jmax], jmax);
         }
         printf("\n");

         double Amax = A[i][jmax];
         for (int j = k; j < nn; j++)
         {
            A[i][j] /= Amax;
            //A[i][j] /= A[i][jmax];
            //printf("%f  ", A[i][j]);
            //printf("%d %d  ", i, j);
            printf("[M  %f  %d  M]", A[i][j], j);
         }
         B[i] /= Amax;
         printf("\n");

         // Перестановка
         int j = jmax;
         for (int ij = i; ij < 1; ij++)
         {
            // Обмен столбцов
            if (ij != j)
            {
               double a = A[ij][k];
               A[ij][k] = A[ij][j];
               A[ij][j] = a;
               //A[i][j] /= A[i][jmax];
               //printf("%f  ", A[i][j]);
               //printf("%d %d  ", i, j);
               printf("[R  %f  %d  R]", A[ij][j], ij);
            }
         }
         // Обмен строк в правой части
         double b = B[k];
         B[k] = B[j];
         B[j] = b;

         for (int i = k + 1; i < nn; i++) {
            for (int j = k; j < nn; j++) {
               A[i][j] -= A[i][k];
            }
            B[i] -= A[i][k];
         }

      }
   }
   printf("\n");
   printf("--------------------------------------\n");

   // Печать матрицы
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++) {
         //A[i][j] = 1.0;
         printf("%f  ", A[i][j]);
      }
      printf("   %f", B[i]);
      printf("\n");
   }
   printf("--------------------------------------\n");

   // Обратный ход алгоритма
   printf("--------------------Obratniy hod algortima------------------\n");
   int k = 0;
   X[nn - 1] = B[nn - 1] / A[nn - 1][nn - 1];
   for (int i = nn - 2; i > -1; i--) {
      //printf("i: %d  \n", i);
      double sum = 0;
      for (int j = nn - 1; j > i; j--) {
         printf(" %d  %d  |  ", i, j);
         sum += X[i + 1] * A[i][j];
         printf(" %d  %d  %f  |  ", i, j, sum);
      }
      X[i] = B[i] - sum;
      printf("\n");
   }
   for (int i = 0; i < nn; i++) {
      printf(" %d  %f  \n", i, X[i]);
   }
   printf("--------------------------------------\n");

   // Проверка - нахождение В, через А и х
   for (int i = 0; i < nn; i++)
   {
      double sum = 0;
      for (int j = 0; j < nn; j++) {
         sum += A[i][j] * X[i];
         //printf(" |  %f  | ", A[i][j]);
      }
      printf(" %f  %f", sum, B[i]);
      printf("\n");
   }
   printf("--------------------------------------\n");


}

int calcLead(std::vector <std::vector <real>> matrix, int N, int i)
{
   double lead = matrix[i][i];
   int leadIndex = i;
   for (int k = i + 1; k < N; k++)
      if (abs(matrix[k][i]) > abs(lead))
      {
         lead = matrix[k][i];
         leadIndex = k;
      }
   return leadIndex;
}

//void matrix::gauss(real** al, real* di, real* b, real* x, int n, int m)
void matrix::gauss()
{
   std::vector <std::vector <real>> matrix;
   std::vector <real> b_vec;
   matrix.resize(n);
   b_vec.resize(n);


   for (int i = 0; i < n; i++) {
      b_vec[i] = b[i];
      matrix[i].resize(n);
      for (int j = 0; j < n; j++) {
         //A[i] = new real[m];
         if (j > i)  matrix[i][j] = au[j][i + m - j];
         else  matrix[i][j] = al[i][j + m - i];
         matrix[i][i] = di[i];
         printf("%f   ", matrix[i][j]);
      }
      printf("\n");
   }
   /*
   for (int i = 0; i < n; i++) {
      b_vec[i] = b[i];
      matrix[i].resize(n);
      matrix[i][i] = di[i];
      for (int j = i - m, j_band = 0; j_band < m; j_band++, j++) {
         if (j < 0) continue;
         matrix[i][j] = al[i][j_band];
         matrix[j][i] = au[i][j_band];
      }
   }*/
   /*for (int i = 0; i < n; i++) {
   }*/

   for (int i = 0; i < n; i++) {
      int leadIndex = calcLead(matrix, n, i);
      matrix[i].swap(matrix[leadIndex]);
      std::swap(b_vec[i], b_vec[leadIndex]);
      for (int k = i + 1; k < n; k++) {
         real coef = matrix[k][i] / matrix[i][i];
         for (int j = i; j < n; j++)
            matrix[k][j] -= coef * matrix[i][j];
         b_vec[k] -= coef * b_vec[i];
      }
   }

   for (int i = n - 1; i >= 0; i--) {
      real sum = 0;
      for (int j = n - 1; j > i; j--)
         sum += matrix[i][j] * x[j];
      x[i] = (b_vec[i] - sum) / matrix[i][i];
   }
}



int main() {
   FILE* inmat, * invec;

   matrix matrix;
   fopen_s(&inmat, "inmat.txt", "r");
   fopen_s(&invec, "invec.txt", "r");

   matrix.input(inmat, invec);
   //matrix.calcLU();

   matrix.gauss();/*
   for (int i = 0; i < matrix.n; i++) {
      for (int j = 0; j < i + 1; j++) {
         printf("%f ", matrix.al[i][j]);
      }
      printf("\n");
   }
   printf("\n");

   for (int i = 0; i < matrix.n; i++) {
      for (int j = i; j < matrix.n; j++) {
         printf("%f ", matrix.au[i][j]);
      }
      printf("\n");
   }*/
   //printf(realin, matrix.y[1]);
   /*for (int i = 0; i < matrix.n; i++) {
      for (int j = 0; j < matrix.n; j++) {
         printf("%f ", matrix.mat[i][j]);
      }
      printf("\n");
   }*/
   //comp();
   for (int j = 0; j < matrix.n; j++) {
      printf("%f \n", matrix.x[j]);
   }
}
