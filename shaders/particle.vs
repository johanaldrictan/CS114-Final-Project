#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;
uniform float frameW;
uniform float frameH;
uniform float numRow;
uniform float numCol;

void main()
{
    float scale = 200.0f;
    //TexCoords = vec2((vertex.z+frameH)/numRow,(vertex.w+frameW)/numCol);
    TexCoords = vec2((vertex.z+frameW)/numCol,(vertex.w+frameH)/numRow);
    
    ParticleColor = color;
    //vec2 adj_xy = vec2(vertex.x/numRow,vertex.y/numCol);
    vec2 adj_xy = vec2(vertex.x/numCol,vertex.y/numRow);
    gl_Position = projection * vec4((adj_xy * scale) + offset, 0.0, 1.0);
}
