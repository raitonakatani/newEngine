
struct inkpos
{
    float3 worldPOS;
    float2 uv : uv;
};

cbuffer cb : register(b0)
{
    float4x4 mvp; 
    float4 mulColor;
    float4x4 mView;
    float4x4 mProj;
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
    float2 inkpos : TEXCOORD02;
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
    
    psIn.inkpos = ink.uv;
    
    //ワールド行列に変換
    //psIn.inkpos = float4(ink.worldPOS, 1.0f);
    //psIn.inkpos = mul(mvp, vsIn.pos);
    //psIn.inkpos = mul(mView, psIn.pos);
    //psIn.inkpos = mul(mProj, psIn.pos);

    
    return psIn;
}

/// <summary>
/// インクが落とされる用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float2 inkUV = psIn.uv;
    inkUV.x -= 0.1f;
    inkUV.y += 0.1f;
    
    if (inkUV.x ==0.5f
        || inkUV.y ==0.5f)
    {
   //     psIn.inkpos += psIn.uv;
    }
    
//    inkUV /= 2;
   // inkUV.x =0.5f;
   // inkUV.y =0.5f;
//    float3 inkTextre = 0.0f;
    float3 inkTextre = g_ink.Sample(g_sampler, psIn.inkpos);
 //   float4 inkTextre = g_ink.Sample(g_sampler, ink.uv) * mulColor;
    
    float4 color = g_albedo.Sample(g_sampler, psIn.uv) * mulColor;

  //  color.xyz *= inkTextre;
    return color;
}
