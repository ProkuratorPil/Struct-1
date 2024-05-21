#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <random>
#include <cblas.h>
#include <omp.h>

using namespace std;
using namespace chrono;

// Обычное перемножение матриц
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

// Перемножение матриц с использованием BLAS
void matrixMultiplyBLAS(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    int n = A.size();
    // Преобразование векторов в массивы для BLAS
    double* a = new double[n * n];
    double* b = new double[n * n];
    double* c = new double[n * n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = A[i][j];
            b[i * n + j] = B[i][j];
        }
    }
    // Вызов BLAS функции
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n,
        1.0, a, n, b, n, 0.0, c, n);
    // Копирование результата из массива в вектор
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = c[i * n + j];
        }
    }
    
    delete[] a;
    delete[] b;
    delete[] c;
}

// Простой алгоритм перемножения матриц
void matrixMultiplyBlock(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    int n = A.size();
    int blockSize = 32; // Размер блока
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

// Генерация случайной матрицы
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

// Измерение производительности
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
    cout << "Время: " << time << endl;
    // Оценка сложности
    double complexity = 2 * pow(n, 3);
    // Оценка производительности в MFlops
    double performance = complexity / time / 1e6;
    return performance;
}

int main() {
    setlocale(LC_ALL, "ru");
    int n = 1000;

    // Генерация случайных матриц
    vector<vector<double>> A = generateRandomMatrix(n);
    vector<vector<double>> B = generateRandomMatrix(n);
    // Обычное перемножение
    cout << "Обычное перемножение матриц:" << endl;
    double performance = measurePerformance(matrixMultiply, A, B);
    cout << "Производительность: " << performance << " MFlops" << endl << endl;
    // BLAS перемножение
    cout << "Перемножение матриц с использованием BLAS:" << endl;
    double performanceBLAS = measurePerformance(matrixMultiplyBLAS, A, B);
    cout << "Производительность: " << performanceBLAS << " MFlops" << endl << endl;

    cout << "Перемножение матриц с блочной структурой:" << endl;
    double performanceBlock = measurePerformance(matrixMultiplyBlock, A, B);
    cout << "Производительность: " << performanceBlock << " MFlops" << endl << endl;

    // Проверка эффективности
    double efficiency = performanceBlock / performanceBLAS;
    cout << "Эффективность алгоритма с блочной структурой относительно BLAS: " << efficiency * 100 << "%" << endl;
    return 0;
}