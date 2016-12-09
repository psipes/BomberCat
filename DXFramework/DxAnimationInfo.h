#pragma once

#if !defined(_DXANIMATIONFRAME_H_)
#define _DXANIMATIONFRAME_H_
#include "Utilities/TTypes.h"
#include "Utilities/Rect.h"
#include "DxFramework/DxTexture.h"

struct DxAnimationFrame
{
   DxAnimationFrame ()
      :texture(NULL), rect()
   {
   }
   DxTexture* texture;
   Rect       rect;

   int width() const { return rect.width(); }

   int height() const { return rect.height(); }
};

#endif //_DXANIMATIONFRAME_H_