SamplerState	sampLinear;

struct PSInput 
{
	//float4 normal : TEXCOORD0;
	float4 position : SV_POSITION;
	float4 color : TEXCOORD0;
//	float2 uv : TEXCOORD2;
//	float distToEye : TEXCOORD3;
};

float4 main(PSInput pixel) : SV_TARGET
{
	return pixel.color;
}