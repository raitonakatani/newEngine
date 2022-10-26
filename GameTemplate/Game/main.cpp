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


    RenderTarget offscreenRenderTarget;
    offscreenRenderTarget.Create(
        FRAME_BUFFER_W,
        FRAME_BUFFER_H,
        1,
        1,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_D32_FLOAT
    );

    ////�����_�����O�G���W����������
    //RenderingEngine renderingEngine;
    //renderingEngine.Init();

    //// �ʏ�`��̃e�B�[�|�b�g���f����������
    // // �w�i���f���̃����_���[��������
    //ModelRender bgModelRender;
    //bgModelRender.InitDeferredRendering(renderingEngine, "Assets/modelData/bg/bg.tkm", true);

    //ModelRender boxModelRender;
    //boxModelRender.InitDeferredRendering(renderingEngine, "Assets/modelData/testModel/box.tkm", true);

    //boxModelRender.UpdateWorldMatrix({ 0.0f,100.0f,0.0f }, g_quatIdentity, g_vec3One);
    // 
    //ModelInitDataFR modelInitData;
    //modelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
    //modelInitData.m_fxFilePath = "Assets/shader/sample.fx";

    //// �g��SRV��ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`�����w�肷��
    //modelInitData.m_expandShaderResoruceView[0] = &renderingEngine.GetZPrepassDepthTexture();

    //// �����������g���ĕ`�揈��������������
    //ModelRender UnityModelRender;

    //// InitForwardRendering()�𗘗p����ƁA
    //// �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳���
    //UnityModelRender.InitForwardRendering(renderingEngine, modelInitData);

    ////�V���h�E�L���X�^�[�t���O���I���ɂ���
    //UnityModelRender.SetShadowCasterFlag(true);
    //
    //UnityModelRender.UpdateWorldMatrix({ 0.0f, 3.0f, 0.0f }, g_quatIdentity, g_vec3One);
    
      //��
    ModelInitData boxModelInitData;
    boxModelInitData.m_tkmFilePath = "Assets/modelData/testModel/box.tkm";
    boxModelInitData.m_fxFilePath = "Assets/shader/sample3D.fx";
    Model boxModel;
    boxModel.Init(boxModelInitData);
    boxModel.UpdateWorldMatrix({ 0.0f,150.0f,0.0f }, g_quatIdentity, g_vec3One);

    //�w�i
    ModelInitData bgModelInitData;
    bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
    bgModelInitData.m_fxFilePath = "Assets/shader/sample3D.fx";
    Model bgModel;
    bgModel.Init(bgModelInitData);

    //�v���C���[
    ModelInitData plModelInitdata;
    plModelInitdata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
    plModelInitdata.m_fxFilePath = "Assets/shader/sample3D.fx";
    Model plModel;
    plModel.Init(plModelInitdata);

    
    //�C���N�̃e�N�X�`��
    //��������f���̃e�N�X�`���ɓh�肽��
    SpriteInitData inkspriteinitdata;
    //DDS�t�@�C��(�摜�f�[�^)�̃t�@�C���p�X���w�肷��B
    inkspriteinitdata.m_ddsFilePath[0] = "Assets/sprite/inku3.DDS";
    //Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
    inkspriteinitdata.m_fxFilePath = "Assets/shader/sprite.fx";
    //�X�v���C�g�̕��ƍ������w�肷��B
    inkspriteinitdata.m_width = FRAME_BUFFER_W;
    inkspriteinitdata.m_height = FRAME_BUFFER_W;
    //Sprite�������I�u�W�F�N�g���g�p���āASprite������������B
    Sprite inksprite;
    inksprite.Init(inkspriteinitdata);


    
    //���f���̃e�N�X�`��
    //����ɃC���N��h���ăe�N�X�`���؂�ւ���������
    SpriteInitData spriteinitdata;
    //DDS�t�@�C��(�摜�f�[�^)�̃t�@�C���p�X���w�肷��B
    spriteinitdata.m_ddsFilePath[0] = boxModel.GetTkmFile()->m_albedo;
    //Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
    spriteinitdata.m_fxFilePath = "Assets/shader/Splatoon/inksprite.fx";
    //�X�v���C�g�̕��ƍ������w�肷��B
    spriteinitdata.m_width = FRAME_BUFFER_W;
    spriteinitdata.m_height = FRAME_BUFFER_H;
    spriteinitdata.m_expandShaderResoruceView[0] = &inksprite.GetTexture(0);
    //Sprite�������I�u�W�F�N�g���g�p���āASprite������������B
    Sprite sprite;
