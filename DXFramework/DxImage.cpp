#include "stdafx.h"
#include <cassert>
#include <windows.h>
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxImage.h"

//=======================================================================
DxImage::DxImage ( )
{
   ZeroMemory( &myFileInfo, sizeof(myFileInfo) );
}

//=======================================================================
DxImage::DxImage ( const DxImage& other )
:myFilename(other.myFilename), myFileInfo(other.myFileInfo)
{
}

//=======================================================================
DxImage& DxImage::operator= ( const DxImage& other )
{
   myFilename = other.myFilename;
   myFileInfo = other.myFileInfo;
   return *this;
}

//=======================================================================
bool DxImage::getInfoFromFile ( const tstring& filename )
{
   myFilename = filename;

	//get image info from file
	HRESULT result = D3DXGetImageInfoFromFile( myFilename.c_str(), &myFileInfo);
   return SUCCEEDED( result );
}

//=======================================================================
bool DxImage::getInfoFromFileInMemory ( LPCVOID pFileData, UINT fileDataSize )
{
	//get image info from file
	HRESULT result = D3DXGetImageInfoFromFileInMemory( pFileData, fileDataSize, &myFileInfo);
   return SUCCEEDED( result );
}

//=======================================================================
void DxImage::removeFileInfo ( )
{
   //todo sas: WTF????
   myFilename = _T("");
   ZeroMemory( &myFileInfo, sizeof(myFileInfo) );
}