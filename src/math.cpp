
#include <iostream>

#include "../include/taylorSeries.h"

#define M_PI 3.14159265358979323846
int floor(double input) { return static_cast<int>(input); }

double _abs(double value) { return value >= 0 ? value : -value; }

double normalizeRadians(double x) {
  const double max = 2 * M_PI;
  return x - floor(x / max) * max;
}

double _pow(double base, double exponent) {
  if (!exponent) {
    return 1;
  }

  double result = base;
  double degree = _abs(exponent);
  for (size_t i = 1; i < degree; i++) {
    result *= base;
  }

  return exponent >= 0 ? result : 1.0 / result;
}

double _sin(double radians) {
  const double normalizedRadians = normalizeRadians(radians);
  return taylorSeries("SIN", normalizedRadians);
}

double _cos(double radians) {
  const double normalizedRadians = normalizeRadians(radians);
  return taylorSeries("COS", normalizedRadians);
}
