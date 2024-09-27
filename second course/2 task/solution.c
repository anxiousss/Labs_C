#include <stdio.h>
#include <math.h>
#include "solution.h"

void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

int number_cmp(double a, double b, double eps) {
    return fabs(a - b) < eps;
}

void triangle_check(double a, double b, double c, double eps) {
    printf("%f %f %f\n", a ,b ,c);
    if (number_cmp(pow(a, 2) + pow(b, 2), pow(c, 2), eps)) {
        printf("triangle exist");
    } else {
        printf("triangle do not exist");
    }
}

void permute(double *arr, int start, int end, double eps, void (*func)(double, double, double, double)) {
    if (start == end) {
        func(arr[0], arr[1], arr[2], eps);
        printf("\n");
    } else {
        for (int i = start; i <= end; i++) {
            swap(&arr[start], &arr[i]);
            permute(arr, start + 1, end, eps, func);
            swap(&arr[start], &arr[i]);
        }
    }
}

void eq_solution(double a, double b, double c, double eps) {
    double D = pow(b, 2) - 4 * a * c;

    printf("%f %f %f\n", a, b, c);

    if (fabs(a) < eps && fabs(b) < eps && fabs(c) < eps) {
        printf("Infinite solutions\n");
        return;
    }

    if (fabs(a) < eps && fabs(b) < eps) {
        printf("No solution\n");
        return;
    }

    if (fabs(a) < eps && fabs(b) > eps) {
        double root = -c / b;
        printf("%.2f\n", root);
        return;
    }

    if (D > eps) {
        double root_1 = (-b + sqrt(D)) / (2 * a);
        double root_2 = (-b - sqrt(D)) / (2 * a);

        if (root_1 < root_2) {
            printf("%.2f %.2f\n", root_1, root_2);
        } else {
            printf("%.2f %.2f\n", root_2, root_1);
        }
        return;
    }

    if (fabs(D) < eps) {
        double root = -b / (2 * a);
        if (fabs(root) < eps) {
            root = 0.0;
        }
        printf("%.2f\n", root);
        return;
    }

    if (D < -eps) {
        printf("No solution\n");
        return;
    }
}