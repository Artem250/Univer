#include "Header.h"

class Vec
{
public:
   int readV(const char* filename);
   void generateXK(int size) { xk.clear(); xk.resize(size, real(0)); }
   int generateVec(int size);
   bool isXcorrect();
   void generateInitualGuess(int size) { xk.clear(); xk.resize(size, real(0)); }
   void writeTableToFile(std::ofstream& fout, int presision, real w, int iterations, real condNumber);


   vector <real> F, xk;
};

