#pragma once

namespace nsK2EngineLow {

    class RenderingEngine;
    /// <summary>
    /// �t�H���[�h�����_�����O�p�̃��f���������\���́B
    /// </summary>
    /// <remark>
    /// ModelInitData���p�����Ă��܂��B
    /// �t�H���[�h�����_�����O�̂��߂ɕK�v�ȃf�t�H���g�ݒ���R���X�g���N�^�ōs���Ă���܂��B
    /// </remark>
    struct ModelInitDataFR : public ModelInitData
    {
        ModelInitDataFR()
        {
            m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
    };
    class ModelRender
    {
    public:

        /// <summary>
        /// ������
        /// </summary>
        /// <remark>
        /// �{�֐��𗘗p���ď��������ꂽ���f���́A
        /// �f�B�t�@�[�h�����_�����O�̕`��p�X�ŕ`�悳��܂�
        /// �f�B�t�@�[�h�����_�����O�ł̕`���PBR�V�F�[�_�[�Ń��C�e�B���O���v�Z����܂�
        /// </remark>
        /// <param name="renderingEngine">�����_�����O�G���W���B</param>
        /// <param name="tkmFilePath">tkm�t�@�C���p�X�B</param>
        /// <param name="isShadowReciever">�V���h�E���V�[�o�[�t���O</param>
        void InitDeferredRendering(RenderingEngine& renderingEngine, const char* tkmFilePath, bool isShadowReciever);

        /// <summary>
        /// ������
        /// </summary>
        /// <remark>
        /// �{�֐��𗘗p���ď��������ꂽ���f����
        /// �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��܂�
        /// ����ȃV�F�[�f�B���O���s�������ꍇ�́A��������g�p���Ă�������
        /// </remark>
        /// <param name="renderingEngine">�����_�����O�G���W��</param>
        /// <param name="modelInitData"></param>
        void InitForwardRendering(RenderingEngine& renderingEngine, ModelInitDataFR& modelInitData);

        /// <summary>
        /// ���[���h�s����X�V����
        /// </summary>
        /// <param name="pos">���W</param>
        /// <param name="rot">��]�N�H�[�^�j�I��</param>
        /// <param name="scale">�g�嗦</param>
        void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
        {
 //           m_zprepassModel.UpdateWorldMatrix(pos, rot, scale);
            if (m_renderToGBufferModel.IsInited())
            {
                m_renderToGBufferModel.UpdateWorldMatrix(pos, rot, scale);
            }
            if (m_forwardRenderModel.IsInited())
            {
                m_forwardRenderModel.UpdateWorldMatrix(pos, rot, scale);
            }
        }

        /// <summary>
        /// �`��
        /// </summary>
        void Draw(bool offscreen =false);

        /// <summary>
        /// �V���h�E�L���X�^�[�̃t���O��ݒ肷��
        /// </summary>
        void SetShadowCasterFlag(bool flag)
        {
            m_isShadowCaster = flag;
        }



		TkmFile* GetTkm()
		{
			return m_renderToGBufferModel.GetTkmFile();
		}

		void ChangeAlbedoMap(const char* materialName, Texture& albedoMap)
		{
			m_renderToGBufferModel.ChangeAlbedoMap(materialName, albedoMap);
		}

//		Sprite* Getsprite()
//		{
//			m_renderToGBufferModel;
//		}


		bool m_isHit;

		//�x�N�g������
		double dot_product(const Vector3& vl, const Vector3 vr) {
			return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
		}

