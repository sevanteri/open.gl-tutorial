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

    float waterSurface = 0.72 + sin(time/18)*0.005;
    float surfaceToBottom = waterSurface - Texcoord.y;
    float waveLength = Texcoord.y * 60;

    if (Texcoord.y <= waterSurface) {
        outColor = mix(colKitty, colPuppy, 0.5);
    } else {
        vec2 asd = vec2(
            Texcoord.x + (
                sin(Texcoord.y * waveLength + time/2) / (waveLength/2)
            ) * surfaceToBottom,

            waterSurface*2.0 - Texcoord.y
        );
        outColor = mix(texture(texPuppy, asd), texture(texKitty, asd), 0.5);
    }
    //outColor = mix(colKitty, colPuppy, ((sin(time / 2) + 1.0) / 2.0));
}
