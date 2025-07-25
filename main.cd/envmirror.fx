// Environment glass shader
// (c) oP group 2009
// based on a shader by Ichiro on the Gamestudio Wiki

#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <tangent>
#include <vecskill>

bool AUTORELOAD;
float4 vecLight;
float fAlpha;
float fAlbedo;

texture entSkin1;
texture entSkin2;
texture mtlSkin1;

samplerCUBE sEnvironment = sampler_state { Texture = <mtlSkin1>; MipFilter = LINEAR; };
sampler sBase = sampler_state { Texture = <entSkin1>;	MipFilter = LINEAR; };
sampler sNormal = sampler_state { Texture = <entSkin2>; MipFilter = LINEAR; };

// vertex shader output structure
struct VS_OUTPUT
{
	float4 Pos: POSITION;
	float  Fog:  FOG;
	float2 Tex: TEXCOORD0;      
	float3 Eye: TEXCOORD1;
	float3 Norm: TEXCOORD2;
   float factor: TEXCOORD6;
   float3 rainbow: TEXCOORD7;
};


VS_OUTPUT VS(
   float4 inPos  : POSITION,
   float3 inNormal : NORMAL,
   float2 inTex  : TEXCOORD0,
   float4 inTangent : TEXCOORD2) 
{
	VS_OUTPUT Out;
	float4 P = DoPos(inPos);
	float3 N = DoNormal(inNormal);
	Out.Pos  = DoTransform(inPos);
	Out.Fog = DoFog(inPos);
	Out.Tex = inTex;	
	Out.Eye = P-vecViewPos ;
	Out.Norm = -N;
	
	Out.factor = 0;//DoDefault(vecSkill41.y*0.005,0.25); // Texture	
	
	float fSpread = 0;//DoDefault(vecSkill41.x*0.0005,0.025);
	Out.rainbow.z = 0;//1.0-2*fSpread;
	Out.rainbow.y = 0;Out.rainbow.z-fSpread;
	Out.rainbow.x = 0;Out.rainbow.y-fSpread;
	return Out;
}

float4 envglass_PS ( VS_OUTPUT In) : COLOR
{
	float4 OutColor;
	float4 color = tex2D(sBase, In.Tex);
	color.a *= In.factor;
	float3 norm = In.Norm;
	float3 vec = reflect(normalize(In.Eye), norm);
	float4 Environment = texCUBE(sEnvironment,vec);

	OutColor.rgb = (0.8*lerp(color.rgb,0.5,0) + fAlbedo*Environment.rgb/2);
	OutColor.a = 1;
	return OutColor;
}

technique glas
{
	pass one
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 envglass_PS();
	}
}

technique fallback { pass one { } }