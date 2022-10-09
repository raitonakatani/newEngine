/*!
 * @brief シンプルなモデル表示用のシェーダー
 */


struct Directionlight
{
	   // ディレクションライト用のデータ
    float3 dirDirection; // ライトの方向
    float3 dirColor; // ライトのカラー
};

struct Pointlight
{
    float3 ptPosition; //ポイントライトの位置。
    float3 ptColor; //ポイントライトのカラー。
    float ptRange; //ポイントライトの影響範囲。
};

struct Spotlight
{
    float3 spPosition; //スポットライトの位置。
    float3 spColor; //スポットライトのカラー。
    float spRange; //スポットライトの射出範囲。
    float3 spDirection; //スポットライトの射出方向。
    float spAngle; //スポットライトの射出角度。
};

// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

cbuffer LightCb : register(b1)
{
    Directionlight directionlight; //ディレクションライト
    Pointlight pointlight; //ポイントライト
    Spotlight spotlight; //スポットライト
    float3 ambientLight; //環境光。
    float3 eyePos; //視点の位置。
    float4x4 mLVP;
};


// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 worldPos : TEXCOORD1; // ワールド座標
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0; // アルベド
    float3 normal : SV_Target1; // 法線
    float3 worldPos : SV_Target2; // ワールド座標
};

// モデルテクスチャ
Texture2D<float4> g_texture : register(t0);

// サンプラーステート
sampler g_sampler : register(s0);

/// <summary>
/// モデル用の頂点シェーダーのエントリーポイント
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos); // モデルの頂点をワールド座標系に変換

    //頂点シェーダーからワールド座標を出力
    psIn.worldPos = psIn.pos;

    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
    psIn.normal = normalize(mul(mWorld, vsIn.normal));
    psIn.uv = vsIn.uv;

    return psIn;
}

/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
SPSOut PSMain(SPSIn psIn)
{
    //G-Bufferに出力
    SPSOut psOut;

    //アルベドカラーを出力
    psOut.albedo = g_texture.Sample(g_sampler, psIn.uv);

    //法線を出力
    psOut.normal = psIn.normal;

    psOut.worldPos = psIn.worldPos;
    return psOut;
}

// step-4 半透明オブジェクト用のピクセルシェーダーを実装
float4 PSMainTrans(SPSIn psIn) : SV_Target0
{
    //フォワードレンダリング
    //普通にライティングをする
    //拡散反射光を計算
    float3 lig = 0.0f;
    float3 normal = psIn.normal;
    float t = max(0.0f, dot(psIn.normal, directionlight.dirDirection) * -1.0f);
    lig = directionlight.dirColor* t;

    float4 finalColor = g_texture.Sample(g_sampler, psIn.uv);
    finalColor.xyz *= lig;
    return finalColor;
}
