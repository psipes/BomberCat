#pragma once

#if !defined ( __DxWAVEFILE_H__ )
#define __DxWAVEFILE_H__

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include "Utilities/TTypes.h"

class DxWaveFile
{
public:
    DxWaveFile ();
    ~DxWaveFile ();

    bool open ( tstring fileName, WAVEFORMATEX* fx );
    bool close ();

    bool read ( BYTE* buffer, DWORD sizeToRead, DWORD* sizeRead );
    bool write ( UINT sizeToWrite, BYTE* data, UINT* sizeWrote );

    DWORD getSize () { return mySize; }
    bool resetFile ();
    WAVEFORMATEX* getFormat () { return myFormat; };

private:
    bool readMMIO ();
    bool writeMMIO ( WAVEFORMATEX *dest );

private:
    WAVEFORMATEX* myFormat;        // Pointer to WAVEFORMATEX structure
    HMMIO         myHmmio;         // MM I/O handle for the WAVE
    MMCKINFO      myCK;            // Multimedia RIFF chunk
    MMCKINFO      myCKRiff;        // Use in opening a WAVE file
    DWORD         mySize;          // The size of the wave file
    MMIOINFO      myMmioInfoOut;
    bool          myIsReadingFromMemory;
    BYTE*         myData;
    BYTE*         myDataCur;
    ULONG         myDataSize;
    char*         myResourceBuffer;

};

#endif