//
// Created by ktiays on 2022/11/29.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef MINES_MINEFIELD_HPP
#define MINES_MINEFIELD_HPP

#include <array>

#include "vector_2d.hpp"

namespace mines {

enum class FlagType {
    none,
    flag,
    maybe
};

struct Location {
    /* true if contains a mine */
    bool has_mine = false;

    /* true if cleared */
    bool cleared = false;

    /* Flag */
    FlagType flag = FlagType::none;

    /// Number of mines in the neighbourhood.
    int adjacent_mines = 0;

    /* Table of offsets to adjacent squares */
    struct Neighbour {
        int x;
        int y;
    };

    static std::array<Neighbour, 8>& neighbour_map() {
        static std::array<Neighbour, 8> map = {
            Neighbour{ -1, 1 },
            { 0, 1 },
            { 1, 1 },
            { 1, 0 },
            { 1, -1 },
            { 0, -1 },
            { -1, -1 },
            { -1, 0 }
        };
        return map;
    }
};

class Minefield {
public:
    using uint = unsigned int;

    /* Size of map */
    uint width = 0;
    uint height = 0;

    /// Number of mines in map.
    uint n_mines = 0;

    /* State of each location */
    Vector2D<Location> locations;

    bool use_autoflag = false;

    Minefield(uint width, uint height, uint n_mines)
        : width(width), height(height), n_mines(n_mines),
          locations(Vector2D<Location>(width, height)) {}

    bool has_mine(uint x, uint y) const {
        return locations.at(x, y).has_mine;
    }

    bool is_location(int x, int y) const {
        return x >= 0 && y >= 0 && x < width && y < height;
    }

    bool is_cleared(uint x, uint y) const {
        return locations.at(x, y).cleared;
    }

    bool is_complete() const {
        return n_cleared_ == width * height - n_mines;
    }

    /// Returns a Boolean value that indicates whether a mine is hit.
    bool exploded() const {
        return exploded_;
    }

    FlagType get_flag(uint x, uint y) const {
        return locations.at(x, y).flag;
    }

    void set_flag(uint x, uint y, FlagType flag);

    uint get_n_adjacent_mines(uint x, uint y) const {
        return locations.at(x, y).adjacent_mines;
    }

    bool has_flag_warning(uint x, uint y) const;

    void multi_release(uint x, uint y);

    void clear_mine(uint x, uint y);

private:
    /* Keep track of flags and cleared squares */
    uint n_cleared_ = 0;
    uint n_flags_ = 0;

    /// A Boolean value that indicates whether mines were placed onto the map.
    bool placed_mines_ = false;

    /// A Boolean value that indicates whether a mine is hit.
    bool exploded_ = false;

    /// Randomly set the mines, but avoid the current and adjacent locations.
    void place_mines(uint x, uint y);

    void clear_mines_recursive(uint x, uint y);
};

}

#endif //MINES_MINEFIELD_HPP
