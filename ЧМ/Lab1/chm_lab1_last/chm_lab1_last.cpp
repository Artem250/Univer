#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <math.h>
//#include <vector>
#include <algorithm>
#include <string>
#include <stdio.h>

#define enter printf("\n")

#define FF
#ifdef FF
typedef float real, realsum;
#define realout "%.7f "
#define realin "%f"
#endif

#ifdef DD
typedef double real, realsum;
#define realout "%.15f "
#define realin "%lf"

#endif

#ifdef FD
typedef float real;
typedef double realsum;
#define realout "%.7f "
#define realin "%f"
#endif



using namespace std;

class matrix {
public:
   int n, m, k;
   real** al, ** au, ** A;
   real* di, * b, * x, * y;
   real** mat;
   //matrix() {};


   void allocateMemory();
   void calcLU();
   void calcLUkonec();
   void calcLUr();
   void calcLUwA();
   void calcLUU();
   void input(FILE* inmat, FILE* invec);
   void output(FILE* res);
   void calcY(real** al, real* b, real* y, int n);
   void calcX(real** au, real* y, real* x, int n);
   //void createGilbert(real** g, real* D, int n, int m);
   void createGilbert(real** gl, real** gu, real* di, int n, int m);

   //void Gauss();
   void gauss();
   //void gausss();
   string check(real a);
};

void matrix::input(FILE* inmat, FILE* invec) {

   fscanf_s(inmat, "%d %d", &n, &m);
   //k = 10;
   allocateMemory();

   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
         fscanf_s(inmat, realin, &al[i][j]);
      }
   }

   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
         fscanf_s(inmat, realin, &au[i][j]);
      }
   }

   for (int i = 0; i < n; i++)
      fscanf_s(inmat, realin, &di[i]);
   //di[0] += pow(10.0, -k);

   for (int i = 0; i < n; i++) {
      fscanf_s(invec, realin, &b[i]);
   }
   //b[0] += pow(10.0, -k);

   /*for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
         fscanf_s(inmat, realin, &mat[i][j]);
      }
   }*/
   fclose(inmat);
   fclose(invec);

}




void matrix::calcLUkonec()
{

   //for (int i = 0; i < n; i++)
   //{
   //   for (int j = 0; j < n; j++)
   //   {
   //      // Расчёт индексов ленточной матрицы
   //      //printf("%f | ", A[i][j]);
   //      if (j > i) {
   //         int il = j, jl = m + i - j + 0;
   //         if (il < 0 || jl < 0) continue;
   //         au[il][jl] = mat[i][j];
   //      }
   //      else if (i > j)
   //      {
   //         int il = i, jl = m - i + j + 0;
   //         if (il < 0 || jl < 0) continue;
   //         al[il][jl] = mat[i][j];
   //      }
   //      else // i=j
   //      {
   //         di[i] = mat[i][i];
   //         //printf(realout "\n", di[i]);
   //      }
   //      //printf("%d %d  ", i, j);
   //   }
   //}
   //for (int i = 0; i < n; i++) {
   //   for (int j = 0; j < m; j++) {

   //      printf(realout, al[i][j]);
   //   }
   //   printf("\n");
   //}
   //printf("\n");

   //for (int i = 0; i < n; i++) {
   //   for (int j = 0; j < m; j++) {
   //      printf(realout, au[i][j]);
   //   }
   //   printf("\n");
   //}
   //printf("\n");

   //for (int i = 0; i < n; i++) {
   //   printf(realout "\n", di[i]);
   //}

   for (int ij = 0; ij < n - 1; ij++)
   {
      int ijl = ij;
      int ilu = ij + 1;

      for (int i = ij + 1; i < n; i++)
      {
         int il = i;
         int jl = ij + m - i;
         if (il < 0 || jl < 0) continue;

         realsum sum = 0.0;
         for (int k = 0; k < ij; k++)
         {

            int kll = k + m - i;
            int klu = ij;
            if (kll < 0 || klu < 0) continue;

            int jlu = k + m - ij;
            if (jlu < 0) continue;
            sum += al[il][kll] * au[klu][jlu];

         }
         al[il][jl] = (al[il][jl] - sum) / di[ij]; // au[ijl][ijl];
      }

      int il = ilu;
      for (int j = il; j < n; j++)
      {
         int jl = il + m - j;
         if (il < 0 || jl < 0) continue;

         realsum sum = 0.0;
         for (int k = 0; k < ilu; k++)
         {
            int kll = k + m - ilu;
            int klu = j;
            if (kll < 0 || klu < 0) continue;

            int jlu = k + m - j;
            if (jlu < 0) continue;
            sum += al[il][kll] * au[klu][jlu];
         }

         if (jl == m) {
            di[il] = (di[il] - sum);
         }
         else {
            int klu = j;
            au[klu][jl] = (au[klu][jl] - sum);
         }
      }
   }
}


