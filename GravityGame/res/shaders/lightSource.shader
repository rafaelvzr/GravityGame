#shader vertex
#version 450 core
layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 viewProjection;  

void main(){
    gl_Position = viewProjection * model * vec4(position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 outColor;
uniform vec3 lightColor;

void main(void) {
    outColor = vec4(lightColor,1.0);
}