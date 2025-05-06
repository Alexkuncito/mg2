#version 450 core

layout (location = 0) in vec3 aPos;       // posición del vértice
layout (location = 1) in vec2 aTexCoord;  // coordenadas de textura

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

void main()
{
    TexCoords = aTexCoord;
    ParticleColor = color;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
