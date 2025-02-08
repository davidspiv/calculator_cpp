
#include <iostream>

#include "../include/taylorSeries.h"

#define M_PI 3.14159265358979323846

int _floor(double input) { return static_cast<int>(input); }

double _abs(double value) { return value >= 0 ? value : -value; }

double normalize(double x, double min, double max) {
  const double diff = max - min;
  return x - _floor(x / diff) * diff;
}

double _pow(double base, int exp) {
  if (!exp || base == 1) {
    return 1;
  } else if (!(exp % 2)) {
    const double y = _pow(base, exp / 2);
    return y * y;
  } else {
    return base * _pow(base, exp - 1);
  }
}

double _sin(double radians) {
  const double normalizedRadians = normalize(radians, 0, 2 * M_PI);
  return taylorSeries("SIN", normalizedRadians);
}

double _cos(double radians) {
  const double normalizedRadians = normalize(radians, 0, 2 * M_PI);
  return taylorSeries("COS", normalizedRadians);
}
