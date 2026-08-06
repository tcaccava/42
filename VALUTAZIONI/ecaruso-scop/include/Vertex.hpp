#pragma once

#include "Math/Vector3.hpp"
#include "Math/Vector2.hpp"

struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector3 color;
    Vector2 texCoord;
};