#shader vertex
#version 450 core
layout(location = 0) in vec3 position;

uniform mat4 viewProjection;  

void main(){
    gl_Position = viewProjection * vec4(position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 outColor;


void main(void) {
    outColor = vec4(1.0);
}