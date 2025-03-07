#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 luzPos;
uniform vec3 luzColor;
uniform vec3 viewPos;
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
    vec4 texColor = texture(textura, TexCoord);
    
    if (texColor.a < 0.1) {
        discard;
    }

    // Usar valores por defecto si el material no está definido correctamente
    Material mat = material.ambient != vec3(0.0) ? material : defaultMaterial;

    // Componente ambiental
    vec3 ambient = luzColor * mat.ambient;

    // Componente difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(luzPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = luzColor * (diff * mat.diffuse);

    // Componente especular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = luzColor * (spec * mat.specular);

    // Si el objeto no tiene textura, aplicar el color como aditivo con la iluminación
    vec3 finalColor = texColor.rgb;

    // Si no tiene textura (es transparente o negro), usamos el color proporcionado
    if (texColor.rgb == vec3(0.0)) {
        finalColor = color.rgb + (ambient + diffuse + specular); // Aditivo
    } else {
        // Si tiene textura, aplicamos la textura con la iluminación
        finalColor = (ambient + diffuse + specular) * texColor.rgb;
    }

    // Color final
    FragColor = vec4(finalColor, texColor.a);
}
