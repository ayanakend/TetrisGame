#include "game.h"
#include <random>
#include "blocks.cpp"

Game::Game()
{
    srand(static_cast<unsigned int>(time(0))); // »нициализаци€ генератора случайных чисел

    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
    dropInterval = 0.5; // Initial drop interval
    gameState = WELCOME_SCREEN; // ”станавливаем начальное состо€ние как приветственный экран
    InitAudioDevice();
    music = LoadMusicStream("C:/Users/Lenovo/Downloads/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("C:/Users/Lenovo/Downloads/rotate.mp3");
    clearSound = LoadSound("C:/Users/Lenovo/Downloads/clear.mp3");
    gameOverSound = LoadSound("C:/Users/Lenovo/Downloads/gameover.mp3");
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return { IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock() };
}

void Game::Draw()
{
    switch (gameState)
    {
    case WELCOME_SCREEN:
        DrawWelcomeScreen();
        break;
    case PLAYING:
        grid.Draw();
        currentBlock.Draw(11, 11);
        switch (nextBlock.id)
        {
        case 3:
            nextBlock.Draw(255, 290);
            break;
        case 4:
            nextBlock.Draw(255, 280);
            break;
        default:
            nextBlock.Draw(270, 270);
            break;
        }
        break;
    }
}

void Game::DrawWelcomeScreen()
{
    // ќтрисовка непрозрачного фона
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(darkBlue, 1.0f));

    DrawText("Welcome to Tetris", 80, 200, 40, RAYWHITE);
    DrawText("Press Enter to Start", 80, 250, 20, RAYWHITE);
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if (gameState == WELCOME_SCREEN && keyPressed == KEY_ENTER)
    {
        gameState = PLAYING;
        Reset();
        return;
    }

    if (gameState == PLAYING)
    {
        if (gameOver && keyPressed != 0)
        {
            gameOver = false;
            Reset();
        }
        switch (keyPressed)
        {
        case KEY_LEFT:
            MoveBlockLeft();
            break;
        case KEY_RIGHT:
            MoveBlockRight();
            break;
        case KEY_DOWN:
            MoveBlockDown();
            break;
        case KEY_UP:
            RotateBlock();
            break;
        }
    }
}

void Game::MoveBlockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown()
{
    if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver)
    {
        currentBlock.Rotate();
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.UndoRotation();
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
        if (item.row <= 0) // ≈сли фигура достигла верха пол€
        {
            gameOver = true;

            // ќстановить воспроизведение фоновой музыки при проигрыше
            StopMusicStream(music);

            // ¬оспроизвести звук проигрыша
            PlaySound(clearSound);

            return; // ¬ыходим из функции, чтобы прекратить обработку фигуры
        }
    }
    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsCleared); // ќбновл€ем счет только на основе очищенных строк
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
}

void Game::UpdateScore(int linesCleared)
{
    score += linesCleared * 40;

    if (score / 200 > (score - linesCleared * 40) / 200) // ”величение скорости каждые 200 очков
    {
        dropInterval *= 0.9; // ”величение скорости путем уменьшени€ интервала
    }
}

void Game::Update()
{
    //  уменьшение интервала падени€
    if (!gameOver && GetTime() - lastDropTime > dropInterval)
    {
        MoveBlockDown();
        lastDropTime = GetTime();
    }
    UpdateMusicStream(music);
}

int Game::GetScore() const
{
    return score;
}

int Game::GetLevel() const
{
    return score / 200;
}

void Game::SetDropInterval(double interval)
{
    dropInterval = interval;
}

double Game::GetDropInterval()
{
    return dropInterval;
}
