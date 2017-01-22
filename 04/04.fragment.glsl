#version 330 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    //color = texture(ourTexture, texCoord);
    //color = texture(ourTexture, texCoord) * vec4(ourColor, 1.0f);
    color = mix(
        texture(ourTexture1, texCoord),
        texture(ourTexture2, texCoord),
        0.3                                 // 0.0 - arg[0], 1.0 - arg[1]
    );
}
