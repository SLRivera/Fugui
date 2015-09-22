cbuffer Matrices
{
	matrix view;
	matrix projection;
	matrix world;
};

struct PSInput
{
	float4 posH : SV_POSITION;
	//float4 normal : TEXCOORD0;
	float4 color : TEXCOORD0;
	//float2 uv : TEXCOORD2;
	//float distToEye : TEXCOORD3;
};

struct VSInputVPNT
{
	float3 posL : POSITION;
	float3 normalL : NORMAL;
	float2 uv : TEXCOORD0;
};

struct VSInputVPNTBT
{
	float3 posL : POSITION;
	float3 normalL : NORMAL;
	float3 tangentL : TANGENT;
	float3 binormL : BINORMAL;
	float2 uv : TEXCOORD0;
};


PSInput main(VSInputVPNT pixel)
{
	PSInput outpix = (PSInput)0;
	matrix temp = mul(view, projection);
	temp = mul(world, temp);
	outpix.posH = mul(float4(pixel.posL, 1.0f), temp);
//	outpix.posH = mul(float4(pixel.posL, 1.0f), world);
//	outpix.posH = mul(outpix.posH, view);
//	outpix.posH = mul(outpix.posH, projection);
	outpix.color.rgb = float3( 1.0f, 0.0f, 0.0f );
	outpix.color.a = 1.0f;
	return outpix;
}