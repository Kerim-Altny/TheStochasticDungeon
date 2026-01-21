#include "Agent.h"

Agent::Agent(int startX, int startY) {
    position = {float(startX), float(startY)};
    color = BLUE; 
}
void Agent::Update(MapGenerator& map) {
    int dx = 0;
    int dy = 0;

    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    dy = -1;
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  dy = 1;
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  dx = -1;
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) dx = 1;

    if (dx != 0 || dy != 0) {
        int targetX = position.x + dx;
        int targetY = position.y + dy;

        if (map.GetTile(targetX, targetY) != WALL) {
            position.x = targetX;
            position.y = targetY;
        }
    }
}

void Agent::Draw() {
    int tileSize = 12; 
    int pixelX = position.x * tileSize + tileSize/2;
    int pixelY = position.y * tileSize + tileSize/2;

    DrawCircle(pixelX, pixelY, tileSize/2 - 2, color);
}