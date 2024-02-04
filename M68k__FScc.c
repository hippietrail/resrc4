
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

//  0 000000 F
//  1 000001 EQ
//  2 000010 OGT
//  3 000011 OGE
//  4 000100 OLT
//  5 000101 OLE
//  6 000110 OGL
//  7 000111 OR
//  8 001000 UN
//  9 001001 UEQ
//  A 001010 UGT
//  B 001011 UGE
//  C 001100 ULT
//  D 001101 ULE
//  E 001110 NE
//  F 001111 T
// 10 010000 SF
// 11 010001 SEQ
// 12 010010 GT
// 13 010011 GE
// 14 010100 LT
// 15 010101 LE
// 16 010110 GL
// 17 010111 GLE
// 18 011000 NGLE
// 19 011001 NGL
// 1A 011010 NLE
// 1B 011011 NLT
// 1C 011100 NGE
// 1D 011101 NGT
// 1E 011110 SNE
// 1F 011111 ST

void Cmd_FScc( struct M68kStruct *ms )
{
struct HunkRef *isRef;
int emode;
int ereg;
int cond;

	cond = ( ms->ms_Opcode & 0x0000003f );
	emode= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ereg = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	if ( cond > 0x1f )
	{
		printf( "Unsupported 'FScc' Opcode at %08x\n", ms->ms_MemoryAdr );
		ms->ms_DecodeStatus = MSTAT_Error;
		goto bailout;
	}

	static const char *fscc_RegNames[] = 
	{
		"FSf.b",	"FSeq.b",	"FSogt.b",	"FSoge.b",	// 0x00
		"FSolt.b",	"FSole.b",	"FSogl.b",	"FSor.b",	// 0x04
		"FSun.b",	"FSueq.b",	"FSugt.b",	"FSuge.b",	// 0x08
		"FSult.b",	"FSule.b",	"FSne.b",	"FSt.b",	// 0x0c
		"FSf.b",	"FSseq.b",	"FSgt.b",	"FSge.b",	// 0x10
		"FSlt.b",	"FSle.b",	"FSgl.b",	"FSgle.b",	// 0x14
		"FSngle.b",	"FSngl.b",	"FSnle.b",	"FSnlt.b",	// 0x18
		"FSnge.b",	"FSngt.b",	"FSsne.b",	"FSst.b"	// 0x1c
	};

	ms->ms_Str_Opcode = fscc_RegNames[cond];
	ms->ms_ArgType = OS_Byte;
	ms->ms_ArgEMode = emode;
	ms->ms_ArgEReg  = ereg;
	ms->ms_ArgSize	= 4;

// printf( "FScc -> %s at %08x\n",ms->ms_Str_Opcode, ms->ms_MemoryAdr );

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, 0 ))
	{
		isRef->hr_Used = true;
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;

	// --

bailout:

	return;
}

// --
