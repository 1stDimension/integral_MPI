#pragma once

double integrate(double (*func)(double), double begin, double end, int num_points);

double linear(double x);

double parabole(double x);