#include <cmath>
#include <string>

extern inline double deadzone(double input, double bound) {
    return std::abs(input) > bound ? input : 0;
}

extern inline std::string BoolToString(bool b){
    return b ? "true" : "false";
}