#include "stdafx.h"
#include "Player.h"
#include "Game.h"
//#include "Magic.h"


// EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

namespace
{
	// Vector3
	const Vector3 MODEL_SCALE = { 1.0f, 1.0f, 1.0f };						//モデルの大きさ
	const Vector3 COLLISION_SCALE = { 20.0f, 20.0f, 120.0f };				//コリジョンの大きさ
	const Vector3 COLLISION_PORK_SCALE = { 40.0f, 30.0f, 140.0f };			//コリジョン（ポーク）の大きさ
	//int
	const int PRIORITY_ZERO = 0;								// プライオリティ 優先権
	const int PAD_NUMBER = 0;									// ゲームパッドの番号
	// float													
	const float COLLISION_PLUS_POSITION = 100.0f;				// コリジョンのポジション
	const float CHARACON_RADIUS = 30.0f;						// キャラコンの半径
	const float CHARACON_HEIGHT = 100.0f;						// キャラコンの高さ
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;				// 移動速度の最低値
	const float WALK_MOVESPEED = 200.0f;						// 歩きステートの移動速度
	const float GRAVITY = 1000.0f;								// 重力
	const float START_MOVE = 0.0f;								// 初期設定のスピード
}

bool Player::Start()
{

	//// モデルの読み込み
	//m_modelRender.Init("Assets/modelData/unityChan.tkm", false);
	//m_modelRender.SetPosition(m_position);
	//m_modelRender.Update();


	// キャラコン
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
	// ゲームのクラスを探してもってくる。
	m_game = FindGO<Game>("game");

	return true;
}

void Player::Update()
{

	//if (g_pad[0]->IsTrigger(enButtonA)) {
	//	//ファイヤーボールを作成する。
	//	Magic* fireBall = NewGO<Magic>(0);
	//	Vector3 fireBallPosition = m_position;
	//	//座標を少し上にする。
	//	fireBallPosition.y += 70.0f;
	//	//座標を設定する。
	//	fireBall->SetPosition(fireBallPosition);
	//	fireBall->SetRotation(m_rotation);
	//	//術者はプレイヤーにする。
	//	fireBall->SetEnMagician(Magic::enMagician_Enemy2);
	//}



	// 移動処理
	Move();
	// 回転処理
	Rotation();

	//// 座標、回転、大きさの更新
	//m_modelRender.SetPosition(m_position);
	//m_modelRender.SetRotation(m_rotation);
	//m_modelRender.SetScale(MODEL_SCALE);

	//// モデルの更新
	//m_modelRender.Update();

}

void Player::Move()
{
	// 移動速度
	m_moveSpeed.x = START_MOVE;
	m_moveSpeed.z = START_MOVE;
	// ステックの入力量を取得
	float lStick_x = g_pad[PAD_NUMBER]->GetLStickXF();
	float lStick_y = g_pad[PAD_NUMBER]->GetLStickYF();
	// cameraの前方向と右方向を取得
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	// XZ平面での前方方向、右方向に変換する
	cameraForward.y = START_MOVE;
	cameraForward.Normalize();
	cameraRight.y = START_MOVE;
	cameraRight.Normalize();

	m_moveSpeed += cameraForward * lStick_y * WALK_MOVESPEED;
	m_moveSpeed += cameraRight * lStick_x * WALK_MOVESPEED;

	// 重力
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	// キャラコンを使用して、座標を更新する
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	// 座標の更新
	Vector3 modelPosition = m_position;
	//m_modelRender.SetPosition(modelPosition);
}

void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) < MOVE_SPEED_MINIMUMVALUE
		&& fabsf(m_moveSpeed.z) < MOVE_SPEED_MINIMUMVALUE) {
		return;
	}
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);
	// 回転を設定する
	//m_modelRender.SetRotation(m_rotation);
	// プレイヤーの正面ベクトルを計算する
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Render()
{
	// モデルをドロー。
	m_modelRender.Draw();
}