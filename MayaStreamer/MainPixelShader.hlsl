Texture2D txDiffuse  : register(t0);

SamplerState sampAni : register(s0);

cbuffer LBUFFER : register (b0)
{
	float4 lightPos;
}

struct FS_IN
{
	float4 Pos	: SV_POSITION;
	float4 Norm : NORMAL;
	float2 uv	: UV;
	float3 wPos : POSITION;
};

float4 FS_main(FS_IN input) : SV_Target
{
	float4 ambient = { 0.1f, 0.1f, 0.1f, 1.0f };

float2 uv = input.uv;
uv.y = 1 - uv.y;
float3 tex = txDiffuse.Sample(sampAni, uv).xyz;
float3 lightDir = normalize(lightPos.xyz - input.wPos.xyz);

//Specular
float3 reflection = reflect(lightDir, input.Norm);
float specAngl = dot(reflection, normalize(-input.wPos.xyz));

float cos = dot(lightDir, input.Norm);
clamp(cos, 0, 1);

float3 s = ambient;
s = s + tex * cos;
s = s + clamp(pow(specAngl, 5.f) * float3(1.0f, 1.0f, 1.0f), 0, 1);

return saturate(float4(s, 1.0f));
}