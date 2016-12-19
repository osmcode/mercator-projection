#ifndef PROJ_HPP
#define PROJ_HPP

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

// polynomial approximation
inline double lat_to_y_with_poly(double lat) {
    // Approximate the inverse Gudermannian function with the Padé approximant [11/11]: deg → deg
    // Coefficients are computed for the argument range [-70°,70°] by Remez algorithm
    // |err|_∞=3.387e-12
    return deg_to_rad( horner(lat,
        0.00000000000000000000000000e+00,
        1.00000000000089108431373566e+00,
        2.34439410386997223035693483e-06,
        -3.21291701673364717170998957e-04,
        -6.62778508496089940141103135e-10,
        3.68188055470304769936079078e-08,
        6.31192702320492485752941578e-14,
        -1.77274453235716299127325443e-12,
        -2.24563810831776747318521450e-18,
        3.13524754818073129982475171e-17,
        2.09014225025314211415458228e-23,
        -9.82938075991732185095509716e-23) /
    horner(lat,
        1.00000000000000000000000000e+00,
        2.34439410398970701719081061e-06,
        -3.72061271627251952928813333e-04,
        -7.81802389685429267252612620e-10,
        5.18418724186576447072888605e-08,
        9.37468561198098681003717477e-14,
        -3.30833288607921773936702558e-12,
        -4.78446279888774903983338274e-18,
        9.32999229169156878168234191e-17,
        9.17695141954265959600965170e-23,
        -8.72130728982012387640166055e-22,
        -3.23083224835967391884404730e-28)) * earth_radius_for_epsg3857;
}

inline double lat_to_y(double lat) {
    if (lat < -70.0 || lat > 70.0) {
        return lat_to_y_with_sin(lat);
    } else {
        return lat_to_y_with_poly(lat);
    }
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
