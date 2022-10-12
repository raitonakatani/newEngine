#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow {
	SceneLight g_Light;

	void SceneLight::Init()
	{

		//�f�B���N�V�������C�g
		// ���C�g�̃J���[
		m_light.directionlight.dirColor.x = 4.8f;
		m_light.directionlight.dirColor.y = 4.8f;
		m_light.directionlight.dirColor.z = 4.8f;

		//���C�g�̌���
		m_light.directionlight.dirDirection.x = 1.0f;
		m_light.directionlight.dirDirection.y = -1.0f;
		m_light.directionlight.dirDirection.z = -1.0f;
		m_light.directionlight.dirDirection.Normalize();
		m_light.directionlight.castShadow = true;

		//�|�C���g���C�g
		// �|�C���g���C�g�̍��W��ݒ肷��
		m_light.pointlight.ptPosition.x = 0.0f;
		m_light.pointlight.ptPosition.y = 50.0f;
		m_light.pointlight.ptPosition.z = 50.0f;
		// �|�C���g���C�g�̃J���[��ݒ肷��
		m_light.pointlight.ptColor.x = 0.0f;
		m_light.pointlight.ptColor.y = 0.0f;
		m_light.pointlight.ptColor.z = 0.0f;
		// �|�C���g���C�g�̉e���͈͂�ݒ肷��
		m_light.pointlight.ptRange = 0.0f;

		//�X�|�b�g���C�g
		//�X�|�b�g���C�g�̃f�[�^������������
		//�������W��X = 0�AY = 50�AZ = 0�ɂ���B
		m_light.spotlight.spPosition.x = 0.0f;
		m_light.spotlight.spPosition.y = 0.0f;
		m_light.spotlight.spPosition.z = 0.0f;
		//�X�|�b�g���C�g�̃J���[��ݒ�BR = 10�AG = 10�AB = 10�ɂ���B
		m_light.spotlight.spColor.x = 5.0f;
		m_light.spotlight.spColor.y = 5.0f;
		m_light.spotlight.spColor.z = 5.0f;
		//�ˏo�͈͂�300
		m_light.spotlight.spRange = 600.0f;
		//���������͎΂߉��ɂ���B
		m_light.spotlight.spDirection.x = 0.0f;
		m_light.spotlight.spDirection.y = -1.0f;
		m_light.spotlight.spDirection.z = 0.0f;
		//�����f�[�^�Ȃ̂ŁA�傫�����P�ɂ���K�v������̂Ő��K������B
		m_light.spotlight.spDirection.Normalize();
		//�ˏo�p�x��25�x�B
		m_light.spotlight.spAngle = Math::DegToRad(50.0f);

		//����
		m_light.ambientLight.x = 0.2f;
		m_light.ambientLight.y = 0.2f;
		m_light.ambientLight.z = 0.2f;

		//���_�̈ʒu��ݒ肷��
		m_light.eyePos = g_camera3D->GetPosition();

		m_light.specPow = 5.0f;

	}

	SceneLight::~SceneLight()
	{

	}

	void SceneLight::Update()
	{
		// ���_�̈ʒu��ݒ肷��
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
				//�v�Z�����N�H�[�^�j�I���Ń��C�g�̕������񂷁B
				qRotY.Apply(m_light.spotlight.spDirection);

				//X������̉�]�N�H�[�^�j�I�����v�Z����B
				Vector3 rotAxis;
				rotAxis.Cross(g_vec3AxisY, m_light.spotlight.spDirection);
				Quaternion qRotX;
				qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);
				//�v�Z�����N�H�[�^�j�I���Ń��C�g�̕������񂷁B
				qRotX.Apply(m_light.spotlight.spDirection);
				*/
	}
}