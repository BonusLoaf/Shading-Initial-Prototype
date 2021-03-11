#version 460

in vec3 Position;
in vec3 Normal;

in vec3 Vec;


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

layout(binding=0) uniform samplerCube SkyBoxTex;
layout(binding=1) uniform sampler2D PyTex;
layout(binding=2) uniform sampler2D StaffTex;
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

vec3 TexColor;

vec3 blinnPhongModel(vec3 position, vec3 normal)
{


if(texID > 0)
{

if(texID == 1)
{
TexColor = texture(PyTex, TexCoord).rgb;
}
else
{
TexColor = texture(StaffTex, TexCoord).rgb;
}

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

//vec3 texColor = texture(Tex1, TexCoord).rgb;



void main() {

//unpack the normal and set it to a range between 0 and 1
//vec3 norm = texture(NormalMapTex, TexCoord).xyz;
//norm.xy = 2.0 * norm.xy - 1.0f;




if(texID > 0)
{
    FragColor = vec4 (blinnPhongModel(Position, normalize(Normal)),1.0);
}
else
{

vec3 texColor = texture(SkyBoxTex, normalize(Vec)).rgb;

FragColor = vec4 (texColor,1.0f);
    }

}
