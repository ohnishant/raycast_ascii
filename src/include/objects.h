#ifndef RAYCAST_OBJECTS_H
#define RAYCAST_OBJECTS_H
struct GameState {
    float playerX;
    float playerY;
    float deltaX;
    float deltaY;

    float cameraAngle;
    int screenRows;
    int screenCols;
};
#endif // #ifndef RAYCAST_OBJECTS_H
