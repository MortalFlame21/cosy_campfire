#version 330 core

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3  position;
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	float constant;
	float linear;
	float quadratic;
};

struct Material {
	sampler2D emission1;
	sampler2D specular1;
	sampler2D diffuse1;
	float shininess; // not really used atm
};

const int g_MAX_POINT_LIGHTS = 4;

uniform DirectionalLight u_directional_light;
uniform PointLight u_point_lights[g_MAX_POINT_LIGHTS];
uniform Material u_material;
uniform vec3 u_camera_position;

in vec3 p_world_position;
in vec2 p_tex_coords;
in vec3 p_normal;

out vec4 p_frag_color;

vec3  directionalLight(Material material, DirectionalLight light);
vec3  pointLight(Material material, PointLight light);
vec3  ambient(Material material, vec3 light_ambient);
vec3  diffuse(Material material, vec3 normal, vec3 light_diffuse, vec3 light_direction);
vec3  specular(Material material, vec3 normal, vec3 light_specular, vec3 light_direction);
float attenuation(float constant, float linear, float quadratic, float distance);

void main() {    
	vec3 result = directionalLight(u_material, u_directional_light);
	for (int i = 0; i < g_MAX_POINT_LIGHTS; ++i)
		result += pointLight(u_material, u_point_lights[i]);
	result += texture(u_material.emission1, p_tex_coords).rgb;
	p_frag_color = vec4(result, 1.0);
}

vec3 directionalLight(Material material, DirectionalLight light) {
	vec3 normal          = normalize(p_normal);
	vec3 light_direction = normalize(-light.direction); // light is pointing from surface to light-source

	vec3 ambient  = ambient(material, light.ambient);
	vec3 diffuse  = diffuse(material, normal, light.diffuse, light_direction);
	vec3 specular = specular(material, normal, light.specular, light_direction);

	return ambient + diffuse + specular;
}

vec3 pointLight(Material material, PointLight light) {
	vec3 normal          = normalize(p_normal);
	vec3 light_direction = normalize(light.position - p_world_position);
	float distance       = length(light.position - p_world_position);

	vec3 ambient      = ambient(material, light.ambient);
	vec3 diffuse      = diffuse(material, normal, light.diffuse, light_direction);
	vec3 specular     = specular(material, normal, light.specular, light_direction);
	float attenuation = attenuation(light.constant, light.linear, light.quadratic, distance);

	return (ambient + diffuse + specular) * attenuation;
}

vec3 ambient(Material material, vec3 light_ambient) {
	return light_ambient * vec3(texture(material.diffuse1, p_tex_coords));
}

vec3 diffuse(Material material, vec3 normal, vec3 light_diffuse, vec3 light_direction) {
	float d = max(dot(normal, light_direction), 0.0);
	return d * vec3(texture(u_material.diffuse1, p_tex_coords)) * light_diffuse;
}

vec3 specular(Material material, vec3 normal, vec3 light_specular, vec3 light_direction) {
	vec3 light_reflect_direction = reflect(-light_direction, normal);  
	vec3 view_direction = normalize(u_camera_position - p_world_position);
	float s = pow(max(dot(view_direction, light_reflect_direction), 0.0), 32.0);
	return s * vec3(texture(material.specular1, p_tex_coords)) * light_specular;
}

float attenuation(float constant, float linear, float quadratic, float distance) {
	return 1.f / (constant + (linear * distance) + (quadratic * distance * distance));
}
