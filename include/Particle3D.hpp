#ifndef PARTICLE3D_HPP
#define PARTICLE3D_HPP

#include "glm/glm.hpp"

struct Particle3D {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle3D()
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

#endif // PARTICLE3D_HPP
