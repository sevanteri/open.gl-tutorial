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
    //vec4 colPuppy = texture(texPuppy, Texcoord);
    float a = 0.6;
    float b = a - Texcoord.y;
    float c = Texcoord.y * 50;
    if (Texcoord.y <= a) {
        outColor = colKitty;
    } else {
        outColor = texture(texKitty,
            vec2(
                //sin(time) * Texcoord.x,
                Texcoord.x + (sin(Texcoord.y * c + time/2) / (c/2)) * b,
                a*2.0 - Texcoord.y
            )
        );
    }
    //outColor = mix(colKitty, colPuppy, ((sin(time / 2) + 1.0) / 2.0));
}
