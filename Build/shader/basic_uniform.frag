#version 460

in vec3 Position;
in vec3 Normal;

in vec3 Vec;


in vec3 LightDir;
in vec3 ViewDir;

uniform struct lightInfo
{
vec4 Position;
vec3 La;
vec3 L;
}lights;

uniform int texID;

uniform struct materialInfo
{
vec3 Ka;
vec3 Kd;
vec3 Ks;
float Shininess;
}Material;



uniform struct SpotLightInfo {
vec3 Position; // Position in cam coords
vec3 L; // Diffuse/spec intensity
vec3 La; // Amb intensity
vec3 Direction; // Direction of the spotlight in cam coords.
float Exponent; // Angular attenuation exponent
float Cutoff; // Cutoff angle (between 0 and pi/2)
} Spot;

layout(binding=0) uniform samplerCube SkyBoxTex;
layout(binding=1) uniform sampler2D PyTex;
layout(binding=2) uniform sampler2D StaffTex;
layout(binding=3) uniform sampler2D NormalMapTex;
layout(binding=4) uniform sampler2D DirtTex;
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

vec3 TexColor = texture(PyTex, TexCoord).rgb;

vec4 TexPyramid = texture(PyTex, TexCoord);

vec4 TexOverlay = texture(DirtTex, TexCoord);

vec3 blinnPhongModelWithNormal(vec3 LightDir, vec3 normal)
{

if(texID > 0)
{


TexColor = mix(TexPyramid.rgb, TexOverlay.rgb, TexOverlay.a);


//Ambient
vec3 ambient = lights.La * TexColor;

//Diffuse
vec3 s = normalize(vec3(lights.Position) - LightDir);

float sDotN = dot(LightDir,normal);

vec3 diffuse = TexColor * sDotN;

//Specular

vec3 specular = Material.Ks * lights.L * sDotN;

//vec3 v = normalize(-position.xyz);




//Shininess
if(sDotN > 0.0)
{

//Half Vector
vec3 h = normalize(ViewDir+LightDir);

//Ignore Reflection Vector
//vec3 r = reflect(-s,normal);

//Calculate specular with Shininess
specular = Material.Ks * pow(max(dot(h,normal),0.0), Material.Shininess);

}

return ambient + lights.L * (diffuse + specular);


}




//vec3 texCol = mix(brickTexColor.rgb, dirtTexColor.rgb, dirtTexColor.a);


return vec3(1.0f);



}

vec3 blinnPhongSpot( vec3 position, vec3 n ) {



//Ambient
vec3 ambient = Spot.La * Material.Ka;



//Diffuse
vec3 s = normalize(vec3(Spot.Position) - position);

float cosAng = dot(-s, normalize(Spot.Direction));

float angle = acos(cosAng);

float spotScale = 0.0f;

vec3 diffuse = vec3(0.0);

vec3 specular = vec3(0.0);


if(angle < Spot.Cutoff)
{

spotScale = pow(cosAng, Spot.Exponent);
float sDotN = dot(s,n);

vec3 diffuse = Material.Kd * sDotN;

if(sDotN > 0.0)
{
vec3 v = normalize(-position.xyz);
vec3 h = normalize(v+s);
specular = Material.Ks * pow(max(dot(h,n),0.0),Material.Shininess);

}
return ambient + spotScale * Spot.L * (diffuse + specular);
}


return ambient + spotScale * Spot.L * (diffuse + specular);

}


vec3 blinnPhongModel(vec3 position, vec3 normal)
{


TexColor = texture(StaffTex, TexCoord).rgb;




//Ambient
vec3 ambient = lights.La * TexColor;

//Diffuse
vec3 s = normalize(vec3(lights.Position) - position);

float sDotN = dot(s,normal);

vec3 diffuse = TexColor * sDotN;

//Specular
vec3 specular = Material.Ks * lights.L * sDotN;

vec3 v = normalize(-position.xyz);




//Shininess
if(sDotN > 0.0)
{

//Half Vector
vec3 h = normalize(v+s);

//Reflection Vector
//vec3 r = reflect(-s,normal);

//Calculate specular with Shininess
specular = Material.Ks * pow(max(dot(h,normal),0.0), Material.Shininess);

}

return ambient + lights.L * (diffuse + specular);



}

//vec3 texColor = texture(Tex1, TexCoord).rgb;



void main() {

//unpack the normal and set it to a range between 0 and 1
//vec3 norm = texture(NormalMapTex, TexCoord).xyz;
//norm.xy = 2.0 * norm.xy - 1.0f;




if(texID > 0)
{

if(texID == 1)
{
//unpack the normal and set it to a range between 0 and 1
vec3 norm = texture(NormalMapTex, TexCoord).xyz;
norm.xy = 2.0 * norm.xy - 1.0f;

vec3 fullLightModel = blinnPhongSpot(Position, normalize(Normal)) + blinnPhongModelWithNormal(LightDir, normalize(norm));

    FragColor = vec4(fullLightModel,1.0);
}
else
{
FragColor = vec4 (blinnPhongModel(Position, normalize(Normal)),1.0);
}


}
else
{
vec3 texColor = texture(SkyBoxTex, normalize(Vec)).rgb;

FragColor = vec4 (texColor,1.0f);
}

}
