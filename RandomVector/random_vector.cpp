#include "random_vector.h"

#include <cstdlib>
#include <stdexcept>
#include <vector>

RandomVector::RandomVector(int size, double max_val) {
  if (size < 0 || max_val < 0.0) {
    throw std::invalid_argument("size and max_val must be nonnegative");
  }
  vect.reserve(static_cast<std::size_t>(size));
  for (int i = 0; i < size; ++i) {
    vect.push_back(max_val * static_cast<double>(std::rand()) / RAND_MAX);
  }
}

void RandomVector::print() {
  for (std::size_t i = 0; i < vect.size(); ++i) {
    if (i != 0) std::cout << ' ';
    std::cout << vect[i];
  }
  std::cout << '\n';
}

double RandomVector::mean() {
  if (vect.empty()) throw std::domain_error("mean of empty vector");
  double sum = 0.0;
  for (double value : vect) sum += value;
  return sum / vect.size();
}

double RandomVector::max() {
  if (vect.empty()) throw std::domain_error("max of empty vector");
  double result = vect[0];
  for (double value : vect) if (value > result) result = value;
  return result;
}

double RandomVector::min() {
  if (vect.empty()) throw std::domain_error("min of empty vector");
  double result = vect[0];
  for (double value : vect) if (value < result) result = value;
  return result;
}

void RandomVector::printHistogram(int bins) {
  if (bins <= 0) throw std::invalid_argument("bins must be positive");
  if (vect.empty()) return;

  const double low = min();
  const double high = max();
  std::vector<int> counts(static_cast<std::size_t>(bins), 0);
  for (double value : vect) {
    int index = high == low ? 0 :
        static_cast<int>((value - low) / (high - low) * bins);
    if (index == bins) index = bins - 1;
    ++counts[static_cast<std::size_t>(index)];
  }

  int tallest = 0;
  for (int count : counts) if (count > tallest) tallest = count;
  for (int row = tallest; row > 0; --row) {
    for (int count : counts) std::cout << (count >= row ? "*** " : "    ");
    std::cout << '\n';
  }
}
