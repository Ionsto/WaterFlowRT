#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main()
{
    FragColor = mix(texture(Texture0, TexCoord),texture(Texture1, TexCoord),texture(Texture2, TexCoord));
    FragColor = texture(Texture0, TexCoord);
}
