#include "Header.h"
#include "Renderpass.h"

IRenderpass::IRenderpass(PipelineManager* pipe, Renderer* render)
{
    m_pipeline = pipe;
    m_renderer = render;
}

void IRenderpass::SetActive(const bool& tog)
{
    m_isActive = tog;
}

const bool& IRenderpass::IsActive() const
{
    return m_isActive;
}
