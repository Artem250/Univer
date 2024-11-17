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
#define realoutsum "%.7f "
#define realin "%f"
#define N 10
#endif

#ifdef DD
typedef double real, realsum;
#define realout "%.15f "
#define realoutsum "%.15f "
#define realin "%lf"
#define N 18
#endif

#ifdef FD
typedef float real;
typedef double realsum;
#define realout "%.7f "
#define realoutsum "%.15f "
//#define realout "%.15f "
#define realin "%f"
#define N 10
#endif



using namespace std;
//
////#define D_REAL
//#define D_REALSUM
//
//#ifdef D_REALSUM
//typedef double real_sum;
//#else
//typedef float real_sum;
//#endif
//
//#ifdef D_REAL
//typedef double real;
//#define realfmt_out "%.15lf"
//#define realfmt_in "%lf"
//#else
//typedef float real;
//#define realfmt_out "%.7f"
//#define realfmt_in "%f"
//#endif


//
//typedef double real;
//typedef double realsum;
////#define realsum double;  
//
//#define realin "%lf"
//
//#define realsum "%f" 
//#define EPS 1e-13

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
   void createGilbert(real** gl, real** gu, real* D, int n, int m);

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
   di[0] += pow(10.0, -k);

   for (int i = 0; i < n; i++) {
      fscanf_s(invec, realin, &b[i]);
   }
   b[0] += pow(10.0, -k);

   //for (int i = 0; i < n; ++i) {
   //   for (int j = 0; j < n; ++j) {
   //      fscanf_s(inmat, realin, &mat[i][j]);
   //   }
   //}
   //mat[0][0] += pow(10.0, -k);

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