void matrix::calcY(real** al, real* b, real* y, int n)
{
   for (int i = 0; i < n; i++)
   {
      realsum sum = 0.0;
      for (int j = 0; j < i; j++) {
         if (j + m - i < 0) continue;
         sum += al[i][j + m - i] * y[j];
      }
      y[i] = (b[i] - sum);
      printf(realout "\n", y[i]);

   }
}

void matrix::calcX(real** au, real* y, real* x, int n)
{
   for (int i = n - 1; i >= 0; i--)
   {

      realsum sum = 0.0;
      for (int j = i + 1; j < n; j++) {
         if (i + m - j < 0) continue;
         sum += au[j][i + m - j] * x[j];
      }
      x[i] = (y[i] - sum) / di[i];
   }
   for (int i = 0; i < n; i++)
      printf(realout "\n", x[i]);
}



void matrix::createGilbert(real** gl, real** gu, real* di, int n, int m)
{
   m = n - 1;
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < i; j++)
      {
         if (j < 0) gl[i][j + m - i] = gu[i][j + m - i] = 0;
         else gl[i][j + m - i] = gu[i][j + m - i] = 1.0 / (i + j + 1);
      }
      di[i] = 1.0 / (i + i + 1);
   }
}

void matrix::allocateMemory() {
   al = new real * [n];
   for (int i = 0; i < n; i++) {
      al[i] = new real[m]{ 0 };
   }
   au = new real * [n];
   for (int i = 0; i < n; i++) {
      au[i] = new real[m]{ 0 };
   }
   mat = new real * [n];
   for (int i = 0; i < n; ++i) {
      mat[i] = new real[n];
   }
   di = new real[n]{ 0 };
   b = new real[n]{ 0 };
   x = new real[n]{ 0 };
   y = new real[n]{ 0 };
}

void matrix::gauss()
{
   real** matrix = new real * [n];
   real* b_vec = new real[n];



   for (int i = 0; i < n; i++) {
      b_vec[i] = b[i];
      matrix[i] = new real[n];

      for (int j = 0; j < n; j++) {
         if (j + m - i < 0 || i + m - j < 0)
            matrix[i][j] = 0.0;
         else if (j > i)  matrix[i][j] = au[j][i + m - j];
         else  matrix[i][j] = al[i][j + m - i];
         matrix[i][i] = di[i];
      }
   }

   for (int i = 0; i < n; i++) {
      int leadIndex = i;
      real lead = matrix[i][i];
      for (int k = i + 1; k < n; k++)
         if (abs(matrix[k][i]) > abs(lead))
         {
            lead = matrix[k][i];
            leadIndex = k;
         }
      std::swap(matrix[i], matrix[leadIndex]);

      std::swap(b_vec[i], b_vec[leadIndex]);
      for (int k = i + 1; k < n; k++) {
         real m_i = matrix[k][i] / matrix[i][i];
         for (int j = i; j < n; j++)
            matrix[k][j] -= m_i * matrix[i][j];
         b_vec[k] -= m_i * b_vec[i];
      }
   }

   for (int i = n - 1; i >= 0; i--) {
      real sum = 0;
      for (int j = n - 1; j > i; j--)
         sum += matrix[i][j] * x[j];
      x[i] = (b_vec[i] - sum) / matrix[i][i];
      //printf("\n%.12f", x[i]);
   }


   for (int i = 0; i < n; i++)
      printf("\n" realout, x[i]);
}