//    sprite.Init(spriteinitdata);


    Vector3 startVector = Vector3::Zero;
    Vector3 endVector = Vector3::Zero;
    //0.223011225
    //0.313250005

    Vector3 position;
    position = { 0.0f,200.0f,200.0f };


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

  //     bgModelRender.Draw(true);
  //
  //     // step-2 �e�B�[�|�b�g��`�悷��
  //     UnityModelRender.Draw(true);
  //
  //     renderingEngine.OFFScreenRendering(renderContext);

        //////////////////////////////////////
        // ��������G��`���R�[�h���L�q����
        //////////////////////////////////////

 //       //�����_�����O�G���W�������s
 //       renderingEngine.Execute(renderContext);
 
        // step-7 ��ʂɕ\������郌���_�����O�^�[�Q�b�g�Ɋe�탂�f����`�悷��
        bgModel.Draw(renderContext);
    //    plModel.UpdateWorldMatrix(position, g_quatIdentity, g_vec3One);
    //    plModel.Draw(renderContext);
        boxModel.Draw(renderContext);

        Vector3 POS;
        Vector2 UV;

        //�O�p�`�̍��W�������Ă��郊�X�g�������Ă���B
        std::vector<nsK2EngineLow::TkmFile::VectorBuffer> bufferList = boxModel.GetTkmFile()->GetBuffer();
             
                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    startVector = { 0.0f,00.0f,300.0f };
                    endVector = { 0.0f,200.0f,-300.0f };
                    position = endVector;
                }

                //���ʂƐ����̌�_�����߂�B�@POS�i��_�̍��W�j�Avector3d(�����n�_)�Avector3dend(�����I�_)�A�|���S����3���_
                if (boxModel.IntersectPlaneAndLine(POS, UV, startVector, endVector, bufferList) == true) {

                    auto Vector = POS;
                    Vector2 UVPOS = UV;
                    
                    boxModel.ChangeAlbedoMap(
                        "",
                        offscreenRenderTarget.GetRenderTargetTexture()
                    );

                    // step-4 �����_�����O�^�[�Q�b�g��offscreenRenderTarget�ɕύX����
                    RenderTarget* rtArray[] = { &offscreenRenderTarget };
                    renderContext.WaitUntilToPossibleSetRenderTargets(1, rtArray);
                    renderContext.SetRenderTargets(1, rtArray);
                    renderContext.ClearRenderTargetViews(1, rtArray);

                    // ���߂�UV���W���V�F�[�_�[�Ɏ����Ă����B
                    if (g_pad[0]->IsPress(enButtonA)) {
                        spriteinitdata.m_posi.x = UVPOS.x;
                        spriteinitdata.m_posi.y = UVPOS.y;
                        sprite.Init(spriteinitdata);
                    };
                    //�e�N�X�`�����h���[
                    sprite.Draw(renderContext);

                    renderContext.WaitUntilFinishDrawingToRenderTargets(1, rtArray);
                    // step-6 ��ʂɕ\������郌���_�����O�^�[�Q�b�g�ɖ߂�
                    renderContext.SetRenderTarget(
                        g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
                        g_graphicsEngine->GetCurrentFrameBuffuerDSV()
                    );

                }


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
    pos.x -= g_pad[0]->GetLStickXF() * 2.0f;
    target.x -= g_pad[0]->GetLStickXF() * 2.0f;
    pos.y += g_pad[0]->GetRStickYF() * 2.0f;
    target.y += g_pad[0]->GetRStickYF() * 2.0f;

    g_camera3D->SetPosition(pos);
    g_camera3D->SetTarget(target);
}