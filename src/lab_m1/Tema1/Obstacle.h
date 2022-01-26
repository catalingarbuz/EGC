#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Obstacle
    {
    public:
        Obstacle();
        Obstacle(float posX, float posY, float width, float height);
        ~Obstacle();

    public:
        float posX, posY;
        float width, height;
    };
}   // namespace m1
#pragma once
