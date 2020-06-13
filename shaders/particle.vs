#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;
uniform float frame;
uniform float numRow;
uniform float numCol;

void main()
{
    float scale = 25.0f;
    TexCoords = vec2((vertex.z+frame)/numRow,vertex.w/numCol);

    ParticleColor = color;
    vec2 adj_xy = vec2(vertex.x/numRow,vertex.y/numCol);
    gl_Position = projection * vec4((adj_xy * scale) + offset, 0.0, 1.0);
}
