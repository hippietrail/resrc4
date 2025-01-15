
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

//enum RS4DecodeStat AOS3_Bullet_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_BulletBase[] =
{
	AOS3_LVO_Name( -30, OpenEngine ),
	AOS3_LVO_Name( -36, CloseEngine ),
	AOS3_LVO_Name( -42, SetInfoA ),
	AOS3_LVO_Name( -48, ObtainInfoA ),
	AOS3_LVO_Name( -54, ReleaseInfoA ),
	AOS3_LVO_EndMarker()
};

// --
