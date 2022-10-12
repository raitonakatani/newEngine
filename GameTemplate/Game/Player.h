#pragma once

class Game;

// �v���C���[�N���X�B
class Player : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render();

	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}


private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();


	// �N���X IGameObject
	Game* m_game = nullptr;							// �Q�[���B
	// �N���X Engine
	ModelRender				m_modelRender;                              // ���f�������_�[
	CharacterController     m_charaCon;	                                // �L�����R��
	// �֐�
	Vector3                 m_forward = Vector3::AxisZ;                 // �O�����̃x�N�g��
	Vector3					m_position;					                // ���W
	Vector3                 m_moveSpeed;                                // �ړ����x
	Vector3					m_scale = g_vec3One;		                // �g�嗦
	Quaternion				m_rotation;					                // ��]
};