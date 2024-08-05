#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform float time;
uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

vec2 rotate(vec2 v, float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, s, -s, c);
	return m * v;
}

void main()
{
	vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(light.position - FragPos);
	vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * max(dot(lightDirection, norm), 0.0) * light.diffuse;
 
	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specular = texture(material.specular, TexCoord).xyz * spec * light.specular;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}