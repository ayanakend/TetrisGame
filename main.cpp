#include "raylib.h"
#include "game.h"

int main(void)
{
    const int screenWidth = 500;
    const int screenHeight = 620;

    InitWindow(screenWidth, screenHeight, "Tetris");

    Font font = LoadFontEx("C:/Users/Lenovo/Downloads/monogram.ttf", 64, 0, 0);

    Game game;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        game.HandleInput();
        game.Update();  // Вызываем функцию Update для обновления состояния игры

        BeginDrawing();
        ClearBackground(darkBlue);
        DrawTextEx(font, "Score", { 365, 15 }, 38, 2, WHITE);
        DrawTextEx(font, "Next", { 370, 175 }, 38, 2, WHITE);

        DrawRectangleRounded({ 320, 55, 170, 60 }, 0.3, 6, lightBlue);

        char scoreText[10];
        sprintf_s(scoreText, "%d", game.GetScore());
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

        DrawTextEx(font, scoreText, { 320 + (170 - textSize.x) / 2, 65 }, 38, 2, WHITE);

        DrawRectangleRounded({ 320, 215, 170, 180 }, 0.3, 6, lightBlue);

        // Вычисляем и отображаем уровень
        int level = game.GetLevel();
        char levelText[20];
        sprintf_s(levelText, "Level: %d", level + 1); // Уровень на один больше, чем индекс
        Vector2 levelTextSize = MeasureTextEx(font, levelText, 38, 2);

        // Опускаем уровень чуть ниже
        DrawTextEx(font, levelText, { 320 + (170 - levelTextSize.x) / 2, 460 }, 38, 2, WHITE);

        game.Draw();

        if (game.gameOver)
        {
            // Полупрозрачный фон на весь экран
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(darkBlue, 0.8f));
            DrawTextEx(font, "GAME OVER", { 100, 200 }, 38, 2, WHITE);
            DrawTextEx(font, "Press any button to restart", { 80, 250 }, 20, 2, WHITE);
        }

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}
