struct VertexInput
{
    float3 position : POSITION;
    float4 color    : TEXCOORD0;
};

struct VertexOutput
{
    float4 sv_position : SV_Position;
    float4 fragColor   : TEXCOORD0;
};

[shader("vertex")]
VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    output.sv_position = float4(input.position, 1.0);
    
    output.fragColor   = input.color;
    
    return output;
}

[shader("pixel")]
float4 fragMain(VertexOutput inVert) : SV_Target
{
    return inVert.fragColor;
}
