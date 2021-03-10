#version 460

in vec3 Position;
in vec3 Normal;


in vec3 LightDir;
in vec3 ViewDir;

uniform struct lightInfo
{
vec4 Position;
vec3 La;
vec3 L;
}lights;

uniform struct materialInfo
{
vec3 Ka;
vec3 Kd;
vec3 Ks;
float Shininess;
}Material;

layout(binding=0) uniform sampler2D BrickTex;
layout(binding=1) uniform sampler2D NormalMapTex;
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;


vec3 blinnPhongModel(vec3 LightDir, vec3 normal)
{


vec3 brickTexColor = texture(BrickTex, TexCoord).rgb;


//vec3 texCol = mix(brickTexColor.rgb, dirtTexColor.rgb, dirtTexColor.a);





//Ambient
vec3 ambient = lights.La * brickTexColor;

//Diffuse
vec3 s = normalize(vec3(lights.Position) - LightDir);

float sDotN = dot(LightDir,normal);

vec3 diffuse = brickTexColor * sDotN;

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


//vec3 texColor = texture(Tex1, TexCoord).rgb;

}

void main() {

//unpack the normal and set it to a range between 0 and 1
vec3 norm = texture(NormalMapTex, TexCoord).xyz;
norm.xy = 2.0 * norm.xy - 1.0f;


    FragColor = vec4 (blinnPhongModel(LightDir, normalize(norm)),1.0);


}
