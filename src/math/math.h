#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

#define PI M_PI
#define TWO_PI 2.0 * M_PI
#define HALF_PI M_PI_2
#define QUART_PI M_PI_4

int math_init();
int math_randi(int n);
double math_randf();
double math_randf_range(double min, double max);
double math_randf_gaussian(double mu, double sigma);
double math_fmod(double a, double b);
double math_dist(double x1, double y1, double x2, double y2);

#endif
