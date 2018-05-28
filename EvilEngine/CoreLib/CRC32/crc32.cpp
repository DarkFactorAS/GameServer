/*************************************************************************************
* Project     : Evil Engine 2012
* File        : CRC32
* Description : Generates a crc checksum for a data block.
*               
* Author      : Thor Richard Hansen
**************************************************************************************/

//#define _CRT_SECURE_NO_WARNINGS     //Disables fopen() security warning on Microsoft compilers.

#include "Precompile.h"
#include <stdio.h>
#include <stdlib.h>
#include "crc32.h"

/***********************************************************************************
* Constructor:
* 
* @author Thor Richard Hansen
***********************************************************************************/
CCRC32::CCRC32()
{
  this->Initialize();
}

/***********************************************************************************
* Destructor:
* 
* @author Thor Richard Hansen
***********************************************************************************/
CCRC32::~CCRC32()
{
}

/***********************************************************************************
* Initialize:
* This function initializes "CRC Lookup Table". You only need to call it once to
* initialize the table before using any of the other CRC32 calculation functions.
* 
* @return (uint32) - Returns the connection instance
* 
* @author Thor Richard Hansen
***********************************************************************************/
void CCRC32::Initialize()
{
  //0x04C11DB7 is the official polynomial used by PKZip, WinZip and Ethernet.
  unsigned long ulPolynomial = 0x04C11DB7;

  //memset(&this->ulTable, 0, sizeof(this->ulTable));

  // 256 values representing ASCII character codes.
  for(int iCodes = 0; iCodes <= 0xFF; iCodes++)
  {
    this->ulTable[iCodes] = this->Reflect(iCodes, 8) << 24;

    for(int iPos = 0; iPos < 8; iPos++)
    {
      this->ulTable[iCodes] = (this->ulTable[iCodes] << 1)
        ^ ((this->ulTable[iCodes] & (1 << 31)) ? ulPolynomial : 0);
    }

    this->ulTable[iCodes] = this->Reflect(this->ulTable[iCodes], 32);
  }
}

/***********************************************************************************
* Reflect:
* Reflection is a requirement for the official CRC-32 standard.
* You can create CRCs without it, but they won't conform to the standard. 
* 
* @param  (ulong) ulReflect - The number we want to mirror
* @return (ulong)           - The mirrored number
* 
* @author Thor Richard Hansen
***********************************************************************************/
unsigned long CCRC32::Reflect(unsigned long ulReflect, const char cChar)
{
  unsigned long ulValue = 0;

  // Swap bit 0 for bit 7, bit 1 For bit 6, etc....
  for(int iPos = 1; iPos < (cChar + 1); iPos++)
  {
    if(ulReflect & 1)
    {
      ulValue |= (1 << (cChar - iPos));
    }
    ulReflect >>= 1;
  }

  return ulValue;
}

/***********************************************************************************
* PartialCRC:
* Calculates the CRC32 by looping through each of the bytes in sData.
* 
* @param  (ulong) ulCRC        - The previous crc number
* @param  (uchar) sData        - The previous crc number
* @param  (ilong) ulDataLength - Length of the datablock
* 
* @author Thor Richard Hansen
***********************************************************************************/
void CCRC32::PartialCRC(unsigned long *ulCRC, const unsigned char *sData, unsigned long ulDataLength)
{
  while(ulDataLength--)
  {
    //If your compiler complains about the following line, try changing each
    //    occurrence of *ulCRC with "((unsigned long)*ulCRC)" or "*(unsigned long *)ulCRC".

    *(unsigned long *)ulCRC =
      ((*(unsigned long *)ulCRC) >> 8) ^ this->ulTable[((*(unsigned long *)ulCRC) & 0xFF) ^ *sData++];
  }
}

/***********************************************************************************
* FullCRC:
* Returns the calculated CRC32 (through ulOutCRC) for the given string.
* 
* @param  (uchar) sData        - The previous crc number
* @param  (ilong) ulDataLength - Length of the datablock
* @param  (ulong) ulOutCRC     - The crc result
* 
* @author Thor Richard Hansen
***********************************************************************************/
void CCRC32::FullCRC(const unsigned char *sData, unsigned long ulDataLength, unsigned long *ulOutCRC)
{
  *(unsigned long *)ulOutCRC = 0xffffffff; //Initilaize the CRC.
  this->PartialCRC(ulOutCRC, sData, ulDataLength);
  *(unsigned long *)ulOutCRC ^= 0xffffffff; //Finalize the CRC.
}

/***********************************************************************************
* FullCRC:
* Returns the calculated CRC32 (through ulOutCRC) for the given string.
* 
* @param  (uchar) sData        - The previous crc number
* @param  (ilong) ulDataLength - Length of the datablock
* @return (ulong)              - The crc result
* 
* @author Thor Richard Hansen
***********************************************************************************/
unsigned long CCRC32::FullCRC(const unsigned char *sData, unsigned long ulDataLength)
{
  unsigned long ulCRC = 0xffffffff; //Initilaize the CRC.
  this->PartialCRC(&ulCRC, sData, ulDataLength);
  return(ulCRC ^ 0xffffffff); //Finalize the CRC and return.
}
