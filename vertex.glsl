#version 130

in vec2 position;
in float color;
in vec2 texcoord;

out float Color;
out vec2 Texcoord;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

void main()
{
    Color = color;
    Texcoord = texcoord;
    gl_Position = proj * view * model * vec4(position, 0.0, 1.0);
}
