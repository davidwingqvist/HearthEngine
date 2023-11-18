#include "Header.h"
#include "DrawObject.h"

IDrawObject::IDrawObject()
{

}

void IDrawObject::SetVisibility(const bool& toggle)
{
    m_isVisible = toggle;
}

const bool& IDrawObject::IsVisible()
{
    return m_isVisible;
}
