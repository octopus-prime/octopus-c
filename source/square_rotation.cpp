/*
 * square_rotation.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "square_rotation.hpp"

namespace chess {

constexpr std::array<square_t, 64> square_rotation<north_west>::rotation;
constexpr std::array<square_t, 64> square_rotation<north_east>::rotation;
constexpr std::array<square_t, 64> square_rotation<south_west>::rotation;
constexpr std::array<square_t, 64> square_rotation<south_east>::rotation;

}
