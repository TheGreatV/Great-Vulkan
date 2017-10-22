#version 450

#define PI 3.1415926535897932384626433832795


layout (set = 0, binding = 1) uniform sampler2D imageAlbedo;
layout (set = 0, binding = 2) uniform sampler2D imageNormals;
layout (set = 0, binding = 3) uniform sampler2D imageRoughness;
layout (set = 0, binding = 4) uniform sampler2D imageMetalness;
layout (set = 0, binding = 5) uniform sampler2D imageOcclusion;
layout (set = 0, binding = 6) uniform samplerCube imageEnvironment;


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
    vec3    view = normalize(-fPos);
    vec3    tangent = normalize(fTBN[0]);
    vec3    binormal = normalize(fTBN[1]);
    
	float	stepsCount = 32.0f;
	float	stepFactor = 1.0f / stepsCount;
	float	emboss = 0.08f;
	float	scale = 1.0f;
	float	embossScaleFactor = emboss / scale; // 0.1f;
    
	vec2	scanDirectionOnMap = vec2( dot(tangent, -view) , dot(binormal, -view) );
	vec2	scanStepOnMap = embossScaleFactor * stepFactor * scanDirectionOnMap; // 1 step must scan 'stepFactor' part of 'emboss' and corresponding part of 
	float	scanStepOnHeight = stepFactor;
    
	vec2	tex = fTex;
	vec2	previousTex = tex;
    
	float	scanHeight = 1.0f;
    float   previousHeight = 1.0f;
	float	currentHeight = texture(imageNormals, tex).w;
	
    while(scanHeight > currentHeight) {
		previousTex = tex;
        previousHeight = currentHeight;
        
		scanHeight -= scanStepOnHeight;
		tex += scanStepOnMap;
        
		currentHeight = texture(imageNormals, tex).w;
	}
    
	float	a = previousHeight, b = currentHeight, x = scanHeight + scanStepOnHeight, y = scanHeight;
	float	t = (a - x) / ((y - x) - (b - a));
	tex = mix(previousTex, tex, t);

	// oColor = vec4(fTex, 0.0f, 1.0f);
    
    vec4    dataAlbedo     = texture(imageAlbedo, tex);
    vec4    dataNormals    = texture(imageNormals, tex);
    vec4    dataRoughness   = texture(imageRoughness, tex);
    vec4    dataOcclusion  = texture(imageOcclusion, tex);
    vec4    dataMetalness  = texture(imageMetalness, tex);
    
    vec3    albedo = dataAlbedo.xyz;
    vec3    normal = normalize(
        fTBN[0] * (dataNormals.x * 2.0f - 1.0f) +
        fTBN[1] * (dataNormals.y * 2.0f - 1.0f) +
        fTBN[2] * (dataNormals.z * 2.0f - 1.0f)
    );
    // vec3    normal = normalize(fTBN * (dataNormals.xyz * 2.0f - 1.0f));
    // vec3    normal = fTBN[2];
    float   height = dataNormals.w;
    float   roughness = dataRoughness.x;
    float   occlusion = dataOcclusion.x;
    float   metalness = dataMetalness.x;
    float   ambient = 0.2f;
    
    vec3    lightDirection = normalize(vec3(-1.0f, -2.0f, +1.0f));
    // vec3    lightDirection = normalize(vec3(0.0f, -1.0f, -1.0f));
    vec3    light = -lightDirection;
    

	float	diffuseIntensity = Diffuse(normal, light, view, roughness, ambient * occlusion);
	float	specularIntensity = Specular(normal, light, view, roughness);

	float	gloss = 1.0f - roughness;
	
    vec3    reflection = reflect(-view, normal);
    
    vec4    dataEnvironment = textureLod(imageEnvironment, reflection, pow(roughness, 2.0f) * 12.0f);
    vec3    environment = dataEnvironment.xyz;

    float   fresnel = Fresnel(normal, view, roughness);
    
	vec3 oDiffuse = (1.0f - gloss) * diffuseIntensity * albedo * vec3(1.0f);
	vec3 oSpecular = gloss * (specularIntensity * mix(vec3(1.0f), albedo, metalness) * vec3(1.0f) + environment * fresnel * occlusion);
    
    oColor = vec4(oDiffuse + oSpecular, 1.0f);
    // oColor = vec4(albedo * (ambient * occlusion + (1.0f - ambient) * diffuseIntensity) + vec3(specularIntensity), 1.0f);
    // oColor = vec4(vec3(specularIntensity), 1.0f);
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
	
	return orenNayar;
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