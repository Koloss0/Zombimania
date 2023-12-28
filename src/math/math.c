#include "math.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>

static bool initialised = false;

int math_init()
{
	srand((unsigned int)time(NULL));
	initialised = true;
	return true;
}

int math_randi(int n)
{
	assert(initialised);
	assert(n > 0);
	return rand() % n;
}

double math_randf()
{
	return (double)rand() / RAND_MAX;
}

double math_randf_range(double min, double max)
{
	double div = RAND_MAX / (max - min);
	return min + (rand() / div);
}

// TODO: optimize (can make use of z1)
double math_randf_gaussian(double mu, double sigma)
{
	double u1, u2;
	do
	{
		u1 = math_randf();
	} while (u1 <= DBL_EPSILON);
	u2 = math_randf();

	//compute z0 and z1
	double mag = sigma * sqrt(-2.0 * log(u1));
	double z0  = mag * cos(TWO_PI * u2) + mu;
	//double z1  = mag * sin(TWO_PI * u2) + mu;

	return z0;
}

double math_fmod(double a, double b)
{
	double r = fmod(a, b);
	return r < 0.0 ? r + b : r;
}

double math_dist(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	return sqrt(dx*dx + dy*dy);
}
