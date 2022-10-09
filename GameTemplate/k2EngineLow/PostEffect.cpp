#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {

    void PostEffect::Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget)
    {
        m_bloom.Init(mainRenderTarget);
    //    m_dof.Init(mainRenderTarget, zprepassRenderTarget);
    }
    void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
    {
        m_bloom.Render(rc, mainRenderTarget);
    //    m_dof.Render(rc, mainRenderTarget);
    }
}