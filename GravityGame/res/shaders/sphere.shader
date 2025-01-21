#shader vertex
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 viewProjection;  

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 fragPos;

void main(){
    gl_Position = viewProjection * model * vec4(position, 1.0);
    v_texCoord = texCoord;
    v_normal = normal;
    fragPos = vec3(model * vec4(position, 1.0));    
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 outColor;

in vec3 v_normal;
in vec2 v_texCoord;
in vec3 fragPos;
uniform sampler2D u_Texture;

uniform vec3 lightColor;
uniform vec3 lightPos;  

void main(void) {
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;
        
    //diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(lightPos - fragPos);     
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    outColor = texture(u_Texture,v_texCoord);
    //outColor = vec4(1.0);
    vec3 result = (ambient + diffuse);
    outColor = outColor * vec4(result,1.0);
}