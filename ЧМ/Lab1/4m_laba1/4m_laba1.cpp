#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;
typedef double real;
typedef double real2;

class mat {
public:
   int n, nLU, * ia;
   real* al, * di, * b, * vec;
   mat() {};
   void input(ifstream& inmat, ifstream& invec);
   void giveMemory();
   void LU();
   void output(ofstream& res);
   void getY();
   void getX();
   void Gilbert();
};

void mat::getY()
{
   for (int i = 0; i < n; i++)
   {
      real2 sum = 0;
      int i0 = ia[i] - 1;
      int i1 = ia[i + 1] - 1;
      int j = i - (i1 - i0);
      for (int k = i0; k < i1; k++, j++)
         sum += b[j] * al[k];
      b[i] = (b[i] - sum) / di[i];
   }
}

void mat::getX()
{
   for (int i = n - 1; i >= 0; i--)
   {
      real2 sum = 0;
      for (int k = i + 1; k < n; k++)
      {
         if (i >= (k - (ia[k + 1] - ia[k])))
            sum += al[ia[k + 1] + i - 1 - k] * b[k];
      }
      b[i] = (b[i] - sum) / di[i];
      cout << b[i] << " " << endl;
   }
}

void mat::input(ifstream& inmat, ifstream& invec)
{
   inmat >> n;
   ia = new int[n + 1];
   for (int i = 0; i < n + 1; i++)
      inmat >> ia[i];

   nLU = ia[n] - 1;

   giveMemory();

   for (int i = 0; i < nLU; i++)
      inmat >> al[i];
   for (int i = 0; i < n; i++)
      inmat >> di[i];
   for (int i = 0; i < n; i++)
      invec >> b[i];
}

void mat::giveMemory() {
   al = new real[nLU];
   di = new real[n];
   b = new real[n];
   vec = new real[n];
}

void mat::output(ofstream& res)
{
   res << "x" << " " << endl;
   for (int i = 0; i < n; i++)
   {
      res << " " << b[i] << endl;
   }
   res << "\n";
   res << "di" << " ";
   for (int i = 0; i < n; i++)
   {
      res << " " << di[i];
   }
   res << "\n";
   res << "ia" << " ";
   for (int i = 0; i < n + 1; i++)
   {
      res << " " << ia[i];
   }
   res << "\n";
   res << "al" << " ";
   for (int i = 0; i < nLU; i++)
   {
      res << " " << al[i];
   }
   res << "\n";
}

void MV(int n, real* b, real* vec, int* ia, real* al)
{
   for (int i = 0; i < n; i++)
   {
      b[i] *= vec[i];
      int i0 = ia[i] - 1;
      int i1 = ia[i + 1] - 1;
      int j = i - (i1 - i0);
      for (int k = i0; k < i1; k++, j++)
      {
         b[i] += al[k] * vec[j];
         b[j] += al[k] * vec[i];
      }
   }
}

void mat::LU()
{
   for (int i = 0; i < n; i++)
   {
      int i0 = ia[i] - 1;
      int i1 = ia[i + 1] - 1;
      real2 sum = 0;
      for (int p = i0; p < i1; p++)
         sum += al[p] * al[p];
      if (sum > di[i])
      {
         n = 0;
         cout << "koren iz otritsatelnogo";
      }
      di[i] = sqrt(di[i] - sum);

      for (int j = i + 1; j < n; j++) {
         sum = 0;
         int j0 = ia[j] - 1;
         int j1 = ia[j + 1] - 1;
         int J = j - (j1 - j0);

         for (int p = i0, k = i - (i1 - i0); p < i1; p++, k++)
         {
            if (k >= J)
            {
               sum += al[p] * al[j1 - j + k];
            }
         }

         if (i >= J)
         {
            al[j1 - j + i] = (al[j1 - j + i] - sum) / di[i];
         }
      }
   }
}

void mat::Gilbert()
{
   ia = new int[n + 1];
   ia[0] = 1;
   ia[1] = 1;
   for (int i = 2; i < n + 1; i++)
   {
      ia[i] = ia[i - 1] + i - 1;
   }
   nLU = ia[n] - 1;
   giveMemory();


   for (int i = 0; i < n; i++)
   {
      vec[i] = real(i + 1);
   }

   for (int i = 0; i < n; i++)
   {
      di[i] = real(1.0 / (2 * i + 1));
      b[i] = di[i];
   }

   int k = 0;
   for (int i = 1; i < n + 1; i++)
      for (int j = 1; j < i; j++, k++)
         al[k] = real(1.0 / (i + j - 1));


   MV(n, b, vec, ia, al);
}

void Gauss(int* ia, real* b, real* di, real* al, int n)
{
   real** A = new real * [n];
   for (int i = 0; i < n; i++)
      A[i] = new real[n];

   for (int i = 0; i < n; i++)
   {
      for (int j = i + 1; j < n; j++)
      {
         if (i >= j - (ia[j + 1] - ia[j]))
            A[i][j] = A[j][i] = al[ia[j + 1] - 1 + i - j];
         else
            A[i][j] = A[j][i] = 0;
      }
   }

   for (int i = 0; i < n; i++)
      A[i][i] = di[i];

   double del;
   for (int k = 0; k < n; k++)
   {
      for (int j = k + 1; j < n; j++)
      {
         del = A[j][k] / A[k][k];
         for (int i = k; i < n; i++)
            A[j][i] = A[j][i] - del * A[k][i];
         b[j] = b[j] - del * b[k];
      }
   }

   real* x = new real[n];
   for (int i = 0; i < n; i++)
      x[i] = 0;
   for (int k = n - 1; k >= 0; k--)
   {
      del = 0;
      for (int j = k; j < n; j++)
         del += A[k][j] * x[j];
      x[k] = (b[k] - del) / A[k][k];
   }
   for (int i = 0; i < n; i++)
      b[i] = x[i];
   for (int i = 0; i < n; i++)
      delete[] A[i];
   delete[] x;
}

int main()
{
   mat A;

   ifstream inmat("inmat.txt");
   ifstream invec("invec.txt");
   ofstream res("out.txt");

   cout.precision(16);
   res.precision(16);
   int a = 0;
   cout << "normal-1\ngilbert-2\nGaus-3\n";
   cin >> a;
   switch (a)
   {
   case 1:
      A.input(inmat, invec);
      A.LU();
      A.getY();
      A.getX();
      A.output(res);
      break;
   case 2:
      cin >> A.n;
      A.Gilbert();
      A.LU();
      A.getY();
      A.getX();
      A.output(res);
      break;
   case 3:
      A.input(inmat, invec);
      Gauss(A.ia, A.b, A.di, A.al, A.n);
      A.output(res);
   default:
      break;
   }
}
