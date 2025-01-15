
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "ReSrc4.h"

// --

int64_t RS4CurrentVirtAdr;

// --

void RS4FreeSection( enum RS4ErrorCode *errcode, RS4FileSection *sec )
{
enum RS4ErrorCode ec;

	ec = RS4ErrStat_Error;

	if ( sec )
	{
		goto bailout;
	}

	if ( sec->rfs_ID != RS4ID_FileSection )
	{
		ec = RS4ErrStat_InvalidStructID;

		#ifdef DEBUG
		printf( "%s:%04d: Error Invalid Struct ID\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	sec->rfs_ID = 0;

	// -- free other stuff here, still todo

	if ( sec->rfs_MemoryType )
	{
		free( sec->rfs_MemoryType );
		sec->rfs_MemoryType = NULL;
	}

	if ( sec->rfs_MemoryBuf )
	{
		free( sec->rfs_MemoryBuf );
		sec->rfs_MemoryBuf = NULL;
	}



	free( sec );

	// --

	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}
};

// --

RS4FileSection *RS4AllocSection( enum RS4ErrorCode *errcode, int secnr, int64_t memsize )
{
enum RS4ErrorCode ec;
RS4FileSection *sec;

	ec = RS4ErrStat_Error;

	sec = calloc( 1, sizeof( RS4FileSection ));

	if ( ! sec )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( RS4FileSection ) );
		#endif

		goto bailout;
	}

	sec->rfs_ID = RS4ID_FileSection;

	// --

	sec->rfs_MemoryAdr = RS4CurrentVirtAdr;

	RS4CurrentVirtAdr += 0x00010000;
	RS4CurrentVirtAdr += memsize;
	RS4CurrentVirtAdr &= 0xffff0000;

	// --

	sec->rfs_SecNr = secnr;

	if ( memsize )
	{
		// Added +10 for safety

		sec->rfs_MemorySize	= memsize;
		sec->rfs_MemoryBuf	= calloc( 1, memsize + 10 ); // BSS need to be cleared

		if ( ! sec->rfs_MemoryBuf )
		{
			ec = RS4ErrStat_OutOfMemory;

			#ifdef DEBUG
			printf( "%s:%04d: Error allocating memory (%ld Bytes)\n", __FILE__, __LINE__, memsize );
			#endif

			goto bailout;
		}

		sec->rfs_MemoryType	= calloc( 1, memsize + 10 );

		if ( ! sec->rfs_MemoryType )
		{
			ec = RS4ErrStat_OutOfMemory;

			#ifdef DEBUG
			printf( "%s:%04d: Error allocating memory (%ld Bytes)\n", __FILE__, __LINE__, memsize );
			#endif

			goto bailout;
		}

		memset( sec->rfs_MemoryType, RS4MT_Unset, memsize );
	}

	ec = RS4ErrStat_Okay;

	// --

bailout:

	if (( sec ) && ( ec != RS4ErrStat_Okay ))
	{
		RS4FreeSection( NULL, sec );
		sec = NULL;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return(	sec );
}

// --

RS4FileSection *RS4FindSection_File( RS4FileHeader *fh, int64_t addr )
{
RS4FileSection *sec;
int64_t memadr;
int cnt;

	if ( ! addr )
	{
		return( NULL );
	}

	for( cnt=0 ; cnt<fh->rfh_SecArraySize ; cnt++ )
	{
		memadr = fh->rfh_SecArray[cnt].rsi_MemoryAdr;

		if ( addr < memadr )
		{
			continue;
		}
		
		memadr += fh->rfh_SecArray[cnt].rsi_MemorySize;

		if ( addr > memadr )
		{
			continue;
		}

		break;
	}

	// Hunk found or is it an address outside our code?

	if ( cnt == fh->rfh_SecArraySize )
	{
		sec = NULL;
	}
	else
	{
		sec = fh->rfh_SecArray[cnt].rsi_Section;
	}

	#ifdef DEBUG
	if (( sec ) && ( sec->rfs_ID != RS4ID_FileSection ))
	{
		printf( "Invalid Section ID : %08x\n", sec->rfs_ID );
		sec = NULL;
	}
	#endif

	return( sec );
}

// --
