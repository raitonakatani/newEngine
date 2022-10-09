#pragma once
#include "Bloom.h"

namespace nsK2EngineLow {
    class PostEffect
    {
    public:
        void Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget);
        void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
    private:
        Bloom m_bloom;	//�u���[��
     //   Dof m_dof;		//��ʊE�[�x
    };
}