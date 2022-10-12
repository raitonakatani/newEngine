#include "stdafx.h"
#include "system/system.h"
#include "ModelStandard.h"
#include "Game.h"

// �֐��錾
void InitRootSignature(RootSignature& rs);
void MoveCamera();
void InitLightCamera(Camera& lightCamera);

/// <summary>
/// �f�B���N�V���i�����C�g
/// </summary>
struct DirectionalLight
{
	Matrix viewProjInvMatrix;
	Vector3  color;
	float pad0;         // �p�f�B���O
	Vector3  direction;
	float pad1;         // �p�f�B���O
	Vector3 eyePos;     // ���_
	float specPow;      // �X�y�L�����̍i��
};



// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	// k2EngineLow�̏������B
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
    g_camera3D->SetPosition(0, 100.0f, 350.0f);
    g_camera3D->SetTarget(0, 100.0f, 0);
    RootSignature rs;
    InitRootSignature(rs);


    //�����_�����O�G���W����������
    RenderingEngine renderingEngine;
    renderingEngine.Init();

    // �ʏ�`��̃e�B�[�|�b�g���f����������
       // �w�i���f���̃����_���[��������
    ModelRender bgModelRender;
    bgModelRender.InitDeferredRendering(renderingEngine, "Assets/modelData/bg/bg.tkm", true);

    ModelRender boxModelRender;
    boxModelRender.InitDeferredRendering(renderingEngine, "Assets/modelData/testModel/box.tkm", true);

    boxModelRender.UpdateWorldMatrix({ 0.0f,200.0f,0.0f }, g_quatIdentity, g_vec3One);

    ModelInitDataFR modelInitData;
    modelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
    modelInitData.m_fxFilePath = "Assets/shader/sample.fx";

    // �g��SRV��ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`�����w�肷��
    modelInitData.m_expandShaderResoruceView[0] = &renderingEngine.GetZPrepassDepthTexture();

    // �����������g���ĕ`�揈��������������
    ModelRender teapotModelRender;

    // InitForwardRendering()�𗘗p����ƁA
    // �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳���
    teapotModelRender.InitForwardRendering(renderingEngine, modelInitData);

    //�V���h�E�L���X�^�[�t���O���I���ɂ���
    teapotModelRender.SetShadowCasterFlag(true);

    teapotModelRender.UpdateWorldMatrix({ 0.0f, 3.0f, 0.0f }, g_quatIdentity, g_vec3One);



    //////////////////////////////////////
    // ���������s���R�[�h�������̂͂����܂ŁI�I�I
    //////////////////////////////////////
    auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
    while (DispatchWindowMessage())
    {
        // �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
        g_k2EngineLow->BeginFrame();

        // �J�����𓮂���
        MoveCamera();

        //////////////////////////////////////
        // ��������G��`���R�[�h���L�q����
        //////////////////////////////////////

        bgModelRender.Draw();

        boxModelRender.Draw();

        // step-2 �e�B�[�|�b�g��`�悷��
        teapotModelRender.Draw();


        //�����_�����O�G���W�������s
        renderingEngine.Execute(renderContext);


        Vector3 POS;

        //�O�p�`�̍��W�������Ă��郊�X�g�������Ă���B
        std::vector<nsK2EngineLow::TkmFile::VectorBuffer> bufferList = boxModelRender.GetTkm()->GetBuffer();

   
                Vector3 startVector;
                Vector3 endVector;
                
                startVector = {0.0f,200.0f,-500.0f};
                endVector = { 0.0f,200.0f,500.0f };

                //���ʂƐ����̌�_�����߂�B�@POS�i��_�̍��W�j�Avector3d(�����n�_)�Avector3dend(�����I�_)�A�|���S����3���_
                boxModelRender.IntersectPlaneAndLine(POS, startVector, endVector, bufferList);

                auto Vector = POS;

                auto Vector2 = POS;








        // �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
        g_k2EngineLow->ExecuteUpdate();

        // �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
        g_k2EngineLow->ExecuteRender();

        // �f�o�b�O�`�揈�������s����B
        g_k2EngineLow->DebubDrawWorld();

        // �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
        g_k2EngineLow->EndFrame();
    }

	delete g_k2EngineLow;

	return 0;
}

void InitLightCamera(Camera& lightCamera)
{
    // ���C�g�܂ł̃x�N�g��
    Vector3 toLigDir = { 1, 2, 1 };
    toLigDir.Normalize();
    toLigDir *= 5000.0f; // ���C�g�܂ł̋���

    // �J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
    lightCamera.SetPosition(toLigDir);

    // �J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
    lightCamera.SetTarget({ 0, 0, 0 });

    // �������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
    lightCamera.SetUp(1, 0, 0);

    lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
    lightCamera.SetWidth(5000.0f);
    lightCamera.SetHeight(5000.0f);
    lightCamera.SetNear(1.0f);
    lightCamera.SetFar(10000.0f);

    // ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
    lightCamera.Update();
}

// ���[�g�V�O�l�`���̏�����
void InitRootSignature(RootSignature& rs)
{
    rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}

void MoveCamera()
{
    auto pos = g_camera3D->GetPosition();
    auto target = g_camera3D->GetTarget();
    pos.z -= g_pad[0]->GetLStickYF() * 2.0f;
    target.z -= g_pad[0]->GetLStickYF() * 2.0f;
    pos.y += g_pad[0]->GetRStickYF() * 2.0f;
    target.y += g_pad[0]->GetRStickYF() * 2.0f;
    g_camera3D->SetPosition(pos);
    g_camera3D->SetTarget(target);
}