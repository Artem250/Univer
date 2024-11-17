#include "Vec.h"

int Vec::readV(const char* filename)
{
   fstream inputV;
   int n;
   inputV.open(filename);
   inputV >> n;
   F.resize(n);
   for (int i = 0; i < n; i++)
      inputV >> F[i];
   inputV.close();
   return 0;
}

int Vec::generateVec(int size)
{
   xk.resize(size);
   for (int i = 0; i < size; i++)
      //xk[i] = i + 1;
      xk[i] = 0;
   return 0;
}

bool Vec::isXcorrect()
{
   for (int i = 0; i < xk.size(); ++i)
   {
      if (abs(xk[i] - (real)(i + 1)) > std::numeric_limits<real>::digits10 + 2)
         return false;
   }
   return true;
}

void Vec::writeTableToFile(std::ofstream& fout, int presision, real w, int iterations, real condNumber)
{

   fout << std::fixed << std::setprecision(presision) << w << "\t";
   fout << std::fixed << std::setprecision(std::numeric_limits<real>::digits10 + 1);
   for (int i = 0; i < xk.size(); ++i)
      fout << xk[i] << " ";
   fout << " \t";

   fout << std::scientific;
   for (int i = 0; i < xk.size(); ++i)
      fout << xk[i] - real(i + 1) << " ";
   fout << " \t";

   fout << iterations << "\t";
   fout << condNumber << endl;
}

