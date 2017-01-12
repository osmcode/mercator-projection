#ifndef PROJ_HPP
#define PROJ_HPP

#include <array>
#include <cmath>
#include <iostream>

constexpr double PI = 3.14159265358979323846;

/// Convert angle from degrees to radians.
inline constexpr double deg_to_rad(double degree) noexcept {
    return degree * (PI / 180.0);
}

/// Convert angle from radians to degrees.
inline constexpr double rad_to_deg(double radians) noexcept {
    return radians * (180.0 / PI);
}

constexpr double earth_radius_for_epsg3857 = 6378137.0;
constexpr double max_coordinate_epsg3857 = 20037508.34;

constexpr inline double lon_to_x(double lon) noexcept {
    return earth_radius_for_epsg3857 * deg_to_rad(lon);
}

// canonical log(tan()) version
inline double lat_to_y_with_tan(double lat) { // not constexpr because math functions aren't
    return earth_radius_for_epsg3857 * std::log(std::tan(PI/4 + deg_to_rad(lat)/2));
}

// faster version of the canonical log(tan()) version
inline double lat_to_y_with_sin(double lat) {
    const double f = std::sin(deg_to_rad(lat));
    const double y = earth_radius_for_epsg3857 * 0.5 * std::log((1 + f) / (1 - f));
    return y;
}

template <typename T>
constexpr double horner(double, T an) noexcept {
    return an;
}

template <typename T, typename... U>
constexpr double horner(double x, T an, U... a) noexcept {
    return horner(x, a...) * x + an;
}

template <typename Arg, class... Ts>
Arg horner_i(Arg x, Ts... coeffs)
{
    constexpr size_t degree = sizeof...(coeffs);
    std::array<Arg, degree> temp{coeffs...};

    Arg result{0};
    for (std::size_t index = temp.size(); index > 0; --index)
        result = temp[index - 1] + result * x;

    return result;
}

// polynomial approximation
inline double lat_to_y_with_poly(double lat) {
    if (lat < -78.0 || lat > 78.0)
        return lat_to_y_with_sin(lat);

    // Approximate the inverse Gudermannian function with the Padé approximant [10/10]: deg → deg
    // Coefficients are computed for the argument range [-78°,78°] by Remez algorithm
    // |err|_∞=5.486e-10
    return earth_radius_for_epsg3857 *
        horner(lat,
               0.00000000000000000e+00,  1.74532925181541924e-02, -9.48886714733577619e-06,
               -5.14483236972284849e-06, 2.53801360698030180e-09,  5.16175373655094521e-10,
               -2.20225881581151031e-13, -1.96854479399833152e-14, 6.49052820186726701e-18,
               2.04658527439432671e-19, -3.11125833784600832e-23) /
        horner(lat,
               1.00000000000000000e+00, -5.43672036010859947e-04, -3.45546751987863368e-04,
               1.73019445085169732e-07, 4.32516091068641779e-08, -1.93000947857361296e-11,
               -2.32987434393775411e-12,  8.68760908701761693e-16, 4.81414832735723520e-17,
               -1.25851403124467958e-20, -1.97411360668142313e-22);
}

inline double lat_to_y_unrolled(double lat) {
    if (lat < -78.0 || lat > 78.0)
        return lat_to_y_with_sin(lat);

    return earth_radius_for_epsg3857 *
    ((((((((((-3.1112583378460085319e-23 * lat + 2.0465852743943268009e-19) * lat + 6.4905282018672673884e-18) * lat + -1.9685447939983315591e-14) * lat + -2.2022588158115104182e-13) * lat + 5.1617537365509453239e-10) * lat + 2.5380136069803016519e-9) * lat + -5.1448323697228488745e-6) * lat + -9.4888671473357768301e-6) * lat + 1.7453292518154191887e-2) * lat + 0.0e0)
    /
    ((((((((((-1.9741136066814230637e-22 * lat + -1.258514031244679556e-20) * lat + 4.8141483273572351796e-17) * lat + 8.6876090870176172185e-16) * lat + -2.3298743439377541768e-12) * lat + -1.9300094785736130185e-11) * lat + 4.3251609106864178231e-8) * lat + 1.7301944508516974048e-7) * lat + -3.4554675198786337842e-4) * lat + -5.4367203601085991108e-4) * lat + 1.0e0);
}

inline double lat_to_y_fixed(double lat) {
    if (lat < -70.0 || lat > 70.0)
        return lat_to_y_with_sin(lat);

    // Approximate the inverse Gudermannian function with the Padé approximant [8/8]: deg → deg
    // Coefficients are computed for the argument range [-70e7,70e7] by Remez algorithm
    // |err|_∞=3.428e-03

    std::int64_t x = static_cast<std::int64_t>(std::round(lat * 1e7));
    __int128_t p = 5509742251;
    p = -5960547410 + ((x * p) >> 48);
    p = -5334146698 + ((x * p) >> 16);
    p = 8386657114 + ((x * p) >> 46);
    p = 5367384452 + ((x * p) >> 15);
    p = -5905576228 + ((x * p) >> 46);
    p = -5363030634 + ((x * p) >> 13);
    p = 4797524499 + ((x * p) >> 46);
    p = 0 + ((x * p) >> 6);
    __int128_t q = 4971461300;
    q = 8194925537 + ((x * q) >> 16);
    q = -4446786581 + ((x * q) >> 48);
    q = -4789069729 + ((x * q) >> 15);
    q = 6480822406 + ((x * q) >> 46);
    q = 6210295445 + ((x * q) >> 14);
    q = -6543941038 + ((x * q) >> 46);
    q = -4801234761 + ((x * q) >> 13);
    q = 4294967296 + ((x * q) >> 46);
    return earth_radius_for_epsg3857 * double(p) / double(q) / 1e7;
}

constexpr inline double x_to_lon(double x) {
    return rad_to_deg(x) / earth_radius_for_epsg3857;
}

inline double y_to_lat(double y) { // not constexpr because math functions aren't
    return rad_to_deg(2 * std::atan(std::exp(y / earth_radius_for_epsg3857)) - PI/2);
}

/**
 * The maximum latitude that can be projected with the Web Mercator
 * (EPSG:3857) projection.
 */
constexpr double MERCATOR_MAX_LAT = 85.0511288;

#endif // PROJ_HPP
