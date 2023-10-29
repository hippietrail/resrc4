
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
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

void Cmd_CHK2( struct M68kStruct *ms )
{
struct HunkRef *isRef;
uint32_t size;

	size = ( ms->ms_Opcode & 0x06000000 ) >> 25;

	switch( size )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Chk2.b";
			ms->ms_ArgType  = OS_Byte;
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Chk2.w";
			ms->ms_ArgType  = OS_Word;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Chk2.l";
			ms->ms_ArgType  = OS_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Chk2' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

    // --

	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	ms->ms_ArgSize	= 4;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, 0 ))
	{
		isRef->hr_Used = true;
	}

	// --

	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00008000 ) ? 0x01 : 0x00 ;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00007000 ) >> 12;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms, NULL, 0 );

	// --

	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}
