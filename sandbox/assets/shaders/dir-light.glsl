###vertex

#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUv;

layout(location = 0) out vec3 color;
layout(location = 1) out vec4 ws_pos;
layout(location = 2) out vec3 ws_norm;

struct pLight {
	vec4 color;
	vec4 pos;
};

layout(push_constant) uniform Push {
	mat4 model;
	mat4 norm;
} push;

layout(set = 0, binding = 0) uniform  GlobalUbo{
	mat4 proj;
	mat4 view;
	mat4 invView;
	mat4 projView;

	vec4 globalLightDir;
	vec4 globalLightColor;
	vec4 ambient;

	pLight lights[100];
} ubo;

layout(set = 1, binding = 0) uniform  ObjectUbo{
	vec4 color;
} mat;

void main() {
	// converting to world space
	ws_pos = push.model * vec4(inPosition, 1.0);
	ws_norm = normalize(mat3(push.norm) * inNormal);
	color = mat.color.xyz;

	gl_Position = ubo.projView * ws_pos;
}

###fragment

#version 450

layout(location = 0) in vec3 color;
layout(location = 1) in vec4 ws_pos;
layout(location = 2) in vec3 ws_norm;

layout(location = 0) out vec4 out_color;

const int MAX_POINT_LIGHT = 100;

struct PointLight {
	vec4 color;
	vec4 pos;
};

layout(push_constant) uniform Push {
	mat4 model;
	mat4 norm;
} push;

layout(set = 0, binding = 0) uniform  GlobalUbo{
	mat4 proj;
	mat4 view;
	mat4 invView;
	mat4 projView;

	vec4 globalLightDir;
	vec4 globalLightColor;
	vec4 ambient;

	PointLight lights[MAX_POINT_LIGHT];
} ubo;

layout(set = 1, binding = 0) uniform  ObjectUbo{
	vec4 color;
} mat;

void main() {

	const vec3 ws_NORM = normalize(ws_norm);
	const vec3 g_LIGHT_DIR = normalize(ubo.globalLightDir.xyz);
	const vec3 g_LIGHT_COLOR = ubo.globalLightColor.xyz * ubo.globalLightColor.w;
	const vec3 ws_CAM_POS = ubo.invView[3].xyz;
	const vec3 VIEW_DIR = normalize(ws_CAM_POS - ws_pos.xyz);

	vec3 ambient_light = ubo.ambient.xyz * ubo.ambient.w;
	vec3 diffuse_light = g_LIGHT_COLOR * max(dot(ws_NORM, g_LIGHT_DIR), 0);
	vec3 specular_light = vec3(0.0);

	for(int i = 0; i < MAX_POINT_LIGHT; i++) {
		vec4 plight_pos = ubo.lights[i].pos;
		vec3 plight_color = ubo.lights[i].color.xyz * ubo.lights[i].color.w;

		vec3 light_dir = plight_pos.xyz - ws_pos.xyz;
		float attenuation = 1.0 / dot(light_dir, light_dir);
		light_dir = normalize(light_dir);
		vec3 light_intensity = attenuation * plight_color;

		// diffuse lighting
		float cos_angle_inc = max(dot(ws_NORM, normalize(light_dir)), 0);
		diffuse_light += light_intensity * cos_angle_inc;

		// specular lighting
		vec3 half_angle = normalize(light_dir + VIEW_DIR);
		float blinn_term = dot(ws_NORM, half_angle);
		blinn_term = clamp(blinn_term, 0, 1);
		blinn_term = pow(blinn_term, 512);
		specular_light += light_intensity * blinn_term;
	}

	vec3 frag_color = ambient_light * color + diffuse_light * color + specular_light * color;
	out_color = vec4(frag_color, 1);
}