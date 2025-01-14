#version 410

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

uniform int xDivisions = 1;
uniform int yDivisions = 1;

uniform int frame = 0;

void main()
{   
    vec2 frameSize = vec2(1.0) / vec2(xDivisions, yDivisions);
    
    int row = frame % xDivisions;
    int column = frame / xDivisions;

    vec2 frameOffset = vec2(row, column) * frameSize;
    vec2 frameCoords = TexCoords * frameSize + frameOffset;

    color = vec4(spriteColor, 1.0) * texture(image, frameCoords);
}  