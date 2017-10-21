#version 450

#define PI 3.1415926535897932384626433832795


layout (set = 0, binding = 1) uniform sampler2D imageAlbedo;
layout (set = 0, binding = 2) uniform sampler2D imageNormals;
layout (set = 0, binding = 3) uniform sampler2D imageRoughness;
// layout (set = 0, binding = 4) uniform sampler2D imageOcclusion;


layout (location = 0) in vec3 fPos;
layout (location = 1) in vec2 fTex;
layout (location = 2) in mat3 fTBN;


layout (location = 0) out vec4 oColor;


float Fresnel(vec3 normal, vec3 view, float roughness);
float GeometricAttenuation(vec3 normal, vec3 view, vec3 light);
float Distribution(vec3 normal, vec3 view, vec3 light, float roughness);
float Diffuse(vec3 normal, vec3 light, vec3 view, float roughness, float ambient);
float Specular(vec3 normal, vec3 light, vec3 view, float roughness);


void main() {
	// oColor = vec4(fTex, 0.0f, 1.0f);
    
    vec4    dataAlbedo     = texture(imageAlbedo, fTex);
    vec4    dataNormals    = texture(imageNormals, fTex);
    vec4    dataRoughnes   = texture(imageRoughness, fTex);
    // vec4    dataOcclusion  = texture(imageOcclusion, fTex);
    
    vec3    albedo = dataAlbedo.xyz;
    vec3    normal = normalize(
        fTBN[0] * (dataNormals.x * 2.0f - 1.0f) +
        fTBN[1] * (dataNormals.y * 2.0f - 1.0f) +
        fTBN[2] * (dataNormals.z * 2.0f - 1.0f)
    );
    // vec3    normal = normalize(fTBN * (dataNormals.xyz * 2.0f - 1.0f));
    // vec3    normal = fTBN[2];
    float   height = dataNormals.w;
    float   roughness = dataRoughnes.x;
    float   occlusion = 1.0f; // dataOcclusion.x;
    float   metalness = 0.0f;
    float   ambient = 0.2f;
    
    vec3    lightDirection = normalize(vec3(-1.0f, -2.0f, +1.0f));
    // vec3    lightDirection = normalize(vec3(0.0f, -1.0f, -1.0f));
    vec3    light = -lightDirection;
    
    // float   diffuseIntensity = clamp(dot(-lightDirection, normal), 0.0f, 1.0f);
    
    vec3    view = normalize(-fPos);
    
	// float	specularIntensity = pow(max(dot(reflect(lightDirection, normal), view), 0.0f), 128.0f);

	float	diffuseIntensity = Diffuse(normal, light, view, roughness, ambient * occlusion);
	float	specularIntensity = Specular(normal, light, view, roughness);
	// float	diffuseIntensity = Diffuse(normal, light, view, roughness, lightAmbient);
	// float	specularIntensity = Specular(normal, light, view, roughness);

	float	gloss = 1.0f - roughness;
	
	vec4 oDiffuse = vec4((1.0f - gloss) * diffuseIntensity * albedo * vec3(1.0f), 1.0f);
	vec4 oSpecular = vec4(gloss * specularIntensity * mix(vec3(1.0f), albedo, metalness) * vec3(1.0f), 1.0f);
    
    oColor = vec4(oDiffuse.xyz + oSpecular.xyz, 1.0f);
    // oColor = vec4(albedo * (ambient * occlusion + (1.0f - ambient) * diffuseIntensity) + vec3(specularIntensity), 1.0f);
    // oColor = vec4(vec3(roughness), 1.0f);
}

float Fresnel(vec3 normal, vec3 view, float roughness) {
	float	r = roughness + 0.01f;
	
	float	A = 1.0f + 5.0f * r; // 5.0f
	float	B = (1.0f - pow(r, 2.0f)) / 3.0f;
	
	float	fresnel = 1.0f / pow(1.0f + pow(dot(view, normal), 2.0), A);
	float	schlick = pow(1.0f - pow(dot(view, normal), 2.0), A) * (1.0f - B) + B;
	
	return	fresnel;
}
float GeometricAttenuation(vec3 normal, vec3 view, vec3 light) {
	vec3	halfVector = normalize(view + light);
	
	float	implicit = dot(normal, light) * dot(normal, view);
	float	neumann = (dot(normal, light) * dot(normal, view)) / max(dot(normal, light), dot(normal, view));
	float	cookTorrance = min(	1.0f, min(
								(2.0f * dot(normal, halfVector) * dot(normal, view)) / dot(view, halfVector),
								(2.0f * dot(normal, halfVector) * dot(normal, light)) / dot(view, halfVector)));
	float	kelemen = (dot(normal, light) * dot(normal, view)) / pow(dot(view, halfVector), 2.0f);
	
	return	kelemen;
}
float Distribution(vec3 normal, vec3 view, vec3 light, float roughness) {
	float	r = roughness + 0.01f;
	vec3	halfVector = normalize(view + light);
	float	r2 = pow(r, 2.0f);
	
	float	blinnPhong = (1.0f / (PI * r2)) * pow(dot(normal, halfVector), (2.0f / r2) - 2.0f);
	float	beckmann = (1.0f / (PI * r2 * pow(dot(normal, halfVector), 4.0f))) * exp((pow(dot(normal, halfVector), 2.0f) - 1.0f) / (r2 * pow(dot(normal, halfVector), 2.0f)));
	float 	trowbridgeReitz = r2 / (PI * pow(pow(dot(normal, halfVector), 2.0f) * (r2 - 1.0f) + 1.0f, 2.0f));
	
	return	beckmann;
}
float Diffuse(vec3 normal, vec3 light, vec3 view, float roughness, float ambient) {
	float r = pow(roughness, 2.0f);
	float A = 1.0f - 0.5f * (r / (r + 0.33f));
	float B = 0.45f * (r / (r + 0.09f));
	float a = max( dot(normal, light), dot(normal, view) );
	float b = min( dot(normal, light), dot(normal, view) );
	float fL = acos(dot(normal, light));
	float fV = acos(dot(normal, view));
	
	float lambert = max(dot(normal, light), ambient);
	float orenNayar = ambient + (1.0f - ambient) * max(dot(normal, light) * (A + B * max(0.0f, cos(fV - fL)) * sin(a) * tan(b)), 0.0f);
	
	return lambert;
}
float Specular(vec3 normal, vec3 light, vec3 view, float roughness) { // +light -view
	float	distribution = Distribution(normal, view, light, roughness);
	float	fresnel = Fresnel(normal, view, roughness);
	float	geometricAttenuation = GeometricAttenuation(normal, view, light);

	
	float p = 1.01f / pow(roughness + 0.01f, 2.0f);
	
	float	phong = pow(max(dot(reflect(-light, normal), view), 0.0f), p) * fresnel;
	float	blinn = pow(max(dot(normal, normalize(light + view)), 0.0f), p); // l+v / |l+v|
	float	cookTorrance = max(
		pow(max(dot(light, normal) ,0.0f), 2.0f) * // just to cover artifacts of high roughness
		(distribution * fresnel * geometricAttenuation) /
		(dot(normal, view) * dot(normal, light)),
		0.0f);
	
	return	cookTorrance;
}