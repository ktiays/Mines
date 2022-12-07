//
// Created by ktiays on 2022/11/29.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef MINES_VECTOR_2D_HPP
#define MINES_VECTOR_2D_HPP

#include <vector>

namespace mines {

template<typename T>
struct Vector2D {
public:
    using SizeType = typename std::vector<T>::size_type;

    Vector2D(SizeType column, SizeType row)
        : row_(row), column_(column) {
        data_.resize(row * column);
    }

    T& at(SizeType x, SizeType y) {
        return data_.at(y * row_ + x);
    }

    const T& at(SizeType x, SizeType y) const {
        return data_.at(y * row_ + x);
    }

    SizeType size() const {
        return row_ * column_;
    }

private:
    std::vector<T> data_;

    SizeType row_;
    SizeType column_;
};

}

#endif //MINES_VECTOR_2D_HPP
