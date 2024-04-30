#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <utility>
#include <vector>

#include "Direction.h"

bool valid(const int& nx, const int& ny, const int& row, const int& col) {
    return nx >= 0 && nx < col && ny >= 0 && ny < row;
}

std::pair<int, int> genRandCell(int row, int col) {
    int edge = (std::rand() * std::rand()) % 4;  // 0 = 上邊, 1 = 下邊, 2 = 左邊, 3 = 右邊

    if (edge == 0) {
        return {std::rand() % col, 0};
    } else if (edge == 1) {
        return {std::rand() % col, row - 1};
    } else if (edge == 2) {
        return {0, std::rand() % row};
    } else {
        return {col - 1, std::rand() % row};
    }
}

#endif