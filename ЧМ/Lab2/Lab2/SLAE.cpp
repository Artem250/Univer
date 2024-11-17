#include "SLAE.h"


void SLAE::blockRelaxationStep(int block_start, int block_end) {
   for (int i = block_start; i < block_end; ++i) {
      if (A[i][i] == 0.0) {
         //std::cerr << "Ошибка: Нулевой элемент на диагонали, невозможно выполнить деление." << std::endl;
         //exit(1);
         continue;
      }
      else {
         double sum = 0.0;
         for (int j = 0; j < n; ++j) {
            if (i != j) {
               sum += A[i][j] * xk[j];
            }
         }
         xk[i] = (F[i] - sum) / A[i][i];
      }
   }
}




// Преобразование 7-ми диагональной матрицы в плотный формат
void SLAE::convM()
{

   A.clear();
   A.resize(n);

   for (int i = 0; i < n; ++i) {
      A[i].resize(n, 0);
      A[i][i] = di[i];
   }

   int j = 1;
   for (int i = 0; i < al1.size(); ++i, ++j) {

      A[i][j] = au1[i];
      A[j][i] = al1[i];
   }

   j = m + 2;
   for (int i = 0; i < al2.size(); ++i, ++j) {

      A[i][j] = au2[i];
      A[j][i] = al2[i];
   }

   j = m + 3;
   for (int i = 0; i < al3.size(); ++i, ++j) {

      A[i][j] = au3[i];
      A[j][i] = al3[i];
   }
}





void SLAE::convMto()
{

   //A.clear();
   //A.resize(n);
   //al1.clear();
   //al2.clear();
   //al3.clear();
   //au1.clear();
   //au2.clear();
  // au3.clear();
   //di.clear();
   for (int i = 0; i < n; ++i) {
      //A[i].resize(n, 0);
      di[i] = A[i][i];
   }

   int j = 1;
   for (int i = 0; i < al1.size(); ++i, ++j) {

      au1[i] = A[i][j];
      al1[i] = A[j][i];
   }

   j = m + 2;
   for (int i = 0; i < al2.size(); ++i, ++j) {

      au2[i] = A[i][j];
      al2[i] = A[j][i];
   }

   j = m + 3;
   for (int i = 0; i < al3.size(); ++i, ++j) {

      au3[i] = A[i][j];
      al3[i] = A[j][i];
   }
}








//Умножение i-й строки матрицы на вектор
real SLAE::multLine(vector <real>& line, int i, int mode) {

   real sum = 0;
   if (mode == 1 || mode == 3) {	// Нижний треугольник

      if (i > 0) {

         sum += al1[i - 1] * line[i - 1];
         if (i > m + 1) {
            sum += al2[i - m - 2] * line[i - m - 2];
            if (i > m + 2)
               sum += al3[i - m - 3] * line[i - m - 3];
         }
      }
   }


   if (mode == 2 || mode == 3) {	// Главная диагональ
      // и верхний треугольник
      sum += di[i] * line[i];
      if (i < n - 1) {
         sum += au1[i] * line[i + 1];
         if (i < n - m - 2) {
            sum += au2[i] * line[i + m + 2];
            if (i < n - m - 3)
               sum += au3[i] * line[i + m + 3];
         }
      }
   }

   return sum;
}

void SLAE::mult() {
   //convMto();
   int index;
   F.clear();
   F.resize(n, 0);
   // Нижний треугольник
   index = 1;
   for (int i = 0; i < al1.size(); ++i, ++index)
      F[index] += al1[i] * xk[i];
   index = m + 2;
   for (int i = 0; i < al2.size(); ++i, ++index)
      F[index] += al2[i] * xk[i];
   index = m + 3;
   for (int i = 0; i < al3.size(); ++i, ++index)
      F[index] += al3[i] * xk[i];


   // Главная диагональ
   for (int i = 0; i < di.size(); ++i)
      F[i] += di[i] * xk[i];


   // Верхний треугольник
   index = 1;
   for (int i = 0; i < au1.size(); ++i, ++index)
      F[i] += au1[i] * xk[index];
   index = m + 2;
   for (int i = 0; i < au2.size(); ++i, ++index)
      F[i] += au2[i] * xk[index];
   index = m + 3;
   for (int i = 0; i < au3.size(); ++i, ++index)
      F[i] += au3[i] * xk[index];
}

void SLAE::calcJacobi(real w) {

   real sum;
   vector <real> xk1;
   xk1.resize(n);

   for (int i = 0; i < n; ++i) {
      sum = multLine(xk, i, 3);
      //xk[i] += w * (F[i] - sum) / di[i];
      xk1[i] = xk[i] + w * (F[i] - sum) / di[i];
      //cout << "            " << xk1[i] << " ";
   }
   //cout << endl;
   xk = xk1;
}

void SLAE::calcGaussSeildel(real w) {

   real sum;
   vector <real> xk1 = xk;

   for (int i = 0; i < n; ++i) {

      sum = multLine(xk1, i, 1);
      sum += multLine(xk, i, 2);

      xk1[i] = xk[i] + w * (F[i] - sum) / di[i];
      //cout << "            " << xk1[i] << " ";

   }
   //cout << endl;

   xk = xk1;
}

int SLAE::calcIterative(int mode, real w) {
   int i = 0;
   while (i < maxiter && calcRelativeResidual() >= eps) {

      if (mode == 1) {
         calcJacobi(w);
         //cout << calcRelativeResidual() << endl;
      }

      else {
         calcGaussSeildel(w);
         //cout << "                    " << calcRelativeResidual() << endl;
      }
      i++;
   }
   return i;
}

real SLAE::findOptimalW(int mode, std::ofstream& fout) {

   real optimalW, tmpW;
   int max_i, min_i = maxiter, tmp_i;
   if (mode == 1) max_i = 101;
   else max_i = 200;

   for (int i = 0; i < max_i; ++i) {

      generateInitualGuess(n);
      //generateInitualGuess(6);
      tmpW = real(i) / 100;
      tmp_i = calcIterative(mode, tmpW);
      if (tmp_i < min_i) {
         min_i = tmp_i;
         optimalW = tmpW;
      }
      if (i % 10 == 0) // Выводим таблицу с точность 0.1
         writeTableToFile(fout, 1, tmpW, tmp_i, calcCondNumber());
      //cout << calcRelativeResidual() << endl;
   }
   generateInitualGuess(n);
   //generateInitualGuess(6);
   min_i = calcIterative(mode, optimalW);
   writeTableToFile(fout, 2, optimalW, min_i, calcCondNumber());

   return optimalW;

}

real SLAE::calcNormE(vector <real>& x) {

   real normE = 0;
   for (int i = 0; i < n; i++)
      normE += x[i] * x[i];

   return sqrt(normE);
}

real SLAE::calcRelativeResidual() {

   vector <real> numerator, denominator = F;
   numerator.resize(n);

   mult(); // F = A*xk

   for (int i = 0; i < n; ++i)
      numerator[i] = denominator[i] - F[i]; // F - A*xk

   // || F - A*xk || / || F ||
   real res = calcNormE(numerator) / calcNormE(denominator);
   F = denominator;
   return res;
}

real SLAE::calcCondNumber() {

   vector <real> numerator, denominator;
   numerator.resize(n);
   denominator.resize(n);
   for (int i = 0; i < n; ++i) {
      numerator[i] = xk[i] - (i + 1);	// x - x*
      denominator[i] = i + 1;		// x
   }

   return calcNormE(numerator) / (calcNormE(denominator) * calcRelativeResidual());
}
