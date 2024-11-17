/*#include <stdio.h>
#include <stdlib.h>

// LU-разложение для ленточной матрицы произвольной ширины
void lu_decomposition_band(double* AL, double* AU, double* Di, double* L, double* U, int n, int kl, int ku) {
   for (int j = 0; j < n; j++) {
      // Заполнение верхнетреугольной матрицы U
      for (int i = 0; i <= j && i < n; i++) {
         if (j - i <= ku) { // если j - i в пределах ширины ленты сверху
            U[j * (ku + kl + 1) + (j - i)] = Di[j];
            for (int m = 0; m < i && (j - m <= ku); m++) {
               U[j * (ku + kl + 1) + (j - i)] -= L[i * (ku + kl + 1) + (i - m)] * U[m * (ku + kl + 1) + (j - m)];
            }
         }
      }

      // Заполнение нижнетреугольной матрицы L
      for (int i = j + 1; i < n; i++) {
         if (i - j <= kl) { // если i - j в пределах ширины ленты снизу
            L[i * (ku + kl + 1) + (i - j)] = AL[i * kl + (i - j)];
            for (int m = 0; m < j && (i - m <= kl); m++) {
               L[i * (ku + kl + 1) + (i - j)] -= L[i * (ku + kl + 1) + (i - m)] * U[m * (ku + kl + 1) + (j - m)];
            }
            L[i * (ku + kl + 1) + (i - j)] /= U[j * (ku + kl + 1) + (j - j)];
         }
      }
   }
}

// Вывод ленточной матрицы
void print_band_matrix(double* matrix, int n, int kl, int ku) {
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         if (abs(i - j) <= kl || abs(i - j) <= ku) {
            printf("%lf ", matrix[i * (kl + ku + 1) + j]);
         }
         else {
            printf("0.000000 ");
         }
      }
      printf("\n");
   }
}

int main() {
   int n = 4;
   int kl = 3; // количество поддиагональных элементов (ширина ленты снизу)
   int ku = 3; // количество наддиагональных элементов (ширина ленты сверху)

   double AL[] = { 5, 8 ,7 ,4 ,3 ,2 }; // Элементы ниже главной диагонали
   double AU[] = { 2, 3, 4, 7, 8, 5 }; // Элементы выше главной диагонали
   double Di[] = { 1, 6, 6, 1 }; // Элементы главной диагонали

   double* L = (double*)calloc(n * (kl + ku + 1), sizeof(double));
   double* U = (double*)calloc(n * (kl + ku + 1), sizeof(double));

   lu_decomposition_band(AL, AU, Di, L, U, n, kl, ku);

   printf("L:\n");
   print_band_matrix(L, n, kl, ku);
   printf("U:\n");
   print_band_matrix(U, n, kl, ku);

   free(L);
   free(U);

   return 0;
}
*/


#include <stdio.h>
#include <stdlib.h>

void lu_decomposition(double** L, double** U, double* D, int n, int m) {
   double** A = (double**)malloc(n * sizeof(double*));
   for (int i = 0; i < n; i++) {
      A[i] = (double*)malloc(n * sizeof(double));
      for (int j = 0; j < n; j++) {
         if (j > i) {
            A[i][j] = U[j][i + m - j];
            U[j][i + m - j] = 0.0;
         }
         else {
            A[i][j] = L[i][j + m - i];
            L[i][j + m - i] = 0.0;
         }
         A[i][i] = D[i];
         //L[i][i] = 1.0;
         printf("%f   ", A[i][j]);
      }
      printf("\n");
   }

   for (int j = 0; j < n; j++) {
      // Заполнение верхнетреугольной матрицы U
      for (int i = 0; i <= j; i++) {
         U[i][j] = A[i][j];

         for (int k = 0; k < i; k++) {
            U[i][j] -= L[i][k] * U[k][j];
         }

      }
      // Заполнение нижнетреугольной матрицы L
      for (int i = j + 1; i < n; i++) {
         L[i][j] = A[i][j];
         for (int k = 0; k < j; k++) {
            L[i][j] -= L[i][k] * U[k][j];
         }

         L[i][j] /= U[j][j];
      }
   }


   /*
   for (int j = 0; j < n; j++) {
      // Заполнение верхнетреугольной матрицы U
      for (int i = 0; i < j; i++) {
         U[i][j] = A[i][j];
         float sumU = 0;
         for (int k = 0; k < i; k++) {
            sumU += L[i][k] * U[k][j];
         }
         U[i][j] = U[i][j] - sumU;
         if (i < j) {
            U[i][j] = 0.0; // Элементы ниже главной диагонали равны нулю
         }
      }
      // Заполнение нижнетреугольной матрицы L
      for (int i = j; i < n; i++) {
         L[i][j] = A[i][j];
         float sumL = 0;
         for (int k = 0; k < j; k++) {
            sumL += L[i][k] * U[k][j];
         }
         L[i][j] = L[i][j] - sumL;

         L[i][j] /= D[j];
      }
   }*/
}

