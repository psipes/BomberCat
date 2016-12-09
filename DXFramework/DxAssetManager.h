#pragma once

#if !defined(_DXASSETMANAGER_H_)
#define _DXASSETMANAGER_H_

#include <windows.h>
#include <map>
#include "Utilities/TTypes.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxSurface.h"
#include "DxFramework/DxTexture.h"
#include "DxFramework/DxAnimation.h"
#include "DxFramework/DxSound.h"

class DxAssetManager : private NonCopyable
{
public:
   static const unsigned ourMaxCachedItemsCount = 256;

   static DxAssetManager& getInstance ();
   // if rootAssetPath is null will use "c:/.../working_dir/Assets".
   // if not null will append "Assets" to given path.
   // if createRootAssetDir is true and Rootpath is not found, creates only the root asset path.
   bool init ( const TCHAR* rootAssetPath = NULL,  bool createRootAssetDir = false );
   
   // load from current asset path, unless specifilced.
   bool load ( const tstring& configFilename );


   void shutdown ();
   
   tstring getAssetPath ( const tstring& name );
   tstring getConfigAssetPath ( const tstring& name );
   tstring getSoundAssetPath ( const tstring& name );
   DxTexture* getTexture ( const tstring& name );
   DxSurface* getSurface ( const tstring& name );
   DxAnimation* getAnimation ( const tstring& name );
   DxAnimation getAnimationCopy ( const tstring& name, float speed, D3DCOLOR excludeColor = D3DCOLOR_ARGB( 0, 0, 0, 0 ) );
   DxSoundIdentifier* getSound ( const tstring& name );
   //[SoundType] getSoundAsset(const tstring& soundFilename);

private:
   bool parseConfig ( const tstring& name );
   bool loadTextureFile ( const tstring& name, const tstring& filename, D3DCOLOR excludeColor );
   bool loadTexture     ( const tstring& name, DxTexture* fileTexture, Rect& srcRect );
   bool loadSurface     ( const tstring& name, DxTexture* fileTexture, Rect& srcRect );
   bool loadSound       ( const tstring& name, int volume, bool loop, const tstring& filename );
   bool loadAnimation   ( const tstring& name, DxTexture* fileTexture, const tstring& animationDescr );


   DxAssetManager ();
   ~DxAssetManager ();

   tstring                               myAssetPath;
   std::map<tstring, DxTexture>          myTextures;
   std::map<tstring, DxSurface>          mySurfaces;
   std::map<tstring, DxAnimation>        myAnimations;
   std::map<tstring, DxSoundIdentifier*> mySounds;
};

#endif
