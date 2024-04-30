#ifndef PIPE_H
#define PIPE_H

#include <queue>
#include <sstream>
#include <fstream>
#include <irrKlang.h>
#include "Algorithm.h"
#include "Direction.h"
#include "Maze.h"

class Pipe {
    public:
    void generate() {
        maze.setting();
        do {
            maze.genMaze();
            inlet  = maze.getStartCell();
            outlet = maze.getEndCell();
            row    = maze.getRow();
            col    = maze.getCol();

            pipe.clear();
            pipe.resize(
              row, std::vector<std::vector<std::vector<char>>>(col, std::vector<std::vector<char>>(3, std::vector<char>(3, '#'))));
            for (int row = 0; row < this->row; row++) {
                for (int col = 0; col < this->col; col++) {
                    pipe[row][col][1][1] = 'P';
                }
            }

            pipe[inlet.second][inlet.first][1][1]   = 'I';
            pipe[outlet.second][outlet.first][1][1] = 'O';
            genPipeSolution();
            disruptPipe();
        } while (smooth());
    }

    void readBoard() {
        irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
        engine->play2D("./soundEffect/enterSound.wav");
        system("cls");
        std::cout << "Please enter your board file name with its extension: ";
        std::string   filename = "board.txt";
        std::cin >> filename;
        std::ifstream file(filename);

        while (!file.is_open()) {
            system("cls");
            std::cout << "Error opening file: " << filename << "\nPlease try again: ";
            std::cin >> filename;
            file.close();
            file.open(filename);
        }

        file >> row >> col;
        maze.init(this->row, this->col);
        inlet  = maze.getStartCell();
        outlet = maze.getEndCell();

        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        pipe.clear();
        pipe.resize(row, std::vector<std::vector<std::vector<char>>>(col, std::vector<std::vector<char>>(3, std::vector<char>(3, '#'))));
        for (int r = 0; r < this->row; r++) {
            for (int segmentRow = 0; segmentRow < 3; segmentRow++) {
                std::string input;
                while (getline(file, input)) { 
                    if (!input.empty()) {
                        break;
                    }
                }

                std::stringstream ss(input);
                char              character;
                for (int c = 0; c < this->col; c++) {
                    for (int segmentCol = 0; segmentCol < 3; segmentCol++) {
                        if (ss >> character) {
                            if (character == '#' || character == 'P') {
                                pipe[r][c][segmentRow][segmentCol] = character;
                            } else {
                                segmentCol--;
                            }
                        }
                    }
                }
            }
        }

        file.close();
        pipe[inlet.second][inlet.first][1][1]   = 'I';
        pipe[outlet.second][outlet.first][1][1] = 'O';
    }

    void display(std::pair<int, int> selectedPipe) {
        system("cls");

        bool isSmooth = smooth();
        for (int row = 0; row < this->row; row++) {
            for (int segmentRow = 0; segmentRow < 3; segmentRow++) {
                for (int col = 0; col < this->col; col++) {
                    for (int segmentCol = 0; segmentCol < 3; segmentCol++) {
                        char block = pipe[row][col][segmentRow][segmentCol];
                        if (row == selectedPipe.second && col == selectedPipe.first)
                            std::cout << "\033[48;5;255m";

                        if (block == 'O') {
                            if (isSmooth) {
                                std::cout << "\033[38;5;33m";
                            } else {
                                std::cout << "\033[38;5;196m";
                            }
                            std::cout << 'O' << " ";
                        } else if (block == 'I') {
                            std::cout << "\033[38;5;33m";
                            std::cout << 'I' << " ";
                        } else if (block == 'P') {
                            if (maze.getSolution()[row][col] == 0 && !isSmooth) {
                                std::cout << "\033[38;5;226m";
                            } else {
                                std::cout << "\033[38;5;34m";
                            }

                            std::cout << 'P' << " ";
                        } else if (block == 'W') {
                            std::cout << "\033[38;5;33m";
                            std::cout << 'P' << " ";
                        } else {
                            std::cout << "\033[38;5;237m";
                            std::cout << block << " ";
                        }

                        std::cout << "\033[0m";
                    }
                }
                std::cout << std::endl;
            }
        }
        std::cout << "\033[0m";
    }

    void selectedRotate(std::pair<int, int> selectedPipe, int dir) {
        rotatePipe(pipe[selectedPipe.second][selectedPipe.first], dir);
    }

    bool smooth() {
        drainWater();
        fillWater();

        for (int segmentRow = 0; segmentRow < 3; segmentRow++) {
            for (int segmentCol = 0; segmentCol < 3; segmentCol++) {
                if (pipe[outlet.second][outlet.first][segmentRow][segmentCol] == 'W')
                    return true;
            }
        }

        return false;
    }

    int getRow() {
        return row;
    }

    int getCol() {
        return col;
    }

    std::pair<int, int> getInlet() {
        return inlet;
    }

