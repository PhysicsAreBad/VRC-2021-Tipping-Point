#include "utils.h"

#include <cmath>

double utils::deadzone(double input, double bound) {
    return std::abs(input) > bound ? input : 0;
}