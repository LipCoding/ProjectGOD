
#include "Share.fx"

// ======================= Particle Shader ============================

// Single Target Output Structure
struct PS_OUTPUT_SINGLE
{
	float4	vTarget0	: SV_Target;
};

struct VS_INPUT_PARTICLE
{
	float3	vPos		: POSITION;
	float3	vVelocity	: VELOCITY;
	float2	vSize		: SIZE;
	float	fLifeTime : LIFETIME;
	float	fCreateTime : CREATETIME;
	unsigned int iType : TYPE;
	float	fLightRange : LIGHTRANGE;
};

struct VS_OUTPUT_PARTICLE
{
	float3	vPos		: POSITION;
	float3	vVelocity	: VELOCITY;
	float2	vSize		: SIZE;
	float	fLifeTime : LIFETIME;
	float	fCreateTime : CREATETIME;
	unsigned int iType : TYPE;
	float	fLightRange : LIGHTRANGE;
};
cbuffer ParticleCBuffer	: register(b7)
{
	float3	g_vParticlePos;
	float	g_fDeltaTime;
	float3	g_vCamAxisX;
	float	g_fCreateTime;
	float3	g_vCamAxisY;
	float	g_fParticleSpeed;
	float	g_fParticleTime;
	float3	g_vCreateDir;
}

//SamplerState	g_DiffuseSmp	: register(s10);
//texture2D		g_GBufferNormal	: register(t12);
//texture2D		g_GBufferDepth	: register(t13);
//texture2D		g_GBufferSpecular	: register(t14);

#define PT_EMITTER 0
#define PT_FLARE 1

float3 RandDir(float fGameTime)
{
	return (float3)sin(fGameTime);
}

float Rand(float2 co) {
	return 0.5 + (frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453)) * 0.5;
}


float2 ComputeAtlasUV(float2 vCurUV)
{
	float2	vUV = (float2)0;

	/*vUV.x = (g_iAnimFrameX + vCurUV.x) / g_iAnimFrameMaxX;
	vUV.y = (g_iAnimFrameY + vCurUV.y) / g_iAnimFrameMaxY;*/

	return vUV;
}

VS_INPUT_PARTICLE ParticleStreamOutVS(VS_INPUT_PARTICLE input)
{
	return input;
}

[maxvertexcount(2)]
void ParticleStreamOutGS(point VS_INPUT_PARTICLE input[1],
	inout PointStream<VS_INPUT_PARTICLE> pointStream)
{
	if (input[0].iType == PT_EMITTER)
	{
		input[0].fCreateTime += g_fDeltaTime;

		if (input[0].fCreateTime > g_fCreateTime)
		{
			input[0].fCreateTime -= g_fCreateTime;

			VS_INPUT_PARTICLE	pt;

			pt.vPos = g_vParticlePos;
			pt.vVelocity = g_vCreateDir * g_fParticleSpeed;
			//pt.vVelocity = normalize(RandDir(g_fParticleTime)) * g_fParticleSpeed;
			//pt.vVelocity = float3(0.f, 1.f, 0.f) * g_fParticleSpeed;
			pt.vSize = input[0].vSize;
			pt.fLifeTime = input[0].fLifeTime;
			pt.fCreateTime = 0.f;
			pt.iType = PT_FLARE;
			pt.fLightRange = input[0].fLightRange;

			pointStream.Append(pt);
		}

		pointStream.Append(input[0]);
	}

	else
	{
		input[0].fLifeTime -= g_fDeltaTime;
		if (input[0].fLifeTime > 0.f)
		{
			input[0].vPos += input[0].vVelocity * g_fDeltaTime;
			pointStream.Append(input[0]);
		}
	}

}

VS_INPUT_PARTICLE ParticleVS(VS_INPUT_PARTICLE input)
{
	return input;
}

struct GS_OUTPUT_PARTICLE
{
	float4	vPos		: SV_POSITION;
	float2	vUV			: TEXCOORD;
	float4	vProjPos	: POSITION;
};

