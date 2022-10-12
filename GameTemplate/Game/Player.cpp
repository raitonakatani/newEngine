#include "stdafx.h"
#include "Player.h"
#include "Game.h"
//#include "Magic.h"


// EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

namespace
{
	// Vector3
	const Vector3 MODEL_SCALE = { 1.0f, 1.0f, 1.0f };						//���f���̑傫��
	const Vector3 COLLISION_SCALE = { 20.0f, 20.0f, 120.0f };				//�R���W�����̑傫��
	const Vector3 COLLISION_PORK_SCALE = { 40.0f, 30.0f, 140.0f };			//�R���W�����i�|�[�N�j�̑傫��
	//int
	const int PRIORITY_ZERO = 0;								// �v���C�I���e�B �D�挠
	const int PAD_NUMBER = 0;									// �Q�[���p�b�h�̔ԍ�
	// float													
	const float COLLISION_PLUS_POSITION = 100.0f;				// �R���W�����̃|�W�V����
	const float CHARACON_RADIUS = 30.0f;						// �L�����R���̔��a
	const float CHARACON_HEIGHT = 100.0f;						// �L�����R���̍���
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;				// �ړ����x�̍Œ�l
	const float WALK_MOVESPEED = 200.0f;						// �����X�e�[�g�̈ړ����x
	const float GRAVITY = 1000.0f;								// �d��
	const float START_MOVE = 0.0f;								// �����ݒ�̃X�s�[�h
}

bool Player::Start()
{

	//// ���f���̓ǂݍ���
	//m_modelRender.Init("Assets/modelData/unityChan.tkm", false);
	//m_modelRender.SetPosition(m_position);
	//m_modelRender.Update();


	// �L�����R��
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
	// �Q�[���̃N���X��T���Ă����Ă���B
	m_game = FindGO<Game>("game");

	return true;
}

void Player::Update()
{

	//if (g_pad[0]->IsTrigger(enButtonA)) {
	//	//�t�@�C���[�{�[�����쐬����B
	//	Magic* fireBall = NewGO<Magic>(0);
	//	Vector3 fireBallPosition = m_position;
	//	//���W��������ɂ���B
	//	fireBallPosition.y += 70.0f;
	//	//���W��ݒ肷��B
	//	fireBall->SetPosition(fireBallPosition);
	//	fireBall->SetRotation(m_rotation);
	//	//�p�҂̓v���C���[�ɂ���B
	//	fireBall->SetEnMagician(Magic::enMagician_Enemy2);
	//}



	// �ړ�����
	Move();
	// ��]����
	Rotation();

	//// ���W�A��]�A�傫���̍X�V
	//m_modelRender.SetPosition(m_position);
	//m_modelRender.SetRotation(m_rotation);
	//m_modelRender.SetScale(MODEL_SCALE);

	//// ���f���̍X�V
	//m_modelRender.Update();

}

void Player::Move()
{
	// �ړ����x
	m_moveSpeed.x = START_MOVE;
	m_moveSpeed.z = START_MOVE;
	// �X�e�b�N�̓��͗ʂ��擾
	float lStick_x = g_pad[PAD_NUMBER]->GetLStickXF();
	float lStick_y = g_pad[PAD_NUMBER]->GetLStickYF();
	// camera�̑O�����ƉE�������擾
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	// XZ���ʂł̑O�������A�E�����ɕϊ�����
	cameraForward.y = START_MOVE;
	cameraForward.Normalize();
	cameraRight.y = START_MOVE;
	cameraRight.Normalize();

	m_moveSpeed += cameraForward * lStick_y * WALK_MOVESPEED;
	m_moveSpeed += cameraRight * lStick_x * WALK_MOVESPEED;

	// �d��
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	// �L�����R�����g�p���āA���W���X�V����
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	// ���W�̍X�V
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
	// ��]��ݒ肷��
	//m_modelRender.SetRotation(m_rotation);
	// �v���C���[�̐��ʃx�N�g�����v�Z����
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Render()
{
	// ���f�����h���[�B
	m_modelRender.Draw();
}