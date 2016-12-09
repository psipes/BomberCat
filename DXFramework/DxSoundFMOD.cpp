#include "stdafx.h"
#include "DxFramework/DxFramework.h"
#include "DxFramework/DxSoundFMOD.h"
#include <delayimp.h>


#pragma comment( lib, "fmod_vc.lib" )



DxSoundFMOD::DxSoundFMOD ()
{
}

DxSoundFMOD::~DxSoundFMOD ()
{
}

//--------------------------------------------------------------------------
bool delayLoad ()
{
   // This is the heart of the delayed loading mechanism.  It is "similar" to LoadLibrary,
   // but works beneath the scenes to build the bindings to the exported functions from
   // the library.  
   // NOTE: CASE SENSITIVE - the name MUST BE EXACTLY AS SHOWN HERE!
   // Other Notes: See Properties->Linker->Input->Delay Loaded DLL's - I'm listing BOTH fmod.dll and fmodL.dll
   //              which seems to work fine.  
   //              The import library specified at the top (#pragma comment(lib,....))
   //              specifies the imports, which calls out the proper dll name. 
   //              NOTE THAT A LINKER WARNING IS TO BE EXPECTED IF BOTH DLL's ARE LISTED, AS I'VE DONE.
   HRESULT hrDllLoad = S_FALSE;

   //printf( "ATTEMPTING TO LOAD FMOD DLL \"%s\"\n", "fmod.dll" );
   
   __try 
   {
      hrDllLoad = __HrLoadAllImportsForDll( "fmod.dll" );
   }
   __except ( EXCEPTION_EXECUTE_HANDLER )
   {
      // SEH exception handling acts very strange, because - I believe - it returns on a *DIFFERENT STACK*
      // the only acceptable
      return false;
   }


   return hrDllLoad == S_OK;
}



//--------------------------------------------------------------------------
DxSoundFMOD* DxSoundFMOD::create ( void* context )
{
   DxSoundFMOD* p = NULL;
   if ( delayLoad() )
   {
      p = new DxSoundFMOD;
      p->init( HWND( context ) );
   }
   
   return p;
}


//-----------------------------------------------------------------------------
bool DxSoundFMOD::init ( HWND hwnd )
{
   UNREFERENCED_PARAMETER( hwnd );

   // initialize FMOD
   FMOD_RESULT fresult;

   fresult = FMOD::System_Create( &fmSystem );
   if ( fresult != FMOD_OK )
      return false;

   unsigned int   version = 0;

   fresult = fmSystem->getVersion( &version );
   if ( fresult != FMOD_OK )
      return false;

   if ( version < FMOD_VERSION )
   {
      log( "FMOD lib version dosn't match header version" );
   }
   log( "FMOD interface version loaded OK" );

   fresult = fmSystem->init( 32, FMOD_INIT_NORMAL, NULL );
   if ( fresult != FMOD_OK )
      return false;

   return true;
}

void DxSoundFMOD::update ()
{
   // update interfaces
   fmSystem->update();
}

void DxSoundFMOD::shutdown ()
{
   // unload interfaces
   fmSystem->close();
}

void DxSoundFMOD::releaseSound ( DxSoundIdentifier*& identifier )
{
   DxFMODSoundIdentifier* fmodID;

   fmodID = dynamic_cast<DxFMODSoundIdentifier*>( identifier );

   if ( !fmodID )
   {
      return;
   }

   if ( fmodID->myFsound )
   {
      fmodID->myFsound->release();
   }
   delete fmodID;
   identifier = NULL;
}

// continuously loop through a sound
void DxSoundFMOD::loop ( DxSoundIdentifier* identifier, bool enable )
{
   identifier->loop( enable );
}

// load a sound from a file
bool DxSoundFMOD::load ( const tstring& soundFileName , DxSoundIdentifier*& identifier )
{
   FMOD_RESULT fresult;
   FMOD::Sound* fsound = NULL;

   fresult = fmSystem->createSound( soundFileName.c_str(), FMOD_DEFAULT, 0, &fsound );
   DxFMODSoundIdentifier* fmodID = new DxFMODSoundIdentifier;
   fmodID->myFsound = fsound;
   fmodID->myFileName = soundFileName;

   identifier = fmodID;


   return (fresult == FMOD_OK);
}

