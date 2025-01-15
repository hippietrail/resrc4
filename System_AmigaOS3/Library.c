
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

#include "Resourcer/ReSrc4.h"

// --

struct myLibType1
{
	int						Used1;
	enum AOS3_LibType		Type1;
	AOS3_LVOStruct *		Data1;
	char *					Name1;
};

#define myentry1(x,y)			{ 0, AOS3_LibType_##x, AOS3_##x, y }

static struct myLibType1 myLibs[] =
{
	myentry1( AmigaguideBase,			"Amigaguide" ),
	myentry1( AslBase,					"ASL" ),
	myentry1( BattclockBase,			"Battclock" ),
	myentry1( BattmemBase,				"Battmem" ),
	myentry1( BulletBase,				"Bullet" ),
	myentry1( CardresBase,				"Cardres" ),
	myentry1( CiaBase,					"CIA" ),
	myentry1( ColorwheelBase,			"Colorwheel" ),
	myentry1( CommoditiesBase,			"Commodities" ),
//	myentry1( ConsoleBase,				"Console" ),
	myentry1( DatatypesBase,			"Datatypes" ),
	myentry1( DiskBase,					"Disk" ),
	myentry1( DiskfontBase,				"Diskfont" ),
	myentry1( DosBase,					"DOS" ),
	myentry1( DTClassBase,				"DTClass" ),
	myentry1( ExecBase,					"Exec" ),
	myentry1( ExpansionBase,			"Expansion" ),
	myentry1( GadToolsBase,				"GadTools" ),
	myentry1( GraphicsBase,				"Graphics" ),
	myentry1( IconBase,					"Icon" ),
	myentry1( IFFParseBase,				"IFFParse" ),
//	myentry1( InputBase,				"Input" ),
	myentry1( IntuitionBase,			"Intuition" ),
	myentry1( KeymapBase,				"keymap" ),
	myentry1( LayersBase,				"Layers" ),
	myentry1( LocaleBase,				"Locale" ),
	myentry1( MathffpBase,				"Mathffp" ),
	myentry1( MathieeedoubbasBase,		"Mathieeedoubbas" ),
	myentry1( MathieeedoubtransBase,	"Mathieeedoubtrans" ),
	myentry1( MathieeesingbasBase,		"Mathieeesingbas" ),
	myentry1( MathieeesingtransBase,	"Mathieeesingtrans" ),
	myentry1( MathtransBase,			"Mathtrans" ),
	myentry1( MiscBase,					"Misc" ),
	myentry1( PotgoBase,				"Potgo" ),
//	myentry1( RamdriveBase,				"Ramdrive" ),
	myentry1( RexxsyslibBase,			"Rexxsyslib" ),
//	myentry1( TimerBase,				"Timer" ),
	myentry1( UtilityBase,				"Utility" ),
	myentry1( WorkbenchBase,			"Workbench" ),
	{ 0, 0, NULL, NULL }
};

// --

