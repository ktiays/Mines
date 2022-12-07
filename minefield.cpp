//
// Created by ktiays on 2022/11/29.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#include <random>

#include "minefield.hpp"

namespace mines {

Minefield::uint random_int(Minefield::uint from, Minefield::uint to) {
    using ValueType = Minefield::uint;

    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<ValueType> uniform_dist(from, to);
    return uniform_dist(e);
}

void Minefield::set_flag(uint x, uint y, FlagType flag) {
    if (locations.at(x, y).cleared || locations.at(x, y).flag == flag)
        return;

    if (flag == FlagType::flag)
        n_flags_++;
    else if (locations.at(x, y).flag == FlagType::flag)
        n_flags_--;

    locations.at(x, y).flag = flag;
}

bool Minefield::has_flag_warning(uint x, uint y) const {
    if (!is_cleared(x, y))
        return false;

    uint number_of_mines = 0, n_flags = 0;
    for (auto neighbour: Location::neighbour_map()) {
        const auto nx = static_cast<int>(x) + neighbour.x;
        const auto ny = static_cast<int>(y) + neighbour.y;
        if (!is_location(nx, ny))
            continue;
        if (has_mine(nx, ny))
            number_of_mines++;
        if (get_flag(nx, ny) == FlagType::flag)
            n_flags++;
    }

    return n_flags > number_of_mines;
}

void Minefield::place_mines(uint x, uint y) {
    for (auto n = 0; n < n_mines;) {
        const auto rx = random_int(0, width - 1);
        const auto ry = random_int(0, height - 1);

        if (rx == x && ry == y)
            continue;

        if (!locations.at(rx, ry).has_mine) {
            bool adj_found = false;

            for (auto neighbour: Location::neighbour_map()) {
                if (rx == x + neighbour.x && ry == y + neighbour.y) {
                    adj_found = true;
                    break;
                }
            }

            if (!adj_found) {
                locations.at(rx, ry).has_mine = true;
                n++;
                for (auto neighbour: Location::neighbour_map()) {
                    const auto nx = static_cast<int>(rx) + neighbour.x;
                    const auto ny = static_cast<int>(ry) + neighbour.y;
                    if (is_location(nx, ny))
                        locations.at(nx, ny).adjacent_mines++;
                }
            }
        }
    }
}

void Minefield::multi_release(uint x, uint y) {
    if (!is_cleared(x, y) || get_flag(x, y) == FlagType::flag)
        return;

    /* Work out how many flags / unknown squares surround this one */
    const auto number_of_mines = get_n_adjacent_mines(x, y);
    uint n_flags = 0;
    uint n_unknown = 0;
    for (auto neighbour: Location::neighbour_map()) {
        const auto nx = static_cast<int>(x) + neighbour.x;
        const auto ny = static_cast<int>(y) + neighbour.y;
        if (!is_location(nx, ny))
            continue;
        if (get_flag(nx, ny) == FlagType::flag)
            n_flags++;
        if (!is_cleared(nx, ny))
            n_unknown++;
    }

    /* If we have correct number of flags to mines then clear the other
     * locations, otherwise if the number of unknown squares is the
     * same as the number of mines flag them all. */
    bool do_clear;
    if (number_of_mines == n_flags)
        do_clear = true;
    else if (use_autoflag && n_unknown == number_of_mines)
        do_clear = false;
    else
        return;

    /* Use the same minefield for the whole time (it may complete as we do it) */

    for (auto neighbour: Location::neighbour_map()) {
        const auto nx = static_cast<int>(x) + neighbour.x;
        const auto ny = static_cast<int>(y) + neighbour.y;
        if (!is_location(nx, ny))
            continue;

        if (do_clear && get_flag(nx, ny) != FlagType::flag) {
            clear_mine(nx, ny);
        } else {
            set_flag(nx, ny, FlagType::flag);
        }
    }
}

void Minefield::clear_mine(uint x, uint y) {
    /* Place mines on first attempt to clear */
    if (!placed_mines_) {
        place_mines(x, y);
        placed_mines_ = true;
    }

    const auto& location = locations.at(x, y);
    if (location.cleared || location.flag == FlagType::flag)
        return;

    clear_mines_recursive(x, y);

    /* Failed if this contained a mine */
    if (location.has_mine) {
        if (!exploded_) {
            exploded_ = true;
        }
        return;
    }

    /* Mark unmarked mines when won */
    if (is_complete() && !exploded()) {
        for (auto tx = 0; tx < width; tx++)
            for (auto ty = 0; ty < height; ty++)
                if (has_mine(tx, ty))
                    set_flag(tx, ty, FlagType::flag);
    }
}

void Minefield::clear_mines_recursive(uint x, uint y) {
    auto& location = locations.at(x, y);
    /* Ignore if already cleared */
    if (location.cleared)
        return;

    location.cleared = true;
    n_cleared_++;
    if (location.flag == FlagType::flag)
        n_flags_--;
    location.flag = FlagType::none;

    /* Automatically clear locations if no adjacent mines */
    if (!location.has_mine && get_n_adjacent_mines(x, y) == 0) {
        for (auto neighbour: Location::neighbour_map()) {
            const auto nx = static_cast<int>(x) + neighbour.x;
            const auto ny = static_cast<int>(y) + neighbour.y;
            if (is_location(nx, ny))
                clear_mines_recursive(nx, ny);
        }
    }
}

}