    std::pair<int, int> getOutlet() {
        return outlet;
    }

    private:
    Maze                                                     maze;
    int                                                      row;
    int                                                      col;
    std::vector<std::vector<std::vector<std::vector<char>>>> pipe;
    std::pair<int, int>                                      inlet;
    std::pair<int, int>                                      outlet;

    void genPipeSolution() {
        for (int row = 0; row < this->row; row++) {
            for (int col = 0; col < this->col; col++) {
                int path = 0;
                for (int dir = 0; dir < DIR.size(); dir++) {
                    int nx = col + DIR[dir].first, ny = row + DIR[dir].second;
                    if (valid(nx, ny, this->row, this->col) && maze.getMaze()[ny][nx] == 0 || std::rand() % 100 == 0) {
                        pipe[row][col][1 + DIR[dir].second][1 + DIR[dir].first] = 'P';
                        path++;
                    }
                }

                if (path < 2) {
                    int rand = std::rand();
                    while (pipe[row][col][1 + DIR[rand % 4].second][1 + DIR[rand % 4].first] == 'P')
                        rand = std::rand();

                    pipe[row][col][1 + DIR[rand % 4].second][1 + DIR[rand % 4].first] = 'P';
                    path++;
                }
            }
        }

        pipe[maze.getStartCell().second][maze.getStartCell().first][1][1] = 'I';
    }

    void rotatePipe(std::vector<std::vector<char>>& pipe, int dir) {
        std::vector<std::vector<char>> prevPipe = pipe;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                switch (dir) {
                    case 0:
                        pipe[2 - i][2 - j] = prevPipe[i][j];
                        break;
                    case 1:  // 90
                        pipe[j][2 - i] = prevPipe[i][j];
                        break;
                    case 2:
                        pipe[2 - i][2 - j] = prevPipe[i][j];
                        break;
                    case 3:  // 270
                        pipe[2 - j][i] = prevPipe[i][j];
                        break;
                }
            }
        }
    }

    void disruptPipe() {
        for (int row = 0; row < this->row; row++) {
            for (int col = 0; col < this->col; col++) {
                int rand = std::rand() % 4;
                rotatePipe(pipe[row][col], rand);
            }
        }
    }

    void drainWater() {
        for (int row = 0; row < this->row; row++) {
            for (int segmentRow = 0; segmentRow < 3; segmentRow++) {
                for (int col = 0; col < this->col; col++) {
                    for (int segmentCol = 0; segmentCol < 3; segmentCol++) {
                        if (pipe[row][col][segmentRow][segmentCol] == 'W')
                            pipe[row][col][segmentRow][segmentCol] = 'P';
                    }
                }
            }
        }
    }

    void fillWater() {
        std::queue<std::vector<int>> coordinates;
        coordinates.push({inlet.second, inlet.first, 1, 1});

        const std::vector<int> notP     = {-1, -1, -1, -1};
        const std::vector<int> inletCor = {inlet.second, inlet.first, 1, 1};
        while (!coordinates.empty()) {
            std::vector<int> curCor = coordinates.front();
            coordinates.pop();

            for (int around = 0; around < 4; around++) {
                std::vector<int> blockCor = thereIsP(curCor, around);
                if (blockCor != notP && blockCor != inletCor) {
                    pipe[blockCor[0]][blockCor[1]][blockCor[2]][blockCor[3]] = 'W';
                    coordinates.push(blockCor);
                }
            }
        }
    }

    std::vector<int> thereIsP(std::vector<int> curCor, int around) {
        const std::vector<int> notP = {-1, -1, -1, -1};

        switch (around) {
            case 1:
                if (curCor[3] == 2 && curCor[1] == col - 1) {
                    curCor = notP;
                } else if (curCor[3] == 2) {
                    curCor[1]++;
                    curCor[3] = 0;
                } else {
                    curCor[3]++;
                }
                break;
            case 2:
                if (curCor[2] == 2 && curCor[0] == row - 1) {
                    curCor = notP;
                } else if (curCor[2] == 2) {
                    curCor[0]++;
                    curCor[2] = 0;
                } else {
                    curCor[2]++;
                }
                break;
            case 3:
                if (curCor[3] == 0 && curCor[1] == 0) {
                    curCor = notP;
                } else if (curCor[3] == 0) {
                    curCor[1]--;
                    curCor[3] = 2;
                } else {
                    curCor[3]--;
                }
                break;
            case 0:
                if (curCor[2] == 0 && curCor[0] == 0) {
                    curCor = notP;
                } else if (curCor[2] == 0) {
                    curCor[0]--;
                    curCor[2] = 2;
                } else {
                    curCor[2]--;
                }
                break;
        }

        if (curCor != notP && pipe[curCor[0]][curCor[1]][curCor[2]][curCor[3]] == 'P')
            return curCor;

        return notP;
    }
};

#endif