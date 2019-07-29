
#include "Share.fx"

struct PS_OUTPUT_SINGLE
{
	float4	vColor	: SV_Target;
};

// 함수 인자에 레지스터를 지정해놓는 경우 인자를 안넣어주고 해당 레지스터에서
// 값을 끌어다 사용한다.
VS_OUTPUT_COLOR StandardColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE StandardColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor = input.vColor + g_vColor;

	return output;
}

PS_OUTPUT_SINGLE StandardColorForwardPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor = input.vColor + g_vColor;

	return output;
}

VS_OUTPUT_COLOR_NORMAL StandardColorNormalVS(VS_INPUT_COLOR_NORMAL input)
{
	VS_OUTPUT_COLOR_NORMAL	output = (VS_OUTPUT_COLOR_NORMAL)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	// Normal을 뷰공간으로 만들어준다.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vColor = input.vColor;
	output.iDecal = 1;

	return output;
}

PS_OUTPUT StandardColorNormalPS(VS_OUTPUT_COLOR_NORMAL input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	_tagLightInfo	tLight = ComputeLight(input.vViewPos, input.vNormal, float2(0.f, 0.f));


	//output.vColor = input.vColor * (tLight.vDif + tLight.vAmb) + tLight.vSpc;
	output.vColor = input.vColor + g_vColor;
	output.vColor = float4(1.f, 0.f, 0.f, 1.f);

	output.vColor1.xyz = input.vNormal * 0.5f + 0.5f;
	output.vColor1.w = 1.f;
	output.vColor2.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor2.w = input.vProjPos.w;

	output.vColor5.w = (float)input.iDecal;


	// 조명처리 샘플
	// 조명 방향을 만들어준다.
	/*float3	vLightDir = float3(-1.f, -1.f, 0.f);
	vLightDir = -normalize(vLightDir);

	// 조명방향을 뷰공간으로 변환한다.
	vLightDir = normalize(mul(float4(vLightDir, 0.f), g_matView).xyz);

	float4	vDif = float4(0.f, 1.f, 0.f, 1.f) * g_vMtrlDiffuse *
		max(0, dot(vLightDir, input.vNormal));
	float4	vAmb = float4(0.f, 0.2f, 0.f, 1.f) * g_vMtrlAmbient;

	// 정점에서 카메라를 향하는 벡터를 만들어준다.
	float3	vView = -normalize(input.vViewPos);

	// 반사벡터를 구해준다.
	float3	vR = 2.f * dot(vLightDir, input.vNormal) * input.vNormal - vLightDir;

	float4	vSpc = float4(0.f, 1.f, 0.f, 1.f) * g_vMtrlSpecular *
		pow(max(0, dot(vView, vR)), 3.2f);

	output.vColor = input.vColor * (vDif + vAmb) + vSpc;*/
	//output.vColor = float4(1.f, 1.f, 0.f, 1.f);

	return output;
}

VS_OUTPUT_TEX StandardTexVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);

	if (g_iAniType == 0)
		output.vUV = ComputeUV(input.vUV);

	else
		output.vUV = input.vUV;

	output.iDecal = 1;

	return output;
}

PS_OUTPUT_SINGLE StandardTexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	if (g_iAniType == 1)
	{
		float3	vUV;
		vUV.xy = input.vUV;
		vUV.z = g_iAniFrameX;
		output.vColor = g_DifArrTex.Sample(g_DifSmp, vUV) + g_vColor;
	}

	else
	{
		output.vColor = g_DifTex.Sample(g_DifSmp, input.vUV) + g_vColor;
	}

	// clip : 픽셀값을 쓰지 않고 폐기한다.
	// 이경우 깊이버퍼에도 값을 안쓴다.
	if (output.vColor.a == 0.f)
		clip(-1);

	return output;
}

VS_OUTPUT_TEX_NORMAL StandardTexNormalVS(VS_INPUT_TEX_NORMAL input)
{
	VS_OUTPUT_TEX_NORMAL	output = (VS_OUTPUT_TEX_NORMAL)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(input.vPos, 1.f), g_matWV).xyz;

	if (g_iAniType == 0)
		output.vUV = ComputeUV(input.vUV);
	else
		output.vUV = input.vUV;

	output.iDecal = 1;

	return output;
}

