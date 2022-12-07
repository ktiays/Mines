#include <iostream>
#include <limits>

#include "minefield.hpp"

void print_minefield(const mines::Minefield& minefield) {
    const auto row = minefield.width;
    const auto column = minefield.height;

    std::cout << "\n";

    std::cout << "  ";
    for (int x = 0; x < column; ++x) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    for (int y = 0; y < row; ++y) {
        for (int x = 0; x <= column; ++x) {
            if (x == 0) {
                std::cout << y << " ";
                continue;
            }

            const auto& location = minefield.locations.at(x - 1, y);

            if (location.cleared) {
                if (location.adjacent_mines > 0) std::cout << location.adjacent_mines;
                else std::cout << "□";
            } else {
                switch (location.flag) {
                    case mines::FlagType::none:
                        std::cout << "■";
                        break;
                    case mines::FlagType::flag:
                        std::cout << "x";
                        break;
                    case mines::FlagType::maybe:
                        std::cout << "?";
                        break;
                }
            }

            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

enum class Operation : int {
    unknown = -1,
    open = 0,
    flag = 1,
};

Operation parse_operation(int x);

int main() {
    mines::Minefield minefield(8, 8, 10);

    while (true) {
        print_minefield(minefield);

        int operation = 0;
        std::cin >> operation;

        const auto op = parse_operation(operation);
        if (op == Operation::unknown) continue;

        std::cout << "Input location: ";
        int x = 0;
        int y = 0;
        std::cin >> x >> y;

        if (!minefield.is_location(x, y)) {
            std::cout << "Location is invalid.";
            continue;
        }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
        switch (op) {
            case Operation::open: {
                minefield.clear_mine(x, y);
            }
                break;
            case Operation::flag: {
                minefield.set_flag(x, y, mines::FlagType::flag);
            }
                break;
        }
#pragma clang diagnostic pop

        if (minefield.exploded()) {
            std::cout << "You Failed.";
            break;
        }
        if (minefield.is_complete()) {
            std::cout << "Congratulations!!";
            break;
        }
    }

    return 0;
}

Operation parse_operation(int x) {
    Operation op = Operation::unknown;
    switch (x) {
        case 0:
            op = Operation::open;
            std::cout << "Open Location";
            break;
        case 1:
            op = Operation::flag;
            std::cout << "Flag Mine";
            break;
        default:
            std::cout << "Unknown Operation";
            break;
    }
    std::cout << "\n";
    return op;
}