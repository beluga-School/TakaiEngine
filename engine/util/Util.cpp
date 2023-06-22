#include "Util.h"

bool Util::OutOfScreen(const Vector2& pos)
{
    uint32_t minX = 0;
    uint32_t minY = 0;
    return pos.x < minX || WIN_WIDTH < pos.x || pos.y < minY || WIN_HEIGHT < pos.y;
}
