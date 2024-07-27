#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;
uniform vec3 color;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

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

	vec3 ambient = 0.1 * lightColor;

	vec3 lightDirection = normalize(lightPosition - FragPos);
	vec3 diffuse = max(dot(lightDirection, Normal), 0.0) * lightColor;
 
	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, Normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = 0.5 * spec * lightColor;  

	FragColor = vec4((ambient + diffuse + specular)*color, 1.0);
}