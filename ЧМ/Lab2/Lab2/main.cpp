#include "Header.h"
#include "SLAE.h"


int main()
{
   ofstream output;
   SLAE slae;
   output.open("A_Jacobi.txt");
   //slae.generateVec(slae.getDimention());
   //slae.mult();
   //slae.generateM(10, 2);
   //slae.writeM("output.txt");
   //exit(1);
   //slae.readParams("A.txt");
   slae.readM("A.txt");
   //cout << slae.findOptimalW(1, output) << endl;


   setlocale(LC_ALL, "Russian");

   //int n = 6;
  // int maxiter = 100;
   std::vector<std::vector<double>> A;
   std::vector<double> b;

   // Чтение матрицы и вектора правой части из файлов
   //slae.readM("A.txt");
   //slae.generateVec(n);
   slae.readV("vec.txt");
   //slae.writeM("output.txt");
   slae.generateXK(slae.n);
   //SLAE slae(n, A, b);
   slae.findOptimalW(2, output);
   //cout << slae.calcIterative(2, 1.61);
   //cout << slae.calcRelativeResidual();

   output.close();
   //slae.convM();


   //slae.readV("vec.txt");
   //int block_size = 2;
   //int min_block_size = 1;
   //int max_block_size = n / 2;
   //int delta = 1;
   //double tolerance = 1e-6;
   //double residual_prev = std::numeric_limits<double>::infinity();

   //for (int iter = 0; iter < maxiter; ++iter) {
   //   // Выполнение шага блочной релаксации
   //   for (int i = 0; i < n; i += block_size) {
   //      int block_end = std::min(i + block_size, n);
   //      slae.blockRelaxationStep(i, block_end);
   //   }

   //   // Подсчет относительной невязки
   //   double residual = slae.calcRelativeResidual();

   //   if (residual < tolerance) {
   //      std::cout << "Сходимость достигнута на итерации " << iter << " с относительной невязкой " << residual << std::endl;
   //      break;
   //   }

   //   // Динамическое изменение размера блока
   //   if (residual < residual_prev) {
   //      block_size = std::min(block_size + delta, max_block_size);
   //   }
   //   else {
   //      block_size = std::max(block_size - delta, min_block_size);
   //   }
   //   block_size = 4;
   //   residual_prev = residual;

   //   std::cout << "Итерация " << iter << ", Относительная невязка: " << residual
   //      << ", Текущий размер блока: " << block_size << std::endl;
   //}

   // Запись результата в файл
   //std::ofstream output("solution.txt");
   //for (const auto& xi : slae.getX()) {
      //output << xi << "\n";
   //}
   output.close();

   return 0;
}