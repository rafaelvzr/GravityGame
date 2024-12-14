#shader vertex
#version 450 core
layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;    


void main(){
    gl_Position = proj * view * model * vec4(position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 outColor;

uniform vec3 ballCenter;
uniform vec2 ballCenterScreen; // Screen-space center of the sphere

void main(void) {
    float d = length(gl_FragCoord.xy - ballCenterScreen);
    if (d > 110.0) {
        discard;
    } else {
        outColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
}