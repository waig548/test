#ifndef GAME_H
#define GAME_H

#include <conio.h>
#include <irrKlang.h>
#include <string>

#include "Pipe.h"

class Game {
    public:
    std::string menu() {
        irrklang::ISoundEngine *engine = irrklang::createIrrKlangDevice();
        int key;
        std::string mode = "custom";
        do {
            displayMenu(mode);
            key = _getch();
            if (key == 'a' || key == 'A' && mode == "random") {
                engine->play2D("./soundEffect/clickSound.wav");
                mode = "custom";
            } else if (key == 'd' || key == 'D' && mode == "custom") {
                engine->play2D("./soundEffect/clickSound.wav");
                mode = "random";
            }

        } while (key != ' ');
        engine->drop();

        return mode;
    }
    

    void setting(std::string mode) {
        if (mode == "random") {
            pipe.generate();
        } else if (mode == "custom") {
            pipe.readBoard();
        }
    }

    void play() {
        selectedPipe = pipe.getInlet();

        while (!pipe.smooth()) {
            selectPipe();
        }

        pipe.display(selectedPipe);
        win();
    }

    private:
    Pipe                pipe;
    std::pair<int, int> selectedPipe;
    int                 dir;

    void displayMenu(std::string mode) {
        system("cls");
        const char *plumber[] = {
          ".---------------------------------------------------------.", "|                                                         |",
          "|         ______ _                 _                      |", "|         | ___ \\ |               | |                     |",
          "|         | |_/ / |_   _ _ __ ___ | |__   ___ _ __        |", "|         |  __/| | | | | '_ ` _ \\| '_ \\ / _ \\ '__|       |",
          "|         | |   | | |_| | | | | | | |_) |  __/ |          |", "|         \\_|   |_\\__,_|_| |_| |_|_.__/ \\___|_|           |",
          "|                                                         |", "'---------------------------------------------------------'"};

        const char *customButton[] = {".-------------.", "| Custom Mode |", "'-------------'"};
        const char *randomButton[] = {".-------------.", "| Random Mode |", "'-------------'"};

        for (const char *line: plumber) {
            std::cout << std::string(30, ' ') << line << std::endl;
        }

        std::cout << std::endl;

        for (int i = 0; i < 3; ++i) {
            std::cout << std::string(38, ' ');
            if (mode == "custom")
                std::cout << "\033[38;5;33m";
            std::cout << customButton[i];
            std::cout << "\033[0m";

            std::cout << std::string(11, ' ');
            if (mode == "random")
                std::cout << "\033[38;5;33m";
            std::cout << randomButton[i] << std::endl;
            std::cout << "\033[0m";
        }
    }

    void selectPipe() {
        int key;
        irrklang::ISoundEngine *engine = irrklang::createIrrKlangDevice();
        engine->play2D("./soundEffect/rotateSound.ogg");
        do {
            pipe.display(selectedPipe);
            key = _getch();
            if ((key == 'a' || key == 'A') && valid(selectedPipe.first - 1, selectedPipe.second, pipe.getRow(), pipe.getCol())) {
                engine->play2D("./soundEffect/selectSound.ogg");
                selectedPipe.first--;
            } else if ((key == 's' || key == 'S') && valid(selectedPipe.first, selectedPipe.second + 1, pipe.getRow(), pipe.getCol())) {
                engine->play2D("./soundEffect/selectSound.ogg");
                selectedPipe.second++;
            } else if ((key == 'd' || key == 'D') && valid(selectedPipe.first + 1, selectedPipe.second, pipe.getRow(), pipe.getCol())) {
                engine->play2D("./soundEffect/selectSound.ogg");
                selectedPipe.first++;
            } else if ((key == 'w' || key == 'W') && valid(selectedPipe.first, selectedPipe.second - 1, pipe.getRow(), pipe.getCol())) {
                engine->play2D("./soundEffect/selectSound.ogg");
                selectedPipe.second--;
            } else if (key == 'q' || key == 'Q') {
                pipe.selectedRotate(selectedPipe, 3);
            } else if (key == 'e' || key == 'E') {
                pipe.selectedRotate(selectedPipe, 1);
            }

        } while (key != 'Q' && key != 'q' && key != 'E' && key != 'e');
        engine->drop();
    }

    void win() {
        std::cout << "Stage Clear!" << std::endl;
    }
};

#endif