#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform float frame;
void main()
{
    TexCoords = vec2((vertex.z+frame)/8.0,vertex.w);

    gl_Position = projection * model * vec4(vertex.x/8.0,vertex.y, 0.0, 1.0);
}