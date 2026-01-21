#include "MapGenerator.h"
#include "raylib.h"
#include <cmath>
#include <algorithm>

MapGenerator::MapGenerator()
{
    InitializeMap();
}

void MapGenerator::InitializeMap()
{
    
    map.resize(MAP_WIDTH);

  
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        map[x].resize(MAP_HEIGHT);
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            map[x][y].x = x;
            map[x][y].y = y;
            map[x][y].type = WALL;
        }
    }
}


void MapGenerator::DrawMap(int tileSize)
{
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            Color tileColor;

            switch (map[x][y].type)
            {
            case WALL:
                tileColor = GRAY;
                break;
            case FLOOR:
                tileColor = WHITE;
                break;
            case EMPTY:
                tileColor = BLACK;
                break;
            }

            DrawRectangle(
                x * tileSize,
                y * tileSize,
                tileSize - 1,
                tileSize - 1,
                tileColor);
        }
    }
}

TileType MapGenerator::GetTile(int x, int y)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return WALL;
    return map[x][y].type;
}

void MapGenerator::GenerateRooms(int numAttempts)
{
    srand(time(NULL));
    for (int i = 0; i < numAttempts; i++)
    {
        int roomWidth = rand() % 6 + 3;
        int roomHeight = rand() % 6 + 3;
        int roomX = rand() % (MAP_WIDTH - roomWidth - 2) + 1;
        int roomY = rand() % (MAP_HEIGHT - roomHeight - 2) + 1;

        Room newRoom(roomX, roomY, roomWidth, roomHeight);

        bool intersects = false;
        for (const auto &otherRoom : rooms)
        {
            if (newRoom.Intersects(otherRoom))
            {
                intersects = true;
                break;
            }
        }

        if (!intersects)
        {
            rooms.push_back(newRoom);
            CreateRoom(newRoom);
        }
    }
}
void MapGenerator::CreateRoom(const Room &room)
{
    for (int x = room.x; x < room.x + room.width; x++)
    {
        for (int y = room.y; y < room.y + room.height; y++)
        {
            map[x][y].type = FLOOR;
        }
    }
}
void MapGenerator::CalculateAllEdges()
{
    allEdges.clear();
    for (size_t i = 0; i < rooms.size(); i++)
    {
        for (size_t j = i + 1; j < rooms.size(); j++)
        {
            double dist = Distance(rooms[i], rooms[j]);
            allEdges.emplace_back(i, j, dist);
        }
    }
}
double MapGenerator::Distance(const Room &a, const Room &b)
{
    int dx = a.centerX - b.centerX;
    int dy = a.centerY - b.centerY;
    return std::hypot(dx, dy);
}
void MapGenerator::DrawConnections()
{
    CalculateAllEdges();
    for (const auto &edge : allEdges)
    {
        Room &roomA = rooms[edge.roomA];
        Room &roomB = rooms[edge.roomB];

        DrawLine(
            roomA.centerX * 12,
            roomA.centerY * 12,
            roomB.centerX * 12,
            roomB.centerY * 12,
            YELLOW);
    }
}
void MapGenerator::CalculateMST()
{
    mstEdges.clear();
    std::sort(allEdges.begin(), allEdges.end(),[](const Edge &a, const Edge &b) {
        return a.weight < b.weight;
    });
    UnionFind uf(rooms.size());
    for (const auto &edge : allEdges)
    {
        if (uf.Find(edge.roomA) != uf.Find(edge.roomB))
        {
            uf.Union(edge.roomA, edge.roomB);
            mstEdges.push_back(edge); 
            if (mstEdges.size() == rooms.size() - 1) {
                break;
            }
        }
    }

}
void MapGenerator::DrawMST()
{

    for (const auto &edge : mstEdges)
    {
        Room &roomA = rooms[edge.roomA];
        Room &roomB = rooms[edge.roomB];

        DrawLineEx(
            {(float)roomA.centerX * 12,(float)roomA.centerY * 12},
            {(float)roomB.centerX * 12,(float)roomB.centerY * 12},
            3.0f,GREEN);
    DrawCircle(roomA.centerX * 12, roomA.centerY * 12, 3, RED);
    DrawCircle(roomB.centerX * 12, roomB.centerY * 12, 3, RED);
    }
    
}
void MapGenerator::CreateCorridor(const Room& roomA, const Room& roomB){
    int x1 = roomA.centerX;
    int y1 = roomA.centerY;
    int x2 = roomB.centerX;
    int y2 = roomB.centerY;

    if (rand() % 2 == 0) {
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            map[x][y1].type = FLOOR;
        }
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            map[x2][y].type = FLOOR;
        }
    } else {
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            map[x1][y].type = FLOOR;
        }
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            map[x][y2].type = FLOOR;
        }
    }
}
void MapGenerator::Generate(int numAttempts)
{
    InitializeMap(); 
    rooms.clear();

    GenerateRooms(numAttempts);
  
    CalculateAllEdges();

    CalculateMST();
    for (const auto& edge : mstEdges) {
        Room& roomA = rooms[edge.roomA];
        Room& roomB = rooms[edge.roomB];

        CreateCorridor(roomA, roomB);
    }
}