#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec2 TexCoord;
in vec3 Normal;

#define MAX_DIRECTIONAL_LIGHTS 8
#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 4
#define MAX_TEXTURES 16

uniform vec3 viewPos;

uniform int num_directional_lights;
uniform int num_point_lights;
uniform int num_spot_lights;

struct Directional_Light {
	vec3 direction;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Directional_Light directional_lights[MAX_DIRECTIONAL_LIGHTS];  

struct Point_Light {
    vec3 position;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float intensity;
	float constant;
    float linear;
    float quadratic;
};
uniform Point_Light point_lights[MAX_POINT_LIGHTS];  

struct Spot_Light {
    vec3 position;

	vec3 direction;
    float cutOff;
	float outerCutOff;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float intensity;
	float constant;
    float linear;
    float quadratic;
};
uniform Spot_Light spot_lights[MAX_SPOT_LIGHTS];  

// textures[0] = material diffuse
// textures[1] = material specular
uniform sampler2D textures[MAX_TEXTURES];

uniform float shininess;
uniform float transparency;
uniform int light_mode;

vec3 calculate_directional_light(Directional_Light light, vec3 norm, vec3 view_direction);
vec3 calculate_point_light(Point_Light light, vec3 normal, vec3 view_direction, vec3 fragment_position);
vec3 calculate_spot_light(Spot_Light light, vec3 normal, vec3 view_direction, vec3 fragment_position);

void main()
{
	vec3 norm = normalize(Normal);
    vec3 view_direction = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0);

	// sum directional lights
	for (int i = 0; i < num_directional_lights; ++i) 
	{
		result += calculate_directional_light(directional_lights[i], norm, view_direction);
	}

	// sum point lights
	for (int i = 0; i < num_point_lights; ++i) 
	{
		result += calculate_point_light(point_lights[i], norm, view_direction, FragPos);
	}

	// sum spot lights
	vec3 spot_light_color = vec3(0.0);
	for (int i = 0; i < num_spot_lights; ++i) 
	{
		// result += calculate_spot_light(spot_lights[i], norm, view_direction, FragPos);
		spot_light_color += calculate_spot_light(spot_lights[i], norm, view_direction, FragPos);
	}

	result += spot_light_color;

	if (light_mode == 0) {
		// standard frag color
		FragColor = vec4(result, clamp(transparency, 0.0, 1.0));
	} else {
		// ghost shader
		FragColor = vec4(result, clamp((spot_light_color.x + spot_light_color.y + spot_light_color.z) * 35 / 120 - (1.0 - transparency) / 3 - 0.1, 0.0, 1.0));
	}
}


vec3 calculate_directional_light(Directional_Light light, vec3 normal, vec3 view_direction) 
{
    vec3 light_direction = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    // combine results
	vec3 material_diffuse_texture = vec3(texture(textures[0], TexCoord));
	vec3 material_specular_texture = vec3(texture(textures[1], TexCoord));
    vec3 ambient  = light.ambient  * material_diffuse_texture;
    vec3 diffuse  = light.diffuse  * diff * material_diffuse_texture;
    vec3 specular = light.specular * spec * material_specular_texture;
    return (ambient + diffuse + specular);
}

vec3 calculate_point_light(Point_Light light, vec3 normal, vec3 view_direction, vec3 fragment_position) 
{
    vec3 light_direction = normalize(light.position - fragment_position);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    // attenuation
    float distance    = length(light.position - fragment_position);
    float attenuation = light.intensity / (light.constant + light.linear * distance + 
  										   light.quadratic * (distance * distance));    
    // combine results
	vec3 material_diffuse_texture = vec3(texture(textures[0], TexCoord));
	vec3 material_specular_texture = vec3(texture(textures[1], TexCoord));
    vec3 ambient  = light.ambient  * material_diffuse_texture;
    vec3 diffuse  = light.diffuse  * diff * material_diffuse_texture;
    vec3 specular = light.specular * spec * material_specular_texture;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calculate_spot_light(Spot_Light light, vec3 normal, vec3 view_direction, vec3 fragment_position) 
{
    vec3 light_direction = normalize(light.position - fragment_position);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    // attenuation
    float distance    = length(light.position - fragment_position);
    float attenuation = light.intensity / (light.constant + light.linear * distance + 
  										   light.quadratic * (distance * distance));
	// spotlight shading
	float theta = dot(light_direction, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float spotlight_intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// combine results
	vec3 material_diffuse_texture = vec3(texture(textures[0], TexCoord));
	vec3 material_specular_texture = vec3(texture(textures[1], TexCoord));
	vec3 ambient  = light.ambient  * material_diffuse_texture;
    vec3 diffuse  = light.diffuse  * diff * material_diffuse_texture;
    vec3 specular = light.specular * spec * material_specular_texture;
	ambient  *= attenuation;
    diffuse  *= attenuation * spotlight_intensity;
    specular *= attenuation * spotlight_intensity;
	return (ambient + diffuse + specular);  
}