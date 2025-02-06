#define M_PI 3.14159265358979323846

#include <iostream>
#include <string>

int floor(double input) { return static_cast<int>(input); }

double normalizeRadians(double x) {
  const double max = 2 * M_PI;
  return x - floor(x / max) * max;
}

void createFactorialCache(double *factorialCache, size_t iterations) {
  factorialCache[0] = 1;
  for (size_t i = 1; i <= iterations; i++) {
    factorialCache[i] = i * factorialCache[i - 1];
  }
}

double createCoeff(double denominator, bool &isPositive) {
  double coeff = (1 / denominator);
  coeff = isPositive ? coeff : -coeff;
  isPositive = !isPositive;
  return coeff;
}

double taylorSeries(const std::string &type, double radians) {
  double result = 0;
  const size_t terms = 50;
  double factorialCache[terms + 1];
  bool isPositive = terms % 4 && terms % 4 != 3;

  createFactorialCache(&factorialCache[0], terms);

  // Horner's method
  for (int i = terms; i > 0; i--) {
    if (type == "SIN" && i % 2) {
      result += createCoeff(factorialCache[i], isPositive);
    }
    if (type == "COS" && !(i % 2)) {
      result += createCoeff(factorialCache[i], isPositive);
    }
    result *= radians;
  }
  if (type == "COS") {
    result = 1 - result;
  }
  return result;
}
