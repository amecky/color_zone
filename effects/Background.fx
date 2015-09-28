// ===========================================
// Simple effect to texture the given object
// ===========================================

float4x4 gWVP;
texture gTex;
float3 position;
float timer = 0.0;
float alpha = 0.3;
float3 colorScale = { 0.8, 0.1, 0.0 };

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
};


OutputVS TextureVS(float4 posL : POSITION0, float2 tex0: TEXCOORD0) {
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;	
	// we are already in screen space
	outVS.posH = posL;
	outVS.tex0 = tex0; 
    return outVS;
}

float4 TexturePS(float2 texCoord : TEXCOORD0) : COLOR {    
	texCoord.x += position.x * 0.00025f; 
    texCoord.y += position.y * 0.00025f;
    texCoord *= 0.5f;
	float3 col = tex2D(TexS, texCoord);	
	float ar = 0.4 + sin(timer)*0.2;
	col.r *= ar;
	float ab = 0.8 + sin(timer*0.5)*0.2;
	col.b *= ab;
	return float4(col.r,0.0,col.b, alpha);	
}

technique BackgroundTech {

    pass P0 {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_3_0 TextureVS();
        pixelShader  = compile ps_3_0 TexturePS();
		//FillMode = Wireframe;
    }
}
