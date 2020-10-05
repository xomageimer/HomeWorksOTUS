#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

out vec4 colorFrag;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(aPos, 1.0f);
    colorFrag = aCol;
}
