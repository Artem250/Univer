#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>


using namespace std;
typedef float real;
#define realin "%f"


void input(FILE* inmat, FILE* invec) {
   int n, m;
   void AllocateMemory();
   real** al, ** au;
   real * di, * b, * vec;
   fscanf(inmat, "%d %d", &n, &m);

   //AllocateMemory();

/*
   for (int i = 0; i < n; i++)
      fscanf_s(inmat, real, &al[i]);

*/
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(inmat, realin, &al[j][i]);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(inmat, realin, &au[j][i]);
        }
    }



   /*for (int i = 0; i < nProfile; i++)
      fscanf_s(inmat, REALIN, &au[i]);
*/
   for (int i = 0; i < n; i++)
      fscanf(inmat, realin, &di[i]);

   for (int i = 0; i < n; i++)
      fscanf(inmat, realin, &b[i]);
      //

}
/*
void calcLU() {
    int n = 4;
    int m = 4;
    int k0, k, i0, j0, i1;
    double y[3] {1,1,1};
    double di[3] {1,2,3};
    for (int i = 0; i < n; i++) {

        k0 = 0;
        k = 0;
        y[i] = di[i];
        j0 = i - m;
        i1 = 1;
        for (int j = j0, k = k0; j < i && k < i1; j++, k++) {
            if (j < 0) continue;
            y[i] += al[i][k]*x[j];
            y[j] += au[i][k]*x[i];
        }
    }

    }
    */
/*
void ProfileMatrix::CalculateLU()
{
   for (int i = 0; i < n; i++)
   {
	  int i0 = ia[i];
	  int i1 = ia[i + 1];
	  realSum sumD = 0;
	  int j = i - (i1 - i0);
	  for (int k = i0; k < i1; j++, k++)
	  {
		 realSum sumL = 0, sumU = 0;

		 int j0 = ia[j], j1 = ia[j + 1];

		 int difI = i1 - i0 - (i - j);
		 int difJ = j1 - j0;
		 int difIJ = difI - difJ;

		 int ki = i0, kj = j0;

		 if (difIJ < 0)
			kj -= difIJ;
		 else
			ki += difIJ;

		 for (; ki < k; ki++, kj++)
		 {
			sumL += al[ki] * au[kj];
			sumU += al[kj] * au[ki];
		 }

		 al[k] = al[k] - sumL;
		 au[k] = (au[k] - sumU) / di[j];
		 sumD += al[k] * au[k];
	  }
	  di[i] -= sumD;
   }*/

void AllocateMemory() {
   real** al = new real*[4];
   for (int i = 0; i < 4; ++i) {
      al[i] = new real[1];
   }
   real** au = new real*[4];
   for (int i = 0; i < 4; ++i) {
      au[i] = new real[1];
   }
   real* di = new real[4];
   real* b = new real[4];
   real* x = b;
   real* y = b;
}

int main() {
   int n;
   real* al, * au, * di, * b, * vec;
   FILE* inmat;
   FILE* invec;
   fopen_s(&inmat, "inmat.txt", "r");
	fopen_s(&invec, "invec.txt", "r");
   input(inmat, invec);
   cout << al[1];
}
