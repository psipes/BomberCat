#pragma once

#if !defined(_DXIMAGE_H_)
#define _DXIMAGE_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "Utilities\NonCopyable.h"
#include "Utilities\TTypes.h"
#include "DxTypes.h"

class DxImage
{
public:
   DxImage ( void );
   DxImage ( const DxImage& other );
   virtual ~DxImage ( void ) { };
   DxImage& operator= ( const DxImage& other );

   const D3DXIMAGE_INFO& fileInfo ( void )  { return myFileInfo; }   //todo sas: this isn't working.
   const tstring& filename ( void ) { return myFilename; }
   
protected:
	bool getInfoFromFile ( const tstring& filename );
   bool getInfoFromFileInMemory ( LPCVOID pFileData, UINT fileDataSize );
   void removeFileInfo ( void );

private:
   tstring		       myFilename;
	D3DXIMAGE_INFO	    myFileInfo;
};

#endif //_DXIMAGE_H_