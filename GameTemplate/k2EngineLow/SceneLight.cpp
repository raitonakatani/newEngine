#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow {
	SceneLight g_Light;

	void SceneLight::Init()
	{

		//ディレクションライト
		// ライトのカラー
		m_light.directionlight.dirColor.x = 4.8f;
		m_light.directionlight.dirColor.y = 4.8f;
		m_light.directionlight.dirColor.z = 4.8f;

		//ライトの向き
		m_light.directionlight.dirDirection.x = 1.0f;
		m_light.directionlight.dirDirection.y = -1.0f;
		m_light.directionlight.dirDirection.z = -1.0f;
		m_light.directionlight.dirDirection.Normalize();
		m_light.directionlight.castShadow = true;

		//ポイントライト
		// ポイントライトの座標を設定する
		m_light.pointlight.ptPosition.x = 0.0f;
		m_light.pointlight.ptPosition.y = 50.0f;
		m_light.pointlight.ptPosition.z = 50.0f;
		// ポイントライトのカラーを設定する
		m_light.pointlight.ptColor.x = 0.0f;
		m_light.pointlight.ptColor.y = 0.0f;
		m_light.pointlight.ptColor.z = 0.0f;
		// ポイントライトの影響範囲を設定する
		m_light.pointlight.ptRange = 0.0f;

		//スポットライト
		//スポットライトのデータを初期化する
		//初期座標はX = 0、Y = 50、Z = 0にする。
		m_light.spotlight.spPosition.x = 0.0f;
		m_light.spotlight.spPosition.y = 0.0f;
		m_light.spotlight.spPosition.z = 0.0f;
		//スポットライトのカラーを設定。R = 10、G = 10、B = 10にする。
		m_light.spotlight.spColor.x = 5.0f;
		m_light.spotlight.spColor.y = 5.0f;
		m_light.spotlight.spColor.z = 5.0f;
		//射出範囲は300
		m_light.spotlight.spRange = 600.0f;
		//初期方向は斜め下にする。
		m_light.spotlight.spDirection.x = 0.0f;
		m_light.spotlight.spDirection.y = -1.0f;
		m_light.spotlight.spDirection.z = 0.0f;
		//方向データなので、大きさを１にする必要があるので正規化する。
		m_light.spotlight.spDirection.Normalize();
		//射出角度は25度。
		m_light.spotlight.spAngle = Math::DegToRad(50.0f);

		//環境光
		m_light.ambientLight.x = 0.2f;
		m_light.ambientLight.y = 0.2f;
		m_light.ambientLight.z = 0.2f;

		//視点の位置を設定する
		m_light.eyePos = g_camera3D->GetPosition();

		m_light.specPow = 5.0f;

	}

	SceneLight::~SceneLight()
	{

	}

	void SceneLight::Update()
	{
		// 視点の位置を設定する
		m_light.eyePos = g_camera3D->GetPosition();

		/*		if (g_pad[0]->IsPress(enButtonX))
				{
					posi--;
				}
				if (g_pad[0]->IsPress(enButtonY))
				{
					posi++;
				}

				m_light.pointlight.ptPosition = { posi, 50.0f, 0.0f };

				Quaternion qRotY;
				qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
				//計算したクォータニオンでライトの方向を回す。
				qRotY.Apply(m_light.spotlight.spDirection);

				//X軸周りの回転クォータニオンを計算する。
				Vector3 rotAxis;
				rotAxis.Cross(g_vec3AxisY, m_light.spotlight.spDirection);
				Quaternion qRotX;
				qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);
				//計算したクォータニオンでライトの方向を回す。
				qRotX.Apply(m_light.spotlight.spDirection);
				*/
	}
}