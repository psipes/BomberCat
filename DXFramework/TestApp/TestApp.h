#if !defined(_TESTAPP_H_)
#define _TESTAPP_H_

#include "DxFramework\DxFramework.h"
#include "DxFramework\DxAnimation.h"
#include "DxFramework\DxKeyboard.h"
class Game : public DxFramework
{
public:
   Game ();
   ~Game ();

protected:
   bool gameInit ( );
   void gameRun ( );
   void gameExit ();
   //LPCTSTR 
private:
   DxTexture myTexture;
   D3DCOLOR bgColor;
   D3DCOLOR bgColor2;
   D3DCOLOR bgColor3;
   DxKeyboard myKeyboard;
};

#endif //_TESTAPP_H_