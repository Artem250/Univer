#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;
typedef float real;
#define realin "%f"
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
   void calcLUr();
   void calcLUwA();
   void calcLUU();
   void input(FILE* inmat, FILE* invec);
   void toLent();
   void output(FILE* res);
   void calcY(real** al, real* b, real* y, int n);
   void calcX(real** au, real* y, real* x, int n);
   void createGilbert(double** g, double* D, int n, int m);
   //void Gauss();
   void gauss();
   //void gausss();
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

   fscanf_s(inmat, "%d %d", &n, &m);
   allocateMemory();
   /*
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
   */
   for (int i = 0; i < n; i++)
      fscanf_s(invec, realin, &b[i]);
   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
         fscanf_s(inmat, realin, &mat[i][j]);
      }
   }
   fclose(inmat);
   fclose(invec);

}

void matrix::toLent() {
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         if (j > i) {
            au[j][i + m - j] = mat[i][j];
         }
         else {
            al[i][j + m - i] = mat[i][j];
         }
         if (i == j) di[i] = mat[i][j];
      }
   }
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
/*
void matrix::calcLUU() {
   for (int i = 0; i < n; i++) {
      // Вычисление диагонального элемента (вектор Di)
      real s_diag = 0;
      for (int k = 0; k < min(i, m); k++) {
         int j = i - k - 1;
         s_diag += al[i][k] * au[j][k];
      }
      di[i] = sqrt(di[i] - s_diag);

      // Вычисление элементов L (нижняя треугольная часть)
      for (int j = i + 1; j < min(i + m, n); j++) {
         real sumL = 0;
         for (int k = 0; k < min(i, m); k++) {
            int jj = i - k - 1;
            sumL += al[j][jj - j + m - 1] * au[i][k];
         }
         printf("sumL: %f %d %d\n", sumL, i, j);
         al[j][i - j + m - 1] = (al[j][i - j + m - 1] - sumL) / di[i];
      }

      // Вычисление элементов U (верхняя треугольная часть)
      for (int j = i + 1; j < min(i + m, n); j++) {
         real sumU = 0;
         for (int k = 0; k < min(i, m); k++) {
            int jj = i - k - 1;
            sumU += al[i][jj - i + m - 1] * au[j][jj - j + m - 1];
         }
         printf("sumU: %f %d %d\n", sumU, i, j);
         au[i][j - i - 1] = (au[i][j - i - 1] - sumU) / di[i];
      }
   }
}
*/
void matrix::calcLUU() {

   printf("--------------------------------------\n");
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++)
      {
         int il = i, jl = j;
         printf("%f  +", au[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("\n");
   printf("--------------------------------------\n");


   printf("--------------------------------------\n");

   // Цикл по диагонали
   for (int ij = 0; ij < 1; ij++)
   {
      int ijl = ij;
      //L: i = i
      //   j = j + m - i

      //U: i = j
      //   j = i + m - j
      /*for (int j = 0; j <= ij; j++)
      {
          int jl = j;
          U[ijl][ijl] = A[ijl][ijl] - sum;
      }*/

      // Цикл по строкам ij столбца
      printf("======================Цикл по строкам ij столбца=====================\n");
      for (int i = ij + 1; i < n; i++)
      {
         int il = i;
         int jl = i + m - ijl;
         double sum = 0.0;
         printf("{\n");
         for (int k = 0; k < jl; k++)
         {
            int kl = k;
            //printf("j: %d : ", jl);

            sum += al[il][kl + m - i] * au[kl][i + m - kl];
            printf("[[ %f   %f   %f ]] \n", sum, al[il][kl], au[kl][jl]);

         }
         printf("}\n");
         al[il][jl] = (al[il][jl] - sum) / di[ij]; // U[ijl][ijl];
         printf(":: %f   %f  %d  %d::", al[il][jl], di[ij], il, ijl);
      }
      printf("======================================================================\n");

      for (int j = ij + 1; j < n; j++)
      {
         int il = j;

         int jl = ijl + m - j;
         double sum = 0.0;
         printf("{\n");
         for (int k = 0; k < il; k++)
         {
            int kl = k;
            //printf("j: %d : ", jl);

            sum += al[il][kl] * au[kl][jl];
            printf("[ %f ] ", sum);

         }
         printf("}\n");
         if (il == jl) {
            di[il] = (di[il] - sum);
            //D[il] = (A[il][jl] - sum);
            //D[il] = U[il][jl];
         }
         else {
            au[il][jl] = (au[il][jl] - sum);
            printf("#  %f   %f #  ", al[il][jl], sum);
         }
      }
      //L[ijl][ijl] = L[i][ij] / U[il][il];
      // Диагональный массив

      //printf("#  %f   %f #  ", L[il][ijl], U[il][jl]);
      //printf(":: %f  ::", U[i][j]);
      //printf(" -> %f  ", U[i][j]);
      //printf("***   %f\n", U[ij][ij]);
      //return;
      printf("\n");

      printf("=======================================STEP========================================\n");
   }
   //printf("----------------- %d %f ---------------------\n", ij, L[ij][ij]);
   printf("\n");

   printf("-------------------END-------------------\n");


   for (int i = 0; i < n; i++)
   {
      al[i][i] = 1.0;
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("--------------------L------------------\n");
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++)
      {
         int il = i, jl = j;
         printf("%f  | ", al[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("-------------------U-------------------\n");
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++)
      {
         int il = i, jl = j;
         printf("%f  | ", au[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("-------------------D-------------------\n");
   for (int i = 0; i < n; i++)
   {
      printf("%f  ", di[i]);
      printf("\n");
   }
   printf("--------------------------------------\n");
}

/*
void matrix::calcLU() {
   A = new real * [n];

   for (int i = 0; i < n; i++) {
      A[i] = new real[m];
      for (int j = 0; j < n; j++) {

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
         printf("%f   ", A[i][j]);
      }
      printf("\n");
   }

   for (int j = 0; j < n; j++) {
      for (int i = 0; i <= j; i++) {
         au[i][j] = A[i][j];

         for (int k = 0; k < i; k++) {
            au[i][j] -= al[i][k] * au[k][j];
         }

      }

      for (int i = j + 1; i < n; i++) {
         al[i][j] = A[i][j];
         for (int k = 0; k < j; k++) {
            al[i][j] -= al[i][k] * au[k][j];
         }

         al[i][j] /= au[j][j];
      }
   }
}*/
void matrix::calcLU() {
   /*A = new real * [n];

   for (int i = 0; i < n; i++) {
      A[i] = new real[m];
      for (int j = 0; j < n; j++) {

         if (i > m && j < i - m || j > m && i < j - m) {
            A[i][j] = 0.0;
            //continue;
            printf("%f   ", A[i][j]);

         }
         //else {
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
         //printf("A:\n");
         printf("%f   ", A[i][j]);
         //}
      }
      printf("\n");
   }
   printf("\nne A:\n");
   /*
   *//*
   for (int j = 0; j < n; j++) {
      for (int i = 0; i <= j; i++) {
         au[i][j] = A[i][j];

         for (int k = 0; k < i; k++) {
            au[i][j] -= al[i][k] * au[k][j];
         }

      }

      for (int i = j + 1; i < n; i++) {
         al[i][j] = A[i][j];
         for (int k = 0; k < j; k++) {
            al[i][j] -= al[i][k] * au[k][j];
         }

         al[i][j] /= au[j][j];
      }
   }*/
   double sumD[4] = { 0, 1.5, 0, 0.6 };
   for (int j = 0; j < n; j++) {
      real sumL = 0;
      real sumU = 0;
      for (int i = 0; i < j; i++) {
         //au[i][j] = A[i][j];
         //if (i > j) continue;
         for (int k = 0; k < j; k++) {
            //au[i][j] -= al[i][k] * au[k][j];
            sumU += al[i][k + m - i] * au[j][k + m - j];
         }
         au[j][i + m - j] -= sumU;

         printf("U/// i: %d | j: %d | i + m - j: %d | au[j][i + m - j]: %f | sumU: %f\n", i, j, i + m - j, au[j][i + m - j], sumU);
      }
      for (int i = j; i < n; i++) {
         //al[i][j] = A[i][j];
         //if (i < j) continue;
         for (int k = 0; k < j + m - i; k++) {
            //al[i][j] -= al[i][k] * au[k][j];
            sumL += al[i][k + m - i] * au[j][k + m - j];
         }


         al[i][j + m - i] = (al[i][j + m - i] - sumL) / di[j];
         printf("L/// i: %d | j: %d | j + m - i: %d | al[i][j + m - i]: %f | sumL: %f   | di[j]: %f\n", i, j, j + m - i, al[i][j + m - i], sumL, di[j]);
      }
      di[j] = di[j] - sumD[j];
      printf("D/// j: %d | di[j]: %f | sumL: %f | sumD: %f\n", j, di[j], sumL, sumD[j]);
   }
}

void matrix::calcLUwA() {
   A = new real * [n];

   for (int i = 0; i < n; i++) {
      A[i] = new real[m];
      for (int j = 0; j < n; j++) {

         if (i > m && j < i - m || j > m && i < j - m) {
            A[i][j] = 0.0;
            //continue;
            printf("%f   ", A[i][j]);

         }
         //else {
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
         //printf("A:\n");
         printf("%f   ", A[i][j]);
         //}
      }
      printf("\n");
   }
   printf("\nne A:\n");


   for (int j = 0; j < n; j++) {
      real sumU = 0;
      real sumL = 0;
      real sumD = 0;
      for (int i = 0; i <= j; i++) {
         au[i][j] = A[i][j];

         for (int k = 0; k < i; k++) {
            au[i][j] -= al[i][k] * au[k][j];
         }
         //au[i][j] -= sumU;
         printf("U/// i: %d | j: %d | au[i][j]: %f | sumU: %f\n", i, j, au[i][j], sumU);

      }
      for (int i = j; i < n; i++) {
         al[i][j] = A[i][j];
         for (int k = 0; k < j; k++) {
            al[i][j] -= al[i][k] * au[k][j];
            if (i == j) sumD += al[i][k] * au[k][j];
         }

         al[i][j] /= au[j][j];
         printf("L/// i: %d | j: %d | al[i][j]: %f | sumL: %f | al[j][j]: %f | sumD: %f\n", i, j, al[i][j], sumL, au[j][j], sumD);

      }

   }
}

void matrix::calcLUr() {
   //real sumD[4] = { 0, 1.5, 0, 0.6 };
   //real sumD[6] = { 0, 10, 33, 45, 31.25, 42 };
   //real a = -(5.0 / 4 - 1.0);
   //real b = -(14.0 / 45 - 1.0 / 3);
   //real c = -(37.0 / 3360 - 1.0 / 5);
   //real sumD[4] = { 0, a, b, c };

   for (int j = 0; j < n; j++) {
      real sumU = 0;
      real sumL = 0;
      real sumD = 0;
      for (int i = 0; i < j; i++) {
         //au[i][j] = A[i][j];
         //if (j > m && i + m - j < i - m || i > m && j + m - i < j - m) continue;

         for (int k = 0; k < i; k++) {
            //au[i][j] -= al[i][k] * au[k][j];
            au[j][i + m - j] -= al[i][k + m - i] * au[j][k + m - j];
            //sumU += al[i][k + m - i] * au[j][k + m - j];
            //cout << "                                  al u au: " << al[i][k + m - i] << " " << au[j][k + m - j] << endl;
         }
         //printf("U/// i: %d | j: %d | i + m - j: %d | au[j][i + m - j]: %f | sumU: %f\n", i, j, i + m - j, au[j][i + m - j], sumU);


      }

      for (int i = j; i < n; i++) {
         //al[i][j] = A[i][j];
         //if (i > m && j < i - m || j > m && i < j - m) continue;

         for (int k = 0; k < j; k++) {
            //al[i][j] -= al[i][k] * au[k][j];
            al[i][j + m - i] -= al[i][k + m - i] * au[j][k + m - j];
            sumL += al[i][k + m - i] * au[j][k + m - j];
            if (i == j) sumD += al[i][k + m - i] * au[j][k + m - j];

            cout << "k, i, j" << k << i << j << "sumL: " << sumL << endl;

         }
         al[i][j + m - i] /= di[j];

         if (i == j) di[j] = di[j] - sumD;
      }
      //di[j] = di[j] - sumD;
   }
}

void LU4()
{
   std::cout << "Hello World!\n";

   int nn = 6, mm = 3;
   /*double A[4][4] = {
   { 1, 7, 6, 0 },
   { 5, 8, 2, 3 },
   { 9, 3, 4, 1 },
   { 0, 2, 7, 6 }
   }*/
   /*
   double A[4][4] = {
   { 2, 3, 1, 5 },
   { 1, 4, 2, 6 },
   { 3, 2, 5, 1 },
   { 4, 1, 3, 2 }
   };*/


   double A[6][6] = {
      {1, 2, 3, 4, 0, 0},
      {5, 6, 7, 8, 9, 0},
      {10, 11, 12, 13, 14, 15},
      {16, 17, 18, 19, 20, 21},
      {0, 22, 23, 24, 25, 26},
      {0, 0, 27, 28, 29, 30},
   };
   double L[6][6] = { 0 };
   double U[6][6] = { 0 };
   double D[6] = { 0 };
   //double UU[4][4] = { 0 };


   for (int i = 0; i < nn; i++)
   {
      //L[i][i] = 1.0;
      printf("\n");
   }
   printf("--------------------------------------\n");
   // Инициализация массивов
   // При этом первая строка массива U уже запонена необходимыми элементами: u1j = a1j
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         // Расчёт индексов ленточной матрицы
         printf("%f | ", A[i][j]);

         if (j > i) {
            int il = j, jl = mm + i - j + 0;
            if (il < 0 || jl < 0) continue;
            U[il][jl] = A[i][j];
         }
         else if (i > j)
         {
            int il = i, jl = mm - i + j + 0;
            if (il < 0 || jl < 0) continue;
            L[il][jl] = A[i][j];
         }
         else // i=j
         {
            D[i] = A[i][i];
         }
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("\n");
   printf("--------------------------------------\n");


   // Печать массивов после преобразования в ленту
   printf("-------------------U-------------------\n");
   for (int il = 0; il < nn; il++)
   {
      for (int jl = 0; jl < mm; jl++)
      {
         //int il = i, jl = j;
         printf("%f | ", U[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("-------------------L-------------------\n");
   for (int il = 0; il < nn; il++)
   {
      for (int jl = 0; jl < mm; jl++)
      {
         //int il = i, jl = j;
         printf("%f | ", L[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("-------------------D-------------------\n");
   for (int i = 0; i < nn; i++)
   {
      printf("%f  ", D[i]);
      printf("\n");
   }
   printf("--------------------------------------\n");

   //return;



   printf("--------------------------------------\n");

   // Цикл по диагонали
   for (int ij = 0; ij < nn; ij++)
   {
      int ijl = ij;
      int ilu = ij + 1;

      // L
      //int il = i, jl = mm - i + j + 0;
      // U
      //int il = j, jl = mm + i - j + 0;
      //if (il < 0 || jl < 0) continue;

      // Цикл по столбцу - ищем Lij
      // ===============================================================================

      printf("===============================Lij======================================\n");
      for (int i = ij + 1; i < nn; i++)
      {
         int il = i;
         int jl = mm - i + ij + 0;
         //if (il < 0 || jl < 0) continue;
         //if (jl > 3) continue;

         double sum = 0.0;
         printf("{\n");
         // суммирование членов Lij*Uij
         for (int k = 0; k < ij; k++)
         {
            //int kl = k;
            //printf("j: %d : ", jl);

            // L
            int kll = mm - i + k + 0;
            // U
            int klu = ij;
            //if (kll < 0 || klu < 0) continue;

            int jlu = mm + k - ij;
            sum += L[il][kll] * U[klu][jlu];
            //printf("[i %d  %d  %d  %d  %d  %d i] ", il, kll, klu, jlu, k, i);
            //printf("[[ %f   %f   %f ]] \n", sum, L[il][kll], U[klu][jlu]);

         }
         printf("}\n");
         L[il][jl] = (L[il][jl] - sum) / D[ij]; // U[ijl][ijl];
         //printf(":: %f   %f   %f  %d  %d::", L[il][jl], 0.0, D[ij], il, ijl);
         printf("}\n");
      }
      printf("=======================================================================\n");
      //return;
      printf("===================================Uij===================================\n");

      // Цикл по строке - ищем Uij
      // ===============================================================================
      int il = ilu;
      for (int j = il; j < nn; j++)
      {
         int jl = mm + il - j;
         //int il = j, jl = mm + i - j + 0;
         //if (jl >= nn) continue;
         //printf("j: %d : ", jl);

         double sum = 0.0;
         printf("{\n");
         // суммирование членов Lij*Uij
         for (int k = 0; k < ilu; k++)
         {
            // L
            int kll = mm + k - ilu + 0;
            // U
            int klu = j;
            if (kll < 0 || klu < 0) continue;

            int jlu = mm + k - j;
            sum += L[il][kll] * U[klu][jlu];
            printf("[u %d  %d  %d  %d  %d  %d u] ", il, jl, kll, klu, k, j);
            printf("[ %f  %f  %f ] ", sum, L[il][kll], U[klu][jl]);
            printf("}\n");
         }
         printf("}\n");
         //il = j;
         //jl = mm + ij + 1 - j;

         printf("iljl: % d % d  : % d % f\n", il, jl, j, sum);
         if (jl == mm) {
            D[il] = (D[il] - sum);
            printf("DDDD   %d\n", il);
         }
         else {
            int klu = j;
            U[klu][jl] = (U[klu][jl] - sum);
            //printf("#  %f   %f   %f #  ", U[il][jl], A[il][jl], sum);
            printf("[ %d  %d  %d  %d  %d ] ", il, jl, 0, j, ilu);
            printf(":: %f   %f   %f  %d  %d::", U[il][jl], 0.0, D[ij], il, ijl);
         }
      }
      // Диагональный массив
      //printf("#  %f   %f #  ", L[il][ijl], U[il][jl]);
      //printf(":: %f  ::", U[i][j]);
      //printf(" -> %f  ", U[i][j]);
      //printf("***   %f\n", U[ij][ij]);
      //return;
      printf("\n");

      printf("=======================================STEP========================================\n");
   }
   printf("\n");

   printf("-------------------END-------------------\n");



   printf("--------------------L------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < i; j++)
      {
         //int il = i, jl = j;
         // L
         int il = i, jl = mm - i + j + 0;
         printf("%f  | ", L[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("-------------------U-------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         //int il = i, jl = j;
         // U
         if (i > j) { printf("%f  | ", 0.0); continue; }
         int il = j, jl = mm + i - j + 0;
         printf("%f  | ", U[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("-------------------D-------------------\n");
   for (int i = 0; i < nn; i++)
   {
      printf("%f  ", D[i]);
      printf("\n");
   }
   printf("--------------------------------------\n");
}


void matrix::calcY(real** al, real* b, real* y, int n)
{
   for (int i = 0; i < n; i++)
   {
      double sum = 0.0;
      for (int j = 0; j < i; j++)
         sum += al[i][j + m - i] * y[j];
      y[i] = (b[i] - sum);
      //printf("\n%f", y[i]);
   }
}

void matrix::calcX(real** au, real* y, real* x, int n)
{
   printf("\n");
   for (int i = n - 1; i >= 0; i--)
   {
      double sum = 0.0;
      /*for (int j = i + 1; j < n; j++)
         sum += au[i][j] * x[j];
      x[i] = (y[i] - sum) / au[i][i];
      */
      for (int j = i + 1; j < n; j++)
         sum += au[j][i + m - j] * x[j];
      x[i] = (y[i] - sum) / di[i];
      printf("\n%f", x[i]);
   }
}

void matrix::createGilbert(double** g, double* D, int n, int m)
{
   for (int i = 0; i < n; i++)
   {
      for (int j = i - m, j1 = 0; j1 < m; j++, j1++)
      {
         if (j < 0) g[i][j1] = 0;
         else g[i][j1] = 1.0 / (i + j1 - 1);
      }
      /*for (int k = i + m, j2 = 0; j2 < m; j2++, k++)
      {
         if (k > m) g[i][j2] = 0;
         else g[i][j2] = 1.0 / (i + j2 - 1);
      }*/
      D[i] = 1.0 / (i + i - 1);
   }
}

void matrix::allocateMemory() {
   al = new real * [n];
   for (int i = 0; i < n; i++) {
      al[i] = new real[m]{ 0 };
      //for (int j = 0; j < m; j++) {
         //al[i][j] = 0.0;
         ////printf("%f ", al[i][j]);
      //}
      //printf("\n");
   }
   printf("\n");
   au = new real * [n];
   for (int i = 0; i < n; i++) {
      au[i] = new real[m];
      for (int j = 0; j < m; j++) {
         au[i][j] = 0.0;
         //printf("%f ", au[i][j]);
      }
      //printf("\n");
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

/*
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


void commp()
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
*/

/*
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
*/
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

   for (int i = 0; i < n; i++) {
      int leadIndex = i;
      double lead = matrix[i][i];
      for (int k = i + 1; k < n; k++)
         if (abs(matrix[k][i]) > abs(lead))
         {
            lead = matrix[k][i];
            leadIndex = k;
         }
      matrix[i].swap(matrix[leadIndex]);
      std::swap(b_vec[i], b_vec[leadIndex]);
      for (int k = i + 1; k < n; k++) {
         real m_i = matrix[k][i] / matrix[i][i];
         for (int j = i; j < n; j++)
            matrix[k][j] -= m_i * matrix[i][j];
         b_vec[k] -= m_i * b_vec[i];
      }

   }
   printf("\n");
   printf("\n");


   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         printf("%f   ", matrix[i][j]);

      }
      printf("\n");
   }
   for (int i = n - 1; i >= 0; i--) {
      real sum = 0;
      for (int j = n - 1; j > i; j--)
         sum += matrix[i][j] * x[j];
      x[i] = (b_vec[i] - sum) / matrix[i][i];
      printf("\n%f", x[i]);
   }
}

/*
void comp()
{
   std::cout << "Hellow World!\n";

   int n = 4;
   double A[4][4] = {
   { 1, 7, 6, 4 },
   { 5, 8, 2, 3 },
   { 9, 3, 4, 1 },
   { 8, 2, 7, 6 }
   };
   double B[4] = { 11, 7, 8, 1 };

   double X[4] = { 0 };
   double U[4][4] = { 0 };

   // Прямой ход алгоритма
   for (int k = 0; k < n - 1; k++)
   {
      //int i = k;
      double amax = A[k][k];
      int jmax = k;
      // Поиск главного элемента
      for (int j = k + 1; j < n; j++)
      {
         if (abs(A[k][j]) > abs(A[k][jmax])) { jmax = j; };
      }

      double Amax = A[k][jmax];
      for (int j = k; j < n; j++)
      {
         A[k][j] /= Amax;
      }
      B[k] /= Amax;

      // Перестановка
      int j = jmax;
      for (int ij = k; ij < 1; ij++)
      {
         // Обмен столбцов
         if (ij != j)
         {
            double a = A[ij][k];
            A[ij][k] = A[ij][j];
            A[ij][j] = a;
         }
      }
      // Обмен строк в правой части
      double b = B[k];
      B[k] = B[j];
      B[j] = b;

      // Вычитание
      for (int i = k + 1; i < n; i++) {
         double Aik = A[i][k];
         for (int j = k; j < n; j++) {
            A[i][j] -= Aik * A[k][j] / A[k][k];
         }
         B[i] -= Aik * B[k] / A[k][k];
      }
   }
   // Печать матрицы
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++) {
         //A[i][j] = 1.0;
         printf("%f  ", A[i][j]);
      }
      printf("   %f", B[i]);
      printf("\n");
   }
   printf("-------------------BBB-------------------\n");

   int k = 0;
   X[n - 1] = B[n - 1] / A[n - 1][n - 1];
   for (int i = n - 2; i > -1; i--) {
      double sum = 0;
      for (int j = i + 1; j < n; j++) {
         sum += X[j] * A[i][j];
      }
      X[i] = (B[i] - sum) / A[i][i];
   }


   for (int i = 0; i < n; i++)
   {
      double sum = 0.0;
      for (int j = 0; j < n; j++) {
         sum += A[i][j] * X[j];
      }
   }

}


void comppp()
{
   std::cout << "Hellow World!\n";

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
   for (int k = 0; k < nn - 1; k++)
   {
      printf("k  %d   k\n", k);
      //int i = k;
      //for (int i = k; i < 1; i++)
      {
         double amax = A[k][k];
         int jmax = k;
         // Поиск главного элемента
         for (int j = k + 1; j < nn; j++)
         {
            if (abs(A[k][j]) > abs(A[k][jmax])) { jmax = j; };
            //printf("%f  ", A[i][j]);
            //printf("%d %d  ", i, j);
            printf("[  %f  %d  :max]", A[k][jmax], jmax);
         }
         printf("\n");

         double Amax = A[k][jmax];
         for (int j = k; j < nn; j++)
         {
            A[k][j] /= Amax;
            //A[i][j] /= A[i][jmax];
            //printf("%f  ", A[i][j]);
            //printf("%d %d  ", i, j);
            printf("[M  %f  %d  M]", A[k][j], j);
         }
         B[k] /= Amax;
         printf("\n");

         // Перестановка
         int j = jmax;
         for (int ij = k; ij < 1; ij++)
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

         // Вычитание
         for (int i = k + 1; i < nn; i++) {
            double Aik = A[i][k];
            for (int j = k; j < nn; j++) {
               A[i][j] -= Aik * A[k][j] / A[k][k];
               //A[i][j] -= A[i][k] * A[k][j] / A[k][k];
               //printf(" %d %d %d %f %f %f %f\n", k, i,j, A[i][k], A[k][j], A[k][k], Aik);
            }
            //B[i] -= B[k];
            B[i] -= Aik * B[k] / A[k][k];
            printf(" %d %d %f %f %f\n", k, i, B[i], B[k], Aik);
            //B[i] -= A[i][k] * B[k];
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
   printf("-------------------BBB-------------------\n");
   //return;

   // Обратный ход алгоритма
   printf("--------------------Обратный ход алгоритма | Reverce steps------------------\n");
   int k = 0;
   X[nn - 1] = B[nn - 1] / A[nn - 1][nn - 1];
   for (int i = nn - 2; i > -1; i--) {
      //printf("i: %d  \n", i);
      double sum = 0;
      //for (int j = nn-1; j > i; j--) {
      for (int j = i + 1; j < nn; j++) {
         printf(" %d  %d  |  ", i, j);
         sum += X[j] * A[i][j];
         printf(" %d  %d  %f  |  ", i, j, sum);
      }
      X[i] = (B[i] - sum) / A[i][i];
      printf("\n");
   }
   for (int i = 0; i < nn; i++) {
      printf(" %d  %f  \n", i, X[i]);
   }
   printf("--------------------------------------\n");

   // Проверка - нахождение В, через А и х
   printf("--------------------Проверка - нахождение В, через А и х------------------\n");
   for (int i = 0; i < nn; i++)
   {
      double sum = 0.0;
      for (int j = 0; j < nn; j++) {
         sum += A[i][j] * X[j];
         //printf(" |  %f  | ", A[i][j]);
      }
      printf(" %f  %f", sum, B[i]);
      printf("\n");
   }
   printf("--------------------------------------\n");


}
*/





int main() {
   FILE* inmat, * invec;

   matrix matrix;
   fopen_s(&inmat, "inmat.txt", "r");
   fopen_s(&invec, "invec.txt", "r");
   //LU4();

   matrix.input(inmat, invec);
   matrix.toLent();
   //matrix.createGilbert(matrix.al, matrix.di, matrix.n, matrix.m);
   //matrix.createGilbert(matrix.au, matrix.di, matrix.n, matrix.m);

   //matrix.calcLUr();
   for (int i = 0; i < matrix.n; i++) {
      for (int j = 0; j < matrix.m; j++) {

         printf("%f ", matrix.al[i][j]);
      }
      printf("\n");
   }
   printf("\n");

   for (int i = 0; i < matrix.n; i++) {
      for (int j = 0; j < matrix.m; j++) {
         printf("%f ", matrix.au[i][j]);
      }
      printf("\n");
   }
   printf("\n");

   for (int i = 0; i < matrix.n; i++)
      printf("%f\n", matrix.di[i]);
   //matrix.gauss();
   matrix.calcY(matrix.al, matrix.b, matrix.y, matrix.n);
   matrix.calcX(matrix.au, matrix.y, matrix.x, matrix.n);


   //matrix.gauss();
   /*
   matrix.calcLU();



   matrix.calcY(matrix.al, matrix.b, matrix.y, matrix.n);

   printf("\n");
   matrix.calcX(matrix.au, matrix.y, matrix.x, matrix.n);
   */
   /*
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
   //comppp();
   //for (int j = 0; j < matrix.n; j++) {
    //  printf("%f \n", matrix.x[j]);
   //}
}