//
//void matrix::calcLUwA() {
//   A = new real * [n];
//
//   for (int i = 0; i < n; i++) {
//      A[i] = new real[m];
//      for (int j = 0; j < n; j++) {
//
//         if (i > m && j < i - m || j > m && i < j - m) {
//            A[i][j] = 0.0;
//            //continue;
//            printf("%f   ", A[i][j]);
//
//         }
//         //else {
//         if (j > i) {
//            A[i][j] = au[j][i + m - j];
//            au[j][i + m - j] = 0.0;
//         }
//         else {
//            A[i][j] = al[i][j + m - i];
//            al[i][j + m - i] = 0.0;
//         }
//         if (i == j) al[i][j] = 1.0;
//         A[i][i] = di[i];
//         //printf("A:\n");
//         printf("%f   ", A[i][j]);
//         //}
//      }
//      printf("\n");
//   }
//   printf("\nne A:\n");
//
//
//   for (int j = 0; j < n; j++) {
//      real sumU = 0;
//      real sumL = 0;
//      real sumD = 0;
//      for (int i = 0; i <= j; i++) {
//         au[i][j] = A[i][j];
//
//         for (int k = 0; k < i; k++) {
//            au[i][j] -= al[i][k] * au[k][j];
//         }
//         //au[i][j] -= sumU;
//         printf("U/// i: %d | j: %d | au[i][j]: %f | sumU: %f\n", i, j, au[i][j], sumU);
//
//      }
//      for (int i = j; i < n; i++) {
//         al[i][j] = A[i][j];
//         for (int k = 0; k < j; k++) {
//            al[i][j] -= al[i][k] * au[k][j];
//            if (i == j) sumD += al[i][k] * au[k][j];
//         }
//
//         al[i][j] /= au[j][j];
//         printf("L/// i: %d | j: %d | al[i][j]: %f | sumL: %f | al[j][j]: %f | sumD: %f\n", i, j, al[i][j], sumL, au[j][j], sumD);
//
//      }
//
//   }
//}
//
//void matrix::calcLUr() {
//   //real sumD[4] = { 0, 1.5, 0, 0.6 };
//   //real sumD[6] = { 0, 10, 33, 45, 31.25, 42 };
//   //real a = -(5.0 / 4 - 1.0);
//   //real b = -(14.0 / 45 - 1.0 / 3);
//   //real c = -(37.0 / 3360 - 1.0 / 5);
//   //real sumD[4] = { 0, a, b, c };
//
//   for (int j = 0; j < n; j++) {
//      //au[i][j] = A[i][j];
//      real sumU = 0;
//      real sumL = 0;
//      real sumD = 0;
//      for (int i = 0; i < j; i++) {
//         //if (j > m && i + m - j < i - m || i > m && j + m - i < j - m) continue;
//         for (int k = 0; k < i; k++) {
//            //au[i][j] -= al[i][k] * au[k][j];
//            sumU += al[i][k + m - i] * au[j][k + m - j];
//            au[j][i + m - j] -= al[i][k + m - i] * au[j][k + m - j];
//            //printf("U/// k: %d i: %d j: %d | i + m - j: %d | au[j][i + m - j]: %f | sumU: %f | al_k: %f | au_k: %f\n",
//               //k, i, j, i + m - j, au[j][i + m - j], sumU, al[i][k + m - i], au[j][k + m - j]);
//            printf("U/// k: %d i: %d j: %d | i + m - j: %d | au[j][i + m - j]: %s | sumU: %s | al_k: %s | au_k: %s  ",
//               k, i, j, i + m - j, check(au[j][i + m - j]).c_str(), check(sumU).c_str(), check(al[i][k + m - i]).c_str(), check(au[j][k + m - j]).c_str());
//            printf("U//// k + m - i: %d k + m - j: %d\n", k + m - i, k + m - j);
//
//            //cout << "k, i, j, k + m - i, k + m - j, i + m - j" << k << i << j << k + m - i << "sumU: " << sumU << "al_k u au_k: " <<
//               //al[i][k + m - i] << " " << au[j][k + m - j] << endl;
//         }
//
//
//      }
//
//      for (int i = j; i < n; i++) {
//         //real sumU = 0;
//         //real sumL = 0;
//         //real sumD = 0;
//
//         //al[i][j] = A[i][j];
//         //if (i > m && j < i - m || j > m && i < j - m) continue;
//         //if (j > i) continue;
//
//         for (int k = 0; k < j; k++) {
//            //al[i][j] -= al[i][k] * au[k][j];
//            //printf("L/// k: %d | i: %d | j: %d | j + m - i: %d | al[i][j + m - i]: %.7f | sumU: %f | al_k: %f | al_k: %f\n",
//               //k, i, j, j + m - i, al[i][j + m - i], al[i][k + m - i], au[j][k + m - j], sumU);
//            sumL += al[i][k + m - i] * au[j][k + m - j];
//            al[i][j + m - i] -= al[i][k + m - i] * au[j][k + m - j];
//            if (i == j) sumD += al[i][k + m - i] * au[j][k + m - j];
//            //cout << "k, i, j" << k << i << j << "sumL: " << sumL << "al_k u au_k: " << al[i][k + m - i] << " " << au[j][k + m - j] << endl;
//
//            //printf("L/// k: %d i: %d j: %d | j + m - i: %d | al[i][j + m - i]: %.7f | sumL: %f | al_k: %f | au_k: %f\n",
//              // k, i, j, j + m - i, al[i][j + m - i], sumL, al[i][k + m - i], au[j][k + m - j]);
//            printf("L/// k: %d i: %d j: %d | j + m - i: %d | al[i][j + m - i]: %s | sumL: %s | al_k: %s | au_k: %s  ",
//               k, i, j, j + m - i, check(al[i][j + m - i]).c_str(), check(sumL).c_str(), check(al[i][k + m - i]).c_str(), check(au[j][k + m - j]).c_str());
//
//
//            printf("L//// k + m - i: %d k + m - j: %d\n", k + m - i, k + m - j);
//         }
//         al[i][j + m - i] /= di[j];
//
//         if (i == j) di[j] = di[j] - sumD;
//      }
//      //di[j] = di[j] - sumD;
//   }
//}
/*
real matrix::check(real a) {
   if (fabs(a) > 1000) {
      a = char(a);
   }
   return a;
}*/