int main() {
   FILE* inmat, * invec;

   matrix matrix{};
   int num = 1;

   /*fopen_s(&inmat, "inmat.txt", "r");
    fopen_s(&invec, "invec.txt", "r");
    matrix.input(inmat, invec);
    matrix.calcLUkonec();

    matrix.calcY(matrix.al, matrix.b, matrix.y, matrix.n);
    enter;
    matrix.calcX(matrix.au, matrix.y, matrix.x, matrix.n);*/
    /*for (int i = 0; i < matrix.n; i++) {
       for (int j = 0; j < matrix.m; j++) {

          printf(realout, matrix.al[i][j]);
       }
       printf("\n");
    }
    printf("\n");

    for (int i = 0; i < matrix.n; i++) {
       for (int j = 0; j < matrix.m; j++) {
          printf(realout, matrix.au[i][j]);
       }
       printf("\n");
    }
    printf("\n");

    for (int i = 0; i < matrix.n; i++) {
       printf(realout "\n", matrix.di[i]);
    }*/


   switch (num) {
   case 1:
      printf("\n\n\nN4:\n");
      for (matrix.k = 0; matrix.k < 8; matrix.k++)
      {
         fopen_s(&inmat, "inmat.txt", "r");
         fopen_s(&invec, "invec.txt", "r");
         matrix.input(inmat, invec);
         matrix.calcLUkonec();

         matrix.calcY(matrix.al, matrix.b, matrix.y, matrix.n);

         matrix.calcX(matrix.au, matrix.y, matrix.x, matrix.n);

      }
      break;
   case 2:
      //printf("Matrix size: ");
      //scanf_s("%d", &matrix.n);
      printf("\n\n\nN5:\n");
      for (matrix.n = 2; matrix.n < 9; matrix.n++) {
         matrix.m = matrix.n - 1;
         matrix.allocateMemory();
         matrix.createGilbert(matrix.al, matrix.au, matrix.di, matrix.n, matrix.m);

         ///printf("///D:\n");
         matrix.calcLUkonec();


         real  bbb[12][12] = { {2.0,7.0 / 6},
         { 3.0,23.0 / 12,43.0 / 30 },
         { 4.0, 163.0 / 60,21.0 / 10,241.0 / 140 },
         { 5.0, 71.0 / 20, 197.0 / 70, 657.0 / 280, 1271.0 / 630 },
         { 6.0,617.0 / 140,499.0 / 140,2531.0 / 840,1649.0 / 630,12847.0 / 5544 },
         { 7.0,1479.0 / 280,5471.0 / 1260,3119.0 / 840,22549.0 / 6930,16081.0 / 5544,157309.0 / 60060 },
         { 8.0,15551.0 / 2520,6479.0 / 1260,41029.0 / 9240,27169.0 / 6930,253405.0 / 72072,191629.0 / 60060,30089.0 / 10296 },
         { 9.0,17819.0 / 2520,82609.0 / 13860,47959.0 / 9240,415567.0 / 90090,299737.0 / 72072,45533.0 / 12012,71761.0 / 20592,988277.0 / 306306 },
         { 10.0,221209.0 / 27720,94159.0 / 13860,715867.0 / 120120,479917.0 / 90090,347785.0 / 72072,106081.0 / 24024,1425857.0 / 350064,386149.0 / 102102,18266753.0 / 5173168 },
         { 11.0,246619.0 / 27720,1376527.0 / 180180,810247.0 / 120120,545983.0 / 90090,794669.0 / 144144,2067641.0 / 408408,546595.0 / 116688,8459953.0 / 1939938,105559977.0 / 25865840,89301449.0 / 23279256 },
         { 12.0,3538687.0 / 360360,1530967.0 / 180180,906343.0 / 120120,1227101.0 / 180180,15239101.0 / 2450448,779971.0 / 136136,11785561.0 / 2217072,48119579.0 / 9699690,120340457.0 / 25865840,101999225.0 / 23279256,2015839999.0 / 486748080 }
         };

         matrix.b = bbb[matrix.n - 2];
         matrix.calcY(matrix.al, matrix.b, matrix.y, matrix.n);
         matrix.calcX(matrix.au, matrix.y, matrix.x, matrix.n);

      }
      break;
   case 3:
      printf("\n\n\nN7:\n");
      for (matrix.k = 0; matrix.k < 15; matrix.k++)
      {
         fopen_s(&inmat, "inmat.txt", "r");
         fopen_s(&invec, "invec.txt", "r");
         matrix.input(inmat, invec);
         matrix.gauss();
      }
      break;
   default:
      return 0;
   }
}


