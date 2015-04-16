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
    float a = 0.75 + sin(time/15)*0.005;
    float aToBottom = a - Texcoord.y;
    float c = Texcoord.y * 60;
    if (Texcoord.y <= a) {
        outColor = colKitty;
    } else {
        vec2 asd = vec2(
            //sin(time) * Texcoord.x,
            Texcoord.x + (sin(Texcoord.y * c + time/2) / (c/2)) * aToBottom,
            a*2.0 - Texcoord.y
        );
        outColor = texture(texKitty, asd);
    }
    //outColor = mix(colKitty, colPuppy, ((sin(time / 2) + 1.0) / 2.0));
}
