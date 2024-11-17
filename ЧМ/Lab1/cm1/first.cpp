#include <iostream>
#include <fstream>
using namespace std;
const int N = 4, M = 2;

void input(float (*AL)[M], float (*AU)[M], float (*DI));
void outputM(int n, float (*A)[M]);
void outputN(int n, float (*A)[N]);
void outputDI(int n, float (*A));
void makeMatrix(float (*AL)[M], float (*AU)[M], float (*DI), float (*A)[N]);
void makeLUMatrix(float (*AL)[M], float (*AU)[M], float (*DI));
void popa(float (*AL)[M], float (*AU)[M], float (*DI), float (*L)[N], float (*U)[N], float (*A)[N]);
void mult(float (*L)[N], float (*U)[N], float (*A)[N]);

int main() {
    float A[N][N];
    float AL[N][M];
    float AU[N][M];
    float DI[N];

    float L[N][N] = {0};
    float U[N][N] = {0};

    input(AL, AU, DI);
    makeMatrix(AL, AU, DI, A);
    cout << "A" << endl;
    outputN(N, A);

    popa(AL, AU, DI, L, U, A);

    makeLUMatrix(AL, AU, DI);
    cout << "L" << endl;
    outputN(N, L);
    outputM(M, AL);
    cout << "U" << endl;
    outputN(N, U); 
    outputM(M, AU);
    //cout << "DI" << endl;
    //outputDI(M, DI);
        
    //mult(L, U, A);
    // cout << "Ans" << endl;
    //output(N, U);
    // cout << "Ll, Ul" << endl;

   
}


void popa(float (*AL)[M], float (*AU)[M], float (*DI), float (*L)[N], float (*U)[N], float (*A)[N]) {
    for (int i = 0; i < N; i++) {
        int h = i - M;
        int cnt = 0;
        if (h < 0) {
          cnt = abs(i-M);
          h = 0;
        } else {
          cnt = 0;
        }
        for (int j = 0; j <= i; j++) {
            if(j >= h) {
              float s = 0.0;
              for (int k = 0; k < j; k++) {
                  s += L[i][k] * U[k][j];
              }
              if (i == j) {
                L[i][j] = DI[i] - s;
              } else {
                L[i][j] = AL[i][cnt] - s;
              }
              cnt++;
            }
        }
        
        cnt = M;
        if (h > N) { 
          h = N; 
        } else {
          h = i + M + 1;
        }
        for (int j = i; j < N; j++) {
            if (j < h) {
                float s = 0.0;
                for (int k = 0; k < i; k++) {
                    s += L[i][k] * U[k][j];
                }
                if (i == j) {
                    U[i][j] = DI[i] - s;
                } else {
                    U[i][j] = AU[j][cnt] - s;
                }
                
                if (L[i][i] == 0) {
                    throw runtime_error("Matrix is singular."); // бросаем исключение
                } else {
                    U[i][j] /= L[i][i];
                }
            }
            cnt--;
        }
    }
}

void makeLUMatrix(float (*AL)[M], float (*AU)[M], float (*DI)) {
    float sAU = 0, sAL = 0, sDI = 0;
    for (int i = 1; i < N; ++i) {
        sDI = 0;
        if (i < M) {
            for (int j = M - i; j < M; ++j) {
                sAU = 0;
                sAL = 0;
                for (int k = 0; k  < j - (M - i); ++k) {
                    sAL += AL[i][M - i + k] * AU[j - (M - i)][(2 * M) - j - i + k];
                    sAU += AL[M - i + k][i] * AU[(2 * M) - j - i + k][j - (M - i)];
                }

                AL[i][j] -= sAL;
                AL[i][j] /= DI[ - (M - i)];
                AU[i][j] -= sAU;
            }
        } else {
            for (int j = 0; j < M; ++j) {
                sAL = 0;
                sAU = 0;
                for (int  k = 1; k <= j; ++k) {
                    sAL += AL[i][k - 1] * AU[j - (M - i)][M - j + k - 1];
                    sAU += AL[j - (M - i)][M - j + k - 1] * AU[i][k - 1];
                }
                AL[i][j] -= sAL;
                AL[i][j] /= DI[i - (M - j)];
                AU[i][j] -= sAU;
            }
        }
        for (int j = 0; j < M; j++) {
            sDI += AL[i][j] * AU[i][j];
        }
        DI[i] -= sDI;
    }
}


void mult(float (*L)[N], float (*U)[N], float (*A)[N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float s = 0;
            for (int k = 0; k < N; k++) {
                s += L[i][k] * U[k][j]; // поправлено
            }
            if (s != A[i][j]) {
                cout << "erroe ij: " << i << j << "  " << A[i][j] << "  " << s << endl;
            }
            A[i][j] = s;
        }
    }
}

void makeMatrix(float (*AL)[M], float (*AU)[M], float (*DI), float (*A)[N]) {
    for (int i = 0; i < N; i++) A[i][i] = DI[i];

    for (int i = 1; i < M + 1; i++) {
        for (int j = 0; j < i; j++) {
            A[i][j] = AL[i][M-j-1];
            A[j][i] = AU[i][M-j-1];
        }
    }
    for (int i = M + 1, i1 = M + 1; i < N; i++, i1++) {
        for (int j = i - M, j1 = 0; j < i; j++, j1++) {
            A[i][j] = AL[i1][j1];
            A[j][i] = AU[i1][j1];
        }
    }
    for (int i = M + 1, i1 = M + 1; i < N; i++, i1++) {
        for (int j = 0; j < i - M; j++) {
            A[i][j] = 0;
            A[j][i] = 0;
        }
    }
}

void input(float (*AL)[M], float (*AU)[M], float (*DI)) {
    
    ifstream inAL("al.txt");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) inAL >> AL[i][j];
    }
    inAL.close();

    ifstream inAU("au.txt");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) inAU >> AU[i][j];
    }
    inAU.close();

    ifstream inDI("di.txt");
    for (int i = 0; i < N; i++) inDI >> DI[i];
    inDI.close();
}

void outputN(int n, float (*A)[N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < n; j++) {
            cout << '\t' << A[i][j];
        }
        cout << endl;
    }
    cout << endl << endl;
}

void outputM(int n, float (*A)[M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < n; j++) {
            cout << '\t' << A[i][j];
        }
        cout << endl;
    }
    cout << endl << endl;
}

void outputDI(int n, float (*A)) {
    for (int i = 0; i < N; i++) {
        cout << '\t' << A[i] << endl;
    }
    cout << endl << endl;
}
