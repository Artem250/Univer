#include "matrix.h"

int matrix::readParams(const char* filename)
{
   fstream inputP;
   //inputP.open(filename);
   inputP >> n >> m;
   inputP >> eps >> maxiter;
   return 0;
}

int matrix::readM(const char* filename)
{
   fstream inputM;
   inputM.open(filename);

   inputM >> n >> m;
   inputM >> eps >> maxiter;

   di.resize(n);
   for (int i = 0; i < di.size(); i++)
      inputM >> di[i];

   al1.resize(n - 1);
   for (int i = 0; i < al1.size(); i++)
      inputM >> al1[i];

   al2.resize(n - m - 2);
   for (int i = 0; i < al2.size(); i++)
      inputM >> al2[i];

   al3.resize(n - m - 3);
   for (int i = 0; i < al3.size(); i++)
      inputM >> al3[i];

   au1.resize(n - 1);
   for (int i = 0; i < au1.size(); i++)
      inputM >> au1[i];

   au2.resize(n - m - 2);
   for (int i = 0; i < au2.size(); i++)
      inputM >> au2[i];

   au3.resize(n - m - 3);
   for (int i = 0; i < au3.size(); i++)
      inputM >> au3[i];

   inputM.close();
   return 0;
}

int matrix::writeM(const char* filename)
{
   ofstream output;
   output.open(filename);
   output << n << ' ' << m << endl;
   output << eps << ' ' << maxiter << endl;

   for (int i = 0; i < n; ++i)
      output << di[i] << " ";
   output << endl;

   for (int i = 0; i < al1.size(); ++i)
      output << al1[i] << " ";
   output << endl;

   for (int i = 0; i < al2.size(); ++i)
      output << al2[i] << " ";
   output << endl;

   for (int i = 0; i < al3.size(); ++i)
      output << al3[i] << " ";
   output << endl;

   for (int i = 0; i < au1.size(); ++i)
      output << au1[i] << " ";
   output << endl;

   for (int i = 0; i < au2.size(); ++i)
      output << au2[i] << " ";
   output << endl;

   for (int i = 0; i < au3.size(); ++i)
      output << au3[i] << " ";
   output << endl;

   output.close();
   return 0;
}

void matrix::generateM(int new_n, int new_m)
{
   n = new_n;
   m = new_m;
   di.clear();
   di.resize(n, 0);
   au1.resize(n - 1);
   al1.resize(n - 1);
   au2.resize(n - m - 2);
   al2.resize(n - m - 2);
   au3.resize(n - m - 3);
   al3.resize(n - m - 3);
   for (int i = 0; i < al1.size(); ++i)
   {
      au1[i] = -rand() % 5;
      al1[i] = -rand() % 5;
   }

   for (int i = 0; i < al2.size(); ++i)
   {
      au2[i] = -rand() % 5;
      al2[i] = -rand() % 5;
   }

   for (int i = 0; i < al3.size(); ++i)
   {
      au3[i] = -rand() % 5;
      al3[i] = -rand() % 5;
   }

   for (int i = 0; i < di.size(); ++i)
      di[i] = -calcAii(i);

   di[0]++;
   eps = 1e-6;
   maxiter = 10;
}

real matrix::calcAii(int i) {

   real sum = 0;

   if (i >= 1)
   {
      sum += al1[i - 1];
      if (i >= m + 2)
      {
         sum += al2[i - m - 2];
         if (i >= m + 3)
            sum += al3[i - m - 3];
      }
   }

   sum += di[i];

   if (i < n - 1)
   {
      sum += au1[i];
      if (i < n - m - 2)
      {
         sum += au2[i];
         if (i < n - m - 3)
            sum += au3[i];
      }
   }
   return sum;
}

void matrix::invertSigns()
{

   for (int i = 0; i < al1.size(); ++i)
   {
      au1[i] = abs(au1[i]);
      al1[i] = abs(al1[i]);
   }

   for (int i = 0; i < al2.size(); ++i)
   {
      au2[i] = abs(au2[i]);
      al2[i] = abs(al2[i]);
   }

   for (int i = 0; i < al3.size(); ++i)
   {
      au3[i] = abs(au3[i]);
      al3[i] = abs(al3[i]);
   }
}
