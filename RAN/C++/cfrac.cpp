#include <iostream>

using namespace std;

static int ctr = 0;

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

class CFract {
    private:
        int n, d;
    public:
        CFract (int n, int d) {
            this->n = n;
            this->d = d;
        };
        void Display() {
            cout << "Fraction: " << n << " / " << d << endl;
        };
        void MultTo(CFract &fract) {
            n *= fract.n;
            d *= fract.d;
            Normalize();
        };
        CFract AddTo(const CFract &fract) {
            n = n * fract.d + fract.n * d;
            d *= fract.d;
            Normalize();
            return *this;
        };
        CFract &Normalize() {
            int pgcd = gcd(n, d);
            n /= pgcd;
            d /= pgcd;
            return *this;
        }
        bool operator==(const CFract &x) {
            return x.n * d == n * x.d;
        }
        CFract operator+(const CFract &x) {
            return CFract(n * x.d + x.n * d, d * x.d).Normalize();
        }
};

int main() {
    CFract f1(2, 4);
    CFract f2(1, 2);
    f1.Display();
    (f1 + f2).Display();
    f1.Display();
    f1.AddTo(CFract(1, 6));
    f1 = CFract(2, 4);
    f2 = CFract(1, 2);
    cout << (f1 == f2) << endl;
    f1.Display();
    return 0;
}
