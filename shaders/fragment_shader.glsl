#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;   // Posición de la luz
uniform vec3 viewPos;    // Posición de la cámara
uniform vec3 lightColor; // Color de la luz

uniform sampler2D texture1;

void main() {
    // Propiedades del material
    vec3 objectColor = texture(texture1, TexCoord).rgb;
    
    // Normalizar normal
    vec3 norm = normalize(Normal);
    
    // Dirección de la luz
    vec3 lightDir = normalize(lightPos - FragPos);
    
    // Componente difusa (Lambert)
    float diff = max(dot(norm, lightDir), 0.0);
    
    // Componente especular (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Brillo
    
    // Combinación de componentes
    vec3 ambient = 0.1 * lightColor;  // Luz ambiental
    vec3 diffuse = diff * lightColor; // Luz difusa
    vec3 specular = spec * lightColor; // Luz especular
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
