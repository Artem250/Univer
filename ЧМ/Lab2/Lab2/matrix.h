#include "Header.h"

class matrix
{

public:
   int readParams(const char* filename);
   int readM(const char* filename);
   int writeM(const char* filename);
   //int getDimention() { return n; }
   void setE(real new_eps) { eps = new_eps; }
   void setMaxiter(real new_maxiter) { maxiter = new_maxiter; }
   void generateM(int new_n, int new_m);
   void invertSigns();
   //void blockRelaxationStep(int start, int end);

   int n, m, maxiter;

   real calcAii(int i);
   vector <real> di, au1, au2, au3, al1, al2, al3;
   real eps;
   vector <vector <real>> A;

};

//всё, что закоменчено, будет использоваться в будущем, но реализации сейчас нема