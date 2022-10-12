#pragma once

class Game;

// プレイヤークラス。
class Player : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render();

	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}


private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();


	// クラス IGameObject
	Game* m_game = nullptr;							// ゲーム。
	// クラス Engine
	ModelRender				m_modelRender;                              // モデルレンダー
	CharacterController     m_charaCon;	                                // キャラコン
	// 関数
	Vector3                 m_forward = Vector3::AxisZ;                 // 前方向のベクトル
	Vector3					m_position;					                // 座標
	Vector3                 m_moveSpeed;                                // 移動速度
	Vector3					m_scale = g_vec3One;		                // 拡大率
	Quaternion				m_rotation;					                // 回転
};