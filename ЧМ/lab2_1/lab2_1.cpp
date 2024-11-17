#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <limits>

class SLAE {
private:
   int n;
   std::vector<std::vector<double>> A;
   std::vector<double> x; // Текущее приближение
   std::vector<double> F; // Вектор правой части

public:
   SLAE(int size, const std::vector<std::vector<double>>& matrix, const std::vector<double>& rhs)
      : n(size), A(matrix), F(rhs), x(size, 0.0) {}

   // Метод для умножения матрицы A на вектор x и сохранения в F
   void mult() {
      for (int i = 0; i < n; ++i) {
         double sum = 0.0;
         for (int j = 0; j < n; ++j) {
            sum += A[i][j] * x[j];
         }
         F[i] = sum;
      }
   }

   // Метод для расчета евклидовой нормы
   double calcNormE(const std::vector<double>& vec) {
      double norm = 0.0;
      for (double v : vec) {
         norm += v * v;
      }
      return std::sqrt(norm);
   }

   // Подсчет относительной невязки
   double calcRelativeDiscrepancy() {
      std::vector<double> numerator(n);
      std::vector<double> denominator = F;

      mult(); // F = A * x

      for (int i = 0; i < n; ++i) {
         numerator[i] = denominator[i] - F[i]; // F - A * x
      }

      double res = calcNormE(numerator) / calcNormE(denominator);
      F = denominator;  // Восстанавливаем F для следующего вызова
      return res;
   }

   // Метод для обновления вектора x в процессе итераций
   void setX(const std::vector<double>& newX) {
      x = newX;
   }

   // Метод для выполнения одного шага блочной релаксации
   void blockRelaxationStep(int block_start, int block_end) {
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
                  sum += A[i][j] * x[j];
               }
            }
            x[i] = (F[i] - sum) / A[i][i];
         }
      }
   }

   // Получение текущего значения x
   std::vector<double> getX() const {
      return x;
   }

};

// Функция для чтения матрицы из файла
void readMatrix(const std::string& filename, std::vector<std::vector<double>>& A, int& n) {
   std::ifstream file(filename);
   if (!file) {
      std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
      exit(1);
   }
   file >> n;
   A.resize(n, std::vector<double>(n, 0.0));
   for (int i = 0; i < n; ++i) {
      for (int j = std::max(0, i - 3); j <= std::min(n - 1, i + 3); ++j) {
         file >> A[i][j];
      }
   }
}

// Функция для чтения вектора из файла
void readVector(const std::string& filename, std::vector<double>& b) {
   std::ifstream file(filename);
   if (!file) {
      std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
      exit(1);
   }
   int n;
   file >> n;
   b.resize(n);
   for (int i = 0; i < n; ++i) {
      file >> b[i];
   }
}

int main() {
   setlocale(LC_ALL, "Russian");

   int n;
   int maxiter = 100;
   std::vector<std::vector<double>> A;
   std::vector<double> b;

   // Чтение матрицы и вектора правой части из файлов
   readMatrix("matrix.txt", A, n);
   readVector("vector.txt", b);

   SLAE slae(n, A, b);

   int block_size = 2;
   int min_block_size = 1;
   int max_block_size = n / 2;
   int delta = 1;
   double tolerance = 1e-6;
   double residual_prev = std::numeric_limits<double>::infinity();

   for (int iter = 0; iter < maxiter; ++iter) {
      // Выполнение шага блочной релаксации
      for (int i = 0; i < n; i += block_size) {
         int block_end = std::min(i + block_size, n);
         slae.blockRelaxationStep(i, block_end);
      }

      // Подсчет относительной невязки
      double residual = slae.calcRelativeDiscrepancy();

      if (residual < tolerance) {
         std::cout << "Сходимость достигнута на итерации " << iter << " с относительной невязкой " << residual << std::endl;
         break;
      }

      // Динамическое изменение размера блока
      if (residual < residual_prev) {
         block_size = std::min(block_size + delta, max_block_size);
      }
      else {
         block_size = std::max(block_size - delta, min_block_size);
      }

      residual_prev = residual;

      std::cout << "Итерация " << iter << ", Относительная невязка: " << residual
         << ", Текущий размер блока: " << block_size << std::endl;
   }

   // Запись результата в файл
   std::ofstream output("solution.txt");
   for (const auto& xi : slae.getX()) {
      output << xi << "\n";
   }
   output.close();

   return 0;
}