		//�O�p�`�Ɠ_�̓����蔻��
		bool hittest_polygon_3d(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
		{
			Vector3 AB, BP, BC, CP, CA, AP;

			//�x�N�g���̌��Z
			AB.Subtract(B, A);
			BP.Subtract(P, B);

			BC.Subtract(C, B);
			CP.Subtract(P, C);

			CA.Subtract(A, C);
			AP.Subtract(P, A);

			Vector3 c1, c2, c3;

			//�O�ς����߂�
			c1.Cross(AB, BP);
			c2.Cross(BC, CP);
			c3.Cross(CA, AP);

			//���ςŏ��������t���������ׂ�
			double dot_12 = dot_product(c1, c2);
			double dot_13 = dot_product(c1, c3);

			if (dot_12 > 0 && dot_13 > 0) {

				int hit = 0;
				//�O�p�`�̓����ɓ_������
				return true;
			}

			int nohit = 0;
			//�O�p�`�̊O���ɓ_������
			return false;
		}



		//����AB�ƕ��ʂ̌�_���v�Z����
		bool IntersectPlaneAndLine(
			Vector3& out,	//�߂�l�@��_��������Ίi�[����� �Փ˓_
			Vector2& uv,	//�߂�l�@��_��������Ίi�[����� �Փ˂����|���S���̂R���_��UV���W
			Vector3& A,		//�����n�_
			Vector3& B,		//�����I�_
			std::vector<nsK2EngineLow::TkmFile::VectorBuffer>& vectorBuffer
		) //����
		{
			for (int i = 0;i < vectorBuffer.size();i++)
			{


				Matrix matrix;

				//���[���h�s����擾
				matrix = m_renderToGBufferModel.GetWorldMatrix();

				//���_���W�����[���h���W�ɕϊ�
				matrix.Apply(vectorBuffer[i].buffer[0]);
				matrix.Apply(vectorBuffer[i].buffer[1]);
				matrix.Apply(vectorBuffer[i].buffer[2]);

				//�@�������[���h���W�ɕϊ�
				matrix.Apply(vectorBuffer[i].normal);


				//���ʏ�̓_P
				//�_P�P�����_�P�iVector3�j
				Vector3 P = Vector3(vectorBuffer[i].buffer[0]);

				//PA PB�x�N�g��
				Vector3 PA = Vector3(P.x - A.x, P.y - A.y, P.z - A.z);
				Vector3 PB = Vector3(P.x - B.x, P.y - B.y, P.z - B.z);

				//PA PB���ꂼ��𕽖ʖ@���Ɠ���
				double dot_PA = PA.Dot(vectorBuffer[i].normal);
				double dot_PB = PB.Dot(vectorBuffer[i].normal);

				//����͐��[�����ʏ�ɂ��������̌v�Z�̌덷���z�����Ă��܂��B�������Ďg���Ă��������B
				if (abs(dot_PA) < 0.000001) { dot_PA = 0.0; }
				if (abs(dot_PB) < 0.000001) { dot_PB = 0.0; }

				//��������
				if (dot_PA == 0.0 && dot_PB == 0.0) {
					//���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
					continue;
				}
				if ((dot_PA >= 0.0 && dot_PB <= 0.0) ||
					(dot_PA <= 0.0 && dot_PB >= 0.0)) {
					//���ς̕Е����v���X�ŕЕ����}�C�i�X�Ȃ̂ŁA�������Ă���

				}
				else {		
						NOHit();
					//�������Ă��Ȃ�
					continue;
				}

				//�ȉ��A��_�����߂� 

				Vector3 AB = Vector3(B.x - A.x, B.y - A.y, B.z - A.z);

				//��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB
				double hiritu = abs(dot_PA) / (abs(dot_PA) + abs(dot_PB));

				//�Փ˓_�̍��W�����߂�B
				out.x = A.x + (AB.x * hiritu);
				out.y = A.y + (AB.y * hiritu);
				out.z = A.z + (AB.z * hiritu);


				if (hittest_polygon_3d(vectorBuffer[i].buffer[0], vectorBuffer[i].buffer[1], vectorBuffer[i].buffer[2], out) == false)
				{
					continue;
				}

				//
				auto v0v1 = vectorBuffer[i].buffer[1] - vectorBuffer[i].buffer[0];
				auto v0h = out - vectorBuffer[i].buffer[0];
				float z = dot_product(v0v1,v0h);
				z *= 0.5f;

				auto v1v2 = vectorBuffer[i].buffer[2] - vectorBuffer[i].buffer[1];
				auto v1h = out - vectorBuffer[i].buffer[1];
				float x= dot_product(v1v2,v1h);
				x *= 0.5f;

				auto v2v0 = vectorBuffer[i].buffer[0] - vectorBuffer[i].buffer[2];
				auto v2h = out - vectorBuffer[i].buffer[2];
				float y = dot_product(v2v0, v2h);
				y *= 0.5f;


				float xyz = x + y + z;
				float uvx, uvy, uvz;


				uvx = x / xyz;
				uvy = y / xyz;
				uvz = z / xyz;
				
				//���v���P�ɂȂ�
				float a = uvx+uvy+uvz;

				Vector2 Huv0;
				Huv0.x = vectorBuffer[i].uv[0].x * uvx;
				Huv0.y = vectorBuffer[i].uv[0].y * uvx;
				Vector2 Huv1;
				Huv1.x = vectorBuffer[i].uv[1].x * uvy;
				Huv1.y = vectorBuffer[i].uv[1].y * uvy;
				Vector2 Huv2;
				Huv2.x = vectorBuffer[i].uv[2].x * uvz;
				Huv2.y = vectorBuffer[i].uv[2].y * uvz;

				//�Փ˓_��UV���W�����߂�B
				uv.x = Huv0.x + Huv1.x + Huv2.x;
				uv.y = Huv0.y + Huv1.y + Huv2.y;


				HitFlag();

				int ai = 0;
				return true;
			}

			return false;
		}


		void NOHit()
		{
			m_isHit = false;
		}

		void HitFlag()
		{
			m_isHit = true;
		}

		bool IsHitFlag()
		{
			return m_isHit;
		}

		///��肽�����ƃ��X�g�i���Ȃ���΂Ȃ�Ȃ��j
		///���̂P�@���������ꏊ�����߂�@�@�@�@�@�@�@						�ł���		modelRender.h�ōs���Ă���
		///�@�@�@�@�Փ˂����|���S���̂R���_��UV���W�������Ă���				�ł���
		///���̂Q�@�����������ǂ����̔���									�ł���		modelRender.h�ōs���Ă���
		///���̂R�@�C���N���e�N�X�`���ɃI�t�X�N���[�������_�����O			�o���ĂȂ��@�@DrawTexture�N���X�ōs��
		///�@�@�@�@���C�������@�@�@���̎��Ƀe�N�X�`�������ւ�				�ł���
		///�@�@�@�@���C�������@�Փ˓_��UV���W�����߂�@�@�@�@�@�@�@�@�@		�ł���
		///���̂S�@�ł�������I���X�N���[�������_�����O					��������
		///        ���C�������@�@�@�����ւ����e�N�X�`�������ɖ߂�			�܂�����ĂȂ�


		///�I�t�X�N���[�������_�����O��p��draw����i�����_�[�^�[�Q�b�g�̃Z�b�g���܂߁j�֐������
		/// �����_�����Oexecute�֐��̒��ŌĂяo��

		///���̌�@�C���Q�[������
		/// �v���C���[�̃A�j���[�V���������̍쐬
		/// �A�C�h���i�_�����j�@����@���i�~�܂�Ȃ���j�@���i����Ȃ���j
		/// �X�v���Q�̃C�N�����[�h��z�肵�ă}�b�v�����
		/// �Q�[�����e�̓K�`�G���A�@�G�Ɉ��ȏ�w�n��h��ꂽ��Q�[���I�[�o�[
		/// �E�F�[�u�����z������Q�[���N���A
		/// ���X�g��ւ�Ń��b�V�������
		/// 
		/// 
		/// 





    private:
        /// <summary>
        /// ���ʂ̏���������
        /// </summary>
        /// <param name="renderingEngine">�����_�����O�G���W��</param>
        /// <param name="tkmFilePath">tkm�t�@�C���p�X</param>
        void InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath);

    private:
        RenderingEngine* m_renderingEngine = nullptr;   //�����_�����O�G���W��
		Model m_offScreenModel;
        Model m_zprepassModel;                  // ZPrepass�ŕ`�悳��郂�f��
        Model m_forwardRenderModel;             // �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
        Model m_renderToGBufferModel;           // RenderToGBuffer�ŕ`�悳��郂�f��
        Model m_shadowModels[MAX_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];	//�V���h�E�}�b�v�ɕ`�悷�郂�f��
        bool m_isShadowCaster = false;          // �V���h�E�L���X�^�[�t���O
    };
}