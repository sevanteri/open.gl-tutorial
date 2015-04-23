#version 130

#define M_PI 3.1415926535897932384626433832795

in float Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texKitty;
/*uniform sampler2D texPuppy;*/

uniform float time;

void main()
{
    vec4 colKitty = texture(texKitty, Texcoord);
    /*vec4 colPuppy = texture(texPuppy, Texcoord);*/

    float waterSurface = 0.72 + sin(time/1000)*0.005;
    float surfaceToBottom = waterSurface - Texcoord.y;
    float waveLength = 60;
    if (Texcoord.y <= waterSurface) {
        /*outColor = mix(colKitty, colPuppy, 0.5);*/
        outColor = colKitty;
    } else {
        vec2 waterEffect = vec2(
            Texcoord.x + (
                sin(Texcoord.y*2 * waveLength + time/200) / (waveLength/2)
            ) * surfaceToBottom,

            waterSurface*2.0 - Texcoord.y
        );
        /*outColor = mix(*/
            /*texture(texPuppy, waterEffect),*/
            /*texture(texKitty, waterEffect),*/
            /*0.5*/
        /*);*/
        outColor = texture(texKitty, waterEffect);
    }
}
