#version 130

in float Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texKitty;
uniform sampler2D texPuppy;

uniform float time;

void main()
{
    vec4 colKitty = texture(texKitty, Texcoord);
    vec4 colPuppy = texture(texPuppy, Texcoord);
    outColor = mix(colKitty, colPuppy, ((sin(time / 2) + 1.0) / 2.0));
}
