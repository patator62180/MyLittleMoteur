#version 330 core
out vec4 FragColor;
  
uniform float ourColor;

void main()
{
    FragColor = vec4(1.0,1.0,ourColor,1.0);
} 