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
    float a = 0.4;
    if (Texcoord.y <= a) {
        outColor = colKitty;
    } else {
        outColor = texture(texKitty,
            vec2(
                //sin(time) * Texcoord.x,
                Texcoord.x + (sin(Texcoord.y * 60.0 + time/2) / 30.0) * (a - Texcoord.y),
                a*2.0 - Texcoord.y
            )
        );
    }
    //outColor = mix(colKitty, colPuppy, ((sin(time / 2) + 1.0) / 2.0));
}
