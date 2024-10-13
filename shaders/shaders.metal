vertex float4 vertex_main(const device float3* vertexArray [[buffer(0)]], 
                          unsigned int vid [[vertex_id]]) 
{
    return float4(vertexArray[vid], 1.0);
}

fragment float4 fragment_main() 
{
    return float4(0.0, 1.0, 0.0, 1.0);
}
