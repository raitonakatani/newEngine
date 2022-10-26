#include "stdafx.h"
#include "system/system.h"
#include "ModelStandard.h"
#include "Game.h"

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


    RenderTarget offscreenRenderTarget;
    offscreenRenderTarget.Create(
        FRAME_BUFFER_W,
        FRAME_BUFFER_H,
        1,
        1,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_D32_FLOAT
    );

    ////レンダリングエンジンを初期化
    //RenderingEngine renderingEngine;
    //renderingEngine.Init();

    //// 通常描画のティーポットモデルを初期化
    // // 背景モデルのレンダラーを初期化
    //ModelRender bgModelRender;
    //bgModelRender.InitDeferredRendering(renderingEngine, "Assets/modelData/bg/bg.tkm", true);

    //ModelRender boxModelRender;
    //boxModelRender.InitDeferredRendering(renderingEngine, "Assets/modelData/testModel/box.tkm", true);

    //boxModelRender.UpdateWorldMatrix({ 0.0f,100.0f,0.0f }, g_quatIdentity, g_vec3One);
    // 
    //ModelInitDataFR modelInitData;
    //modelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
    //modelInitData.m_fxFilePath = "Assets/shader/sample.fx";

    //// 拡張SRVにZPrepassで作成された深度テクスチャを指定する
    //modelInitData.m_expandShaderResoruceView[0] = &renderingEngine.GetZPrepassDepthTexture();

    //// 初期化情報を使って描画処理を初期化する
    //ModelRender UnityModelRender;

    //// InitForwardRendering()を利用すると、
    //// フォワードレンダリングの描画パスで描画される
    //UnityModelRender.InitForwardRendering(renderingEngine, modelInitData);

    ////シャドウキャスターフラグをオンにする
    //UnityModelRender.SetShadowCasterFlag(true);
    //
    //UnityModelRender.UpdateWorldMatrix({ 0.0f, 3.0f, 0.0f }, g_quatIdentity, g_vec3One);
    
      //箱
    ModelInitData boxModelInitData;
    boxModelInitData.m_tkmFilePath = "Assets/modelData/testModel/box.tkm";
    boxModelInitData.m_fxFilePath = "Assets/shader/sample3D.fx";
    Model boxModel;
    boxModel.Init(boxModelInitData);
    boxModel.UpdateWorldMatrix({ 0.0f,150.0f,0.0f }, g_quatIdentity, g_vec3One);

    //背景
    ModelInitData bgModelInitData;
    bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
    bgModelInitData.m_fxFilePath = "Assets/shader/sample3D.fx";
    Model bgModel;
    bgModel.Init(bgModelInitData);

    //プレイヤー
    ModelInitData plModelInitdata;
    plModelInitdata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
    plModelInitdata.m_fxFilePath = "Assets/shader/sample3D.fx";
    Model plModel;
    plModel.Init(plModelInitdata);

    
    //インクのテクスチャ
    //これをモデルのテクスチャに塗りたい
    SpriteInitData inkspriteinitdata;
    //DDSファイル(画像データ)のファイルパスを指定する。
    inkspriteinitdata.m_ddsFilePath[0] = "Assets/sprite/inku3.DDS";
    //Sprite表示用のシェーダーのファイルパスを指定する。
    inkspriteinitdata.m_fxFilePath = "Assets/shader/sprite.fx";
    //スプライトの幅と高さを指定する。
    inkspriteinitdata.m_width = FRAME_BUFFER_W;
    inkspriteinitdata.m_height = FRAME_BUFFER_W;
    //Sprite初期化オブジェクトを使用して、Spriteを初期化する。
    Sprite inksprite;
    inksprite.Init(inkspriteinitdata);


    
    //モデルのテクスチャ
    //これにインクを塗ってテクスチャ切り替えをしたい
    SpriteInitData spriteinitdata;
    //DDSファイル(画像データ)のファイルパスを指定する。
    spriteinitdata.m_ddsFilePath[0] = boxModel.GetTkmFile()->m_albedo;
    //Sprite表示用のシェーダーのファイルパスを指定する。
    spriteinitdata.m_fxFilePath = "Assets/shader/Splatoon/inksprite.fx";
    //スプライトの幅と高さを指定する。
    spriteinitdata.m_width = FRAME_BUFFER_W;
    spriteinitdata.m_height = FRAME_BUFFER_H;
    spriteinitdata.m_expandShaderResoruceView[0] = &inksprite.GetTexture(0);
    //Sprite初期化オブジェクトを使用して、Spriteを初期化する。
    Sprite sprite;
