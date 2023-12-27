#include <stdio.h>
#include <math.h>
#pragma warning(disable : 4996)

long double calculate_taylor(int* ptr_n, long double x, long double eps) {
    int sign = 1;
    long double numerator = x;
    long double denominator = 1;

    long double value = 0, term;
    int i = 0;

    do {
        term = sign * numerator / denominator;
        value += term;
        sign = -sign;
        numerator *= x * x;
        denominator += 2;
        ++i;
    } while (fabs(term) > eps && i < 100);

    *ptr_n = i;

    return value;
}

int main(void) {
    long double lower_bound = 0.0, upper_bound = 1.0;
    long double epsilon = 1.0, x = lower_bound;

    while (1.0 + epsilon / 2 > 1) {
        epsilon /= 2.0;
    }

    printf("Machine epsilon = %.20f\n\n", epsilon);

    printf("Enter the number of intervals within the range [%1.1f, %1.1f]: ", lower_bound, upper_bound);
    int steps;
    scanf("%d", &steps);

    double step = (upper_bound - lower_bound) / steps;

    printf("\n==========================================================================\n");
    printf("|| Step\t x\t\t Taylor sum\t\t arctg\t Iterations     ||\n");
    printf("||----------------------------------------------------------------------||\n");

    long double current_term, sum;
    int n;
    for (int i = 0; i <= steps; i++) {
        sum = calculate_taylor(&n, x, epsilon);
        printf("|| %2d\t %.2lf\t %.18lf\t %.18lf\t %2d\t||\n", i, x, sum, atan(x), n - 1);
        x += step;
    }

    printf("==========================================================================\n");
    return 0;
}
