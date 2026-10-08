#version 330 core

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

out vec3 p_world_position;
out vec2 p_tex_coords;
out vec3 p_normal;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);

	p_world_position = vec3(u_model * vec4(position, 1.0));
    p_tex_coords = tex_coords;    
    p_normal = normal;
}
