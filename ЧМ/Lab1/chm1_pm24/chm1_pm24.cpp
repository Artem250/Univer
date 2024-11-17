#include <iostream>
#include <fstream>
using namespace std;
const int N = 10, M = 9;

typedef float pupupu;

void input(pupupu(*AL)[M], pupupu(*AU)[M], pupupu(*DI), pupupu(*B));
void outputM(pupupu(*A)[M]);
void output1(pupupu(*A));

void makeLUMatrix(pupupu(*AL)[M], pupupu(*AU)[M], pupupu(*DI));
void forwardWay(pupupu(*AL)[M], pupupu(*B), pupupu(*Y));
void backwardWay(pupupu(*AU)[M], pupupu(*DI), pupupu(*X), pupupu(*Y));

int main() {
   pupupu AL[N][M];
   pupupu AU[N][M];
   pupupu DI[N];
   pupupu B[N];
   pupupu X[N];
   pupupu Y[N];

   input(AL, AU, DI, B);
   makeLUMatrix(AL, AU, DI);

   cout << "L" << endl;
   outputM(AL);
   cout << "U" << endl;
   outputM(AU);
   output1(DI);

   forwardWay(AL, B, Y);
   backwardWay(AU, DI, X, Y);

   output1(Y);
   output1(X);
}

void backwardWay(pupupu(*AU)[M], pupupu(*DI), pupupu(*X), pupupu(*Y)) {
   X[N - 1] = Y[N - 1] / DI[N - 1];
   for (int i = N - 1; i > 0; --i) {
      pupupu sX = 0;
      int help = i - 1;
      if (i > N - M) {
         for (int j = 0; j < N - i; ++j) sX += AU[i + j][M - j - 1] * X[i + j];
      }
      else {
         for (int j = 0; j < M; ++j) sX += AU[i + j][M - j - 1] * X[i + j];
      }
      X[help] = (Y[help] - sX) / DI[help];
   }
}

void forwardWay(pupupu(*AL)[M], pupupu(*B), pupupu(*Y)) {
   Y[0] = B[0];
   for (int i = 0; i < N; i++) {
      pupupu sY = 0;
      if (i < M) {
         for (int j = 0; j < i; ++j) sY += AL[i][M - i + j] * B[j];
      }
      else {
         for (int j = 0; j < M; ++j) sY += AL[i][j] * B[j + i - M];
      }
      Y[i] = sY + 1 * B[i];
   }
}

void makeLUMatrix(pupupu(*AL)[M], pupupu(*AU)[M], pupupu(*DI)) {
   pupupu sAU = 0, sAL = 0, sDI = 0;
   for (int i = 1; i < N; ++i) {
      sDI = 0;
      if (i < M) {
         for (int j = M - i; j < M; ++j) {
            //cout << "1" << endl;
            sAU = 0;
            sAL = 0;
            for (int k = 0; k < j - (M - i); ++k) {
               //cout << "popa: " << i << M - i + k << "    " << j - (M - i) << (2 * M) - j - i + k << endl;
               sAL += AL[i][M - i + k] * AU[j - (M - i)][(2 * M) - j - i + k];
               sAU += AL[M - i + k][i] * AU[(2 * M) - j - i + k][j - (M - i)];
            }

            AL[i][j] -= sAL;
            AL[i][j] /= DI[j - (M - i)];
            AU[i][j] -= sAU;
         }
      }
      else {
         for (int j = 0; j < M; ++j) {
            //cout << "2" << endl;
            sAL = 0;
            sAU = 0;
            for (int k = 1; k <= j; ++k) {
               //cout << "popa: " << i << k - 1 << "    " << j - (M - i) << M - j + k - 1 << endl;
               sAL += AL[i][k - 1] * AU[j - (M - i)][M - j + k - 1];
               sAU += AL[j - (M - i)][M - j + k - 1] * AU[i][k - 1];
            }
            AL[i][j] -= sAL;
            AL[i][j] /= DI[i - (M - j)];
            AU[i][j] -= sAU;
         }
      }
      for (int j = 0; j < M; j++) sDI += AL[i][j] * AU[i][j];
      DI[i] -= sDI;
   }
}

void input(pupupu(*AL)[M], pupupu(*AU)[M], pupupu(*DI), pupupu(*B)) {

   ifstream inAL("al.txt");
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         inAL >> AL[i][j];
      }
   }
   inAL.close();

   ifstream inAU("au.txt");
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         inAU >> AU[i][j];
      }
   }
   inAU.close();

   ifstream inDI("di.txt");
   for (int i = 0; i < N; i++) {
      inDI >> DI[i];
   }
   inDI.close();

   ifstream inB("b.txt");
   for (int i = 0; i < N; i++) {
      inB >> B[i];
   }
   inB.close();
}

void outputM(pupupu(*A)[M]) {
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
         cout << '\t' << A[i][j];
      }
      cout << endl;
   }
   cout << endl << endl;
}

void output1(pupupu(*A)) {
   for (int i = 0; i < N; i++) {
      cout << '\t' << A[i] << endl;
   }
   cout << endl << endl;
}