void matrix::calcLUkonec()
{

   //for (int i = 0; i < n; i++)
   //{
   //   for (int j = 0; j < n; j++)
   //   {
   //      // Расчёт индексов ленточной матрицы
   //      //printf(realout, mat[i][j]);
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
   //   }
   //   //enter;
   //}

   for (int i = 0; i < n; i++)
      printf(realout "\n", di[i]);

   printf("\n///L:\n");

   for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {

         printf(realout, al[i][j]);
      }
      printf("\n");
   }
   printf("\n");
   printf("\n///U:\n");

   for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
         printf(realout, au[i][j]);
      }
      printf("\n");
   }

   //printf("\n///D[0]: %.7f\n\n", di[0]);
   /*for (int i = 0; i < n; i++)
      printf(realout "\n", di[i]);*/

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
         //printf("sumL: " realoutsum "  noviy al[%d][%d]:" realout, sum, il, jl, al[il][jl]);
         //enter;
         ////printf("/////AL[%d][%d]: %f Di[%d]: %f\n", il, jl, al[il][jl], ij, di[ij]);
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
            ////di[il] = fabsf(di[il] - sum);

            di[il] = di[il] - sum;
            ////enter;
            ////printf("/////Sum AL dlya Di[%d]:%f\n", il, sum);
            ////enter;
            ////enter;
            //printf("sumD: " realoutsum "  noviy di[%d]:" realout, sum, il, di[il]);
            //enter;

         }

         else {
            int klu = j;
            au[klu][jl] = (au[klu][jl] - sum);
            //printf("sumU: " realoutsum "  noviy au[%d][%d]:" realout, sum, klu, jl, au[klu][jl]);
            //enter;
         }

      }

   }
   //di[9] *= 10;
}



void matrix::calcY(real** al, real* b, real* y, int n)
{
   //printf("\n///Y:\n");

   for (int i = 0; i < n; i++)
   {
      realsum sum = 0.0;
      for (int j = 0; j < i; j++) {
         if (j + m - i < 0) continue;
         sum += al[i][j + m - i] * y[j];
         //printf("\ni: %d | j + m - i: %d | %f | %f\n", i, j + m - i, al[i][j + m - i], y[i]);
      }
      y[i] = (b[i] - sum);

   }
}

void matrix::calcX(real** au, real* y, real* x, int n)
{
   //printf("\n///Y:\n");

   for (int i = n - 1; i >= 0; i--)
   {

      realsum sum = 0.0;
      //printf("y[%d] = %f   sum = ", i, y[i]);
      for (int j = i + 1; j < n; j++) {
         if (i + m - j < 0) continue;
         sum += au[j][i + m - j] * x[j];
         //printf("(%.7f * %.7f) + ", au[j][i + m - j], x[j]);
      }
      x[i] = (y[i] - sum) / di[i];
      //printf("= %.7f        y[%d] - sum : % .7f \n", sum, i, y[i] - sum);

   }
   //printf("B[0]: " realout "\n", b[0]);
   //printf("\n///X:\n");

   for (int i = 0; i < n; i++)
      printf("                  " realout "\n", x[i]);
   //for (int i = n - 1; i >= 0; i--)
   //   printf("                  " realout "\n", x[i]);
}




void matrix::createGilbert(real** gl, real** gu, real* D, int n, int m)
{
   m = n - 1;
   for (int i = 0; i < n; i++)
   {
      for (int j = i - m, j1 = 0; j1 < m; j++, j1++)
      {
         if (j < 0) gl[i][j1] = 0;
         else gl[i][j1] = 1.0 / (i + j + 1);

         if (j < 0) gu[i][j1] = 0;
         else gu[i][j1] = 1.0 / (i + j + 1);
         //printf("%f                ", gl[i][j1]);

      }
      //enter;
      /*for (int k = i + m, j2 = 0; j2 < m; j2++, k++)
      {
         if (k > m) g[i][j2] = 0;
         else g[i][j2] = 1.0 / (i + j2 - 1);
      }*/
      D[i] = 1.0 / (i + i + 1);
   }
}

