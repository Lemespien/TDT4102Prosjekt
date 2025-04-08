#pragma once
#include "math.h"


namespace leme_sim {
    static constexpr double gravity_acc_e = 9.81;

    // static constexpr double mass_sun = 2e30;
    // static constexpr double mass_earth = 5.97e24;

    static constexpr double radius_sun = 6.96340e8;
    static constexpr double radius_earth = 6.38e6;

    static constexpr double distance_avg_sun_earth = 149e9;

    static constexpr double G_const = 6.674*10e-11;
    // SCALED VERSIONS
    static constexpr double mass_sun = 2e17;
    static constexpr double mass_earth = 5.97e11;
    
}