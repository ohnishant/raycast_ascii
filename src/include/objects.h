#ifndef RAYCAST_OBJECTS_H
#define RAYCAST_OBJECTS_H

typedef struct {
    float playerX;
    float playerY;
    float deltaX;
    float deltaY;

    float cameraAngle;
} Player;

struct GameState {
    Player p;
    int screenRows;
    int screenCols;
};

#endif // #ifndef RAYCAST_OBJECTS_H
