/*
 * score.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "score.hpp"

namespace chess {

constexpr std::array<score_t, 13> score_of_piece::scoring;
constexpr std::array<score_t, 5> score_of_promotion_t::scoring;

}
