#include "stdafx.h"
#include "system/system.h"
#include "ModelStandard.h"


// 関数宣言
void InitRootSignature(RootSignature& rs);
void MoveCamera();
void InitLightCamera(Camera& lightCamera);

/// <summary>
/// ディレクショナルライト
/// </summary>
struct DirectionalLight
{
	Matrix viewProjInvMatrix;
	Vector3  color;
	float pad0;         // パディング
	Vector3  direction;
	float pad1;         // パディング
	Vector3 eyePos;     // 視点
	float specPow;      // スペキュラの絞り
};



// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
    g_camera3D->SetPosition(0, 100.0f, 350.0f);
    g_camera3D->SetTarget(0, 100.0f, 0);
    RootSignature rs;
    InitRootSignature(rs);


    //レンダリングエンジンを初期化
    RenderingEngine renderingEngine;
    renderingEngine.Init();

    // 通常描画のティーポットモデルを初期化
       // 背景モデルのレンダラーを初期化
    ModelRender bgModelRender;
    bgModelRender.InitDeferredRendering(renderingEngine, "Assets/modelData/bg/bg.tkm", true);

    ModelInitDataFR modelInitData;
    modelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
    modelInitData.m_fxFilePath = "Assets/shader/sample.fx";

    // 拡張SRVにZPrepassで作成された深度テクスチャを指定する
    modelInitData.m_expandShaderResoruceView[0] = &renderingEngine.GetZPrepassDepthTexture();

    // 初期化情報を使って描画処理を初期化する
    ModelRender teapotModelRender;

    // InitForwardRendering()を利用すると、
    // フォワードレンダリングの描画パスで描画される
    teapotModelRender.InitForwardRendering(renderingEngine, modelInitData);

    //シャドウキャスターフラグをオンにする
    teapotModelRender.SetShadowCasterFlag(true);

    teapotModelRender.UpdateWorldMatrix({ 0.0f, 3.0f, 0.0f }, g_quatIdentity, g_vec3One);



    //////////////////////////////////////
    // 初期化を行うコードを書くのはここまで！！！
    //////////////////////////////////////
    auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
    while (DispatchWindowMessage())
    {
        // フレームの開始時に呼び出す必要がある処理を実行
        g_k2EngineLow->BeginFrame();

        // カメラを動かす
        MoveCamera();

        //////////////////////////////////////
        // ここから絵を描くコードを記述する
        //////////////////////////////////////

        bgModelRender.Draw();

        // step-2 ティーポットを描画する
        teapotModelRender.Draw();


        //レンダリングエンジンを実行
        renderingEngine.Execute(renderContext);


        // ゲームオブジェクトマネージャーの更新処理を呼び出す。
        g_k2EngineLow->ExecuteUpdate();

        // ゲームオブジェクトマネージャーの描画処理を呼び出す。
        g_k2EngineLow->ExecuteRender();

        // デバッグ描画処理を実行する。
        g_k2EngineLow->DebubDrawWorld();

        // フレームの終了時に呼び出す必要がある処理を実行。
        g_k2EngineLow->EndFrame();
    }

	delete g_k2EngineLow;

	return 0;
}

void InitLightCamera(Camera& lightCamera)
{
    // ライトまでのベクトル
    Vector3 toLigDir = { 1, 2, 1 };
    toLigDir.Normalize();
    toLigDir *= 5000.0f; // ライトまでの距離

    // カメラの位置を設定。これはライトの位置
    lightCamera.SetPosition(toLigDir);

    // カメラの注視点を設定。これがライトが照らしている場所
    lightCamera.SetTarget({ 0, 0, 0 });

    // 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
    lightCamera.SetUp(1, 0, 0);

    lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
    lightCamera.SetWidth(5000.0f);
    lightCamera.SetHeight(5000.0f);
    lightCamera.SetNear(1.0f);
    lightCamera.SetFar(10000.0f);

    // ライトビュープロジェクション行列を計算している
    lightCamera.Update();
}

// ルートシグネチャの初期化
void InitRootSignature(RootSignature& rs)
{
    rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}

void MoveCamera()
{
    auto pos = g_camera3D->GetPosition();
    auto target = g_camera3D->GetTarget();
    pos.z -= g_pad[0]->GetLStickYF() * 2.0f;
    target.z -= g_pad[0]->GetLStickYF() * 2.0f;
    pos.y += g_pad[0]->GetRStickYF() * 2.0f;
    target.y += g_pad[0]->GetRStickYF() * 2.0f;
    g_camera3D->SetPosition(pos);
    g_camera3D->SetTarget(target);
}