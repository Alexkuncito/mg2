#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 luzPos;
uniform vec3 luzColor;
uniform vec3 viewPos;
uniform sampler2D textura;

void main() {
    vec4 texColor = texture(textura, TexCoord);

    if (texColor.a < 0.1) {
        discard;
    }

    vec3 ambient = 0.2 * luzColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(luzPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * luzColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * luzColor;

    vec3 resultado = (ambient + diffuse + specular) * texColor.rgb;
    FragColor = vec4(resultado, texColor.a);
}
