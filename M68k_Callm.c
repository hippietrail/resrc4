
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

void Cmd_CALLM( struct M68kStruct *ms )
{
struct HunkRef *isRef;
uint8_t val;

	ms->ms_Str_Opcode = "Callm";
	ms->ms_ArgType  = OS_Byte; // Unsized
	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	ms->ms_OpcodeSize = ms->ms_ArgSize;

	val = ( ms->ms_Opcode & 0x000000ff );
	sprintf( ms->ms_Buf_Argument, "#%d", val );

	ms->ms_CurRegister = & ms->ms_DstRegister;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, 0 ))
	{
		isRef->hr_Used = true;
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}