#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

uniform sampler2D texture1;
uniform sampler2D texture2;
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
    //FragColor =  mix(texture(texture1, TexCoord), texture(texture2, TexCoord), abs(sin(time)));
	//FragColor = texture(texture1, TexCoord);

	vec3 ambient = material.ambient * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(light.position - FragPos);
	vec3 diffuse = material.diffuse * max(dot(lightDirection, norm), 0.0) * light.diffuse;
 
	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}