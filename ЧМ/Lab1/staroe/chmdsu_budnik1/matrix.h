#include <cstdio>
#include <math.h>

typedef float real;
typedef double realSum;
#define REALIN "%f"
#define REALOUT "%.7f\n"
#define REALOUTD "%.4f\t"
#define EPS 1e-13
class BaseMatrix {
public:
   int n;
   real* x, * b;

   virtual void Input(FILE* matrixFile, FILE* vectorFile) {}
   virtual void SolveSLAE() {}
   virtual void OutputDense() {}
   virtual void GenerateHilbertMatrix(int n) {}
   void OutputSolutionVector(FILE* out);
protected:
   virtual void AllocateMemory() {}
};
class ProfileMatrix : public virtual BaseMatrix {
public:
   real* al, * au, * di;
   int* ia, nProfile;

   void Input(FILE* matrixFile, FILE* vectorFile);
   void GenerateHilbertMatrix(int n);
   void SolveSLAE();
   void OutputDense();
   ProfileMatrix() {};
private:
   real* y;
   void AllocateMemory();
   void CalculateLU();
   void CalculateY();
   void CalculateX();
};
class DenseMatrix : public virtual BaseMatrix {
public:
   real** matrix;

   void Input(FILE* matrixFile, FILE* vectorFile);
   void SolveSLAE();
   void OutputDense();
private:
   void AllocateMemory();
   void GaussForwardElimination();
   void GaussBackSubstitution();
};
#pragma once
