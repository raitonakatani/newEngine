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

struct inkpos
{
    Vector2 uv;
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
        1280,
        720,
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
    boxModel.UpdateWorldMatrix({ 0.0f,250.0f,0.0f }, g_quatIdentity, g_vec3One);

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


    inkpos ink;
    ink.uv.x = 0.5f;
    ink.uv.y = 0.5f;


    //やりたいことリスト（やらなければならない）
    //その１　当たった場所を求める　　　　　　　						できた		modelRender.hで行っている
    //　　　　衝突したポリゴンの３頂点のUV座標を持ってくる				できた
    //その２　当たったかどうかの判定									できた		modelRender.hで行っている
    //その３　インクをテクスチャにオフスクリーンレンダリング			出来てない　　DrawTextureクラスで行う
    //　　　　メイン部分　　　その時にテクスチャ差し替え				できた
    //　　　　メイン部分　衝突点のUV座標を求める　　　　　　　　　		できた
    //その４　できたやつをオンスクリーンレンダリング					←今ここ
    //        メイン部分　　　差し替えたテクスチャを元に戻す			まだやってない
    
    
    //インクのテクスチャ
    //これをモデルのテクスチャに塗りたい
    SpriteInitData inkspriteinitdata;
    //DDSファイル(画像データ)のファイルパスを指定する。
    inkspriteinitdata.m_ddsFilePath[0] = "Assets/sprite/blue.DDS";
    //spriteinitdata.m_textures[0] = &TexturerenderTarget.GetRenderTargetTexture();
    //Sprite表示用のシェーダーのファイルパスを指定する。
    inkspriteinitdata.m_fxFilePath = "Assets/shader/sprite.fx";
    //スプライトの幅と高さを指定する。
    inkspriteinitdata.m_width = 256;
    inkspriteinitdata.m_height = 256;
    //Sprite初期化オブジェクトを使用して、Spriteを初期化する。
    Sprite inksprite;
    inksprite.Init(inkspriteinitdata);


    
    //モデルのテクスチャ
    //これにインクを塗ってテクスチャ切り替えをしたい
    
    // ↓今これがしたい
    // m_expandShaderResoruceViewでインクのテクスチャをシェーダーに持っていきたいのにレジスタt10に入っていない
    SpriteInitData spriteinitdata;
    //DDSファイル(画像データ)のファイルパスを指定する。
    spriteinitdata.m_ddsFilePath[0] = "Assets/modelData/testModel/boxtexture.DDS";
    //Sprite表示用のシェーダーのファイルパスを指定する。
    spriteinitdata.m_fxFilePath = "Assets/shader/Splatoon/inksprite.fx";
    //スプライトの幅と高さを指定する。
    spriteinitdata.m_width = 256;
    spriteinitdata.m_height = 256;
  //  spriteinitdata.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
    spriteinitdata.m_expandShaderResoruceView[0] = &inksprite.GetTexture(0);

    //Sprite初期化オブジェクトを使用して、Spriteを初期化する。
    Sprite sprite;
    sprite.Init(spriteinitdata);


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

 //       bgModelRender.Draw();
 //
 //       boxModelRender.Draw();
 //
 //       // step-2 Unityちゃんを描画する
 //       UnityModelRender.Draw();
 //
 //
 //       //レンダリングエンジンを実行
 //       renderingEngine.Execute(renderContext);
 

        // step-7 画面に表示されるレンダリングターゲットに各種モデルを描画する
        bgModel.Draw(renderContext);
        plModel.Draw(renderContext);
        boxModel.Draw(renderContext);



        sprite.Draw(renderContext);


        Vector3 POS;
        Vector2 UV;

        //三角形の座標が入っているリストを持ってくる。
        std::vector<nsK2EngineLow::TkmFile::VectorBuffer> bufferList = boxModel.GetTkmFile()->GetBuffer();

   
        Vector3 startVector=Vector3::Zero;
        Vector3 endVector= Vector3::Zero;
             
                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    startVector = { 0.0f,250.0f,-200.0f };
                    endVector = { 0.0f,450.0f,200.0f };
                }
                //inksprite.Draw(renderContext);

                //平面と線分の交点を求める。　POS（交点の座標）、vector3d(線分始点)、vector3dend(線分終点)、ポリゴンの3頂点
                if (boxModel.IntersectPlaneAndLine(POS, UV, startVector, endVector, bufferList) == true) {

                    auto Vector = POS;
                    //ink.uv = UV;
                    auto Vector2 = POS;

                    inkspriteinitdata.m_ddsFilePath[0] = "Assets/sprite/blue.DDS";
                   // inkspriteinitdata.m_fxFilePath = "Assets/shader/sprite.fx";
                    inksprite.Init(inkspriteinitdata);


                    boxModel.ChangeAlbedoMap(
                        "",
                        inksprite.GetTexture(0)
                    );

                    // step-4 レンダリングターゲットをoffscreenRenderTargetに変更する
                    RenderTarget* rtArray[] = { &offscreenRenderTarget };
                    renderContext.WaitUntilToPossibleSetRenderTargets(1, rtArray);
                    renderContext.SetRenderTargets(1, rtArray);
                    renderContext.ClearRenderTargetViews(1, rtArray);

                    // step-5 offscreenRenderTargetに背景、プレイヤーを描画する
                    //bgModel.Draw(renderContext);
                    //plModel.Draw(renderContext);
                    inksprite.Draw(renderContext);

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