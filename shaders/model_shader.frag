#version 330 core
out vec4 FragColor;
        
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
        
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseMap;
    bool hasTexture;
};
        
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
        
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
        
void main() {
    vec3 color = material.diffuse;
            
    if (material.hasTexture) {
        color = texture(material.diffuseMap, TexCoord).rgb;
    }

    vec3 ambient = light.ambient * material.ambient * color;         
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;
            
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}