// start sound from saved pause time.  this time will be zero if pause was not called.
bool DxSoundFMOD::play ( DxSoundIdentifier* identifier )
{
   bool isPlaying = false;

   DxFMODSoundIdentifier* fmodID;

   fmodID = dynamic_cast<DxFMODSoundIdentifier*>( identifier );

   if ( !fmodID )
   {
      return false;
   }

   FMOD_RESULT fr;

   // check if the sound is already playing
   if ( fmodID->myFsound )
   {
      fmodID->myFchannel->isPlaying( &(fmodID->myIsPlaying) );

      if ( fmodID->myIsPaused )
      {
         fmodID->myFchannel->setPaused( false );
         fmodID->myFchannel->getPaused( &(fmodID->myIsPaused) );
      }
      // play the sound, checking if it should be looped or not
      else if ( !fmodID->myIsPlaying )
      {
         if ( fmodID->myLoopEnabled )
         {
            fmodID->myFsound->setMode( FMOD_LOOP_NORMAL );
         }
         else
         {
            fmodID->myFsound->setMode( FMOD_LOOP_OFF );
         }

         fr = fmSystem->playSound( fmodID->myFsound, 0, false, &(fmodID->myFchannel) );
      }

      fmodID->myFchannel->isPlaying( &(fmodID->myIsPlaying) );
   }


   return isPlaying;
}

// stop a sound that is playing
bool DxSoundFMOD::stop ( DxSoundIdentifier* identifier )
{
   bool isStopped = false;

   DxFMODSoundIdentifier* fmodID;

   fmodID = dynamic_cast<DxFMODSoundIdentifier*>( identifier );

   if ( !fmodID )
   {
      return false;
   }

   if ( fmodID->myFsound )
   {
      // don't try to stop sound if channel doesn't exist or sound is already stopped
      if ( fmodID->myFchannel == 0 )
      {
         isStopped = true;
         return isStopped;
      }

      fmodID->myFchannel->isPlaying( &(fmodID->myIsPlaying) );
      if ( !fmodID->myIsPlaying )
      {
         isStopped = true;
         return isStopped;
      }

      FMOD_RESULT fr = fmodID->myFchannel->stop();
      if ( fr != FMOD_OK )
      {
         log( _T("Stopping of sound has failed\n") );
      }
      fmodID->myFchannel->isPlaying( &(fmodID->myIsPlaying) );

      isStopped = !(fmodID->myIsPlaying);
   }

   return isStopped;
}

// has a sound stopped playing
bool DxSoundFMOD::isPlaying ( DxSoundIdentifier* identifier )
{
   bool isPlaying = false;

   DxFMODSoundIdentifier* fmodID;

   fmodID = dynamic_cast<DxFMODSoundIdentifier*>( identifier );

   if ( !fmodID )
   {
      return false;
   }

   if ( fmodID->myFsound )
   {

      fmodID->myFchannel->isPlaying( &(fmodID->myIsPlaying) );

      isPlaying = fmodID->myIsPlaying;
   }

   return isPlaying;
}

// pause a sound that is playing (calling play should play from paused point)
bool DxSoundFMOD::pause ( DxSoundIdentifier* identifier )
{
   bool isPaused = false;

   DxFMODSoundIdentifier* fmodID;

   fmodID = dynamic_cast<DxFMODSoundIdentifier*>( identifier );

   if ( !fmodID )
   {
      return false;
   }

   //pause the sound
   if ( fmodID->myFsound )
   {
      fmodID->myFchannel->setPaused( true );
      fmodID->myFchannel->getPaused( &(fmodID->myIsPaused) );
   }

   return isPaused;
}

void DxSoundFMOD::setVolume ( DxSoundIdentifier* identifier, int volume )
{
   identifier->setVolume( volume );

   DxFMODSoundIdentifier* fmodID;

   fmodID = dynamic_cast<DxFMODSoundIdentifier*>( identifier );

   if ( !fmodID )
   {
      return;
   }

   if ( fmodID->myFsound )
   {
      fmodID->myFchannel->setVolume( fmodID->myVolume );
   }
}

void DxSoundFMOD::setSpeed ( DxSoundIdentifier* identifier, float speed )
{
   DxFMODSoundIdentifier* fmodID;

   fmodID = dynamic_cast<DxFMODSoundIdentifier*>( identifier );

   if ( !fmodID )
   {
      return;
   }
  
   if ( fmodID->myFsound )
   {
      fmodID->myFsound->setMusicSpeed( speed );
   }
}