#version 450 core
in vec2 TexCoords;
in vec4 ParticleColor; // Color de la partícula (puede tener alpha)
out vec4 FragColor;

uniform sampler2D sprite; // Textura de la partícula

void main()
{
    vec4 texColor = texture(sprite, TexCoords); // Obtener el color de la textura
    vec4 finalColor = texColor * ParticleColor;  // Aplicar el color de la partícula

    // Si la transparencia (alpha) es baja, descarta el fragmento
    if (finalColor.a < 0.1)
        discard;

    FragColor = finalColor;  // El color final con transparencia
}
