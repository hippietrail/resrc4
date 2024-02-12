
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
// -- Mode 60 - Brief Extension Word Format

inline static void MODE_60_Brief( struct M68kStruct *ms, char *outstr )
{
uint8_t *mem;
int8_t Offset;
int SCALE;
int REG;
int pos;
int AD;
int WL;

	// Brief Extension Word Format
	//
	// [A/D] [REG] [W/L] [Scale] [0] [ signed offset ]
	//   1     3     1      2     1          8
	// [  4 bit  ] [     4 bit     ] [     8 bit     ]

	// Lea $00(Ax,Dx.L),A0
	// lea OFFSET ( ArgReg,   AD[REG],   D0.  WL  *SCALE  )  ,  a1

	// ( d8 , An , Xn.SIZE * SCALE )

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	AD		= ( mem[ pos + 0 ] & 0x80 );
	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
	WL		= ( mem[ pos + 0 ] & 0x08 );
	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	Offset	= ( mem[ pos + 1 ] );

	sprintf( outstr, "(%d,%s,%s%s%s)",
		Offset,
		Ax_RegNames[ ms->ms_ArgEReg ],
		( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
		( WL ) ? ".l" : ".w",
		scale_Names[SCALE]
	);

	ms->ms_ArgSize += 2;
}

// --
