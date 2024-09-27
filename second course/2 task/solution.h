
#ifndef SOLUTION_H
#define SOLUTION_H

void swap(double *a, double *b);
void permute(double *arr, int start, int end, double eps, void (*func)(double, double, double, double));
void eq_solution(double a, double b, double c, double eps);
int number_cmp(double a, double b, double eps);
void triangle_check(double a, double b, double c, double eps);

#endif //SOLUTION_H