//    sprite.Init(spriteinitdata);


    Vector3 startVector = Vector3::Zero;
    Vector3 endVector = Vector3::Zero;
    //0.223011225
    //0.313250005

    Vector3 position;
    position = { 0.0f,200.0f,200.0f };


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

  //     bgModelRender.Draw(true);
  //
  //     // step-2 ティーポットを描画する
  //     UnityModelRender.Draw(true);
  //
  //     renderingEngine.OFFScreenRendering(renderContext);

        //////////////////////////////////////
        // ここから絵を描くコードを記述する
        //////////////////////////////////////

 //       //レンダリングエンジンを実行
 //       renderingEngine.Execute(renderContext);
 
        // step-7 画面に表示されるレンダリングターゲットに各種モデルを描画する
        bgModel.Draw(renderContext);
    //    plModel.UpdateWorldMatrix(position, g_quatIdentity, g_vec3One);
    //    plModel.Draw(renderContext);
        boxModel.Draw(renderContext);

        Vector3 POS;
        Vector2 UV;

        //三角形の座標が入っているリストを持ってくる。
        std::vector<nsK2EngineLow::TkmFile::VectorBuffer> bufferList = boxModel.GetTkmFile()->GetBuffer();
             
                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    startVector = { 0.0f,00.0f,300.0f };
                    endVector = { 0.0f,200.0f,-300.0f };
                    position = endVector;
                }

                //平面と線分の交点を求める。　POS（交点の座標）、vector3d(線分始点)、vector3dend(線分終点)、ポリゴンの3頂点
                if (boxModel.IntersectPlaneAndLine(POS, UV, startVector, endVector, bufferList) == true) {

                    auto Vector = POS;
                    Vector2 UVPOS = UV;
                    
                    boxModel.ChangeAlbedoMap(
                        "",
                        offscreenRenderTarget.GetRenderTargetTexture()
                    );

                    // step-4 レンダリングターゲットをoffscreenRenderTargetに変更する
                    RenderTarget* rtArray[] = { &offscreenRenderTarget };
                    renderContext.WaitUntilToPossibleSetRenderTargets(1, rtArray);
                    renderContext.SetRenderTargets(1, rtArray);
                    renderContext.ClearRenderTargetViews(1, rtArray);

                    // 求めたUV座標をシェーダーに持っていく。
                    if (g_pad[0]->IsPress(enButtonA)) {
                        spriteinitdata.m_posi.x = UVPOS.x;
                        spriteinitdata.m_posi.y = UVPOS.y;
                        sprite.Init(spriteinitdata);
                    };
                    //テクスチャをドロー
                    sprite.Draw(renderContext);

                    renderContext.WaitUntilFinishDrawingToRenderTargets(1, rtArray);
                    // step-6 画面に表示されるレンダリングターゲットに戻す
                    renderContext.SetRenderTarget(
                        g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
                        g_graphicsEngine->GetCurrentFrameBuffuerDSV()
                    );

                }


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
    pos.x -= g_pad[0]->GetLStickXF() * 2.0f;
    target.x -= g_pad[0]->GetLStickXF() * 2.0f;
    pos.y += g_pad[0]->GetRStickYF() * 2.0f;
    target.y += g_pad[0]->GetRStickYF() * 2.0f;

    g_camera3D->SetPosition(pos);
    g_camera3D->SetTarget(target);
}