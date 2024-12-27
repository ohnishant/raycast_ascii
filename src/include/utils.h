#ifndef RAYCAST_UTILS_H
#define RAYCAST_UTILS_H

#include <stdint.h>
#define MAP_TILE_SIZE 64

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef DEG2RAD
#define DEG2RAD (PI / 180.0f)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0f / PI)
#endif

void die(const char* s);
int msleep(uint64_t msec);

#endif // #ifdef RAYCAST_UTILS_H