[maxvertexcount(4)]
void ParticleGS(point VS_INPUT_PARTICLE input[1],
	inout TriangleStream<GS_OUTPUT_PARTICLE> triStream)
{
	if (input[0].iType == PT_EMITTER)
		return;

	float	fHalfX = input[0].vSize.x * 0.5f;
	float	fHalfY = input[0].vSize.y * 0.5f;


	float4	vPos[4];
	vPos[0] = float4(input[0].vPos + fHalfX * g_vCamAxisX - fHalfY * g_vCamAxisY, 1.f);
	vPos[1] = float4(input[0].vPos + fHalfX * g_vCamAxisX + fHalfY * g_vCamAxisY, 1.f);
	vPos[2] = float4(input[0].vPos - fHalfX * g_vCamAxisX - fHalfY * g_vCamAxisY, 1.f);
	vPos[3] = float4(input[0].vPos - fHalfX * g_vCamAxisX + fHalfY * g_vCamAxisY, 1.f);

	/*vPos[0] = float4(-1.f, -1.f, 2.f, 1.f);
	vPos[1] = float4(-1.f, 1.f, 2.f, 1.f);
	vPos[2] = float4(1.f, -1.f, 2.f, 1.f);
	vPos[3] = float4(1.f, 1.f, 2.f, 1.f);*/

	float2	vUV[4] =
	{
		float2(0.f, 1.f),
		float2(0.f, 0.f),
		float2(1.f, 1.f),
		float2(1.f, 0.f)
	};

	// Atlas
	/*if (g_iAnimType == 1)
	{
		for (int i = 0; i < 4; ++i)
		{
			vUV[i] = ComputeAtlasUV(vUV[i]);
		}
	}*/

	GS_OUTPUT_PARTICLE	output;

	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		output.vProjPos = mul(vPos[i], g_matVP);
		output.vUV = vUV[i];
		output.vPos = output.vProjPos;

		triStream.Append(output);
	}
}

PS_OUTPUT_SINGLE ParticlePS(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	/*if (g_iAnimType == 2)
	{
		float3	vUV = float3(input.vUV, g_iAnimFrameX);
		output.vTarget0 = g_EffectTexArray.Sample(g_DiffuseSmp, vUV);
	}

	else
	//output.vTarget0 = g_DiffuseTex.Sample(g_DiffuseSmp, input.vUV);

	////output.vTarget0 = float4(1.f, 0.f, 0.f, 1.f);

	//if (output.vTarget0.a == 0.f)
	//	clip(-1);

	//// 깊이값을 얻어온다.
	//// GBufferDepth에서 깊이를 얻어오기 위한 투영좌표를 이용한 UV 좌표를 구해준다.
	//float2	vDepthUV;
	//vDepthUV.x = input.vProjPos.x / input.vProjPos.w * 0.5f + 0.5f;
	//vDepthUV.y = input.vProjPos.y / input.vProjPos.w * -0.5f + 0.5f;
	//float	fDepth = g_GBufferDepth.Sample(g_GBufferSmp, vDepthUV).w;
	//float	fSrcDepth = input.vProjPos.w;

	//float	fAlpha = (fDepth - fSrcDepth) / 0.5f;

	//if (fAlpha > 1.f)
	//	fAlpha = 1.f;

	//else if (fAlpha < 0.f)
	//	fAlpha = 0.f;

	//if (fDepth == 0.f)
	//	fAlpha = 1.f;

	//output.vTarget0.a *= fAlpha;
	*/
	output.vTarget0 = float4(1.f, 0.f, 0.f, 1.f);

	return output;
}


