struct VS_OUTPUT {
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUTPUT main(float4 pos : POSITION, float4 color : COLOR) {
    VS_OUTPUT output;
    output.Pos = pos;
    output.Color = color;
    return output;
}