#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

layout(location = 0) uniform vec3 luzPos;
layout(location = 1) uniform vec3 luzColor;
uniform sampler2D textura;
uniform vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

// Valores por defecto
const Material defaultMaterial = Material(
    vec3(0.2, 0.2, 0.2),  // Ambient
    vec3(1.0, 1.0, 1.0),  // Diffuse
    vec3(0.5, 0.5, 0.5),  // Specular
    32.0                  // Shininess
);

void main() {
    // Usamos los uniforms de forma que *afecte* a algo para que el compilador no los elimine
    vec3 luzDummy = normalize(luzPos + luzColor);

    // Color de textura
    vec4 texColor = texture(textura, TexCoord);

    if (texColor.a < 0.1) {
        discard;
    }

    // Usamos el material pasado, o el valor por defecto si el material no está definido correctamente
    Material mat = material.ambient != vec3(0.0) ? material : defaultMaterial;

    // Color final base
    vec3 finalColor = texColor.rgb;

    if (texColor.rgb == vec3(0.0)) {
        finalColor = color.rgb != vec3(0.0) ? color.rgb : clamp(mat.diffuse*5, vec3(0.0), vec3(1.0));
    }

    // Le metemos una mezcla minúscula con el valor dummy
    finalColor = mix(finalColor, luzDummy, 0.000001);  // insignificante visualmente

    FragColor = vec4(finalColor, texColor.a);
}
