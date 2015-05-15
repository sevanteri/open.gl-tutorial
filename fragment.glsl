#version 130

#define M_PI 3.1415926535897932384626433832795

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texKitty;

uniform float time;

void main()
{
    vec4 colKitty = texture(texKitty, Texcoord);

    float waterSurface = 0.72 + sin(time)*0.005;
    float surfaceToBottom = waterSurface - Texcoord.y;
    float waveLength = 60;
    if (Texcoord.y <= waterSurface) {
        outColor = colKitty;
    } else {
        vec2 waterEffect = vec2(
            Texcoord.x + (
                sin(Texcoord.y*2 * waveLength + time*5) / (waveLength/2)
            ) * surfaceToBottom,

            waterSurface*2.0 - Texcoord.y
        );
        vec4 color = vec4(Color, 1.0);
        outColor = texture(texKitty, waterEffect) * color;
    }
}
