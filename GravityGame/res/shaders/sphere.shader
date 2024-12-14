#shader vertex
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 viewProjection;  

out vec2 v_texCoord;

void main(){
    v_texCoord = texCoord;
    gl_Position = viewProjection * model * vec4(position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 outColor;

in vec2 v_texCoord;
uniform sampler2D u_Texture;

uniform vec3 ballCenter;

void main(void) {
    outColor = texture(u_Texture,v_texCoord);
    //outColor = vec4(1.0);
}