// GameObject
PS_OUTPUT StandardTexNormalPS(VS_OUTPUT_TEX_NORMAL input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	/*_tagLightInfo	tLight = ComputeLight(input.vViewPos, input.vNormal,
		input.vUV);

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);
	output.vColor = vColor *
		(tLight.vDif + tLight.vAmb) + tLight.vSpc;
	output.vColor.w = vColor.w;*/

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);

	// clip : 픽셀값을 쓰지 않고 폐기한다.
	// 이경우 깊이버퍼에도 값을 안쓴다.
	if (vColor.a == 0.f)
		clip(-1);


	float3 vCamPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matCameraWorld).xyz;
	vCamPos = mul(float4(vCamPos, 1.f), g_matView).xyz;
	// View World를 기준으로 픽셀점에서  카메라를 바라보는 방향을 구한다.
	float3 vCamDir = normalize(vCamPos - input.vViewPos);

	// smoothstep(min, max, x)
	// x가 min보다 작으면 0을 리턴하고 max보다 크다면 1을 리턴한다.
	// 1

	/*float rimWidth = 0.5f;
	float fRimLightColor = smoothstep(1.f - rimWidth, 1.f, 1 - max(0, saturate(dot(input.vNormal,
		vCamDir))));

	vColor += pow(fRimLightColor, 10.f) * 0.55f;*/

	// 2
	/*float fRim = 1.0f - saturate((dot(input.vNormal, vCamDir)));
	float4 rc = float4(1.f, 1.f, 1.f, 1.f) * 0.75f;
	vColor += pow(fRim, 2.f) * rc;*/

	/*float dotProduct = saturate(dot(normalize(input.vNormal), normalize(vCamDir)));
	float degree = float(degrees(acos(dotProduct)));

	if (degree > 70.f)
	{
		vColor *= float4(1.f, 0.f, 0.f, 1.f);
	}

	if (dotProduct < 0.3f)
	{
		vColor = float4(0.f, 0.f, 0.f, 1.f);
	}*/


	//float3 vCam = mul(float4(g_vCameraPos, 0.f), g_matView);
	/*float3 vCam = mul(float4(0.f, 0.f, 1.f, 0.f), g_matCameraWorld);
	vCam = mul(float4(vCam, 0.f), g_matView);

	float dotProduct = saturate(dot(normalize(input.vNormal), -normalize(vCam)));

	if (dotProduct < 0.3f)
	{
		vColor *= float4(0.f, 0.f, 0.f, 1.f);
	}*/

	output.vColor5.w = (float)input.iDecal;

	output.vColor = vColor + g_vColor;

	//output.vColor = float4(1.f, 0.f, 0.f, 1.f);

	//1
	/*
	float3 vLightCamDir = mul(float4(0.f, 0.f, 1.f, 0.f), g_matLightWorld);
	vLightCamDir = mul(float4(vLightCamDir, 0.f), g_matView);

	float fDot = max(0, dot(normalize(input.vNormal), normalize(-vLightCamDir)));
	*/

	//2
	/*
	float3 vLightCamPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matLightWorld);
	vLightCamPos = mul(float4(vLightCamPos, 1.f), g_matView);
	float3 vLightCamDir = normalize(vLightCamPos - input.vViewPos);
	float fDot = max(0, dot(normalize(input.vNormal), normalize(vLightCamDir)));

	fDot = (ceil(fDot * 3) / 3.f);
	output.vColor1.xyz = (input.vNormal * 0.5f + 0.5f) * fDot;
	*/

	output.vColor1.xyz = input.vNormal * 0.5f + 0.5f;
	output.vColor1.w = 1.f;
	output.vColor2.x = input.vProjPos.z / input.vProjPos.w;
	output.vColor2.w = input.vProjPos.w;

	// 재질 Diffuse, Ambient 값을 저장한다.
	output.vColor2.y = g_vMtrlDiffuse.x;
	output.vColor2.z = g_vMtrlAmbient.x;

	float4	vMtrlSpc;
	if (g_vMtrlAmbient.w == 1)
	{
		vMtrlSpc = g_SpecularTex.Sample(g_DifSmp, input.vUV);
		vMtrlSpc.w = g_vMtrlSpecular.w;
		output.vColor4.w = vMtrlSpc.w;
		//vMtrlSpc = float4(g_vMtrlSpecular.xyz, 1.f);
	}

	else
	{
		vMtrlSpc = g_vMtrlSpecular;
		output.vColor4.w = g_vMtrlSpecular.w;
	}
	output.vColor3 = vMtrlSpc;

	_tagLightInfo	tLight = ComputeLight(input.vViewPos, input.vNormal,
		input.vUV);

	output.vColor.xyz = output.vColor.xyz * (tLight.vDif.xyz + tLight.vAmb.xyz) + tLight.vSpc.xyz / 2.f;
	output.vColor.w = output.vColor.w;

	return output;
}

