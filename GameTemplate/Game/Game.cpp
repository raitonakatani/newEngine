#include "stdafx.h"
#include "Game.h"

namespace
{
	const Vector3 PLAYER_SET_POSITION = { 0.0f,0.0f,-400.0f };	//�v���C���[�̃Z�b�g�|�W�V����
	const Vector3 FLOOR_SET_POSITION = { 500.0f,-1.0f,500.0f };		//���̃Z�b�g�|�W�V����


	const int PRIORITY_ZERO = 0;								//�v���C�I���e�B �D�挠
	const int SOUND_NUMBER = 6;									//�T�E���h�̃i���o�[
	const int TITLE = 0;										//�^�C�g��
	const int GAME_START = 1;									//�Q�[���X�^�[�g
	const int GAME_PLAY = 2;									//�v���C��
	const int GAME_CREAR = 1;									//�Q�[���N���A
	const int HAVE_TREASURE = 4;								//�󔠂������Ă���

	const float SOUND_VOLUME = 0.3f;							//�T�E���h�̃{�����[��
	const float PLUS_COLOR = 0.015f;							//���Z����J���[�̒l
	const float MAX_AMBIENT_COLOR = 2.0f;						//�����̍ő�l
	const float SPRITE_COLOR = 1.0f;							//�X�v���C�g�̃J���[�l
	const float PLUS_ALPHA = 0.2f;								//��Z���郿�l
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


	// �����蔻��̕`��
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