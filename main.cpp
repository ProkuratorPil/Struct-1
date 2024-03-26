#include <iostream>

int Euclid(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1;
    int gcd = Euclid(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;

    return gcd;
}

int mod(int a, int n) {
    int x, y;
    int gcd = Euclid(a, n, x, y);
    if (gcd != 1) {
        std::cerr << 0 << std::endl;
        return -1;
    }

    return (x % n + n) % n;
}

int main() {
    setlocale(LC_ALL, "ru");
    std::cout << "Деревягин Дмитрий Иванович\n090304-РПИб-023\n\n";
    int a, n;
    std::cout << "Enter a: ";
    std::cin >> a;
    std::cout << "Enter n: ";
    std::cin >> n;

    int inv = mod(a, n);
    if (inv != -1) {
        std::cout << inv << std::endl;
    }

    return 0;
}