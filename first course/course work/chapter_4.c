#include <stdio.h>
#include <math.h>
typedef long double(*Func)(long double);

long double MachineEpsilon() {
    long double epsilon = 1;
    while (1 + epsilon / 2 > 1) {
        epsilon /= 2;
    }
    return 2 * epsilon;

}


// 23 Вариант
long double func1(long double x) {
    return 3 * x - 4 * logl(x) - 5;
}

long double func1_deriative1(long double x) {
    return  3 - (4 / x);
}

long double func1_eq(long double x) {
    return x - func1(x) / func1_deriative1(x);
}


long double func1_deriative2(long double x) {
    return 4 / (x * x);
}

// 24 вариант
long double func2(long double x) {
    return cos(2 / x) - 2 * sin(1 / x) + (1 / x);
}

long double func2_deriative1(long double x) {
    return (2 * sin(2 / x) + cos(1 / x) - 1) / (x * x);
}

long double func2_eq(long double x) {
    return x - func2(x) / func1_deriative1(x);
}

long double func2_deriative2(long double x) {
    return -(((4 * sin(2 / x) + 2 * cos(1 / x) - 2) * x + 4 * cos(2 / x) - sin(1 / x)) / (x * x * x * x));
}

long double DichotomyMethod(long double a, long double b, Func f) {
    long double epsilon = MachineEpsilon();
    while (fabsl(a - b) > epsilon) {
        if (f(a) * f((a + b) / 2) > 0) {
            a = (a + b) / 2;
        }
        else {
            b = (a + b) / 2;
        }
    }

    return (a + b) / 2;
}


long double IterationMethod(long double a, long double b, Func f_eq) {

    long double x0 = (a + b) / 2, eps = MachineEpsilon(), x_current = 0;
    int iterations = 100, i = 0;
    while (i < iterations) {
        x_current = f_eq(x0);
        if (fabsl(x_current - x0) < eps) {
            return x_current;
        }
        x0 = x_current;
        ++i;
    }
    return x_current;
}


long double NewtonMethod(long double a, long double b, Func f, Func fd1, Func fd2) {
    long double x = (a + b) / 2;

    for (double i = a; i <= b; i += 0.0001) {

        if (f(i) * fd2(i) >= fd1(i) * fd1(i)) {
            printf("Invalid Function");
            return 0;
        }

    }

    while (fabsl(f(x)) >= 0.0001) {
        x = x - (f(x) / fd1(x));
    }
    return x;
}



int main() {
    long double a = 2, b = 4;
    long double root = 3.23;
    printf("Dichtomy method:  %12.9lf diff: %12.9lf\n", DichotomyMethod(a, b, func1), DichotomyMethod(a, b, func1) - root);
    printf("Iteration method: %12.9lf diff: %12.9lf\n", IterationMethod(a, b, func1_eq), IterationMethod(a, b, func1_eq) - root);
    printf("Newton method:    %12.9lf diff: %12.9lf\n",
        NewtonMethod(a, b, func1, func1_deriative1, func1_deriative2),
        NewtonMethod(a, b, func1, func1_deriative1, func1_deriative2) - root);
    long double a2 = 1, b2 = 2;
    long double root2 = 1.8756;
    printf("Dichtomy method:  %12.9lf diff: %12.9lf\n", DichotomyMethod(a2, b2, func2), DichotomyMethod(a2, b2, func2) - root2);
    printf("Iteration method: %12.9lf diff: %12.9lf\n", IterationMethod(a2, b2, func2_eq), IterationMethod(a2, b2, func2_eq) - root2);
    printf("Newton method:    %12.9lf diff: %12.9lf\n",
        NewtonMethod(a2, b2, func2, func2_deriative1,
            func2_deriative2), NewtonMethod(a2, b2, func2, func2_deriative1, func2_deriative2) - root2);

}
