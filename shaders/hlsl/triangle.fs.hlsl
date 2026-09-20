struct VertexOutput
{
    float4 sv_position : SV_Position;
    float3 color       : COLOR0;
};

[shader("pixel")]
float4 main(VertexOutput inVert) : SV_Target
{
    return float4(inVert.color, 1.0);
}