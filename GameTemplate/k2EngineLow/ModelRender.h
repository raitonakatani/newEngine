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
        void Draw();

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

        //�x�N�g������
        double dot_product(const Vector3& vl, const Vector3 vr) {
            return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
        }

        //�O�p�`�Ɠ_�̓����蔻��
        int HitTestPolygon(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
        {
            Vector3 AB, BP, BC, CP, CA, AP;

            //�x�N�g���̏�Z
            //V1(AB),V2(BC),V3(CA)���v�Z�B
            //P0H(BP),P1H(CP),P2H(AP)���v�Z
            AB.Subtract(B, A);
            BP.Subtract(P, B);

            BC.Subtract(C, B);
            CP.Subtract(P, C);

            CA.Subtract(A, C);
            AP.Subtract(P, A);

            Vector3 c1, c2, c3;

            //�O�ς����߂�B
            //V1P0H(AB*BP),V2P1H(BC*CP),V3P2H(CA*AP)���v�Z
            c1.Cross(AB, BP);
            c2.Cross(BC, CP);
            c3.Cross(CA, AP);

            //���ςŏ��������t���������ׂ�
            double dot_12 = dot_product(c1, c2);
            double dot_13 = dot_product(c1, c3);

            if (dot_12 > 0 && dot_13 > 0) {
                //�O�p�`�̓����ɓ_������
                return 0;
            }

            //�O�p�`�̊O���ɓ_������
            return 1;
        }


        //����AB�ƕ��ʂ̌�_���v�Z����
        bool IntersectPlaneAndLine(
            Vector3& out, //�߂�l�@��_��������Ίi�[�����
            Vector3  A,   //�����n�_
            Vector3  B,   //�����I�_
            std::vector<nsK2EngineLow::TkmFile::VectorBuffer> vectorBuffer
        ) //����
        {
            for (int i = 0;i < vectorBuffer.size();i++)
            {

                Matrix matrix;

                matrix = m_renderToGBufferModel.GetWorldMatrix();

                matrix.Apply(vectorBuffer[i].buffer[0]);
                matrix.Apply(vectorBuffer[i].buffer[1]);
                matrix.Apply(vectorBuffer[i].buffer[2]);



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
                    //�������Ă��Ȃ�
                    continue;
                }

                //�ȉ��A��_�����߂� 

                Vector3 AB = Vector3(B.x - A.x, B.y - A.y, B.z - A.z);

                //��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB
                double hiritu = abs(dot_PA) / (abs(dot_PA) + abs(dot_PB));

                out.x = A.x + (AB.x * hiritu);
                out.y = A.y + (AB.y * hiritu);
                out.z = A.z + (AB.z * hiritu);


                if (HitTestPolygon(vectorBuffer[i].buffer[0], vectorBuffer[i].buffer[1], vectorBuffer[i].buffer[2], out) == 1)
                {
                    continue;
                }

                return true;

            }
            return false;
        }





    private:
        /// <summary>
        /// ���ʂ̏���������
        /// </summary>
        /// <param name="renderingEngine">�����_�����O�G���W��</param>
        /// <param name="tkmFilePath">tkm�t�@�C���p�X</param>
        void InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath);

    private:
        RenderingEngine* m_renderingEngine = nullptr;   //�����_�����O�G���W��
        Model m_zprepassModel;                  // ZPrepass�ŕ`�悳��郂�f��
        Model m_forwardRenderModel;             // �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
        Model m_renderToGBufferModel;           // RenderToGBuffer�ŕ`�悳��郂�f��
        Model m_shadowModels[MAX_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];	//�V���h�E�}�b�v�ɕ`�悷�郂�f��
        bool m_isShadowCaster = false;          // �V���h�E�L���X�^�[�t���O
    };
}