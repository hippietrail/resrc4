 
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "Resourcer/ReSrc4.h"

// --

inline static enum RS4DecodeStat Args6( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int size;
int reg;
int ir;

	size	= ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	reg		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	ir		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00200000 );

	if (( size == 0 ) && ( ir == 0 ))
	{
		size = 8;
	}

	if ( ir )
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", Dx_RegNames[size], Dx_RegNames[reg] );
	}
	else
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "#%d,%s", size, Dx_RegNames[reg] );
	}

	rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_D0 + reg ); 

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --

enum RS4DecodeStat M68kCmd_ROXR( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int size;

	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x00c00000 ) >> 22;

	switch( size )
	{
		case 0:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Roxr.b";
			break;
		}

		case 1:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Roxr.w";
			break;
		}

		case 2:
    	{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Roxr.l";
			break;
		}

		default:
		{
			printf( "Unsupported 'Roxr' Opcode (Mode: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	ds = Args6( & ec, rt );

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --

enum RS4DecodeStat M68kCmd_ROXR2( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Roxr.w" ;

	if ( rt->rt_CPU.M68k.mt_Opcode & 0x01000000 )
	{
		ds = RS4DecodeStat_Error;
		printf( "roxr.w error\n" );
	}

	rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
	rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

	ds = M68k_EffectiveAddress( & ec, rt );

	if ( ds != RS4DecodeStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	M68k_Set_Cur_to_Unknown( rt );
	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
