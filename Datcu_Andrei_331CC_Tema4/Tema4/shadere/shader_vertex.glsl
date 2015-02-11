#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

layout(location = 3) in vec3 next_position;
layout(location = 4) in vec3 next_normal;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float current_time;
uniform float total_time;

out vec3 world_pos;
out vec3 world_normal;
out vec2 texcoord;

void main(){

	float t = min(1, current_time / total_time);

	vec3 actual_pos = in_position * (1 - t) + next_position * t;
	vec3 actual_normal = in_normal * (1 - t) + next_normal * t;

	world_pos = vec3(model_matrix * vec4(actual_pos, 1)); // pozitia in coordonate word
	world_normal = normalize(mat3(model_matrix) * actual_normal);
	texcoord = in_texcoord;

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(actual_pos,1); 
}
