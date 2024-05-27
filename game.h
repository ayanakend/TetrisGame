#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <vector>
#include "block.h"
#include "grid.h"

class Game {
public:
    Game();
    ~Game();

    void Draw();
    void HandleInput();
    void Update();
    int GetScore() const;
    int GetLevel() const;
    int score;

    bool gameOver;

private:
    enum GameState { WELCOME_SCREEN, PLAYING } gameState;

    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    double dropInterval;
    double lastDropTime;
    Music music;
    Sound rotateSound;
    Sound clearSound;
    Sound gameOverSound;

    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int linesCleared);
    void SetDropInterval(double interval);
    double GetDropInterval();
    void DrawWelcomeScreen();
};

#endif
