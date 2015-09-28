float4x4 gWVP;
texture gTex;

sampler TexS = sampler_state {
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

struct OutputVS {
    float4 posH   : POSITION0;
	float2 tex0   : TEXCOORD0;
	float4 color  : COLOR0;
};


OutputVS TTCVVS(float4 posL : POSITION0, float2 tex0: TEXCOORD0,float4 color : COLOR0) {
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;	
	// we are already in screen space
	outVS.posH = posL;
	outVS.tex0 = tex0; 
	outVS.color = color;
    return outVS;
}

float4 TTCVPS(float2 texCoord : TEXCOORD0,float4 color : COLOR0) : COLOR {    	
	float4 t = tex2D(TexS, texCoord);
    return color * t;     
}

technique TTCVTech {

    pass P0 {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 TTCVVS();
        pixelShader  = compile ps_2_0 TTCVPS();
    }
}
