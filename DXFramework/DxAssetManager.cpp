
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include "Utilities/Point.h"
#include "Utilities/Logger.h"
#include "Utilities/ConfigParser.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/TStringRoutines.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxSound.h"
#include "DxFramework/DxFramework.h"
#include "DxFramework/DxAssetManager.h"


//=======================================================================
//static
DxAssetManager& DxAssetManager::getInstance ()
{
   static DxAssetManager instance;
   return instance;
}

//=======================================================================
DxAssetManager::DxAssetManager ()
{
}

//=======================================================================
DxAssetManager::~DxAssetManager ()
{
}

//=======================================================================
bool DxAssetManager::init ( const TCHAR* rootPath,  bool createRootAssetDir )
{
   DWORD gle;
   tstring assetDir;
   
   if ( rootPath )
   {
      if ( !PathUtilities::pathCanonicalize( assetDir, rootPath ) )
      {
         return false;
      }
   }
   else
   {
      TCHAR working_directory[MAX_PATH] = { 0 };
      if ( !(gle = GetCurrentDirectory( MAX_PATH, working_directory )) )
      {
         return false;
      }
      assetDir = working_directory;
      if ( !PathUtilities::pathAppend( assetDir, "Assets" ) )
      {
         return false;
      }
   }
   
   // "c:/.../Assets/Assets" should become "c:/.../Assets"


   if ( !PathUtilities::pathIsDirectory( assetDir ) )
   {
      if ( createRootAssetDir )
      {
         if ( !CreateDirectory( assetDir.c_str(), NULL ) )
         {
            return false;
         }
      }
      else
      {
         logln( "Unable to find specified Assets directory!" );
         return false;
      }
      return false;
   }

   myAssetPath = assetDir;
   if ( PathUtilities::pathIsDirectory( myAssetPath ) )
   {
      return true;
   }
   return false;
}

//=======================================================================
bool DxAssetManager::load ( const tstring& configFilename )
{
   tstring configFilepath = myAssetPath;
   
   // configfilename, absolute path vs relative path? 
   // check configfile is exists.
   PathUtilities::pathAppend( configFilepath, configFilename );

   if ( !PathUtilities::pathIsRelative( configFilename ) )
   {
      configFilepath = configFilename;
   }
   else if ( !PathUtilities::pathFileExists( configFilepath ) )
   {
      logln( "Unable to find config file: [%s]!", configFilename.c_str() );
      return false;
   }

   return parseConfig( configFilepath );
}

//=======================================================================
void DxAssetManager::shutdown ()
{
   myAssetPath.clear();
   
   for ( std::map<tstring, DxTexture>::iterator item = myTextures.begin(); 
         item != myTextures.end(); item++ )
   {
      item->second.destroy();
   }
   myTextures.clear();

   for ( std::map<tstring, DxSurface>::iterator item = mySurfaces.begin(); 
         item != mySurfaces.end(); item++ )
   {
      item->second.destroy();
   }
   mySurfaces.clear();

   for ( std::map<tstring, DxAnimation>::iterator item = myAnimations.begin(); 
         item != myAnimations.end(); item++ )
   {
      item->second.shutdown();
   }
   myAnimations.clear();

   for ( std::map<tstring, DxSoundIdentifier*>::iterator item = mySounds.begin(); 
         item != mySounds.end(); item++ )
   {
      DxSound::getInterface()->releaseSound( item->second );
      item->second = NULL;
   }
   mySounds.clear();
}


//=======================================================================
tstring DxAssetManager::getAssetPath ( const tstring& name )
{
   tstring filePath( myAssetPath );
   PathUtilities::pathAppend( filePath, name );
   return filePath;
}

//=======================================================================
tstring DxAssetManager::getConfigAssetPath ( const tstring& configFilename )
{
   return getAssetPath( configFilename ); //todo
}

//=======================================================================
tstring DxAssetManager::getSoundAssetPath ( const tstring& soundFilename )
{
   return getAssetPath( soundFilename ); //todo
}

//=======================================================================
DxTexture* DxAssetManager::getTexture ( const tstring& name )
{
   std::map<tstring, DxTexture>::iterator item = myTextures.find( name );
   if ( item != myTextures.end() )
   {
      return &item->second;
   }
   logln( _T("Unable to find Texture:\"%s\""), name.c_str() );
   return NULL;
}

//=======================================================================
DxSurface* DxAssetManager::getSurface ( const tstring& name )
{
   std::map<tstring, DxSurface>::iterator item = mySurfaces.find( name );
   if ( item != mySurfaces.end() )
   {
      return &item->second;
   }
   logln( _T("Unable to find Surface:\"%s\""), name.c_str() );
   return NULL;
}

//=======================================================================
DxAnimation* DxAssetManager::getAnimation ( const tstring& name )
{
   std::map<tstring, DxAnimation>::iterator item = myAnimations.find( name );
   if ( item != myAnimations.end() )
   {
      return &item->second;
   }
   logln( _T("Unable to find Animation:\"%s\""), name.c_str() );
   assert(false);
   return NULL;
}

