#include "Header.h"
#include "Renderpass.h"

IRenderpass::IRenderpass(PipelineManager* pipe)
{
    m_pipeline = pipe;
}

void IRenderpass::SetActive(const bool& tog)
{
    m_isActive = tog;
}

const bool& IRenderpass::IsActive() const
{
    return m_isActive;
}
