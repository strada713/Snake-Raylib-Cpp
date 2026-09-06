#include <iostream>
#include <vector>
#include "raylib.h"
#include <fstream>
#include <rlgl.h>
struct Point
{
    int x;
    int y;
};

int main()
{
    InitWindow(800, 600, "my first game");
    InitAudioDevice();
    SetTargetFPS(60);
    int snakeX = 390;
    int snakeY = 300;
    int speedX = 0;
    int speedY = 0;
    float shaketime = 0.0f;
    std::vector<Point> snakeBody;
    Point apple;
    apple.x = 600;
    apple.y = 200;
    int score = 0;
    int highScore = 0;
    float exeCloseTimer = -1.0f;
    Music bgMusic = LoadMusicStream("resources/music.mp3");
    Sound eatSound = LoadSound("resources/eat.wav");
    Sound goldSound = LoadSound("resources/gold.wav");
    Sound poisonSound = LoadSound("resources/poison.wav");
    Sound dieSound = LoadSound("resources/die.mp3");
    Music exeMusic = LoadMusicStream("resources/sonic.mp3");
    Sound laughSound = LoadSound("resources/sonic_laugh.mp3");
    Image kol = LoadImage("/home/carson/Downloads/son.png");
    Texture2D kol_Texture = LoadTextureFromImage(kol);
    Image tuxImg = LoadImage("resourcec/tux.png");
    Texture2D tuxTexture = LoadTextureFromImage(tuxImg);
    UnloadImage(tuxImg);
    UnloadImage(kol);
    PlayMusicStream(bgMusic);
    float gameSpeed = 0.1f;
    int appleType = 0;
    float appleTimer = 0.0f;
    bool isExeMode = false;
    std::ifstream readFile("highscore.txt");
    if (readFile.is_open())
    {
        readFile >> highScore;
        readFile.close();
    }
    float moveTimer = 0.0f;
    bool isGameOver = false;
    bool canChangeDirection = true;
    std::vector<Point> obstacles;
    for (int x = 0; x < 800; x = x + 30)
    {
        Point topObs = {x, 0};
        Point bottomObs = {x, 570};
        obstacles.push_back(topObs);
        obstacles.push_back(bottomObs);
    }
    for (int y = 30; y < 570; y = y + 30)
    {
        Point leftObs = {0, y};
        Point rightObs = {770, y};
        obstacles.push_back(leftObs);
        obstacles.push_back(rightObs);
    }
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgMusic);
        UpdateMusicStream(exeMusic);
        if (IsKeyPressed(KEY_D) && speedX <= 0 && canChangeDirection)
        {
            speedX = 30;
            speedY = 0;
            canChangeDirection = false;
        }
        if (IsKeyPressed(KEY_A) && speedX >= 0 && canChangeDirection)
        {
            speedX = -30;
            speedY = 0;
            canChangeDirection = false;
        }
        if (IsKeyPressed(KEY_W) && speedY <= 0 && canChangeDirection)
        {
            speedX = 0;
            speedY = -30;
            canChangeDirection = false;
        }
        if (IsKeyPressed(KEY_S) && speedY >= 0 && canChangeDirection)
        {
            speedX = 0;
            speedY = 30;
            canChangeDirection = false;
        }
        if (snakeX < 0 || snakeX > 800 || snakeY < 0 || snakeY > 600)
        {
            isGameOver = true;
        }
        if (score > 0)
        {
            for (size_t i = 1; i < snakeBody.size(); i++)
            {
                if (snakeX == snakeBody[i].x && snakeY == snakeBody[i].y)
                {
                    isGameOver = true;
                    break;
                }
            }
            for (const auto &obs : obstacles)
            {
                if (snakeX == obs.x && snakeY == obs.y)
                {
                    isGameOver = true;
                    break;
                }
            }
        }
        BeginDrawing();
        if (isExeMode)
        {
            ClearBackground(BLACK);
        }
        else
        {
            ClearBackground(DARKGRAY);
        }
        if (shaketime > 0.0f)
        {
            int offsetX = GetRandomValue(-6, 6);
            int offsetY = GetRandomValue(-6, 6);
            rlTranslatef((float)offsetX, (float)offsetY, 0.0f);
        }
        if (appleType == 1)
        {
            DrawRectangle(apple.x, apple.y, 30, 30, GOLD);
        }
        else if (appleType == 2)
        {
            DrawRectangle(apple.x, apple.y, 30, 30, MAGENTA);
        }
        else if (appleType == 3)
        {
            DrawRectangle(apple.x, apple.y, 30, 30, BLACK);
        }
        else if (appleType == 4)
        {
            DrawTexturePro(
                tuxTexture,
                Rectangle{0.0f, 0.0f, (float)tuxTexture.width, (float)tuxTexture.height},
                Rectangle{(float)apple.x, (float)apple.y, 30.0f, 30.0f},
                Vector2{0.0f, 0.0f},
                0.0f,
                WHITE);
        }
        else
        {
            DrawRectangle(apple.x, apple.y, 30, 30, RED);
        }
        for (const auto &obs : obstacles)
        {
            DrawRectangle(obs.x, obs.y, 30, 30, GRAY);
        }
        for (const auto &cell : snakeBody)
        {
            DrawRectangle(cell.x, cell.y, 30, 30, GREEN);
            if (!snakeBody.empty())
            {
                int headX = snakeBody[0].x;
                int headY = snakeBody[0].y;

                int lookX = speedX / 3;
                int lookY = speedY / 3;

                if (speedX == 0 && speedY == 0)
                {
                    lookX = 10;
                }

                DrawRectangle(headX + 10 + lookX, headY + 5 + lookY, 5, 5, BLACK);
                DrawRectangle(headX + 10 + lookX, headY + 20 + lookY, 5, 5, BLACK);
            }
        }
        DrawText(TextFormat("SCORE: %i", score), 20, 20, 20, RAYWHITE);
        DrawText(TextFormat("BEST: %i", highScore), 160, 20, 20, GOLD);
        if (isGameOver)
        {
            DrawText("GAME OVER", 280, 240, 48, RED);
            DrawText("PRESS ENTER TO RESTART", 240, 310, 24, RAYWHITE);
        }
        Rectangle snakeRec = {(float)snakeX, (float)snakeY, 30, 30};
        Rectangle appleRec = {(float)apple.x, (float)apple.y, 30, 30};
        if (CheckCollisionRecs(snakeRec, appleRec))
        {
            if (appleType == 1)
            {
                DrawRectangle(apple.x, apple.y, 30, 30, GOLD);
                score = score + 3;
                shaketime = 0.2f;
                PlaySound(goldSound);
            }
            else if (appleType == 2)
            {
                DrawRectangle(apple.x, apple.y, 30, 30, MAGENTA);
                score = score - 2;
                PlaySound(poisonSound);
                if (score < 0)
                {
                    isGameOver = true;
                    PlaySound(dieSound);
                }
            }
            else if (appleType == 3)
            {
                DrawRectangle(apple.x, apple.y, 30, 30, BLACK);
                score = score + 5;
                isExeMode = true;
                system("powershell -Command \"Set-ItemProperty -Path 'HKCU:\\Control Panel\\Desktop' -Name Wallpaper -Value 'C:\\Users\\carson\\Downloads\\son.png'; gupdate /force\"");
                StopMusicStream(bgMusic);
                PlayMusicStream(exeMusic);
                PlaySound(laughSound);
                shaketime = 2.0f;
                exeCloseTimer = 3.0f;
            }
            else
            {
                score = score + 1;
                PlaySound(eatSound);
            }
            gameSpeed = 0.1f - (score * 0.003f);
            if (gameSpeed < 0.04f)
            {
                gameSpeed = 0.04f;
            }
            int chance = GetRandomValue(1, 100);
            if (chance <= 15)
            {
                appleType = 1;
                appleTimer = 3.0f;
            }
            else if (chance > 15 && chance <= 30)
            {

                appleType = 2;
                appleTimer = 4.0f;
            }
            else if (chance == 55)
            {
                appleType = 3;
                appleTimer = 6.0f;
            }
            else if (chance > 95 && chance <= 97)
            {
                appleType = 4;
                appleTimer = 8.0f;
            }
            else
            {
                appleType = 0;
            }
            apple.x = GetRandomValue(2, 23) * 30;
            apple.y = GetRandomValue(2, 17) * 30;
        } // gold

        if (!isGameOver)
        {
            if (isExeMode)
            {
                ClearBackground(BLACK);
                DrawTexturePro(
                    kol_Texture,
                    Rectangle{0.0f, 0.0f, (float)kol_Texture.width, (float)kol_Texture.height},
                    Rectangle{0.0f, 0.0f, 800.0f, 600.0f},
                    Vector2{0.0f, 0.0f},
                    0.0f,
                    WHITE);
            }
            else
            {
            }
            if (exeCloseTimer > 0.0f)
            {
                exeCloseTimer = exeCloseTimer - GetFrameTime();
                if (exeCloseTimer <= 0.0f)
                {
                    UnloadTexture(kol_Texture);
                    CloseAudioDevice();
                    CloseWindow();
                    return 0;
                }
            }
            moveTimer = moveTimer + GetFrameTime();
            if (shaketime > 0.0f)
            {
                shaketime = shaketime - GetFrameTime();
            }
            if (appleType != 0)
            {
                appleTimer = appleTimer - GetFrameTime();
                if (appleTimer <= 0.0f)
                {
                    appleType = 0;
                }
            }
            if (moveTimer >= gameSpeed)
            {
                Point currentHead = {snakeX, snakeY};
                snakeBody.insert(snakeBody.begin(), currentHead);
                if (snakeBody.size() > score + 1)
                {
                    snakeBody.pop_back();
                }
                snakeX = snakeX + speedX;
                snakeY = snakeY + speedY;

                canChangeDirection = true;
                moveTimer = 0.0f;
            }
        }
        else
        {
            if (IsMusicStreamPlaying(bgMusic))
            {
                StopMusicStream(bgMusic);
                PlaySound(dieSound);
                shaketime = 0.5f;
            }
            if (score > highScore)
            {
                highScore = score;

                std::ofstream writeFile("highscore.txt");
                if (writeFile.is_open())
                {
                    writeFile << highScore;
                    writeFile.close();
                }
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                snakeX = 390;
                snakeY = 300;
                speedX = 0;
                speedY = 0;
                score = 0;
                appleTimer = 0.0f;
                gameSpeed = 0.1f;
                appleType = 0;
                isGameOver - false;
                snakeBody.clear();
                obstacles.clear();
                isExeMode = false;
                StopMusicStream(exeMusic);
                PlayMusicStream(bgMusic);
                canChangeDirection = true;

                for (int x = 0; x < 800; x = x + 30)
                {
                    obstacles.push_back({x, 0});
                    obstacles.push_back({x, 570});
                }
                for (int y = 30; y < 570; y = y + 30)
                {
                    obstacles.push_back({0, y});
                    obstacles.push_back({770, y});
                }

                apple.x = GetRandomValue(2, 23) * 30;
                apple.y = GetRandomValue(2, 17) * 30;
                isGameOver = false;
            }
        }
        EndDrawing();
    }
    UnloadMusicStream(bgMusic);
    UnloadSound(eatSound);
    UnloadSound(goldSound);
    UnloadSound(poisonSound);
    UnloadSound(dieSound);
    UnloadMusicStream(exeMusic);
    UnloadSound(laughSound);
    UnloadTexture(kol_Texture);
    UnloadTexture(tuxTexture);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}