// Effect
VS_OUTPUT_TEX_NORMAL StandardEffectTexNormalVS(VS_INPUT_TEX_NORMAL input)
{
	VS_OUTPUT_TEX_NORMAL	output = (VS_OUTPUT_TEX_NORMAL)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(input.vPos, 1.f), g_matWV).xyz;

	/* ATLAS */
	if (g_iAniType == 1)
	{
		output.vUV = ComputeUV(input.vUV);
	}
	else
	{
		output.vUV = input.vUV;
	}

	output.iDecal = 1;

	return output;
}

PS_OUTPUT StandardEffectTexNormalPS(VS_OUTPUT_TEX_NORMAL input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	float4 vColor;

	/* ATLAS */
	if (g_iAniType == 1)
	{
		float2 vUV;
		vUV.x = input.vUV.x + g_iAniFrameX;
		vUV.y = input.vUV.y + g_iAniFrameY;
		vColor = g_DifTex.Sample(g_DifSmp, vUV);
	}
	/* FRAME */
	else if (g_iAniType == 2)
	{
		float3	vUV;
		vUV.xy = input.vUV;
		vUV.z = g_iAniFrameX;
		vColor = g_DifArrTex.Sample(g_DifSmp, vUV);
	}
	else
	{
		/* UV Movement */
		input.vUV.x = input.vUV.x + g_fMoveUV_X;
		input.vUV.y = input.vUV.y + g_fMoveUV_Y;
		
		vColor = g_DifTex.Sample(g_DifSmp, input.vUV);
	}

	vColor.a = vColor.a - g_fAlphaFadeOut + g_fAlphaFadeIn;

	//float3 vCamPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matCameraWorld);
	//vCamPos = mul(float4(vCamPos, 1.f), g_matView);
	//float3 vCamDir = normalize(vCamPos - input.vViewPos);

	//float dotProduct = saturate(dot(normalize(input.vNormal), normalize(vCamDir)));
	//float degree = float(degrees(acos(dotProduct)));

	//output.vColor5.w = (float)input.iDecal;

	output.vColor = vColor /*+ g_vColor*/;

	//output.vColor1.xyz = input.vNormal * 0.5f + 0.5f;
	//output.vColor1.w = 1.f;
	//output.vColor2.x = input.vProjPos.z / input.vProjPos.w;
	//output.vColor2.w = input.vProjPos.w;

	//output.vColor2.y = g_vMtrlDiffuse.x;
	//output.vColor2.z = g_vMtrlAmbient.x;

	//float4	vMtrlSpc;
	//if (g_vMtrlAmbient.w == 1)
	//{
	//	vMtrlSpc = g_SpecularTex.Sample(g_DifSmp, input.vUV);
	//	vMtrlSpc.w = g_vMtrlSpecular.w;
	//	output.vColor4.w = vMtrlSpc.w;
	//}

	//else
	//{
	//	vMtrlSpc = g_vMtrlSpecular;
	//	output.vColor4.w = g_vMtrlSpecular.w;
	//}
	//output.vColor3 = vMtrlSpc;

	return output;
}

VS_OUTPUT_BUMP StandardBumpVS(VS_INPUT_BUMP input)
{
	VS_OUTPUT_BUMP	output = (VS_OUTPUT_BUMP)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	// Normal을 뷰공간으로 만들어준다.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(input.vBinormal, 0.f), g_matWV).xyz);
	output.vUV = input.vUV;
	output.iDecal = 1;

	return output;
}