//#define _CRT_SECURE_NO_WARNINGS
//
//
//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <fstream>
//#include <algorithm>
//#include <limits>
//
//const double tolerance = 1e-6;
//const int max_iterations = 10;
//
//// Функция для чтения матрицы из файла (диагональный формат)
//void readMatrix(const std::string& filename, std::vector<std::vector<double>>& A, int& n) {
//   std::ifstream file(filename);
//   if (!file) {
//      std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
//      exit(1);
//   }
//   file >> n;
//   A.resize(n, std::vector<double>(n, 0.0));
//   for (int i = 0; i < n; ++i) {
//      for (int j = std::max(0, i - 3); j <= std::min(n - 1, i + 3); ++j) {
//         file >> A[i][j];
//      }
//   }
//}
//
//// Функция для чтения вектора из файла
//void readVector(const std::string& filename, std::vector<double>& b) {
//   std::ifstream file(filename);
//   if (!file) {
//      std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
//      exit(1);
//   }
//   int n;
//   file >> n;
//   b.resize(n);
//   for (int i = 0; i < n; ++i) {
//      file >> b[i];
//   }
//}
//
//// Функция для вычисления относительной невязки
//double calculateResidual(const std::vector<std::vector<double>>& A, const std::vector<double>& b, const std::vector<double>& x) {
//   int n = A.size();
//   double norm_b = 0.0, norm_residual = 0.0;
//   for (int i = 0; i < n; ++i) {
//      double sum = 0.0;
//      for (int j = 0; j < n; ++j) {
//         sum += A[i][j] * x[j];
//      }
//      norm_b += b[i] * b[i];
//      norm_residual += (sum - b[i]) * (sum - b[i]);
//   }
//   // Избегаем деления на 0
//   if (norm_b == 0.0) return std::numeric_limits<double>::infinity();
//   return std::sqrt(norm_residual) / std::sqrt(norm_b);
//}
//
//// Функция для выполнения одного шага блочной релаксации
//void blockRelaxationStep(const std::vector<std::vector<double>>& A, const std::vector<double>& b, std::vector<double>& x, int block_start, int block_end) {
//   for (int i = block_start; i < block_end; ++i) {
//      if (A[i][i] == 0.0) {
//         std::cerr << "Ошибка: Нулевой элемент на диагонали, невозможно выполнить деление." << std::endl;
//         continue;
//      }
//      else {
//         double sum = 0.0;
//         for (int j = 0; j < A.size(); ++j) {
//            if (i != j) {
//               sum += A[i][j] * x[j];
//            }
//         }
//         x[i] = (b[i] - sum) / A[i][i];
//      }
//   }
//}
//
//int main() {
//   setlocale(LC_ALL, "Russian");
//
//   std::vector<std::vector<double>> A;
//   std::vector<double> b, x;
//   int n;
//
//   // Чтение матрицы и вектора правой части из файлов
//   readMatrix("matrix.txt", A, n);
//   readVector("vector.txt", b);
//
//   x.resize(n, 0.0); // Начальное приближение
//
//   int block_size = 2;
//   int min_block_size = 1;
//   int max_block_size = n / 2;
//   int delta = 1;
//   double residual_prev = std::numeric_limits<double>::infinity();
//
//   for (int iter = 0; iter < max_iterations; ++iter) {
//      for (int i = 0; i < n; i += block_size) {
//         int block_end = std::min(i + block_size, n);
//         blockRelaxationStep(A, b, x, i, block_end);
//      }
//
//      double residual = calculateResidual(A, b, x);
//
//      if (residual < tolerance) {
//         std::cout << "Сходимость достигнута на итерации " << iter << " с относительной невязкой " << residual << std::endl;
//         break;
//      }
//
//      if (residual < residual_prev) {
//         block_size = std::min(block_size + delta, max_block_size);
//      }
//      else {
//         block_size = std::max(block_size - delta, min_block_size);
//      }
//
//      residual_prev = residual;
//
//      std::cout << "Итерация " << iter << ", Относительная невязка: " << residual
//         << ", Текущий размер блока: " << block_size << std::endl;
//   }
//
//   std::ofstream output("solution.txt");
//   for (const auto& xi : x) {
//      output << xi << "\n";
//   }
//   output.close();
//
//   return 0;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////#include <iostream>
////#include <vector>
////#include <cmath>
////#include <fstream>
////#include <algorithm>
////
////using namespace std;
////
////const double tolerance = 1e-6;  // Заданная точность
////const int max_iterations = 1000; // Максимальное количество итераций
////
////// Функция для чтения матрицы из файла (диагональный формат)
////void readMatrix(const std::string& filename, std::vector<std::vector<double>>& A, int& n) {
////   std::ifstream file(filename);
////   if (!file) {
////      std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
////      exit(1);
////   }
////   file >> n;
////   A.resize(n, std::vector<double>(n, 0.0));
////   for (int i = 0; i < n; ++i) {
////      for (int j = std::max(0, i - 3); j <= std::min(n - 1, i + 3); ++j) {
////         file >> A[i][j];
////      }
////   }
////}
////
////// Функция для чтения вектора из файла
////void readVector(const std::string& filename, std::vector<double>& b) {
////   std::ifstream file(filename);
////   if (!file) {
////      std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
////      exit(1);
////   }
////   int n;
////   file >> n;
////   b.resize(n);
////   for (int i = 0; i < n; ++i) {
////      file >> b[i];
////   }
////}
////
////// Функция для вычисления относительной невязки
////double calculateResidual(const std::vector<std::vector<double>>& A, const std::vector<double>& b, const std::vector<double>& x) {
////   int n = A.size();
////   double norm_b = 0.0, norm_residual = 0.0;
////   for (int i = 0; i < n; ++i) {
////      double sum = 0.0;
////      for (int j = 0; j < n; ++j) {
////         sum += A[i][j] * x[j];
////      }
////      norm_b += b[i] * b[i];
////      norm_residual += (sum - b[i]) * (sum - b[i]);
////   }
////   return std::sqrt(norm_residual) / std::sqrt(norm_b);
////}
////
////// Функция для выполнения одного шага блочной релаксации
////void blockRelaxationStep(const std::vector<std::vector<double>>& A, const std::vector<double>& b, std::vector<double>& x, int block_start, int block_end) {
////   for (int i = block_start; i < block_end; ++i) {
////      double sum = 0.0;
////      for (int j = 0; j < A.size(); ++j) {
////         if (i != j) {
////            sum += A[i][j] * x[j];
////         }
////      }
////      x[i] = (b[i] - sum) / A[i][i];
////   }
////}
////
////int main() {
////   setlocale(LC_ALL, "Russian");
////   std::vector<std::vector<double>> A;
////   std::vector<double> b, x;
////   int n;
////
////   // Чтение матрицы и вектора правой части из файлов
////   readMatrix("matrix.txt", A, n);
////   readVector("vector.txt", b);
////
////   x.resize(n, 0.0); // Начальное приближение
////
////   int block_size = 2;       // Начальный размер блока
////   int min_block_size = 1;   // Минимальный размер блока
////   int max_block_size = n / 2; // Максимальный размер блока
////   double delta = 1;         // Шаг изменения размера блока
////   double residual_prev = 1e10; // Высокое начальное значение невязки
////
////   for (int iter = 0; iter < max_iterations; ++iter) {
////      // Итерация по блокам с текущим размером
////      for (int i = 0; i < n; i += block_size) {
////         int block_end = std::min(i + block_size, n);
////         blockRelaxationStep(A, b, x, i, block_end);
////      }
////
////      // Вычисляем текущую относительную невязку
////      double residual = calculateResidual(A, b, x);
////
////      // Проверка выхода по точности
////      if (residual < tolerance) {
////         std::cout << "Сходимость достигнута на итерации " << iter << " с относительной невязкой " << residual << std::endl;
////         break;
////      }
////
////      // Адаптация размера блока на основе сходимости
////      if (residual < residual_prev) {
////         // Если улучшение, увеличиваем размер блока
////         block_size = min(block_size + delta, (double)max_block_size);
////      }
////      else {
////         // Если нет улучшения, уменьшаем размер блока
////         block_size = max(block_size - delta, (double)min_block_size);
////      }
////
////      // Сохраняем текущую невязку для следующей итерации
////      residual_prev = residual;
////
////      // Вывод информации об итерации
////      std::cout << "Итерация " << iter << ", Относительная невязка: " << residual
////         << ", Текущий размер блока: " << block_size << std::endl;
////   }
////
////   // Запись результата в файл
////   std::ofstream output("solution.txt");
////   for (const auto& xi : x) {
////      output << xi << "\n";
////   }
////   output.close();
////
////   return 0;
////}
//
//
//
//
//
//
//
//
//
////#include <iostream>
////
////using namespace std;
////
////class matrix {
////public:
////   void readMatrix();
////   void generateMatrix();
////
////};
////
////
////int main()
////{
////
////}
////
////void matrix::readMatrix()
////{
////
////
////}
////
////void matrix::generateMatrix()
////{
////
////
////}
//
//
//
////#include <iostream>
////#include <vector>
////#include <fstream>
////#include <cmath>
////#include <iomanip>
////
////class DiagonalMatrix {
////public:
////   int n; // Размерность матрицы
////   int m; // Количество нулевых диагоналей
////   std::vector<std::vector<double>> diagonals; // Вектор для хранения диагональных элементов
////
////   DiagonalMatrix(int size, int num_zero_diags) : n(size), m(num_zero_diags) {
////      diagonals.resize(2 * m + 1); // Хранение всех диагоналей
////   }
////
////   void setDiagonal(int offset, const std::vector<double>& values) {
////      diagonals[offset + m] = values;
////   }
////
////   double get(int i, int j) const {
////      int offset = j - i;
////      if (offset + m < 0 || offset + m >= diagonals.size()) {
////         return 0.0; // Если элемент не принадлежит диагоналям
////      }
////      if (diagonals[offset + m].empty()) {
////         return 0.0; // Если диагональ пуста
////      }
////      return diagonals[offset + m][i]; // Возвращаем элемент
////   }
////};
////
////void readInput(const std::string& filename, DiagonalMatrix& matrix, std::vector<double>& b, std::vector<double>& x0, double& tol, int& max_iter) {
////   std::ifstream file(filename);
////   if (!file.is_open()) {
////      throw std::runtime_error("Ошибка открытия файла");
////   }
////
////   file >> matrix.n >> matrix.m;
////   b.resize(matrix.n);
////   x0.resize(matrix.n);
////
////   for (int i = 0; i < matrix.n; ++i) {
////      for (int j = -matrix.m; j <= matrix.m; ++j) {
////         if (j == 0) {
////            double value;
////            file >> value;
////            matrix.setDiagonal(j, { value });
////         }
////         else {
////            // Чтение значений для других диагоналей
////            std::vector<double> diag_values;
////            double value;
////            while (file >> value) {
////               diag_values.push_back(value);
////            }
////            matrix.setDiagonal(j, diag_values);
////         }
////      }
////      file >> b[i];
////      file >> x0[i];
////   }
////
////   file >> tol >> max_iter;
////
////   file.close();
////}
////
////double relativeResidual(const DiagonalMatrix& matrix, const std::vector<double>& x, const std::vector<double>& b) {
////   double norm_b = 0.0;
////   double norm_r = 0.0;
////
////   for (int i = 0; i < matrix.n; ++i) {
////      double Ax = 0.0;
////      for (int j = 0; j < matrix.n; ++j) {
////         Ax += matrix.get(i, j) * x[j];
////      }
////      double r = b[i] - Ax;
////      norm_r += r * r;
////      norm_b += b[i] * b[i];
////   }
////
////   return sqrt(norm_r) / sqrt(norm_b);
////}
////
////void iterativeStep(const DiagonalMatrix& matrix, const std::vector<double>& b, const std::vector<double>& x_old, std::vector<double>& x_new, double relaxation) {
////   for (int i = 0; i < matrix.n; ++i) {
////      double sum = 0.0;
////      for (int j = 0; j < matrix.n; ++j) {
////         if (i != j) {
////            sum += matrix.get(i, j) * x_old[j];
////         }
////      }
////      x_new[i] = (1 - relaxation) * x_old[i] + relaxation * (b[i] - sum) / matrix.get(i, i);
////   }
////}
////
////void jacobiMethod(const DiagonalMatrix& matrix, std::vector<double>& x, const std::vector<double>& b, double relaxation, double tol, int max_iter) {
////   std::vector<double> x_new(matrix.n);
////   for (int iter = 0; iter < max_iter; ++iter) {
////      iterativeStep(matrix, b, x, x_new, relaxation);
////
////      double res = relativeResidual(matrix, x_new, b);
////      std::cout << "Iteration: " << iter + 1 << ", Relative Residual: " << res << std::endl;
////
////      if (res < tol) {
////         break;
////      }
////
////      x = x_new; // Обновляем вектор
////   }
////}
////
////void gaussSeidelMethod(const DiagonalMatrix& matrix, std::vector<double>& x, const std::vector<double>& b, double relaxation, double tol, int max_iter) {
////   for (int iter = 0; iter < max_iter; ++iter) {
////      for (int i = 0; i < matrix.n; ++i) {
////         double sum = 0.0;
////         for (int j = 0; j < matrix.n; ++j) {
////            if (i != j) {
////               sum += matrix.get(i, j) * x[j];
////            }
////         }
////         x[i] = (1 - relaxation) * x[i] + relaxation * (b[i] - sum) / matrix.get(i, i);
////      }
////
////      double res = relativeResidual(matrix, x, b);
////      std::cout << "Iteration: " << iter + 1 << ", Relative Residual: " << res << std::endl;
////
////      if (res < tol) {
////         break;
////      }
////   }
////}
////
////void blockRelaxationMethod(const DiagonalMatrix& matrix, std::vector<double>& x, const std::vector<double>& b, double relaxation, double tol, int max_iter) {
////   // Реализация блочной релаксации
////}
////
////int main() {
////   try {
////      DiagonalMatrix matrix(0, 0);
////      std::vector<double> b;
////      std::vector<double> x0;
////      double tol;
////      int max_iter;
////
////      readInput("input.txt", matrix, b, x0, tol, max_iter);
////
////      // Выбор метода
////      jacobiMethod(matrix, x0, b, 1.0, tol, max_iter);
////
////      // Для метода Гаусса-Зейделя
////      // gaussSeidelMethod(matrix, x0, b, 1.0, tol, max_iter);
////
////      // Для блочной релаксации
////      // blockRelaxationMethod(matrix, x0, b, 1.0, tol, max_iter);
////
////   }
////   catch (const std::exception& e) {
////      std::cerr << "Ошибка: " << e.what() << std::endl;
////   }
////
////   return 0;
////}
//
//
