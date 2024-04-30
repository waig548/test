#include "Game.h"
#include <iostream>
#include <irrKlang.h>

int main() {
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    engine->play2D("./soundEffect/BGM.wav", true);

    Game mainGame;
    std::string mode = mainGame.menu();
    mainGame.setting(mode);
    mainGame.play();

    engine->play2D("./soundEffect/endSound.wav");
    system("pause");
    engine->drop();
}