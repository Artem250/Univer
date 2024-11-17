#include <iostream>
#include <vector>

#define D_REAL
#define D_REALSUM

#ifdef D_REALSUM
typedef double real_sum;
#else
typedef float real_sum;
#endif

#ifdef D_REAL
typedef double real;
#define realfmt_out "%.15lf"
#define realfmt_in "%lf"
#else
typedef float real;
#define realfmt_out "%.7f"
#define realfmt_in "%f"
#endif

void calcLLt(real* al, real* di, int n, int w2) {
   for (int i = 0; i < n; i++) {
      real_sum s_diag = 0;
      for (int j = i - w2, j_band = 0, ij = i * w2; j_band < w2; j_band++, j++, ij++) {
         if (j < 0) continue;

         printf("%d %d\n", i, j_band);

         real_sum s = 0;
         int end = j_band + i * w2;
         for (int i_k = i * w2, j_k = i - j + j * w2; i_k < end; i_k++, j_k++)
            s += al[i_k] * al[j_k];
         al[ij] = (al[ij] - s) / di[j];
         printf("%f\n\n", al[ij]);
         s_diag += al[ij] * al[ij];
      }
      di[i] = sqrt(di[i] - s_diag);
   }
}

void calcLU(real* al, real* di, int n, int w2) {
   for (int i = 0; i < n; i++) {
      real_sum s_diag = 0;
      for (int j = i - w2, j_band = 0, ij = i * w2; j_band < w2; j_band++, j++, ij++) {

         if (j < 0) continue;
         printf("%d %d", i, j);
         real_sum s = 0;
         int end = j_band + i * w2;
         for (int i_k = i * w2, j_k = i - j + j * w2; i_k < end; i_k++, j_k++)
            s += al[i_k] * al[j_k];
         al[ij] = (al[ij] - s) / di[j];

         s_diag += al[ij] * al[ij];
      }
      di[i] = sqrt(di[i] - s_diag);
   }
}

void calcY(real* al, real* di, real* b, real* y, int n, int w2) {
   for (int i = 0; i < n; i++) {
      real_sum s = 0;
      for (int j = i - w2, j_band = 0, ij = i * w2; j_band < w2; j_band++, j++, ij++)
         if (j >= 0)
            s += al[ij] * y[j];
      y[i] = (b[i] - s) / di[i];
   }
}

void calcX(real* al, real* di, real* y, real* x, int n, int w2) {
   for (int i = n - 1; i >= 0; i--) {
      x[i] = y[i] / di[i];
      for (int col = 0, k = i - 1, ij = (i + 1) * w2 - 1; col < w2; col++, k--, ij--)
         y[k] -= x[i] * al[ij];
   }
}

int readSize(int* n, int* w) {
   FILE* file;
   fopen_s(&file, "size.txt", "r");
   if (file) {
      fscanf_s(file, "%d %d", n, w);
      fclose(file);
      return 0;
   }
   else {
      printf("Error reading file: size.txt\n");
      return 1;
   }
}

int readMatrix(real* al, real* di, int n, int w2) {
   FILE* file;

   fopen_s(&file, "al.txt", "r");
   if (file) {
      for (int i = 0; i < w2 * n; i++)
         fscanf_s(file, realfmt_in, &al[i]);
      fclose(file);
   }
   else {
      printf("Error reading file: al.txt\n");
      return 1;
   }

   fopen_s(&file, "di.txt", "r");
   if (file) {
      for (int i = 0; i < n; i++)
         fscanf_s(file, realfmt_in, &di[i]);
      fclose(file);
   }
   else {
      printf("Error reading file: di.txt\n");
      return 1;
   }
}

int readVector(real* b, int n) {
   FILE* file;

   fopen_s(&file, "b.txt", "r");
   if (file) {
      for (int i = 0; i < n; i++)
         fscanf_s(file, realfmt_in, &b[i]);
      fclose(file);
   }
   else {
      printf("Error reading file: b.txt\n");
      return 1;
   }
}

int writeX(real* x, int n) {
   FILE* file;
   fopen_s(&file, "x.txt", "w");
   if (file) {
      for (int i = 0; i < n; i++)
         fprintf_s(file, realfmt_out "\n", x[i]);
      fclose(file);
      return 0;
   }
   else {
      printf("Error writing file: x.txt\n");
      return 1;
   }
}

void createGilbert(real* al, real* di, int n, int w2) {
   for (int i = 0; i < n; i++) {
      for (int j = i - w2, j_band = 0; j_band < w2; j_band++, j++) {
         if (j < 0) al[i * w2 + j_band] = 0;
         else al[i * w2 + j_band] = 1.0 / (i + j + 1);
      }
      di[i] = 1.0 / (i + i + 1);
   }
}

int calcLead(std::vector <std::vector <real>> matrix, int N, int i)
{
   double lead = matrix[i][i];
   int lead_i = i;
   for (int k = i + 1; k < N; k++)
      if (abs(matrix[k][i]) > abs(lead))
      {
         lead = matrix[k][i];
         lead_i = k;
      }
   return lead_i;
}

void gauss(real* al, real* di, real* b, real* x, int n, int w2)
{
   std::vector <std::vector <real>> matrix;
   std::vector <real> b_vec;
   matrix.resize(n);
   b_vec.resize(n);

   for (int i = 0; i < n; i++) {
      b_vec[i] = b[i];
      matrix[i].resize(n);
      matrix[i][i] = di[i];
      for (int j = i - w2, j_band = 0; j_band < w2; j_band++, j++)
         if (j < 0) continue;
         else matrix[i][j] = matrix[j][i] = al[i * w2 + j_band];
   }

   for (int i = 0; i < n; i++) {
      int lead_i = calcLead(matrix, n, i);
      matrix[i].swap(matrix[lead_i]);
      std::swap(b_vec[i], b_vec[lead_i]);
      for (int k = i + 1; k < n; k++) {
         real coef = matrix[k][i] / matrix[i][i];
         for (int j = i; j < n; j++)
            matrix[k][j] -= coef * matrix[i][j];
         b_vec[k] -= coef * b_vec[i];
      }
   }

   for (int i = n - 1; i >= 0; i--) {
      real_sum sum = 0;
      for (int j = n - 1; j > i; j--)
         sum += matrix[i][j] * x[j];
      x[i] = (b_vec[i] - sum) / matrix[i][i];
   }
}

int main() {
   int n;
   int w;
   if (readSize(&n, &w) != 0)
      return 1;
   int w2 = (w - 1) / 2;
   printf("%d\n\n\n", w2);
   real* al = new real[w2 * n], * di = new real[n], * b = new real[n];
   real* y = new real[n], * x = new real[n];
   if (readMatrix(al, di, n, w2) != 0)
      return 1;
   if (readVector(b, n) != 0)
      return 1;

   //createGilbert(al, di, n, w2);
   //gauss(al, di, b, x, n, w2);

   calcLLt(al, di, n, w2);
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         printf("%f ", al[i * w2 + j]);
      }
      printf("\n");
   }
   // calcY(al, di, b, y, n, w2);
   calcX(al, di, y, x, n, w2);

   writeX(y, n);

   return 0;
}
