// LUdecomp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

void f() {}

#define index(i,j,n) {i=1; j=n};





void LU4()
{
   //std::cout << "Hello World!\n";

   const int NN = 6, MM = 3;
   int nn = NN, mm = MM;
   /*double A[NN][NN] = {
   { 1, 7, 6, 0 },
   { 5, 8, 2, 3 },
   { 9, 3, 4, 1 },
   { 0, 2, 7, 6 }
   };*/
   /*double A[NN][NN] = {
   { 1, 7, 6, 3, 5, 0 },
   { 5, 8, 2, 3, 2, 9 },
   { 9, 3, 4, 1, 8, 1 },
   { 8, 2, 1, 6, 9, 10 },
   { 7, 6, 7, 4, 5, 15 },
   { 0, 8, 20, 2, 12, 3 }
   };*/
   /*double A[NN][NN] = {
      {1,  2,  3,  4,  0,  0 },
      {5,  6,  7,  8,  9,  0},
     {10, 11, 12, 13, 14, 15},
     {16, 17, 18, 19, 20, 21},
     {0,  22, 23, 24, 25, 26},
     {0,  0,  27, 28, 29, 30}

   };*/
   double A[NN][NN] = {
   { 1, 7, 6, 3, 0, 0 },
   { 5, 8, 2, 3, 2, 0 },
   { 9, 3, 4, 1, 8, 1 },
   { 8, 2, 1, 6, 9, 10 },
   { 0, 6, 7, 4, 5, 15 },
   { 0, 0, 20, 2, 12, 3 }
   };
   double L[NN][NN - 2] = { 0 };
   double U[NN][NN - 2] = { 0 };
   double D[NN] = { 0 };
   //double UU[NN][NN] = { 0 };


   for (int i = 0; i < nn; i++)
   {
      //L[i][i] = 1.0;
      //printf("\n");
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
   for (int ij = 0; ij < nn - 1; ij++)
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

      printf("===============================Lij %d======================================\n", ij);
      for (int i = ij + 1; i < nn; i++)
      {
         int il = i;
         int jl = mm - i + ij + 0;
         if (il < 0 || jl < 0) continue;
         //if (jl > 3) continue;

         double sum = 0.0;
         printf("{\n");
         // суммирование членов Lij*Uij
         for (int k = 0; k < ij; k++)
            //for (int k = 0; k < i-1; k++)
         {
            //int kl = k;
            //printf("j: %d : ", jl);

            // L
            int kll = mm - i + k + 0;
            // U
            int klu = ij;
            if (kll < 0 || klu < 0) continue;

            int jlu = mm + k - ij;
            if (jlu < 0) continue;
            sum += L[il][kll] * U[klu][jlu];
            printf("[i %d  %d  %d  %d  %d  %d i] ", il, kll, klu, jlu, k, i);
            printf("[[ %f   %f   %f ]] \n", sum, L[il][kll], U[klu][jlu]);

         }
         printf("}\n");
         L[il][jl] = (L[il][jl] - sum) / D[ij]; // U[ijl][ijl];
         printf(":: %f   %f   %f  %d  %d::", L[il][jl], 0.0, D[ij], il, ijl);
         printf("}\n");
      }
      printf("=======================================================================\n");
      //return;

      printf("\n");
      printf("\n");
      printf("\n");

      printf("===================================Uij %d===================================\n", ij);

      // Цикл по строке - ищем Uij
      // ===============================================================================
      int il = ilu;
      for (int j = il; j < nn; j++)
      {
         int jl = mm + il - j;
         if (il < 0 || jl < 0) continue;
         //int il = j, jl = mm + i - j + 0;
         //if (jl >= nn) continue;
         //printf("j: %d : ", jl);

         double sum = 0.0;
         printf("{\n");
         // суммирование членов Lij*Uij
         for (int k = 0; k < ilu; k++)
            //for (int k = 0; k < j; k++)
         {
            // L
            int kll = mm + k - ilu + 0;
            // U
            int klu = j;
            if (kll < 0 || klu < 0) continue;

            int jlu = mm + k - j;
            if (jlu < 0) continue;
            sum += L[il][kll] * U[klu][jlu];
            printf("[u %d  %d  %d  %d  %d  %d  %d u] ", il, jl, jlu, kll, klu, k, j);
            printf("[ %f  %f  %f ] ", sum, L[il][kll], U[klu][jlu]);
            printf("}\n");
         }
         printf("}\n");
         //il = j;
         //jl = mm + ij + 1 - j;

         printf("iljl: % d % d  : % d % f\n", il, jl, j, sum);
         if (jl == mm) {
            printf("DDDD   %d  %f  %f\n", il, sum, D[il]);
            D[il] = (D[il] - sum);
            printf("DDDD   %d  %f\n", il, D[il]);
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
      printf("\n");
      printf("\n");
      printf("\n");
   }
   printf("\n");

   printf("-------------------END-------------------\n");



   printf("--------------------L------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         //int il = i, jl = j;
         // L
         if (i < j) { printf("%f  | ", 0.0); continue; }
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



void LU3()
{
   std::cout << "Hello World!\n";

   const int NN = 6, M = 3;
   int nn = NN, mm = M;
   /*double A[NN][NN] = {
   { 1, 7, 6, 0 },
   { 5, 8, 2, 3 },
   { 9, 3, 4, 1 },
   { 0, 2, 7, 6 }
   };*/
   /*double A[NN][NN] = {
   { 1, 7, 6, 3, 5, 0 },
   { 5, 8, 2, 3, 2, 9 },
   { 9, 3, 4, 1, 8, 1 },
   { 8, 2, 1, 6, 9, 10 },
   { 7, 6, 7, 4, 5, 15 },
   { 0, 8, 20, 2, 12, 3 }
   };*/
   double A[NN][NN] = {
   {1,  2,  3,  4,  0,  0 },
   {5,  6,  7,  8,  9,  0},
   {16, 17, 18, 19, 20, 21},
   {10 ,11 ,12 ,13 ,14, 15},
   {0,  22, 23, 24, 25, 26},
   {0,  0,  27, 28, 29, 30}

   };
   double L[NN][NN] = { 0 };
   double U[NN][NN] = { 0 };
   double D[NN] = { 0 };
   //double UU[NN][NN] = { 0 };


   for (int i = 0; i < nn; i++)
   {
      L[i][i] = 1.0;
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
         int il = i, jl = j;
         printf("%f  +", A[il][jl]);

         if (j > i) {
            U[il][jl] = A[il][jl];
         }
         else if (i > j)
         {
            L[il][jl] = A[il][jl];
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





   printf("--------------------------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         int il = i, jl = j;
         printf("%f  +", U[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("\n");
   printf("--------------------------------------\n");


   printf("--------------------------------------\n");

   // Цикл по диагонали
   for (int ij = 0; ij < nn; ij++)
   {
      int ijl = ij;

      /*for (int j = 0; j <= ij; j++)
      {
          int jl = j;
          U[ijl][ijl] = A[ijl][ijl] - sum;
      }*/

      // Цикл по строкам ij столбца
      printf("======================Цикл по строкам ij столбца=====================\n");
      for (int i = ij + 1; i < nn; i++)
      {
         int il = i;
         int jl = ijl;
         double sum = 0.0;
         printf("{\n");
         for (int k = 0; k < jl; k++)
         {
            int kl = k;
            //printf("j: %d : ", jl);

            sum += L[il][kl] * U[kl][jl];
            printf("[[ %f   %f   %f ]] \n", sum, L[il][kl], U[kl][jl]);

         }
         printf("}\n");
         L[il][jl] = (L[il][jl] - sum) / D[ij]; // U[ijl][ijl];
         printf(":: %f   %f   %f  %d  %d::", L[il][jl], A[il][jl], D[ij], il, ijl);
      }
      printf("======================================================================\n");

      int il = ij + 1;
      for (int j = ij + 1; j < nn; j++)
      {
         int jl = j;
         double sum = 0.0;
         printf("{\n");
         for (int k = 0; k < il; k++)
         {
            int kl = k;
            //printf("j: %d : ", jl);

            sum += L[il][kl] * U[kl][jl];
            printf("[ %f ] ", sum);

         }
         printf("}\n");
         if (il == jl) {
            D[il] = (D[il] - sum);
            //D[il] = (A[il][jl] - sum);
            //D[il] = U[il][jl];
         }
         else {
            U[il][jl] = (U[il][jl] - sum);
            printf("#  %f   %f   %f #  ", U[il][jl], A[il][jl], sum);
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

   for (int i = 0; i < nn; i++)
   {
      //L[i][i] = 1.0;
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("--------------------L------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         int il = i, jl = j;
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
         int il = i, jl = j;
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



void LU2()
{
   std::cout << "Hello World!\n";

   int nn = 4;
   double A[4][4] = {
   { 1, 7, 6, 4 },
   { 5, 8, 2, 3 },
   { 9, 3, 4, 1 },
   { 8, 2, 7, 6 }
   };
   double L[4][4] = { 0 };
   double U[4][4] = { 0 };
   double D[4] = { 0 };
   //double UU[4][4] = { 0 };


   for (int i = 0; i < nn; i++)
   {
      L[i][i] = 1.0;
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
         int il = i, jl = j;
         printf("%f  +", A[il][jl]);

         if (j > i) {
            U[il][jl] = A[il][jl];
         }
         else if (i > j)
         {
            L[il][jl] = A[il][jl];
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





   printf("--------------------------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         int il = i, jl = j;
         printf("%f  +", U[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("\n");
   printf("--------------------------------------\n");


   printf("--------------------------------------\n");

   // Цикл по диагонали
   for (int ij = 0; ij < 3; ij++)
   {
      int ijl = ij;

      /*for (int j = 0; j <= ij; j++)
      {
          int jl = j;
          U[ijl][ijl] = A[ijl][ijl] - sum;
      }*/

      // Цикл по строкам ij столбца
      printf("======================Цикл по строкам ij столбца=====================\n");
      for (int i = ij + 1; i < nn; i++)
      {
         int il = i;
         int jl = ijl;
         double sum = 0.0;
         printf("{\n");
         for (int k = 0; k < jl; k++)
         {
            int kl = k;
            //printf("j: %d : ", jl);

            sum += L[il][kl] * U[kl][jl];
            printf("[[ %f   %f   %f ]] \n", sum, L[il][kl], U[kl][jl]);

         }
         printf("}\n");
         L[il][ijl] = (L[il][ijl] - sum) / D[ij]; // U[ijl][ijl];
         printf(":: %f   %f   %f  %d  %d::", L[il][ijl], A[il][ijl], D[ij], il, ijl);
      }
      printf("======================================================================\n");

      int il = ij + 1;
      for (int j = ij + 1; j < nn; j++)
      {
         int jl = j;
         double sum = 0.0;
         printf("{\n");
         for (int k = 0; k < il; k++)
         {
            int kl = k;
            //printf("j: %d : ", jl);

            sum += L[il][kl] * U[kl][jl];
            printf("[ %f ] ", sum);

         }
         printf("}\n");
         if (il == jl) {
            D[il] = (D[il] - sum);
            //D[il] = (A[il][jl] - sum);
            //D[il] = U[il][jl];
         }
         else {
            U[il][jl] = (U[il][jl] - sum);
            printf("#  %f   %f   %f #  ", U[il][jl], A[il][jl], sum);
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

   for (int i = 0; i < nn; i++)
   {
      L[i][i] = 1.0;
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("--------------------L------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         int il = i, jl = j;
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
         int il = i, jl = j;
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



void LU1()
{
   std::cout << "Hello World!\n";

   int nn = 4;
   double A[4][4] = {
   { 1, 7, 6, 4 },
   { 5, 8, 2, 3 },
   { 9, 3, 4, 1 },
   { 8, 2, 7, 6 }
   };
   double L[4][4] = { 0 };
   double U[4][4] = { 0 };
   double D[4] = { 0 };
   //double UU[4][4] = { 0 };


   for (int i = 0; i < nn; i++)
   {
      L[i][i] = 1.0;
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
         int il = i, jl = j;
         printf("%f  +", A[il][jl]);

         if (j > i) {
            U[il][jl] = A[il][jl];
         }
         else if (i > j)
         {
            L[il][jl] = A[il][jl];
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





   printf("--------------------------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         int il = i, jl = j;
         printf("%f  +", U[il][jl]);
         //printf("%d %d  ", i, j);
      }
      printf("\n");
   }
   printf("\n");
   printf("--------------------------------------\n");


   printf("--------------------------------------\n");

   // Цикл по диагонали
   for (int ij = 0; ij < 3; ij++)
   {
      int ijl = ij;

      /*for (int j = 0; j <= ij; j++)
      {
          int jl = j;
          U[ijl][ijl] = A[ijl][ijl] - sum;
      }*/

      // Цикл по строкам ij столбца
      for (int i = ij + 1; i < nn; i++)
      {
         int il = i;
         int jl = ijl;
         double sum = 0.0;
         printf("{\n");
         for (int k = 0; k < jl; k++)
         {
            int kl = k;
            //printf("j: %d : ", jl);

            sum += L[il][kl] * U[kl][jl];
            printf("[[ %f   %f   %f ]] \n", sum, L[il][kl], U[kl][jl]);

         }
         printf("}\n");
         L[il][ijl] = (A[il][ijl] - sum) / D[ij]; // U[ijl][ijl];
         printf(":: %f   %f   %f  %d  %d::", L[il][ijl], A[il][ijl], D[ij], il, ijl);
      }
      printf("===========================================\n");

      int il = ij + 1;
      for (int j = ij + 1; j < nn; j++)
      {
         int jl = j;
         double sum = 0.0;
         printf("{\n");
         for (int k = 0; k < il; k++)
         {
            int kl = k;
            //printf("j: %d : ", jl);

            sum += L[il][kl] * U[kl][jl];
            printf("[ %f ] ", sum);

         }
         printf("}\n");
         if (il == jl) {
            D[il] = (A[il][jl] - sum);
            //D[il] = U[il][jl];
         }
         else {
            U[il][jl] = (A[il][jl] - sum);
            printf("#  %f   %f   %f #  ", U[il][jl], A[il][jl], sum);
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

   for (int i = 0; i < nn; i++)
   {
      L[i][i] = 1.0;
      printf("\n");
   }
   printf("--------------------------------------\n");

   printf("--------------------L------------------\n");
   for (int i = 0; i < nn; i++)
   {
      for (int j = 0; j < nn; j++)
      {
         int il = i, jl = j;
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
         int il = i, jl = j;
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




int main()
{
   //std::cout << "Hello World!\n";
   LU4();
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
