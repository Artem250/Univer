#include <iostream>
#include <fstream> //для работы с файлами
#include <string>
using namespace std;
typedef float real;
typedef double realSum;
const int SIZE = 10;

class Matrix {
public:
   int n, m;
   real** al, ** au; //двумерн
   real* di, * b, * x, * y;

   void InputParams(ifstream& fileParam); //virtual - применяется в базовом классе
   void Input(ifstream& fileMatrix, ifstream& fileVector);
   void LUDecomposition();
   void CalculateY();

private:
   void AllocateMemory();
   //void LUDecomposition();
};

void Matrix::InputParams(ifstream& fileParam)
{
   if (!fileParam.is_open()) cerr << "\n Ошибка открытия файла" << endl;
   else
   {
      fileParam >> n;
      fileParam >> m;
   }
}

void Matrix::Input(ifstream& fileMatrix, ifstream& fileVector)//, real di[SIZE], real Al[SIZE][SIZE], real Au[SIZE][SIZE])//real *di, real *Al, real *Au)
{
   if (!fileMatrix.is_open() || !fileVector.is_open()) { cerr << "\n Ошибка открытия файла" << endl; return; }
   else
   {
      AllocateMemory();
      for (int i = 0; i < n; i++)
      {
         fileMatrix >> di[i];
      }
      for (int i = 1; i < m + 1; i++) {
         for (int j = 0; j < n; j++) {
            fileMatrix >> al[i - 1][j];
         }
      }
      for (int i = (m + 1); i < 2 * m + 1; i++) {
         for (int j = 0; j < n; j++) {
            fileMatrix >> au[i - m - 1][j];
         }
      }

      for (int i = 0; i < n; i++) {
         fileVector >> b[i];
      }

      for (int i = 0; i < n; i++)
      {
         cout << di[i] << "\t";
      }
      cout << endl;
      for (int i = 0; i < m; ++i) {
         for (int j = 0; j < n; ++j) {
            cout << al[i][j] << "\t";
         }
         cout << endl;
      }
      for (int i = 0; i < m; ++i) {
         for (int j = 0; j < n; ++j) {
            cout << au[i][j] << "\t";
         }
         cout << endl;
      }
   }
}

void Matrix::AllocateMemory()
{
   al = new real * [m];
   for (int i = 0; i < m; i++)
   {
      al[i] = new real[n]; //двумерн 
   }

   au = new real * [m];
   for (int i = 0; i < m; i++)
   {
      au[i] = new real[n];
   }
   di = new real[n];
   b = new real[n];
}


void Matrix::LUDecomposition()
{
   for (int i = 0; i < n; i++) //i = 0;1;2;3
   {
      real sumD = 0;
      int j0 = i - m;

      for (int j = j0, k = 0; j < i; j++, k++)     //при i=0: j= 0-2;-1;(<-пропускаем)     k= 0;1;(<-пропускаем)
         //при i=1: j= 1-2;(<-пропускаем)0;        k= 0;(<-пропускаем)1;   т.к. j<i
         //при i=2: j= 0;1;                           k= 0;1;
         //при i=3: j= 1;2                            k= 0;1;
      {
         if (j < 0) continue;//скипаем отриц j                и идём по     [0][0]  *     *                     [0][0] [0][1] [0][2]   0
         //                                               *   [1][1]  *     *               [1][0] [1][1] [1][2] [1][3]
         //                                               *     *   [2][2]  *               [2][0] [2][1] [2][2] [2][3]
         real sumL = 0, sumU = 0;//                                                 *     *   [3][3]              0    [3][1] [3][2] [3][3]

         for (int k0 = k; k0 <= j; k0++)
         {
            if (k0 - j < 0) continue; if (i - k0 + 1 > m) continue;
            sumL += al[k0 - j][i - k + 1] * au[k0 - i + 2][j]; //k=1,i=2,j=1 l32: al[0][2]*au[1][1]   //в al 2 коэф  j+1 или i-k+1
            //    k=0,i=3,j=1 l42: -*au[1][1]        //в au 2й коэф или i-1 или j
// k=1,i=3,j=2 l43: -*au[0][2] + al[0][3]*au[1][2]
// k=0,i=3,j=1
            sumU += al[k0 - i + 2][j] * au[k0 - j][i - k + 1]; //k=1,i=2,j=1 u23: al[1][1]*au[0][2]
            //k=0,i=3,j=1 u24: al[1][1]*-     //при k0=0 - скипается,при k0=1 al[0][1] не храниться в матрице А и всегда =0
    //k=1,i=3,j=2 u34: al[0][2]*- + al[1][2]*au[0][3]
         }

         al[k][i] = al[k][i] - sumL; //ложим в эти же массивы
         //if (!((k == 1 && i == 1) || k == 0 && i == 2)){
         au[k][i] = (au[k][i] - sumU) / di[i - 1];
         //}
         sumD += al[k][i] * au[k][i]; //i=1, j=0, k=1 l22: al[1][1]*au[1][1]
         //i=2, j=0,1 k=0,1 l33: al[0][2]*au[0][2] + al[1][2]*au[1][2]
//i=3, j=1,2, k=0,1 l44: -*- + al[0][3] * au[0][3] + al[1][3] * au[1][3]
      }
      di[i] -= sumD;
   }
   cout << "al:" << endl;
   for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
         cout << al[i][j] << "\t";
      }
      cout << endl;
   }
   cout << "au:" << endl;
   for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
         cout << au[i][j] << "\t";
      }
      cout << endl;
   }
   cout << "di:" << endl;
   for (int i = 0; i < n; i++)
   {
      cout << di[i] << "\t";
   }
}

void Matrix::CalculateY() {         //прямой ход: Ly=b
   for (int i = 0; i < n; i++)
   {
      realSum sum = 0;
      //int i0 = ia[i], i1 = ia[i + 1];
      //int j = i - (i1 - i0);
      //for (int k = i0; k < i1; k++, j++)
      //    sum += y[j] * al[k];
      //y[i] = (b[i] - sum) / di[i];
   }

}

void MultiplicationMatrix() {

}


int main()
{
   setlocale(LC_ALL, "ru");
   //real *Di, *Al, *Au, *b;
   //real Di[SIZE], Al[SIZE][SIZE], Au[SIZE][SIZE], F[SIZE];

   Matrix matrix;
   ifstream filenameParam, filenameMatrix, filenameVector;
   filenameParam.open("param.txt"), filenameMatrix.open("matrix.txt"), filenameVector.open("vector.txt");


   matrix.InputParams(filenameParam);
   matrix.Input(filenameMatrix, filenameVector);
   matrix.LUDecomposition();

   cout << endl;
   filenameParam.close(), filenameMatrix.close(), filenameVector.close();
}