/*
void lu_decomposition1(double** L, double** U, double* D, int n, int m) {
   for (int j = 0; j < n; j++) {
      // Заполнение верхнетреугольной матрицы U
      for (int i = 0; i <= j; i++) {
         //U[i][j] = A[i][j];
         float sumU = 0;
         for (int k = 0; k < i; k++) {
            sumU += L[i][k + m - i] * U[j][k + m - j];
            printf("%d sumU: %f %d %d al: %f au: %f\n", k, sumU, i, j, L[i][k + m - i], U[j][k + m - j]);

         }
         printf("%d %d sumU: %f \n", i, j, sumU);

         //U[i][j] = U[i][j] - sumU;
         U[i][j] = U[j][i + m - j] - sumU;
         printf("U[%d][%d]: %f\n\n\n", i, j, U[i][j]);


         if (i > j) {
            U[i][j] = 0.0; // Элементы ниже главной диагонали равны нулю
         }
      }
      // Заполнение нижнетреугольной матрицы L
      for (int i = j + 1; i < n; i++) {
         //L[i][j] = A[i][j];
         float sumL = 0;
         for (int k = 0; k < j; k++) {
            sumL += L[i][k + m - i] * U[j][k + m - j];
            printf("%d sumL: %f %d %d al: %f au: %f\n", k, sumL, i, j, L[i][k + m - i], U[j][k + m - j]);
         }
         printf("%d %d sumL: %f \n\n", i, j, sumL);

         L[i][j] = (L[i][j + m - i] - sumL) / D[i];
         printf("L[%d][%d]: %f\n\n\n", i, j, L[i][j]);

         //L[i][j] = L[i][j] - sumL;

         //L[i][j] /= U[j][j];
      }
   }
}*/

void print_matrix(double** matrix, int n) {
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         printf("%lf ", matrix[i][j]);
      }
      printf("\n");
   }
}

int main() {
   int n = 4;
   int m = 3;
   double** A = (double**)malloc(n * sizeof(double*));
   double** L = (double**)malloc(n * sizeof(double*));
   double** U = (double**)malloc(n * sizeof(double*));
   double* D = (double*)malloc(n * sizeof(double));

   for (int i = 0; i < n; i++) {
      A[i] = (double*)malloc(n * sizeof(double));
      L[i] = (double*)malloc(n * sizeof(double));
      U[i] = (double*)malloc(n * sizeof(double));
      for (int j = 0; j < n; j++) {
         if (i == j) {
            L[i][j] = 1.0; // Единичная диагональ для L
         }
         else {
            L[i][j] = 0.0;
         }
         U[i][j] = 0.0;
      }
   }

   L[1][2] = 5;
   L[2][1] = 8;
   L[2][2] = 7;
   L[3][0] = 4;
   L[3][1] = 3;
   L[3][2] = 2;

   U[1][2] = 2;
   U[2][1] = 3;
   U[2][2] = 7;
   U[3][0] = 4;
   U[3][1] = 8;
   U[3][2] = 5;

   D[0] = 1;
   D[1] = 6;
   D[2] = 6;
   D[3] = 1;
   // Пример матрицы A
   /*
   A[0][0] = 1; A[0][1] = 2; A[0][2] = 3; A[0][3] = 4;
   A[1][0] = 5; A[1][1] = 6; A[1][2] = 7; A[1][3] = 8;
   A[2][0] = 8; A[2][1] = 7; A[2][2] = 6; A[2][3] = 5;
   A[3][0] = 4; A[3][1] = 3; A[3][2] = 2; A[3][3] = 1;
   */
   lu_decomposition(L, U, D, n, m);

   printf("L:\n");
   print_matrix(L, n);
   printf("U:\n");
   print_matrix(U, n);

   // Освобождение памяти
   for (int i = 0; i < n; i++) {
      free(A[i]);
      free(L[i]);
      free(U[i]);
   }
   free(A);
   free(L);
   free(U);

   return 0;
}

