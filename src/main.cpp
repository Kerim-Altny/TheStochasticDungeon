#include "raylib.h"
#include "MapGenerator.h"
#include "Agent.h"
#include <iostream>

int main()
{
    std::cout << "Project Ashka Started!" << std::endl;

    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 600;
    const int TILE_SIZE = 12; 

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project Ashka - Day 2");
    SetTargetFPS(60);

    MapGenerator mapGen;
    mapGen.Generate(1000);
    Vector2 startPos = mapGen.GetStartPosition();
    Agent player(startPos.x, startPos.y);
    while (!WindowShouldClose())
    { 
        if (IsKeyPressed(KEY_SPACE))
        {
            mapGen.Generate(1000);
            startPos = mapGen.GetStartPosition();
            player = Agent(startPos.x, startPos.y);
        }

        player.Update(mapGen); 
        BeginDrawing();
        ClearBackground(BLACK);

        mapGen.DrawMap(TILE_SIZE);
        player.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}