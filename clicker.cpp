#include "raylib.h"
int main()
{
    InitWindow(800, 600, "my first clicker");
    SetTargetFPS(60);
    int score = 0;
    int radius = 100;
    int autoClickers = 0;
    float autoTimer = 0.0f;
    int autoClickerCost = 10;
    int clickPower = 1;
    int clickUpgradeCost = 15;
    InitAudioDevice();
    Sound clickSound = LoadSound("resources/gold.wav");
    Color bgcolor = DARKBLUE;
    Music bgMusic = LoadMusicStream("resources/phone.mp3");
    PlayMusicStream(bgMusic);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgMusic);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointCircle(GetMousePosition(), Vector2{400, 300}, 100))
            {
                score = score + clickPower;
                radius = 85;
                PlaySound(clickSound);
            }
        }
        if (radius < 100)
            radius = radius + 1;
        if (autoClickers > 0)
        {
            autoTimer = autoTimer + GetFrameTime();
            if (autoTimer >= 1.0f)
            {
                score = score + autoClickers;
                autoTimer = autoTimer - 1.0f;
            }
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            if (score >= autoClickerCost)
            {
                score = score - autoClickerCost;
                autoClickers = autoClickers + 1;
                autoClickerCost = autoClickerCost + 20;
            }
        }
        if (IsKeyPressed(KEY_C))
        {
            if (score >= clickUpgradeCost)
            {
                score = score - clickUpgradeCost;
                clickPower = clickPower + 1;
                clickUpgradeCost = clickUpgradeCost + 10;
            }
        }
        if (score < 50)
        {
            bgcolor = DARKBLUE;
        }
        else if (score >= 50 && score < 100)
        {
            bgcolor = PURPLE;
        }
        else if (score >= 100 && score < 200)
        {
            bgcolor = MAGENTA;
        }
        else if (score >= 200)
        {
            bgcolor = BLACK;
        }
        BeginDrawing();
        ClearBackground(bgcolor);

        DrawCircle(400, 300, radius, RED);
        DrawText(TextFormat("SCORE: %d", score), 320, 80, 40, YELLOW);
        DrawText(TextFormat("AUTO-CLICKERS: %d", autoClickers), 20, 20, 20, GOLD);
        DrawText(TextFormat("PRESS SPACE TO BUY AUTO-CLICKER (COST: %d)", autoClickerCost), 20, 50, 20, RAYWHITE);
        DrawText(TextFormat("CLICK POWER +%d", clickPower), 20, 80, 20, GREEN);
        DrawText(TextFormat("PRESS 'C' TO UPGRADE CLICK (COST: %d)", clickUpgradeCost), 20, 110, 20, RAYWHITE);
        EndDrawing();
    }
    UnloadSound(clickSound);
    UnloadMusicStream(bgMusic);
    CloseWindow();
    return 0;
}