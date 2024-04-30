#ifndef MAZE_H
#define MAZE_H

#include <ctime>
#include <iostream>
#include <utility>
#include <string>
#include <cctype>
#include <vector>
#include <map>

#include "Algorithm.h"

class Maze {
    public:
    void setting() {
        irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
        engine->play2D("./soundEffect/enterSound.wav");

        srand(time(NULL));
        system("cls");

        std::string rowStr, colStr;
        std::cout << "Please input the row size (must be greater than 3): ";
        std::getline(std::cin, rowStr);
        while (!isNumber(rowStr) || std::stoi(rowStr) < 3) {
            std::cout << "Does not meet input requirements, please try again: ";
            std::getline(std::cin, rowStr);
        }

        std::cout << "Please input the col size (must be greater than 3): ";
        std::getline(std::cin, colStr);
        while (!isNumber(colStr) || std::stoi(colStr) < 3) {
            std::cout << "Does not meet input requirements, please try again: ";
            std::getline(std::cin, colStr);
        }

        row = std::stoi(rowStr);
        col = std::stoi(colStr);

        maze.clear();
        maze.resize(row, std::vector<int>(col, 1));
        engine->drop();
    }

    void init(int row, int col) {
        maze.resize(row, std::vector<int>(col, 1));
        solution = std::vector<std::vector<int>>(row, std::vector<int>(col, 1));
        this->row = row;
        this->col = col;
        this->startCell = {0, 0};
        this->endCell   = {col - 1, row - 1};
    }

    void display() {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (i == this->startCell.second && j == this->startCell.first) {
                    std::cout << 'S';
                } else if (i == this->endCell.second && j == this->endCell.first) {
                    std::cout << 'E';
                } else {
                    std::cout << (maze[i][j] == 1 ? '1' : '0');
                }
            }
            std::cout << std::endl;
        }
    }

    void genMaze() {
        std::vector<std::pair<int, int>> frontierCells;
        maze[startCell.second][startCell.first] = 0;
        frontierCells.push_back(startCell);

        while (!frontierCells.empty()) {
            int  randIndex   = std::rand() % frontierCells.size();
            auto currentCell = frontierCells[randIndex];
            frontierCells.erase(frontierCells.begin() + randIndex);

            for (const auto& dir: DIR) {
                int nx = currentCell.first + dir.first * 2;
                int ny = currentCell.second + dir.second * 2;
                if (valid(nx, ny, row, col) && maze[ny][nx] == 1) {
                    int betweenX = currentCell.first + dir.first;
                    int betweenY = currentCell.second + dir.second;
                    if (maze[betweenY][betweenX] == 1) {
                        maze[betweenY][betweenX] = 0;
                        maze[ny][nx]             = 0;
                        frontierCells.push_back({nx, ny});
                    }
                }
            }
        }

        do {
            this->startCell = genRandCell(row, col);
            this->endCell   = genRandCell(row, col);
        } while (this->endCell == this->startCell || maze[endCell.second][endCell.first] == 1
                 || maze[startCell.second][startCell.first] == 1);

        genSolution();
    }

    int getRow() {
        return row;
    }

    int getCol() {
        return col;
    }

    std::pair<int, int> getStartCell() {
        return startCell;
    }
    
    std::pair<int, int> getEndCell() {
        return endCell;
    }

    std::vector<std::vector<int>> getMaze() {
        return maze;
    }

    std::vector<std::vector<int>> getSolution() {
        return solution;
    }

    private:
    int                           row;
    int                           col;
    std::pair<int, int>           startCell;
    std::pair<int, int>           endCell;
    std::vector<std::vector<int>> maze;
    std::vector<std::vector<int>> solution;

    bool isNumber(const std::string& str) {
        for (char const& c: str) {
            if (!std::isdigit(c))
                return false;
        }
        return true;
    }

    void reset() {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                maze[i][j] = 1;
            }
        }
    }

    void genSolution() {
        solution.clear();
        solution = std::vector<std::vector<int>>(row, std::vector<int>(col, 1));

        std::queue<std::pair<int, int>>                    aboutToVisit;
        std::map<std::pair<int, int>, std::pair<int, int>> prev;
        std::vector<std::vector<bool>>                     visited(row, std::vector<bool>(col, false));

        aboutToVisit.push(startCell);
        visited[startCell.second][startCell.first] = true;

        while (!aboutToVisit.empty()) {
            auto cur = aboutToVisit.front();
            aboutToVisit.pop();

            for (auto& dir: DIR) {
                int nx = cur.first + dir.first;
                int ny = cur.second + dir.second;
                if (valid(nx, ny, row, col) && maze[ny][nx] == 0 && !visited[ny][nx]) {
                    visited[ny][nx] = true;
                    aboutToVisit.push({nx, ny});
                    prev[{nx, ny}] = cur;
                }
            }
        }

        if (visited[endCell.second][endCell.first]) {
            std::pair<int, int> step = endCell;
            while (step != startCell) {
                solution[step.second][step.first] = 0;
                step                              = prev[step];
            }
            solution[startCell.second][startCell.first] = 0;
        }
    }

    
};

#endif