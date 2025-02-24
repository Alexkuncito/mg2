#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 luzPos;
uniform vec3 luzColor;
uniform vec3 viewPos;

void main() {
    // Propiedades del material
    vec3 ambient = 0.2 * luzColor;

    // Componente difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(luzPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * luzColor;

    // Componente especular (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * luzColor;

    vec3 resultado = ambient + diffuse + specular;
    FragColor = vec4(resultado, 1.0);
}
