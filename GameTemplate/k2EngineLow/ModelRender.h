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


        struct PL
        {
            Vector3 Buffer[3];
        };

        std::vector<PL> m_buffer;



        //���_�̒�`(�x�N�g���Ɠ���)


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
                for (int y = 0;y < vectorBuffer.size();y++)
                {

                    //���ʏ�̓_P
                    //�_P�P�����_�P�iVector3�j
                    Vector3 P = Vector3(vectorBuffer[i].buffer[y]);

                    //PA PB�x�N�g��
                    Vector3 PA = Vector3(P.x - A.x, P.y - A.y, P.z - A.z);
                    Vector3 PB = Vector3(P.x - B.x, P.y - B.y, P.z - B.z);

                    //PA PB���ꂼ��𕽖ʖ@���Ɠ���
                    double dot_PA = PA.x * vectorBuffer[i].buffer[y].x + PA.y * vectorBuffer[i].buffer[y].y + PA.z * vectorBuffer[i].buffer[y].z;
                    double dot_PB = PB.x * vectorBuffer[i].buffer[y].x + PB.y * vectorBuffer[i].buffer[y].y + PB.z * vectorBuffer[i].buffer[y].z;

                    //����͐��[�����ʏ�ɂ��������̌v�Z�̌덷���z�����Ă��܂��B�������Ďg���Ă��������B
                    if (abs(dot_PA) < 0.000001) { dot_PA = 0.0; }
                    if (abs(dot_PB) < 0.000001) { dot_PB = 0.0; }

                    //��������
                    if (dot_PA == 0.0 && dot_PB == 0.0) {
                        //���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
                        return false;
                    }
                    else
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

                    return true;
                }
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