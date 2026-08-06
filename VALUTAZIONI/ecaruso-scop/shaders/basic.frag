#version 330 core

in vec3 vertexColor;
in vec3 Normal;
in vec2 texCoord;

out vec4 FragColor;

uniform bool useLighting;
uniform sampler2D texture1;
uniform float textureBlend;

void main()
{
    vec4 color;

    if(useLighting)
    {
        vec3 lightDir = normalize(vec3(0.4, 0.6, 1.0));
        float diffuse = max(dot(normalize(Normal), lightDir), 0.0);
        float gray = 0.35 + diffuse * 0.45;

        color = vec4(gray, gray, gray, 1.0);
    }
    else
    {
        color = vec4(vertexColor, 1.0);
    }

    vec4 textureColor = texture(texture1, texCoord);

    FragColor = mix(color, textureColor, textureBlend);
}