#include "Header.h"
#include"matrix.h"
#include "Vec.h"


class SLAE : public matrix, public Vec {

   vector <vector <real>> A;
public:
   void convM();
   void convMto();
   //void writeDenseMatrixToFile(char* fileName);

   real multLine(vector<real>& line, int i, int mode);
   void mult();

   void calcJacobi(real w);
   void calcGaussSeildel(real w);
   int calcIterative(int mode, real w);
   real findOptimalW(int mode, std::ofstream& fout);
   void blockRelaxationStep(int start, int end);


   real calcRelativeResidual();
   real calcNormE(vector <real>& x);
   real calcCondNumber();

};