void matrix::allocateMemory() {
   al = new real * [n];
   for (int i = 0; i < n; i++) {
      al[i] = new real[m]{ 0 };
      //for (int j = 0; j < m; j++) {
         //al[i][j] = 0.0;
         //printf("%f ", al[i][j]);
      //}
      //printf("\n");
   }
   au = new real * [n];
   for (int i = 0; i < n; i++) {
      au[i] = new real[m]{ 0 };
      //au[i] = { 0 };
      //for (int j = 0; j < m; j++) {
         //au[i][j] = 0.0;
         //printf("%f ", au[i][j]);
   //}
   //printf("\n");
   }
   mat = new real * [n];
   for (int i = 0; i < n; ++i) {
      mat[i] = new real[n];
   }
   di = new real[n]{ 0 };
   b = new real[n]{ 0 };
   x = new real[n]{ 0 };
   y = new real[n]{ 0 };
   //x = b;
   //y = b;
}



void matrix::gauss()
{
   //без векторов
   /*std::vector <std::vector <real>> matrix;
   std::vector <real> b_vec;
   matrix.resize(n);
   b_vec.resize(n);*/

   real** matrix = new real * [n];
   /*for (int i = 0; i < n; i++)
      matrix[i] = new real[n];*/
      //real* b = new real[n];

   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++)
      {
         // Расчёт индексов ленточной матрицы
         //printf("%f | ", A[i][j]);

         if (j > i) {
            int il = j, jl = m + i - j + 0;
            if (il < 0 || jl < 0) continue;
            au[il][jl] = mat[i][j];
         }
         else if (i > j)
         {
            int il = i, jl = m - i + j + 0;
            if (il < 0 || jl < 0) continue;
            al[il][jl] = mat[i][j];
         }
         else // i=j
         {
            di[i] = mat[i][i];
         }
         //printf("%d %d  ", i, j);
      }
      //printf("\n");
   }

   for (int i = 0; i < n; i++) {
      b[i] = b[i];
      //matrix[i].resize(n);
      matrix[i] = new real[n];

      for (int j = 0; j < n; j++) {
         if (j + m - i < 0 || i + m - j < 0)
            matrix[i][j] = 0.0;
         else if (j > i)  matrix[i][j] = au[j][i + m - j];
         else  matrix[i][j] = al[i][j + m - i];
         matrix[i][i] = di[i];
         //printf("%f   ", matrix[i][j]);
      }
      //printf("\n");
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
      real lead = matrix[i][i];
      for (int k = i + 1; k < n; k++)
         if (abs(matrix[k][i]) > abs(lead))
         {
            lead = matrix[k][i];
            leadIndex = k;
         }
      //matrix[i].swap(matrix[leadIndex]);
      std::swap(matrix[i], matrix[leadIndex]);
      /*real temp = matrix[row1][j];
      matrix[row1][j] = matrix[row2][j];
      matrix[row2][j] = temp;*/

      std::swap(b[i], b[leadIndex]);
      for (int k = i + 1; k < n; k++) {
         real m_i = matrix[k][i] / matrix[i][i];
         for (int j = i; j < n; j++)
            matrix[k][j] -= m_i * matrix[i][j];
         b[k] -= m_i * b[i];
      }
   }

   for (int i = n - 1; i >= 0; i--) {
      real sum = 0;
      for (int j = n - 1; j > i; j--)
         sum += matrix[i][j] * x[j];
      x[i] = (b[i] - sum) / matrix[i][i];
      //printf("\n%.12f", x[i]);
   }


   for (int i = 0; i < n; i++)
      printf("\n" realout, x[i]);
}


