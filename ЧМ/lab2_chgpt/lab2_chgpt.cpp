#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

using real = double;

// Функция для факторизации LU-разложения
void luFactorize(std::vector<std::vector<real>>& A) {
   int n = A.size();
   for (int k = 0; k < n; ++k) {
      for (int i = k + 1; i < n; ++i) {
         A[i][k] /= A[k][k];
         for (int j = k + 1; j < n; ++j) {
            A[i][j] -= A[i][k] * A[k][j];
         }
      }
   }
}

// Функция для решения системы с LU-разложением
void luSolve(const std::vector<std::vector<real>>& LU, std::vector<real>& x) {
   int n = LU.size();
   // Прямой ход
   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < i; ++j) {
         x[i] -= LU[i][j] * x[j];
      }
   }
   // Обратный ход
   for (int i = n - 1; i >= 0; --i) {
      for (int j = i + 1; j < n; ++j) {
         x[i] -= LU[i][j] * x[j];
      }
      x[i] /= LU[i][i];
   }
}

// Блочная релаксация
class BlockRelaxationSolver {
private:
   int n; // Размер матрицы
   int blockSize;
   std::vector<std::vector<real>> A;
   std::vector<real> b, x;
   real alpha; // Параметр релаксации

public:
   BlockRelaxationSolver(int size, int blkSize, const std::vector<std::vector<real>>& matrix, const std::vector<real>& rhs, real relaxation)
      : n(size), blockSize(blkSize), A(matrix), b(rhs), x(size, 0.0), alpha(relaxation) {}

   // Метод расчёта блока
   void solveBlock(int i) {
      int start = i * blockSize;
      int end = std::min(start + blockSize, n);

      // Извлекаем блок A_ii
      std::vector<std::vector<real>> Aii(blockSize, std::vector<real>(blockSize, 0.0));
      for (int row = start; row < end; ++row)
         for (int col = start; col < end; ++col)
            Aii[row - start][col - start] = A[row][col];

      // Факторизуем блок A_ii
      luFactorize(Aii);

      // Вычисляем невязку R_i
      std::vector<real> Ri(blockSize, b[start]);
      for (int row = start; row < end; ++row) {
         Ri[row - start] = b[row];
         for (int col = 0; col < n; ++col) {
            if (col < start || col >= end)
               Ri[row - start] -= A[row][col] * x[col];
         }
      }

      // Решаем A_ii * deltaX = R_i
      luSolve(Aii, Ri);

      // Обновляем значения x в блоке
      for (int row = start; row < end; ++row) {
         x[row] += alpha * Ri[row - start];
      }
   }

   // Основной метод решения
   void solve(int maxIterations, real tolerance) {
      for (int iter = 0; iter < maxIterations; ++iter) {
         for (int i = 0; i < n / blockSize; ++i) {
            solveBlock(i);
         }

         // Вычисление невязки для проверки сходимости
         real residual = 0.0;
         real denominator = 0.0;
         for (int i = 0; i < n; ++i) {
            real Ax_i = 0.0;
            for (int j = 0; j < n; ++j) {
               Ax_i += A[i][j] * x[j];
            }
            residual += std::pow(b[i] - Ax_i, 2);
            denominator += std::pow(b[i], 2);
         }
         double resss = residual;
         residual = std::sqrt(residual) / std::sqrt(denominator);

         std::cout << "Итерация " << iter << ", Невязка: " << residual << " u " << resss << "  " << denominator << std::endl;

         if (residual < tolerance) {
            std::cout << "Решение найдено на итерации " << iter << std::endl;
            break;
         }

         std::cout << "Решение: ";
         for (real xi : x) {
            std::cout << xi << " ";
         }
         std::cout << std::endl;
         std::cout << std::endl;
      }
   }

   const std::vector<real>& getSolution() const {
      return x;
   }
};

int main() {
   setlocale(LC_ALL, "Russian");
   int n = 6;
   int blockSize = 3;
   /*std::vector<std::vector<real>> A = {
       {4, 1, 0, 0, 0, 0},
       {1, 4, 1, 0, 0, 0},
       {0, 1, 4, 1, 0, 0},
       {0, 0, 1, 4, 1, 0},
       {0, 0, 0, 1, 4, 1},
       {0, 0, 0, 0, 1, 4}
   };*/

   std::vector<std::vector<real>> A = {
       {9, -2, -1, -2, -1, -2},
       {-2, 6, -1, -1, -1, -1},
       {-1, -1, 5, -1, -1, -1},
       {-2, -1, -1, 6, -1, -1},
       {-1, -1, -1, -1, 5, -1},
       {-2, -1, -1, -1, -1, 6}
   };
   /*std::vector<std::vector<real>> A = {
    {3, -5, 2, -1, 4, -3},
    {-2, 7, -4, 1, -5, -2},
    {6, -3, 8, -1, 2, -4},
    {-1, 9, -2, 5, -3, -6},
    {4, -7, -1, 3, 6, -2},
    {-5, -2, 1, -4, 7, 9}
   };*/
   std::vector<real> b = { -23, -8, -3, 6, 9, 20 };
   real alpha = 1.0;  // Параметр релаксации

   BlockRelaxationSolver solver(n, blockSize, A, b, alpha);
   solver.solve(10, 1e-6);

   const std::vector<real>& solution = solver.getSolution();
   std::cout << "Решение: ";
   for (real xi : solution) {
      std::cout << xi << " ";
   }
   std::cout << std::endl;

   return 0;
}