//=======================================================================
DxAnimation DxAssetManager::getAnimationCopy ( const tstring& name, float speed, D3DCOLOR excludeColor )
{
   DxAnimation animation;
   std::map<tstring, DxAnimation>::iterator item = myAnimations.find( name );
   if ( item != myAnimations.end() )
   {
      animation = item->second;
   }
   else
   {
      assert(false);
   }
   animation.speed( speed );
   animation.excludeColor( excludeColor );
   return animation;
}

//=======================================================================
DxSoundIdentifier* DxAssetManager::getSound ( const tstring& name )
{
   std::map<tstring, DxSoundIdentifier*>::iterator item = mySounds.find( name );
   if ( item != mySounds.end() )
   {
      return item->second;
   }
   logln( _T("Unable to find Sound:\"%s\""), name.c_str() );
   assert(false);
   return NULL;
}

//=======================================================================
bool DxAssetManager::parseConfig ( const tstring& filename )
{
   tifstream script( getConfigAssetPath( filename ).c_str(), std::ios_base::in );
   DxTexture* currentTexture = NULL;
   

   if ( !script.is_open() || script.bad() )
   {
      logln( _T("Unable to load Asset Configuration File:\"%s\""), filename.c_str() );
      return false;
   }

   ConfigParser parser;
   tstring line;
   while ( parser.getNextLine( script, line ) )
   {
      tstringstream ss(line);
      tstring token;
      ss >> token;
      if ( token == "@Image")
      {
         int a = 0, r = 0, g = 0, b = 0;
         tstring name, filename;
         ss >> name >> filename >> a >> r >> g >> b;
         loadTextureFile( name, filename, D3DCOLOR_ARGB(a,r,g,b) );
         currentTexture = getTexture( name );
      }
      else if ( token == "@Sound" )
      {
         int volume = -1;
         bool looping;
         tstring name, filename;

         ss >> name >> volume >> looping >> filename;
         if ( ss.fail() )
         {
            return false;
         }
         loadSound( name, volume, looping, filename );
      }
      else
      {
         tstring name = token;
         int type, x, y, width, height;;
         
         ss >> type >> x >> y >> width >> height;
         if ( ss.fail() )
         {
            return false;
         }

         Rect srcRect( x, y, x + width, y + height );

         switch ( type )
         {
         case 3:
            loadTexture( name, currentTexture, srcRect );
            break;
         case 4:
            loadSurface( name, currentTexture, srcRect );
            break;
         default:
            loadAnimation( name, currentTexture, line );
            break;

         }
      }
   }
   script.close();
   return true;
}

//=======================================================================
bool DxAssetManager::loadTextureFile ( const tstring& name, const tstring& filename, D3DCOLOR excludeColor )
{
   DxTexture texture;
   if ( myTextures.find( name ) == myTextures.end() && texture.create( DxFramework::device(), getAssetPath(filename), excludeColor ) )
   {
      myTextures[name] = texture;
      return true;
   }
   return false;
}

//=======================================================================
bool DxAssetManager::loadTexture ( const tstring& name, DxTexture* fileTexture, Rect& srcRect )
{
   DxTexture texture;
   if ( myTextures.find( name ) == myTextures.end() && texture.create( DxFramework::device(), srcRect.width(), srcRect.height(), D3DUSAGE_RENDERTARGET ) )
   {
      fileTexture->stretchRect( DxFramework::device(), &srcRect, texture, NULL );
      myTextures[name] = texture;
      return true;
   }
   return false;
}

//=======================================================================
bool DxAssetManager::loadSurface ( const tstring& name, DxTexture* fileTexture, Rect& srcRect )
{
   DxSurface surface;
   if ( mySurfaces.find( name ) == mySurfaces.end() && surface.createFromFile( DxFramework::device(), fileTexture->filename(), &srcRect ) )
   {
      mySurfaces[name] = surface;
      return true;
   }
   return false;
}

//=======================================================================
bool DxAssetManager::loadSound ( const tstring& name, int volume, bool loop, const tstring& filename )
{
   DxSound* instance = DxSound::getInterface();
   DxSoundIdentifier* soundId = NULL;
   
   if ( mySounds.find( name ) != mySounds.end() || !instance->load( getAssetPath( filename ), soundId ) )
   {
      return false;
   }
   soundId->setVolume( volume );
   soundId->loop( loop );
   mySounds[name] = soundId;
   return true;
}

//=======================================================================
bool DxAssetManager::loadAnimation ( const tstring& name, DxTexture* fileTexture, const tstring& animationDescr )
{
   DxAnimation animation;
   if ( myAnimations.find( name ) != myAnimations.end() || !animation.init( fileTexture, animationDescr, 0 ) )
   {
      return false;
   }
   myAnimations[name] = animation;
   return true;
}