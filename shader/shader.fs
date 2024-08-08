#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
	bool enabled;
	vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
	bool enabled;
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float attenuationLinear;
	float attenuationQuad;
}; 

struct SpotLight {
	bool enabled;
    vec3 position;
	vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float cutOff;
	float blend;
}; 

out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform float time;
uniform vec3 viewPosition;
uniform Material material;

#define NB_LIGHTS 4
uniform DirLight dirLights[NB_LIGHTS];
uniform PointLight pointLights[NB_LIGHTS];
uniform SpotLight spotLights[NB_LIGHTS];



vec4 computeDirectionnalLight(int index)
{
	DirLight light = dirLights[index];
	if(!light.enabled)
	{
		return vec4(0.0);
	}
	vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(-light.direction);
	vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * max(dot(lightDirection, norm), 0.0) * light.diffuse;
 
	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specular = texture(material.specular, TexCoord).xyz * spec * light.specular;

	return vec4(ambient + diffuse + specular, 1.0);
}

vec4 computePointLight(int index)
{	
	PointLight light = pointLights[index];
	if(!light.enabled)
	{
		return vec4(0.0);
	}
	vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * light.ambient;
	
	vec3 lightDirection = normalize(light.position - FragPos);
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDirection),0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoord)) * light.diffuse;
 
	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specular = texture(material.specular, TexCoord).xyz * spec * light.specular;
	
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / ( 1.0 + light.attenuationLinear * distance + light.attenuationQuad * distance * distance);

	return vec4(attenuation * (ambient + diffuse + specular), 1.0);
}

vec4 computeSpotLight(int index)
{	
	SpotLight light = spotLights[index];
	if(!light.enabled)
	{
		return vec4(0.0);
	}
	vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * light.ambient;
	
	vec3 lightDirection = normalize(light.position - FragPos);
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDirection),0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoord)) * light.diffuse;
 
	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specular = texture(material.specular, TexCoord).xyz * spec * light.specular;

	float cosTheta = dot(normalize(-light.direction), lightDirection);
	float cosPhi = cos(light.cutOff);

	if(cosTheta < cosPhi)
	{
		return vec4(ambient, 1.0);
	}

	float blend = (cosTheta - cosPhi) / (cos((1.0-light.blend) * light.cutOff) - cosPhi);
	blend = clamp(blend, 0.0, 1.0);

	return vec4(blend * (diffuse + specular) + ambient, 1.0);
}

void main()
{
	FragColor = vec4(0.0);
	for(int i = 0; i < NB_LIGHTS ; i++)
	{		
		FragColor += computeDirectionnalLight(i);
		FragColor += computePointLight(i);
		FragColor += computeSpotLight(i);
	}
}