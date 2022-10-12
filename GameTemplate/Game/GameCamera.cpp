#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"


namespace
{

	const int PAD_NUMBER = 0;									//ゲームパッドのナンバー
	const int NOMAL_CAMERA = 0;
	const int SPECIAL_CAMERA = 1;

	const float TARGET_YPOSITION = 150.0f;
	const float ANGLE = 2.0f;					//回転角度
	const float MAX_ANGLE = 0.8f;
	const float SET_NEAR = 1.0f;				// 近平面
	const float SET_FAR = 4000.0f;				// 遠平面

	const float RESET_TIMER = 0.0f;
	const float PLUS_TIMER = 0.01f;
	const float MAX_TIMER = 1.5f;


	const Vector3 SPECIAL_CAMERA_POSI = { 0.0f, 80.0f, 400.0f };
	const Vector3 NOMAL_CAMERA_POSI = { 0.0f, 80.0f, -220.0f };
}

GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
	//インスタンスを削除する。
	DeleteGO(this);
}
bool GameCamera::Start()
{

	m_toCameraPos.Set(SPECIAL_CAMERA_POSI);
	//注視点から視点までのベクトルを設定。
	m_toCameraPos2.Set(NOMAL_CAMERA_POSI);
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(SET_NEAR);
	g_camera3D->SetFar(SET_FAR);



	return true;
}
void GameCamera::Update()
{

	//通常カメラ
	Vector3 target2 = m_player->GetPosition();
	//プレイヤの足元からちょっと上を注視点とする。
	target2.y += TARGET_YPOSITION;
	toCameraPosOld2 = m_toCameraPos2;

	//パッドの入力を使ってカメラを回す。
	float x2 = g_pad[PAD_NUMBER]->GetRStickXF();
	float y2 = g_pad[PAD_NUMBER]->GetRStickYF();
	//Y軸周りの回転

	qRot2.SetRotationDeg(Vector3::AxisY, ANGLE * x2);
	qRot2.Apply(m_toCameraPos2);
	//X軸周りの回転。
	Vector3 axisX2;
	axisX2.Cross(Vector3::AxisY, m_toCameraPos2);
	axisX2.Normalize();
	qRot2.SetRotationDeg(axisX2, ANGLE * y2);
	qRot2.Apply(m_toCameraPos2);
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir2 = m_toCameraPos2;
	toPosDir2.Normalize();
	if (toPosDir2.y < -MAX_ANGLE) {
		//カメラが上向きすぎ。
		m_toCameraPos2 = toCameraPosOld2;
	}
	else if (toPosDir2.y > MAX_ANGLE) {
		//カメラが下向きすぎ。
		m_toCameraPos2 = toCameraPosOld2;
	}


		//視点を計算する。
		pos2 = target2 + m_toCameraPos2;
	
		//メインカメラに注視点と視点を設定する。
	g_camera3D->SetPosition(pos2);
	g_camera3D->SetTarget(target2);

	//カメラの更新。
	g_camera3D->Update();

}

void GameCamera::Render(RenderContext& rc)
{

}
