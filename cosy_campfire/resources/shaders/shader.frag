#version 330 core

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	sampler2D emission1;
	sampler2D specular1;
	sampler2D diffuse1;
	float shininess; // not really used atm
};

uniform DirectionalLight u_dir_light;
uniform Material u_material;
uniform vec3 u_camera_position;

in vec3 p_world_position;
in vec2 p_tex_coords;
in vec3 p_normal;

out vec4 p_frag_color;

vec3 shading() {
	vec4 diffuse_tex     = texture(u_material.diffuse1, p_tex_coords);
	vec3 normal          = normalize(p_normal);
	vec3 light_direction = normalize(-u_dir_light.direction);

	vec3 ambient  = u_dir_light.ambient * vec3(diffuse_tex);

	float d       = max(dot(normal, light_direction), 0.0);
	vec3 diffuse  =  d * vec3(diffuse_tex) * u_dir_light.diffuse;

	vec3 light_reflect_direction = reflect(-light_direction, normal);  
	vec3 view_direction = normalize(u_camera_position - p_world_position);
	float s = pow(max(dot(view_direction, light_reflect_direction), 0.0), 32.0);
	vec3 specular = s * vec3(texture(u_material.specular1, p_tex_coords)) * u_dir_light.specular;

	vec3 emission = texture(u_material.emission1, p_tex_coords).rgb;

	return ambient + diffuse + specular + emission;
}

void main() {    
    p_frag_color = vec4(shading(), 1.0);
}
