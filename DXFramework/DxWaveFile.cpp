#include "stdafx.h"
#include "DxWaveFile.h"
#include "Utilities/logger.h"

DxWaveFile::DxWaveFile ()
{
    myFormat = NULL;
    myHmmio = NULL;      
    mySize = 0;          
    myIsReadingFromMemory = false;
    myData = NULL;
    myDataCur = NULL;
    myResourceBuffer = NULL;
}

DxWaveFile::~DxWaveFile ()
{
   close();

   if ( !myIsReadingFromMemory )
   {
      if ( myFormat )
         delete[] myFormat;
      myFormat = NULL;
   }
}

bool DxWaveFile::open ( tstring fileName, WAVEFORMATEX* fx )
{
   bool result;

  myIsReadingFromMemory = false;

   if ( fileName.empty() )
      return false;

   if ( myFormat )
      delete[] myFormat;
   myFormat = NULL;

   myHmmio = mmioOpen( (char*)fileName.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ );

   if ( !myHmmio )
   {
      log( _T( "Wave file was not found." ) );
      return false;
   }

   result = readMMIO();

   if ( !result )
   {
      // it wasn't a wave file
      mmioClose( myHmmio, 0 );
      return false;
   }

   result = resetFile();

   if ( !result )
   {
      return false;
   }

   mySize = myCK.cksize;

   return true;
}

bool DxWaveFile::readMMIO ()
{
   MMCKINFO ckInfo;
   PCMWAVEFORMAT waveFormat;

   myFormat = NULL;

   if ( mmioDescend( myHmmio, &myCKRiff, NULL, 0 ) != 0 )
      return false;

   if ( ( myCKRiff.ckid != FOURCC_RIFF ) ||
        ( myCKRiff.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) )
        return false;

   ckInfo.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
   if ( mmioDescend( myHmmio, &ckInfo, &myCKRiff, MMIO_FINDCHUNK ) != 0 )
      return false;

   if ( ckInfo.cksize < (long)sizeof( PCMWAVEFORMAT ) )
      return false;

   if ( mmioRead( myHmmio, (HPSTR)&waveFormat, sizeof( waveFormat ) ) != sizeof( waveFormat ) )
      return false;

   if ( waveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
   {
      myFormat = (WAVEFORMATEX*)new char[ sizeof( WAVEFORMATEX ) ];
      if ( !myFormat )
         return false;

      memcpy( myFormat, &waveFormat, sizeof( waveFormat ) );
      myFormat->cbSize = 0;
   }
   else
   {
      WORD extraBytes = 0L;
      if ( mmioRead( myHmmio, (char*)extraBytes, sizeof( WORD ) ) != sizeof( WORD ) )
         return false;

      myFormat = (WAVEFORMATEX*)new char[ sizeof( WAVEFORMATEX ) + extraBytes ];
      if ( !myFormat )
         return false;

      memcpy( myFormat, &waveFormat, sizeof( waveFormat ) );
      myFormat->cbSize = extraBytes;

      if ( mmioRead( myHmmio, (char*)( ( (BYTE*)&(myFormat->cbSize) ) + sizeof( WORD ) ), extraBytes ) != extraBytes )
      {
         if ( myFormat )
            delete myFormat;
         myFormat = NULL;
         return false;
      }
   }

   if ( mmioAscend( myHmmio, &ckInfo, 0 ) != 0 )
   {
      if ( myFormat )
         delete myFormat;
      myFormat = NULL;

      return false;
   }

   return true;
}