int main() {
   FILE* inmat, * invec;

   matrix matrix{};
   int num;
   //cin >> num;
   //printf("1: N4, 2: N5, 3: N6\n");
   //scanf_s("%d", &num);
   num = 2;
   switch (num) {
   case 1:
      for (matrix.k = 0; matrix.k < N; matrix.k++)
         //for (matrix.k = 5; matrix.k < 8; matrix.k++)
      {
         fopen_s(&inmat, "inmat.txt", "r");
         fopen_s(&invec, "invec.txt", "r");
         //LU4();
         //exit(0);


         printf("\n\n\n/////////////K = %d\n", matrix.k);


         matrix.input(inmat, invec);
         /*
         printf("///D:\n");
         for (int i = 0; i < matrix.n; i++)
            printf(realout "\n", matrix.di[i]);
         */

         //printf("\n///Do LU:\n");



         matrix.calcLUkonec();

         printf("\n///Posle LU:\n");
         printf("\n///D:\n");
         /*for (int i = 0; i < matrix.n; i++)
            printf(realout "\n", matrix.di[i]);

         printf("\n///L:\n");

         for (int i = 0; i < matrix.n; i++) {
            for (int j = 0; j < matrix.m; j++) {

               printf(realout, matrix.al[i][j]);
            }
            printf("\n");
         }
         printf("\n");
         printf("\n///U:\n");

         for (int i = 0; i < matrix.n; i++) {
            for (int j = 0; j < matrix.m; j++) {
               printf(realout, matrix.au[i][j]);
            }
            printf("\n");
         }*/


         /*printf("\n");
         printf("\n///B:\n");

         for (int i = 0; i < matrix.n; i++)
            printf(realout "\n", matrix.b[i]);*/


            //matrix.gauss();
            //printf("\n///Y:\n");

         matrix.calcY(matrix.al, matrix.b, matrix.y, matrix.n);

         //printf("\n///X:\n");

         matrix.calcX(matrix.au, matrix.y, matrix.x, matrix.n);
         //enter;

         cout << "---------------------------------------------------------------";
      }
      break;
   case 2:
      //printf("Matrix size: ");
      //scanf_s("%d", &matrix.n);
      for (matrix.n = 2; matrix.n < N; matrix.n++) {
         printf("\n\n\n/////////////K = %d\n", matrix.n);

         matrix.m = matrix.n - 1;
         matrix.allocateMemory();
         matrix.createGilbert(matrix.al, matrix.au, matrix.di, matrix.n, matrix.m);

         ///printf("///D:\n");
         matrix.calcLUkonec();
         printf("\n///Posle LU:\n");
         printf("\n///D:\n");
         for (int i = 0; i < matrix.n; i++)
            printf(realout "\n", matrix.di[i]);

         printf("\n///L:\n");

         for (int i = 0; i < matrix.n; i++) {
            for (int j = 0; j < matrix.m; j++) {

               printf("%f ", matrix.al[i][j]);
            }
            printf("\n");
         }
         printf("\n");
         printf("\n///U:\n");

         for (int i = 0; i < matrix.n; i++) {
            for (int j = 0; j < matrix.m; j++) {
               printf("%f ", matrix.au[i][j]);
            }
            printf("\n");
         }

         /*matrix.b[0] = 5.0;
         matrix.b[1] = 71.0 / 20;
         matrix.b[2] = 197.0 / 70;
         matrix.b[3] = 657.0 / 280;
         matrix.b[4] = 1271.0 / 630;*/
         //matrix.b = {5.0, 2.0 ,5.0 , 2.0, 5.0};

         //real** bbb = new real * [12];
         real  bbb[14][14] = { {2.0,7.0 / 6},
         { 3.0,23.0 / 12,43.0 / 30 },
         { 4.0, 163.0 / 60,21.0 / 10,241.0 / 140 },
         { 5.0, 71.0 / 20, 197.0 / 70, 657.0 / 280, 1271.0 / 630 },
         { 6.0,617.0 / 140,499.0 / 140,2531.0 / 840,1649.0 / 630,12847.0 / 5544 },
         { 7.0,1479.0 / 280,5471.0 / 1260,3119.0 / 840,22549.0 / 6930,16081.0 / 5544,157309.0 / 60060 },
         { 8.0,15551.0 / 2520,6479.0 / 1260,41029.0 / 9240,27169.0 / 6930,253405.0 / 72072,191629.0 / 60060,30089.0 / 10296 },
         { 9.0,17819.0 / 2520,82609.0 / 13860,47959.0 / 9240,415567.0 / 90090,299737.0 / 72072,45533.0 / 12012,71761.0 / 20592,988277.0 / 306306 },
         { 10.0,221209.0 / 27720,94159.0 / 13860,715867.0 / 120120,479917.0 / 90090,347785.0 / 72072,106081.0 / 24024,1425857.0 / 350064,386149.0 / 102102,18266753.0 / 5173168 },
         { 11.0,246619.0 / 27720,1376527.0 / 180180,810247.0 / 120120,545983.0 / 90090,794669.0 / 144144,2067641.0 / 408408,546595.0 / 116688,8459953.0 / 1939938,105559977.0 / 25865840,89301449.0 / 23279256 },
         { 12.0,3538687.0 / 360360,1530967.0 / 180180,906343.0 / 120120,1227101.0 / 180180,15239101.0 / 2450448,779971.0 / 136136,11785561.0 / 2217072,48119579.0 / 9699690,120340457.0 / 25865840,101999225.0 / 23279256,2015839999.0 / 486748080 },
         { 13, 10.7484376734377, 9.36354201354201, 8.35781302031302, 7.57512324277030, 6.94112627568510, 6.41356205713791, 5.96582239999423, 5.57998750475531, 5.24339503375881, 4.94676742881414, 4.68311083836222, 4.44703000548606 },
         {14, 11.6817710067710, 10.2385420135420, 9.18134243207773, 8.35290102054808, 7.67796838094826, 7.11356205713791, 6.63248906666089, 6.21635114111894, 5.85209068593272, 5.53010076214747, 5.24311083836222, 4.98549154394759, 4.75280102446175}
         };


         /*double b2[] = { 2,7 / 6 };
         double b3[] = { 3,23 / 12,43 / 30 };
         double b4[] = { 4, 163 / 60,21 / 10,241 / 140 };
         double b5[] = { 5.0, 71.0 / 20, 197.0 / 70, 657.0 / 280, 1271.0 / 630 };
         double b6[] = { 6,617 / 140,499 / 140,2531 / 840,1649 / 630,12847 / 5544 };
         double b7[] = { 7,1479 / 280,5471 / 1260,3119 / 840,22549 / 6930,16081 / 5544,157309 / 60060 };
         double b8[] = { 8,15551 / 2520,6479 / 1260,41029 / 9240,27169 / 6930,253405 / 72072,191629 / 60060,30089 / 10296 };
         double b9[] = { 9,17819 / 2520,82609 / 13860,47959 / 9240,415567 / 90090,299737 / 72072,45533 / 12012,71761 / 20592,988277 / 306306 };
         double b10[] = { 10,221209 / 27720,94159 / 13860,715867 / 120120,479917 / 90090,347785 / 72072,106081 / 24024,1425857 / 350064,386149 / 102102,18266753 / 5173168 };
         double b11[] = { 11,246619 / 27720,1376527 / 180180,810247 / 120120,545983 / 90090,794669 / 144144,2067641 / 408408,546595 / 116688,8459953 / 1939938,105559977 / 25865840,89301449 / 23279256 };
         double b12[] = { 12,3538687 / 360360,1530967 / 180180,906343 / 120120,1227101 / 180180,15239101 / 2450448,779971 / 136136,11785561 / 2217072,48119579 / 9699690,120340457 / 25865840,101999225 / 23279256,2015839999 / 486748080 };*/

         /*enter;
         enter;
         enter;*/

         matrix.b = bbb[matrix.n - 2];
         /*printf("/////////B:\n");
         for (int i = 0; i < matrix.n; i++)
            printf(realout "\n", matrix.b[i]);

         enter;*/

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
         printf("\n");*/

         matrix.calcY(matrix.al, matrix.b, matrix.y, matrix.n);

         //printf("\n///X:\n");

         matrix.calcX(matrix.au, matrix.y, matrix.x, matrix.n);
         //matrix.createGilbert(matrix.au, matrix.di, matrix.n, matrix.m);

         ////printf("\n///B:\n");

         ////for (int i = 0; i < matrix.n; i++)
         //   //printf(realout "\n", matrix.b[i]);


      }
      break;
   case 3:
      for (matrix.k = 0; matrix.k < N; matrix.k++)
      {
         fopen_s(&inmat, "inmat.txt", "r");
         fopen_s(&invec, "invec.txt", "r");
         //LU4();
         //exit(0);
         printf("\n\n\n/////////////K = %d", matrix.k);
         matrix.input(inmat, invec);
         matrix.gauss();
         cout << "\n---------------------------------------------------------------";

      }
      break;
   default:
      float a = 3.0000001;
      printf("%.7f", a);
   }

}

