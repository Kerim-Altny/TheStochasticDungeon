#pragma once
#include <raylib.h>
#include "MapGenerator.h" 

class Agent{
private:
    Vector2 position; 
    Color color;

public:
    Agent(int x, int y)  ;
    void Draw() ;

    void Update(MapGenerator& map);
};