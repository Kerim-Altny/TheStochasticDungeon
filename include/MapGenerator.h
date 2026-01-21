#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <raylib.h>
class UnionFind {
    std::vector<int> parent;
public:
    UnionFind(int size) { 
        parent.resize(size);
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    int Find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = Find(parent[i]); 
    }

    void Union(int i, int j) {
        int rootA = Find(i);
        int rootB = Find(j);
        if (rootA != rootB) {
            parent[rootA] = rootB; 
        }
    }
};

        
enum TileType {
    EMPTY = 0,
    WALL = 1,
    FLOOR = 2
};

struct Tile {
    int x, y;
    TileType type;
};

struct Room {
    int x, y;          
    int width, height;
    int centerX, centerY;
    
    Room(int _x, int _y, int _w, int _h) {
        x = _x;
        y = _y;
        width = _w;
        height = _h;
        centerX = x + width / 2;
        centerY = y + height / 2;
    }
    bool Intersects(const Room& other) const {
        int padding=1;
       return (x <= other.x + other.width + padding && 
            x + width + padding >= other.x &&
            y <= other.y + other.height + padding && 
            y + height + padding >= other.y);
    }
};

struct Edge {
    int roomA;
    int roomB;
    double weight;
    Edge(int a, int b,double w) : roomA(a), roomB(b), weight(w) {
    }
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class MapGenerator {
private:
    static const int MAP_WIDTH = 50;
    static const int MAP_HEIGHT = 50;
    std::vector<std::vector<Tile>> map;
    std::vector<Room> rooms;
    std::vector<Edge> allEdges;
    std::vector<Edge> mstEdges; 
    
    
public:
    MapGenerator();
    void InitializeMap();
    void DrawMap(int tileSize);
    TileType GetTile(int x, int y);
    void GenerateRooms(int numAttempts);
    void Generate(int numAttempts);
    void CreateRoom(const Room& room);
    void CalculateAllEdges();
    double Distance(const Room& a, const Room& b);
    void DrawConnections();
    void CalculateMST();
    void DrawMST();
    void CreateCorridor(const Room& roomA, const Room& roomB);
    Vector2  GetStartPosition() {
    if (rooms.empty()) return {1, 1}; 
    return { (float)rooms[0].centerX, (float)rooms[0].centerY }; 
}
};
