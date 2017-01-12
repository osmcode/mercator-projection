
#include <cmath>
#include <iostream>

#include "proj.hpp"

void compare(const char* c, double lat, double y1, double y2) {
    if (std::abs(y1 - y2) > 5e-2) {
        std::cout << c << " lat=" << lat << " y1=" << y1 << " y2=" << y2 << "\n";
    }
}

int main() {
    const int factor = 10000000;

    std::cout.precision(12);

    for (int32_t lat_i = 0; lat_i <= 85 * factor; ++lat_i) {
        double lat = double(lat_i) / factor;
        double y_tan    = lat_to_y_with_tan(lat);
        double y_sin    = lat_to_y_with_sin(lat);
        double y_poly   = lat_to_y_with_poly(lat);
        double y_unroll = lat_to_y_unrolled(lat);
        double y_fixed  = lat_to_y_fixed(lat);

        compare("tan-sin ",   lat, y_tan, y_sin);
        compare("sin-poly",   lat, y_sin, y_poly);
        compare("sin-unroll", lat, y_sin, y_unroll);
        compare("sin-fixed",  lat, y_sin, y_fixed);
    }
}
