#pragma once
#include "Bloom.h"

namespace nsK2EngineLow {
    class PostEffect
    {
    public:
        void Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget);
        void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
    private:
        Bloom m_bloom;	//ƒuƒ‹[ƒ€
     //   Dof m_dof;		//”íÊŠE[“x
    };
}