#version 410

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

uniform int divisions = 1;
uniform int frame = 0;

void main()
{   
    float frameWidth = 1.0 / float(divisions);
    vec2 frameCoords = vec2(TexCoords.x * frameWidth + (frame * frameWidth), TexCoords.y);

    color = vec4(spriteColor, 1.0) * texture(image, frameCoords);
}  