char *AmigaOS3_FindLibFunc( enum RS4ErrorCode *errcode, RS4Trace *rt, int16_t offset )
{
enum RS4DecodeStat ds;
enum AOS3_LibType lib;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
AOS3_LVOStruct *liblvos;
AOS3_RegStruct *rs;
RS4Label *rl;
char *lvo;
int pos1;
int pos2;
int pos3;
int reg;

	ec	= RS4ErrStat_Okay;
	lvo	= NULL;
	lib	= ( rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ].mr_Type2 );

	#ifdef DEBUG
	enum RS4RegType type = ( rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ].mr_Type1 );
	if ( type != RRT_Library )
	{
		printf( "Not RRT_Library : was %d\n", type );
	}
	#endif

	// -- Search Library types

	pos1 = 0;

	while( myLibs[pos1].Data1 )
	{
		if ( myLibs[pos1].Type1 == lib )
		{
			break;
		}
		else
		{
			pos1++;
		}
	}

	if ( ! myLibs[pos1].Data1 )
	{
		// Not an Error

		ec = RS4ErrStat_Okay;

		#ifdef DEBUG
		printf( "Unknown : LibType %d : Offset %d : libcall at $%08lx\n", lib, offset, rt->rt_CurMemAdr );
		#endif

		goto bailout;
	}

	// -- Search Offset

	liblvos = myLibs[pos1].Data1;

	pos2 = 0;

	while( liblvos[pos2].Name2 )
	{
		if ( liblvos[pos2].Offset2 == offset )
		{
			break;
		}
		else
		{
			pos2++;
		}
	}

	if ( ! liblvos[pos2].Name2 )
	{
		// Not an Error

		ec = RS4ErrStat_Okay;

		#ifdef DEBUG
		printf( "Unknown : LibType %d : libcall %d : Address $%08lx\n", lib, offset, rt->rt_CurMemAdr );
		#endif

		goto bailout;
	}

	// --

	switch( liblvos[pos2].Type2 )
	{
		case AOS3_LVOType_Name:	// Only define Name and Offset
		{
			// We only have a name, so do nothing
			break;
		}

		case AOS3_LVOType_Regs:	// Name+Offset and Reg struct
		{
			rs = liblvos[pos2].Regs2;

			#ifdef DEBUG
			if ( ! rs )
			{
				ec = RS4ErrStat_Internal;
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				goto bailout;
			}
			#endif

			pos3 = -1;

			while( ++pos3 < 16 )
			{
				reg = rs[pos3].Reg3;

				if ( reg < 0 )
				{
					break;
				}

				switch( rs[pos3].Type3 )
				{
					case AOS3_RegType_String:
					{
						if ( rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Type1 != RRT_Label )
						{
							continue;
						}

						rl = rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Label;

						if ( ! rl )
						{
							continue;
						}

						Mark_NulString( rl );
						break;
					}

					case AOS3_RegType_Struct:
					{
						if ( rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Type1 != RRT_Label )
						{
							continue;
						}

						rl = rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Label;

						if ( ! rl )
						{
							continue;
						}

						fs = Mark_Struct( & ec, rl, rs[ reg ].Val3 );

						if ( fs != RS4FuncStat_Okay )
						{
							// ec allready set

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}
						break;
					}

					default:
					{
						ec = RS4ErrStat_Internal;

						#ifdef DEBUG
						printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
						#endif

						goto bailout;
					}
				}
			}
			break;
		}

		case AOS3_LVOType_Func:	// Name+Offset and Function
		{
			rs = liblvos[pos2].Regs2;

			#ifdef DEBUG
			if ( ! liblvos[pos2].Func2 )
			{
				ec = RS4ErrStat_Internal;
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				goto bailout;
			}
			#endif

			ds = liblvos[pos2].Func2( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
			break;
		}

		default:
		{
			ec = RS4ErrStat_Internal;

			#ifdef DEBUG
			printf( "%s:%04d: Error : Type %d\n", __FILE__, __LINE__, liblvos[pos2].Type2 );
			#endif

			goto bailout;
		}
	}

	// --

	myLibs[pos1].Used1 = TRUE;
	liblvos[pos2].Used2 = TRUE;
	lvo = liblvos[pos2].Name2;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( lvo );
}

// --

enum RS4FuncStat AmigaOS3_SaveLibFunc( enum RS4ErrorCode *errcode )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
AOS3_LVOStruct *liblvos;
int pos1;
int pos2;
int len;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// -- Search Library types

	pos1 = 0;

	while( myLibs[pos1].Data1 )
	{
		if ( myLibs[pos1].Used1 )
		{
			sprintf( SaveLineBuffer, "\n; %s\n", myLibs[pos1].Name1 );

			fs = RS4SaveWriteString( & ec, SaveLineBuffer, strlen( SaveLineBuffer ));

			if ( fs != RS4FuncStat_Okay )
			{
				printf( "%s:%04d: Error Writting Exec lvo's\n", __FILE__, __LINE__ );
				goto bailout;
			}

			liblvos = myLibs[pos1].Data1;

			pos2 = 0;

			while( liblvos[pos2].Name2 )
			{
				if ( liblvos[pos2].Used2 )
				{
					len = strlen( liblvos[pos2].Name2 ) + 1;

					if ( len < 8 )
					{
						sprintf( SaveLineBuffer, "%s:\t\t\tEQU\t%d\n", liblvos[pos2].Name2, liblvos[pos2].Offset2 );
					}
					else if ( len < 16 )
					{
						sprintf( SaveLineBuffer, "%s:\t\tEQU\t%d\n", liblvos[pos2].Name2, liblvos[pos2].Offset2 );
					}
					else if ( len < 24 )
					{
						sprintf( SaveLineBuffer, "%s:\tEQU\t%d\n", liblvos[pos2].Name2, liblvos[pos2].Offset2 );
					}
					else
					{
						sprintf( SaveLineBuffer, "%s: EQU\t%d\n", liblvos[pos2].Name2, liblvos[pos2].Offset2 );
					}

					fs = RS4SaveWriteString( & ec, SaveLineBuffer, strlen( SaveLineBuffer ));

					if ( fs != RS4FuncStat_Okay )
					{
						// ec allready set

						#ifdef DEBUG
						printf( "%s:%04d: Error Writting Exec Data\n", __FILE__, __LINE__ );
						#endif

						goto bailout;
					}
				}

				pos2++;
			}
		}

		pos1++;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}
