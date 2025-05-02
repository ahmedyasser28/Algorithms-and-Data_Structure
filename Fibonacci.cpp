#include <iostream>
#include <vector>
using namespace std;

int fibonacci_recursive(int n) {
    if (n <= 1)
        return n;
    else
        return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}
void matrix_multiply(int mat1[2][2], int mat2[2][2]) {
    int a = mat1[0][0] * mat2[0][0] + mat1[0][1] * mat2[1][0];
    int b = mat1[0][0] * mat2[0][1] + mat1[0][1] * mat2[1][1];
    int c = mat1[1][0] * mat2[0][0] + mat1[1][1] * mat2[1][0];
    int d = mat1[1][0] * mat2[0][1] + mat1[1][1] * mat2[1][1];

    mat1[0][0] = a;
    mat1[0][1] = b;
    mat1[1][0] = c;
    mat1[1][1] = d;
}

void matrix_power(int matrix[2][2], int n) {
    int result[2][2] = {{1, 0}, {0, 1}};
    while (n > 0) {
        if (n % 2 == 1) {
            matrix_multiply(result, matrix);
        }
        matrix_multiply(matrix, matrix);
        n /= 2;
    }
    matrix[0][0] = result[0][0];
    matrix[0][1] = result[0][1];
    matrix[1][0] = result[1][0];
    matrix[1][1] = result[1][1];
}

int fibonacci_matrix(int n) {
    if (n == 0)
        return 0;
    int matrix[2][2] = {{1, 1}, {1, 0}};
    matrix_power(matrix, n - 1);
    return matrix[0][0];
}

int fibonacci(int n) {
    if (n <= 1)
        return n;
    else{

        vector<int> fib(n + 1);
        fib[0] = 0;
        fib[1] = 1;


        for (int i = 2; i <= n; i++) {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
        return fib[n];
    }
}

int main() {
    cout<< "enter the number"<<endl;
    int n;
    cin>>n;
    cout<<fibonacci_recursive(n)<<endl;
    cout<<fibonacci_matrix(n)<<endl;
    cout<<fibonacci(n)<<endl;
    return 0;
}