//// ======================= Particle Light ===============================
//struct GS_OUTPUT_PARTICLE_LIGHT
//{
//	float4	vPos		: SV_POSITION;
//	float2	vUV			: TEXCOORD;
//	float3	vLightPos	: POSITION;
//	float	fLightRange : LIGHTRANGE;
//};
//
//
//[maxvertexcount(4)]
//void ParticleLightGS(point VS_INPUT_PARTICLE input[1],
//	inout TriangleStream<GS_OUTPUT_PARTICLE_LIGHT> triStream)
//{
//	//if (input[0].iType == PT_EMITTER)
//	//	return;
//
//	//float	fHalfX = input[0].fLightRange * 1.5f;
//	//float	fHalfY = input[0].fLightRange * 1.5f;
//
//	//float4	vPos[4];
//	//vPos[0] = float4(input[0].vPos + fHalfX * g_vCamAxisX - fHalfY * g_vCamAxisY, 1.f);
//	//vPos[1] = float4(input[0].vPos + fHalfX * g_vCamAxisX + fHalfY * g_vCamAxisY, 1.f);
//	//vPos[2] = float4(input[0].vPos - fHalfX * g_vCamAxisX - fHalfY * g_vCamAxisY, 1.f);
//	//vPos[3] = float4(input[0].vPos - fHalfX * g_vCamAxisX + fHalfY * g_vCamAxisY, 1.f);
//
//	///*vPos[0] = float4(-1.f, -1.f, 2.f, 1.f);
//	//vPos[1] = float4(-1.f, 1.f, 2.f, 1.f);
//	//vPos[2] = float4(1.f, -1.f, 2.f, 1.f);
//	//vPos[3] = float4(1.f, 1.f, 2.f, 1.f);*/
//
//	//float2	vUV[4] =
//	//{
//	//	float2(0.f, 1.f),
//	//	float2(0.f, 0.f),
//	//	float2(1.f, 1.f),
//	//	float2(1.f, 0.f)
//	//};
//
//	//// Atlas
//	///*if (g_iAnimType == 1)
//	//{
//	//for (int i = 0; i < 4; ++i)
//	//{
//	//vUV[i] = ComputeAtlasUV(vUV[i]);
//	//}
//	//}*/
//
//	//GS_OUTPUT_PARTICLE_LIGHT	output;
//
//	//[unroll]
//	//for (int i = 0; i < 4; ++i)
//	//{
//	//	float4	vProjPos = mul(vPos[i], g_matVP);
//	//	float2	vProjUV = (float2)0;
//	//	vProjUV.x = vProjPos.x / vProjPos.w * 0.5f + 0.5f;
//	//	vProjUV.y = vProjPos.y / vProjPos.w * -0.5f + 0.5f;
//	//	output.vPos = vProjPos;
//	//	output.vUV = vProjUV;
//	//	output.vLightPos = mul(float4(input[0].vPos, 1.f), g_matView);
//	//	output.fLightRange = input[0].fLightRange;
//
//	//	triStream.Append(output);
//	//}
//}
//
//struct PS_OUTPUT_LIGHTACC
//{
//	float4	vTarget0	: SV_Target;
//	float4	vTarget1	: SV_Target1;
//};
////ParticleLightAccPS
//PS_OUTPUT_LIGHTACC ParticleLightAccPS(GS_OUTPUT_PARTICLE_LIGHT input)
//{
//	PS_OUTPUT_LIGHTACC	output = (PS_OUTPUT_LIGHTACC)0;
//
//	//float4	vDepth = g_GBufferDepth.Sample(g_GBufferSmp, input.vUV);
//
//	//if (vDepth.w == 0.f)
//	//	clip(-1);
//
//	///*if (vDepth.w < input.vLightPos.z)
//	//	clip(-1);*/
//
//	//	// uv좌표를 -1 ~ 1 사이로 만든다.
//	//float4	vProjPos;
//	//vProjPos.x = (input.vUV.x * 2.f - 1.f) * vDepth.w;
//	//vProjPos.y = (input.vUV.y * -2.f + 1.f) * vDepth.w;
//	//vProjPos.z = vDepth.x * vDepth.w;
//	//vProjPos.w = vDepth.w;
//
//	//// 그림자 계산을 위한 현재 정점의 월드공간을 구해준다.
//	////float3	vWorldPos = mul(vProjPos, g_matInvVP);
//
//	//// 월드공간의 정점을 조명공간으로 변환해준다.
//	////float4	vShadowPos = mul(float4(vWorldPos, 1.f), g_matShadowVP);
//
//	////float2	vShadowUV;
//	////vShadowUV.x = vShadowPos.x / vShadowPos.w * 0.5f + 0.5f;
//	////vShadowUV.y = vShadowPos.y / vShadowPos.w * -0.5f + 0.5f;
//
//	////float4	vShadowDepth = g_ShadowDepth.Sample(g_GBufferSmp, vShadowUV);
//
//	//// 현재 깊이와 그림자 깊이를 비교한다.
//	///*if (vShadowDepth.x * vShadowPos.w < vShadowPos.z - g_fShadowBias)
//	//{
//	//	output.vTarget0 = (float4)0;
//	//	output.vTarget1 = (float4)0;
//	//}
//
//	//else*/
//	//{
//	//	float3	vViewPos = mul(vProjPos, g_matInvProj).xyz;
//
//	//	float3	vLightDir = input.vLightPos - vViewPos;
//
//	//	float	fDist = length(vLightDir);
//
//	//	if (fDist > input.fLightRange)
//	//		clip(-1);
//
//	//	float4	vNormalCol = g_GBufferNormal.Sample(g_GBufferSmp, input.vUV);
//
//	//	if (vNormalCol.a == 0.f)
//	//		clip(-1);
//
//	//	float3	vNormal = vNormalCol * 2.f - 1.f;
//
//	//	float4	vDif = (float4)vDepth.y;
//	//	float4	vAmb = (float4)vDepth.z;
//
//	//	_tagLightInfo tMtrl;// = ComputeParticleLightAcc(input.vLightPos, input.fLightRange, vNormal, vViewPos, input.vUV,
//	//		//vNormalCol.a, vDif, vAmb);
//
//	//	output.vTarget0 = tMtrl.vDif + tMtrl.vAmb;
//	//	output.vTarget1 = tMtrl.vSpc;
//	//}
//
//	return output;
//}
