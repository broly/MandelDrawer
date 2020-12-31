#include "Color.h"

namespace Mandel
{
    LinearColor LinearColor::operator=(const Color& InColor)
    {
        R = (float)InColor.R / 255.f;
        G = (float)InColor.G / 255.f;
        B = (float)InColor.B / 255.f;
        return *this;
    }
}