VS_OUTPUT_TEX_NORMAL StandardTexNormalAnimVS(VS_INPUT_ANIM input)
{
	VS_OUTPUT_TEX_NORMAL	output = (VS_OUTPUT_TEX_NORMAL)0;

	_tagSkinning tSkinning = Skinning(input.vPos, input.vNormal, input.vWeights,
		input.vIndices);

	tSkinning.vPos = tSkinning.vPos - g_vTrLength * g_vPivot;

	/*_tagSkinning tSkinning;
	tSkinning.vNormal = input.vNormal;
	tSkinning.vPos = input.vPos - g_vTrLength * g_vPivot;*/

	/*float3 vNormal = mul(float4(tSkinning.vNormal, 0.f), g_matWorld);
	float dotResult = saturate(dot(normalize(vNormal), normalize(-g_vCameraPos)));

	if (dotResult < 0.3f)
	{
		output.iDecal = 1;
	}
	else
	{
		output.iDecal = 0;
	}*/

	output.vProjPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	output.vViewPos = mul(float4(tSkinning.vPos, 1.f), g_matWV).xyz;
	output.vNormal = mul(float4(tSkinning.vNormal, 0.f), g_matWV);

	output.vUV = input.vUV;
	output.iDecal = 0;

	return output;
}



VS_OUTPUT_BUMP StandardBumpAnimVS(VS_INPUT_BUMP_ANIM input)
{
	VS_OUTPUT_BUMP	output = (VS_OUTPUT_BUMP)0;

	_tagSkinning tSkinning = Skinning(input.vPos, input.vNormal,
		input.vTangent, input.vBinormal, input.vWeights,
		input.vIndices);

	tSkinning.vPos = tSkinning.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	// Normal을 뷰공간으로 만들어준다.
	output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(tSkinning.vPos, 1.f), g_matWV).xyz;
	output.vTangent = normalize(mul(float4(tSkinning.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(tSkinning.vBinormal, 0.f), g_matWV).xyz);
	output.vUV = input.vUV;
	output.iDecal = 0;

	return output;
}

PS_OUTPUT StandardBumpPS(VS_OUTPUT_BUMP input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);

	if (vColor.a == 0.f)
		clip(-1);

	// Normal을 구해준다.
	float3x3	mat =
	{
		input.vTangent,
		input.vBinormal,
		input.vNormal
	};

	output.vColor4.xyz = input.vTangent * 0.5f + 0.5f;
	output.vColor4.w = 1.f;

	output.vColor5.xyz = input.vBinormal * 0.5f + 0.5f;
	output.vColor5.w = (float)input.iDecal;

	// Normal 텍스쳐에서 픽셀을 얻어와서 법선벡터로 변환한다.
	// 색상은 0 ~ 1 사이의 값이므로 이 값을 단위벡터의 값인 -1 ~ 1 사이로
	// 변환해주려면 * 2 - 1 을 해주면 된다.
	float3	vBumpNormal = g_NormalTex.Sample(g_DifSmp, input.vUV).xyz;
	vBumpNormal = vBumpNormal * 2.f - 1.f;

	float3	vViewNormal = normalize(mul(vBumpNormal, mat));


	output.vColor = vColor + g_vColor;
	output.vColor1.xyz = vViewNormal * 0.5f + 0.5f;
	output.vColor1.w = 1.f;
	output.vColor2.x = (input.vProjPos.z / input.vProjPos.w);
	output.vColor2.w = input.vProjPos.w;

	// 재질 Diffuse, Ambient 값을 저장한다.
	output.vColor2.y = g_vMtrlDiffuse.x;
	output.vColor2.z = g_vMtrlAmbient.x;

	float4	vMtrlSpc;
	if (g_vMtrlAmbient.w == 1)
	{
		vMtrlSpc = g_SpecularTex.Sample(g_DifSmp, input.vUV);
		vMtrlSpc.w = g_vMtrlSpecular.w;
		output.vColor4.w = vMtrlSpc.w;
		//vMtrlSpc = float4(g_vMtrlSpecular.xyz, 1.f);
	}

	else
	{
		vMtrlSpc = g_vMtrlSpecular;
		output.vColor4.w = g_vMtrlSpecular.w;
	}

	output.vColor3 = vMtrlSpc;

	/*_tagLightInfo	tLight = ComputeLight(input.vViewPos, vViewNormal, input.vUV);

	output.vColor.xyz = vColor.xyz * (tLight.vDif.xyz + tLight.vAmb.xyz) + tLight.vSpc.xyz;
	output.vColor.w = vColor.w;*/

	//output.vColor1 = float4(1.f, 0.f, 0.f, 1.f);
	//output.vColor2 = float4(0.f, 1.f, 0.f, 1.f);
	//output.vColor3 = float4(0.f, 0.f, 1.f, 1.f);

	//output.vColor *= float4(0.f, 1.f, 0.f, 1.f);

	return output;
}

