#include "stdafx.h"
#include "Game.h"

namespace
{
	const Vector3 PLAYER_SET_POSITION = { 0.0f,0.0f,-400.0f };	//プレイヤーのセットポジション
	const Vector3 FLOOR_SET_POSITION = { 500.0f,-1.0f,500.0f };		//床のセットポジション


	const int PRIORITY_ZERO = 0;								//プライオリティ 優先権
	const int SOUND_NUMBER = 6;									//サウンドのナンバー
	const int TITLE = 0;										//タイトル
	const int GAME_START = 1;									//ゲームスタート
	const int GAME_PLAY = 2;									//プレイ中
	const int GAME_CREAR = 1;									//ゲームクリア
	const int HAVE_TREASURE = 4;								//宝箱を持っている

	const float SOUND_VOLUME = 0.3f;							//サウンドのボリューム
	const float PLUS_COLOR = 0.015f;							//加算するカラーの値
	const float MAX_AMBIENT_COLOR = 2.0f;						//環境光の最大値
	const float SPRITE_COLOR = 1.0f;							//スプライトのカラー値
	const float PLUS_ALPHA = 0.2f;								//乗算するα値
	const float PLUS_DEATH_TIMER = 1.0f;
	const float DEATH_TIMER = 2.0f;
	const float MAX_DEATH_TIMER = 3.5f;
}

Game::~Game()
{
}
bool Game::Start()
{

	//	ModelRender model;
//	model.Init("Assets/modelData/unityChan.tkm", false);


	// 当たり判定の描画
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

	//	model.Draw(rc);

}