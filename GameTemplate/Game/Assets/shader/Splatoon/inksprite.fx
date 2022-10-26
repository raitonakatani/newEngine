
cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
    float4 screenParam;
    float2 uvposi;//求めたUV座標
};



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
Texture2D<float4> g_ink : register(t20); //  インク
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
    //求めたUV座標
    float2 inkUV = uvposi;
   //float2 inkUV = psIn.uv;

    //インクのテクスチャ
    float3 inkTextre = g_ink.Sample(g_sampler, inkUV);
    
    //モデルのテクスチャ
    float4 color = g_albedo.Sample(g_sampler, psIn.uv) * mulColor;

    //インクを塗る
    color.xyz *= inkTextre;

    return color;
}
