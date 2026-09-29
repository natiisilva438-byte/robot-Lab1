#include "random_vector.h"

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>

int main() {
  std::srand(314159);
  RandomVector values(20);
  assert(values.min() >= 0.0);
  assert(values.max() <= 1.0);
  assert(values.min() <= values.mean());
  assert(values.mean() <= values.max());

  std::ostringstream output;
  std::streambuf* original = std::cout.rdbuf(output.rdbuf());
  values.printHistogram(5);
  std::cout.rdbuf(original);
  const std::string histogram = output.str();
  std::size_t stars = 0;
  for (std::size_t pos = 0; (pos = histogram.find("***", pos)) != std::string::npos; pos += 3) {
    ++stars;
  }
  assert(stars == 20);

  RandomVector equal_values(5, 0.0);
  assert(equal_values.mean() == 0.0);
  assert(equal_values.min() == 0.0);
  assert(equal_values.max() == 0.0);
  output.str("");
  output.clear();
  original = std::cout.rdbuf(output.rdbuf());
  equal_values.printHistogram(3);
  std::cout.rdbuf(original);
  assert(output.str().find("***") != std::string::npos);

  bool rejected = false;
  try { RandomVector invalid(-1); } catch (const std::invalid_argument&) { rejected = true; }
  assert(rejected);
  rejected = false;
  try { RandomVector invalid(2, -1.0); } catch (const std::invalid_argument&) { rejected = true; }
  assert(rejected);
  rejected = false;
  try { values.printHistogram(0); } catch (const std::invalid_argument&) { rejected = true; }
  assert(rejected);
  rejected = false;
  try { RandomVector empty(0); empty.mean(); } catch (const std::domain_error&) { rejected = true; }
  assert(rejected);
}
