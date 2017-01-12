#include <array>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

#include "proj.hpp"

using our_clock = std::chrono::high_resolution_clock;

template <class Func>
std::chrono::milliseconds timethis(Func&& func) {
    std::chrono::time_point<our_clock> start{our_clock::now()};

    func();

    std::chrono::time_point<our_clock> stop{our_clock::now()};

    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
}

int main() {
    std::mt19937_64 gen;
    std::uniform_real_distribution<double> dis{-85.0, 85.0};

    const int num = 10000000;

    std::vector<double> data, r1, r2, r3;
    data.resize(num);
    r1.resize(num);
    r2.resize(num);
    r3.resize(num);

    for (auto& val : data) {
        val = dis(gen);
    }

    auto d_tan = timethis([&](){
        int i = 0;
        for (auto& val : data) {
            r1[i++] = lat_to_y_with_tan(val);
        }
    });

    std::cout << "tan      : " << std::setw(5) << d_tan.count() << "ms  100%\n";

    auto d_sin = timethis([&](){
        int i = 0;
        for (auto& val : data) {
            r2[i++] = lat_to_y_with_sin(val);
        }
    });

    std::cout << "sin      : " << std::setw(5) << d_sin.count() << "ms  " << std::setw(3) << (d_sin * 100 / d_tan) << "%\n";

    auto d_poly = timethis([&](){
        int i = 0;
        for (auto& val : data) {
            r3[i++] = lat_to_y_with_poly(val);
        }
    });

    std::cout << "polynom  : " << std::setw(5) << d_poly.count() << "ms  " << std::setw(3) << (d_poly * 100 / d_tan) << "%\n";

    auto d_poly_unrolled = timethis([&](){
        int i = 0;
        for (auto& val : data) {
            r3[i++] = lat_to_y_unrolled(val);
        }
    });

    std::cout << "unrolled : " << std::setw(5) << d_poly_unrolled.count() << "ms  " << std::setw(3) << (d_poly_unrolled * 100 / d_tan) << "%\n";

    auto d_fixed = timethis([&](){
        int i = 0;
        for (auto& val : data) {
            r3[i++] = lat_to_y_fixed(val);
        }
    });

    std::cout << "fixed    : " << std::setw(5) << d_fixed.count() << "ms  " << std::setw(3) << (d_fixed * 100 / d_tan) << "%\n";

}
