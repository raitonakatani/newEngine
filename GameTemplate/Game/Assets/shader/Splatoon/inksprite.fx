
struct inkpos
{
    float2 uv;
};

cbuffer cb : register(b0)
{
    float4x4 mvp; 
    float4 mulColor;
};

cbuffer inkposition : register(b1)
{
    inkpos ink;
}

struct SVSIn
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct SPSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0); // アルベドマップ
Texture2D<float4> g_ink : register(t10); //  インク
sampler g_sampler : register(s0); // サンプラーステート

/// <summary>
/// インクが落とされる用の頂点シェーダー
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    //座標変換
    SPSIn psIn;
    psIn.pos = mul(mvp, vsIn.pos);
    psIn.uv = vsIn.uv;
    

    
    return psIn;
}

/// <summary>
/// インクが落とされる用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float2 inkUV = psIn.uv;

    inkUV.x += 0.5f;
    inkUV.y += 0.5f;
    
    
//    float3 inkTextre = 0.0f;
    float4 inkTextre = g_ink.Sample(g_sampler, psIn.uv) * mulColor;
    
    float4 color = g_albedo.Sample(g_sampler, psIn.uv) * mulColor;

//    color = float4(inkTextre, 1.0f);
    return inkTextre;
}
