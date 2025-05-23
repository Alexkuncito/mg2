#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

layout(location = 0) uniform vec3 luzPos;
layout(location = 1) uniform vec3 luzColor;
uniform vec3 viewPos;

uniform sampler2D textura;
uniform vec4 color;

uniform bool boolCOLOR;
uniform bool boolTEXTURE;
uniform bool boolMATERIAL;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float opacity;
};

uniform Material material;

const Material defaultMaterial = Material(
    vec3(0.2, 0.2, 0.2),
    vec3(1.0, 1.0, 1.0),
    vec3(0.5, 0.5, 0.5),
    32.0,
    1.0
);

void main() {
    vec3 colTex = vec3(0.0);
    vec3 colClr = vec3(0.0);
    vec3 colMat = vec3(0.0); // <-- usado como color base de material
    float alphaTex = 1.0;
    float alphaClr = 1.0;
    float alphaMat = 1.0;

    int activeSources = 0;

    // TEXTURA
    if (boolTEXTURE) {
        vec4 texColor = texture(textura, TexCoord);
        if (texColor.a < 0.1) discard;
        colTex = texColor.rgb;
        alphaTex = texColor.a;
        activeSources++;
    }

    // COLOR
    if (boolCOLOR) {
        colClr = color.rgb;
        alphaClr = color.a;
        activeSources++;
    }

    // MATERIAL
    Material mat = material;
    if (!boolMATERIAL || mat.ambient == vec3(0.0)) {
        mat = defaultMaterial;
    } else {
        colMat = clamp(mat.diffuse * 3.0, vec3(0.0), vec3(1.0));
        alphaMat = mat.opacity;
        activeSources++;
    }

    // Selección del color base
    vec3 baseColor = vec3(1.0);
    if (boolTEXTURE) baseColor = colTex;
    if (boolMATERIAL) baseColor = colMat;
    if (boolCOLOR) baseColor *= colClr;

    // Iluminación Phong
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(luzPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 ambient = mat.ambient * luzColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = mat.diffuse * diff * luzColor;

    float spec = 0.0;
    if (diff > 0.0)
        spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = mat.specular * spec * luzColor;

    vec3 lighting = ambient + diffuse + specular;

    vec3 finalColor = baseColor * lighting;

    // Alpha final
    float finalAlpha = 1.0;
    if (boolTEXTURE && boolCOLOR)
        finalAlpha = min(alphaTex, alphaClr);
    else if (boolMATERIAL && boolCOLOR)
        finalAlpha = min(alphaMat, alphaClr);
    else if (boolTEXTURE)
        finalAlpha = alphaTex;
    else if (boolMATERIAL)
        finalAlpha = alphaMat;
    else if (boolCOLOR)
        finalAlpha = alphaClr;

    FragColor = vec4(finalColor, finalAlpha);
}
