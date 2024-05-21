#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <random>
#include <cblas.h>
#include <omp.h>

using namespace std;
using namespace chrono;

// ������� ������������ ������
void matrixMultiply(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// ������������ ������ � �������������� BLAS
void matrixMultiplyBLAS(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    int n = A.size();
    // �������������� �������� � ������� ��� BLAS
    double* a = new double[n * n];
    double* b = new double[n * n];
    double* c = new double[n * n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = A[i][j];
            b[i * n + j] = B[i][j];
        }
    }
    // ����� BLAS �������
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n,
        1.0, a, n, b, n, 0.0, c, n);
    // ����������� ���������� �� ������� � ������
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = c[i * n + j];
        }
    }
    
    delete[] a;
    delete[] b;
    delete[] c;
}

// ������� �������� ������������ ������
void matrixMultiplyBlock(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    int n = A.size();
    int blockSize = 32; // ������ �����
    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < n; j += blockSize) {
            for (int k = 0; k < n; k += blockSize) {
                for (int ii = i; ii < min(i + blockSize, n); ii++) {
                    for (int jj = j; jj < min(j + blockSize, n); jj++) {
                        for (int kk = k; kk < min(k + blockSize, n); kk++) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

// ��������� ��������� �������
vector<vector<double>> generateRandomMatrix(int n) {
    vector<vector<double>> matrix(n, vector<double>(n));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(-10.0, 10.0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = dist(gen);
        }
    }
    return matrix;
}

// ��������� ������������������
double measurePerformance(const function<void(const vector<vector<double>>&, const vector<vector<double>>&, vector<vector<double>>&)>& multiplyFunction,
    const vector<vector<double>>& A, const vector<vector<double>>& B) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n));
    auto start = high_resolution_clock::now();
    multiplyFunction(A, B, C);
    auto end = high_resolution_clock::now();
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cout << C[i][j]<< " ";
        }
        cout << endl;
    }
    duration<double> elapsed = end - start;
    double time = elapsed.count();
    cout << "�����: " << time << endl;
    // ������ ���������
    double complexity = 2 * pow(n, 3);
    // ������ ������������������ � MFlops
    double performance = complexity / time / 1e6;
    return performance;
}

int main() {
    setlocale(LC_ALL, "ru");
    int n = 1000;

    // ��������� ��������� ������
    vector<vector<double>> A = generateRandomMatrix(n);
    vector<vector<double>> B = generateRandomMatrix(n);
    // ������� ������������
    cout << "������� ������������ ������:" << endl;
    double performance = measurePerformance(matrixMultiply, A, B);
    cout << "������������������: " << performance << " MFlops" << endl << endl;
    // BLAS ������������
    cout << "������������ ������ � �������������� BLAS:" << endl;
    double performanceBLAS = measurePerformance(matrixMultiplyBLAS, A, B);
    cout << "������������������: " << performanceBLAS << " MFlops" << endl << endl;

    cout << "������������ ������ � ������� ����������:" << endl;
    double performanceBlock = measurePerformance(matrixMultiplyBlock, A, B);
    cout << "������������������: " << performanceBlock << " MFlops" << endl << endl;

    // �������� �������������
    double efficiency = performanceBlock / performanceBLAS;
    cout << "������������� ��������� � ������� ���������� ������������ BLAS: " << efficiency * 100 << "%" << endl;
    return 0;
}