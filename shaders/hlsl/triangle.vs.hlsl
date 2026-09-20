static const float2 positions[3] =
{
    float2(0.0, -0.5),
    float2(0.5,  0.5),
    float2(-0.5, 0.5)
};

static const float3 colors[3] =
{
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
};

struct VertexOutput
{
    float4 sv_position : SV_Position;
    float3 color       : COLOR0;
};

[shader("vertex")]
VertexOutput main(uint vid : SV_VertexID)
{
    VertexOutput output;
    output.sv_position = float4(positions[vid], 0.0, 1.0);
    output.color = colors[vid];
    return output;
}

[shader("pixel")]
float4 fragMain(VertexOutput inVert) : SV_Target
{
    return float4(inVert.color, 1.0);
}