/*#include <iostream>

void comp1()
{
   std::cout << "Hello World!\n";

   int nn = 4;
   double A[4][4] = {
   { 1, 2, 3, 4 },
   { 5, 6, 7, 8 },
   { 8, 7, 6, 5 },
   { 4, 3, 2, 1 }
   };
   double L[4][4] = { 0 };
   double U[4][4] = { 0 };
   double LL[4][4] = { 0 };
   double UU[4][4] = { 0 };

   //U[4][4] = A[4][4];


   for (int i = 0; i < nn; i++)
   {
      L[i][i] = 1.0;
      printf("\n");
   }
   printf("--------------------------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         U[i][j] = A[i][j];
         //printf("%f  ", U[i][j]);
         printf("%f  +", U[i][j]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("\n");
   printf("--------------------------------------\n");


   for (int ij = 0; ij < nn - 1; ij++)
   {
      for (int i = ij + 1; i < nn; i++)
      {
         L[i][ij] = U[i][ij]; // / U[ij][ij];
         for (int j = ij; j < nn; j++)
         {
            printf("j: %d : ", j);
            double sum = 0.0;

            sum += L[i][ij] * U[ij][j];
            printf("#  %f   %f #  ", L[i][ij], U[ij][j]);
            printf(":: %f  ::", U[i][j]);

            U[i][j] = U[i][j] - sum / U[ij][ij];
            printf(" -> %f  ", U[i][j]);
            //printf("%f  [ %f ] ", U[i][j], sum);
            printf("*\n");
         }
         L[i][ij] = L[i][ij] / U[ij][ij];
         printf("***   %f\n", U[ij][ij]);
         //return;
      }
      printf("----------------- %d %f ---------------------\n", ij, L[ij][ij]);
      printf("\n");
   }


   printf("\n");
   printf("--------------------------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         if (j <= i)  printf("%8.4f  ", L[i][j]);
         if (j >= i) printf("%8.4f  ", U[i][j]);
      }
      printf("\n");
   }

}

void comp2()
{
   int nn = 4;
   double A[4][4] = {
   { 1, 2, 3, 4 },
   { 5, 6, 7, 8 },
   { 8, 7, 6, 5 },
   { 4, 3, 2, 1 }
   };
   double L[4][4] = { 0 };
   double U[4][4] = { 0 };
   double LL[4][4] = { 0 };
   double UU[4][4] = { 0 };

   //U[4][4] = A[4][4];

   for (int i = 0; i < nn; i++)
   {
      L[i][i] = 1.0;
   }
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         U[i][j] = A[i][j];
      }
   }

   //цикл 2
   for (int k = 0; k < nn - 1; k++)
   {
      for (int i = k + 1; i < nn; i++)
      {
         L[i][k] = U[i][k]; // / U[k][k];
         for (int j = k; j < nn; j++)
         {
            double sum = 0.0;

            sum += L[i][k] * U[k][j];

            U[i][j] = U[i][j] - sum / U[k][k];
         }
         L[i][k] = L[i][k] / U[k][k];
         //return;
      }
   }

   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         printf("%8.4f  ", L[i][j]);

      }
      printf("\n");
   }

   printf("\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         printf("%8.4f  ", U[i][j]);
      }
      printf("\n");
   }
}
int main()
{
   comp2();
}
*/