PS_OUTPUT_SINGLE StandardBumpForwardPS(VS_OUTPUT_BUMP input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);

	if (vColor.a == 0.f)
		clip(-1);

	// Normal을 구해준다.
	float3x3	mat =
	{
		input.vTangent,
		input.vBinormal,
		input.vNormal
	};

	// Normal 텍스쳐에서 픽셀을 얻어와서 법선벡터로 변환한다.
	// 색상은 0 ~ 1 사이의 값이므로 이 값을 단위벡터의 값인 -1 ~ 1 사이로
	// 변환해주려면 * 2 - 1 을 해주면 된다.
	float3	vBumpNormal = g_NormalTex.Sample(g_DifSmp, input.vUV).xyz;
	vBumpNormal = vBumpNormal * 2.f - 1.f;

	float3	vViewNormal = normalize(mul(vBumpNormal, mat));

	_tagLightInfo	tLight = ComputeLight(input.vViewPos, vViewNormal, input.vUV);

	output.vColor.xyz = vColor.xyz * (tLight.vDif.xyz + tLight.vAmb.xyz) + tLight.vSpc.xyz + g_vColor;
	output.vColor.w = vColor.w;

	return output;
}

PS_OUTPUT_SINGLE StandardTexNormalForwardPS(VS_OUTPUT_TEX_NORMAL input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);

	if (vColor.a == 0.f)
		clip(-1);

	_tagLightInfo	tLight = ComputeLight(input.vViewPos, input.vNormal, input.vUV);

	output.vColor.xyz = vColor.xyz * (tLight.vDif.xyz + tLight.vAmb.xyz) + tLight.vSpc.xyz + g_vColor;
	output.vColor.w = vColor.w;


	return output;
}

VS_OUTPUT_BUMP StandardShadowBumpAnimVS(VS_INPUT_BUMP_ANIM input)
{
	VS_OUTPUT_BUMP	output = (VS_OUTPUT_BUMP)0;

	_tagSkinning tSkinning = Skinning(input.vPos, input.vNormal,
		input.vTangent, input.vBinormal, input.vWeights,
		input.vIndices);

	tSkinning.vPos = tSkinning.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;

	return output;
}

PS_OUTPUT_SINGLE StandardShadowColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor.w = 1.f;

	return output;
}

PS_OUTPUT_SINGLE StandardShadowColorNormalPS(VS_OUTPUT_COLOR_NORMAL input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor.w = 1.f;

	return output;
}

PS_OUTPUT_SINGLE StandardShadowTexNormalPS(VS_OUTPUT_TEX_NORMAL input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor.w = 1.f;

	return output;
}

PS_OUTPUT_SINGLE StandardShadowBumpPS(VS_OUTPUT_BUMP input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor.w = 1.f;

	return output;
}

VS_OUTPUT_SHADOW_TEX ShadowStaticObjectVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_SHADOW_TEX	output = (VS_OUTPUT_SHADOW_TEX)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWorld);
	output.vPos = mul(output.vPos, g_matLightView);
	output.vPos = mul(output.vPos, g_matLightProj);
	output.vDepthPosition = output.vPos;

	return output;
}

VS_OUTPUT_SHADOW_TEX ShadowCharacterVS(VS_INPUT_ANIM input)
{
	VS_OUTPUT_SHADOW_TEX	output = (VS_OUTPUT_SHADOW_TEX)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	_tagSkinning tSkinning = Skinning(input.vPos, input.vNormal, input.vWeights,
		input.vIndices);

	tSkinning.vPos = tSkinning.vPos - g_vTrLength * g_vPivot;


	output.vPos = mul(float4(tSkinning.vPos, 1.f), g_matWorld);
	output.vPos = mul(output.vPos, g_matLightView);
	output.vPos = mul(output.vPos, g_matLightProj);
	output.vDepthPosition = output.vPos;

	return output;
}

PS_OUTPUT_SINGLE ShadowCharacterPS(VS_OUTPUT_SHADOW_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float depthValue;
	depthValue = input.vDepthPosition.z / input.vDepthPosition.w;

	/*output.vColor = float4(depthValue, depthValue, depthValue, 1.f);
*/

	output.vColor = depthValue;

	return output;
}