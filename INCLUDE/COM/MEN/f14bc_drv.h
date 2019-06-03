/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  f14bc_drv.h
 *
 *      \author  dieter.pfeuffer@men.de
 *
 *       \brief  Header file for F14BC driver containing
 *               F14BC specific status codes and
 *               F14BC function prototypes
 *
 *    \switches  _ONE_NAMESPACE_PER_DRIVER_
 *               _LL_DRV_
 */
 /*
 *---------------------------------------------------------------------------
 * Copyright (c) 2019, MEN Mikro Elektronik GmbH
 ****************************************************************************/
/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _F14BC_DRV_H
#define _F14BC_DRV_H

#ifdef __cplusplus
      extern "C" {
#endif

/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/** \name F14BC specific Getstat/Setstat codes
 *  \anchor getstat_setstat_codes
 *  (*) possible with FW-Rev 1.x
 */
/**@{*/
#define F14BC_SWRESET		M_DEV_OF+0x00   /**< S: perform a software reset (*)\n
											      Values: 0xdead\n
												  Default: -\n*/
#define F14BC_VOLT_IN		M_DEV_OF+0x01   /**< G: get PIC input voltage of current channel\n
											      Values: [mV]\n
												  Default: -\n*/
#define F14BC_VOLT_NOM		M_DEV_OF+0x02   /**< G: get nominal voltage of current channel\n
											      Values: [mV]\n
												  Default: -\n*/
#define F14BC_VOLT_DEV		M_DEV_OF+0x03   /**< G: get deviation voltage of current channel\n
											      Values: [mV]\n
												  Default: -\n*/
#define F14BC_VOLT_SIGSET	M_DEV_OF+0x04   /**< S: install an user signal for voltage supervision\n
											      Values: signal number (see usr_os.h)\n
												  Default: -\n*/
#define F14BC_VOLT_SIGCLR	M_DEV_OF+0x05   /**< S: deinstall the user signal for voltage supervision\n
											      Values: -\n
												  Default: -\n*/
#define F14BC_VOLT_SVSTATE	M_DEV_OF+0x06   /**< G: last supervision error state,
												  querried from the driver’s poll-loop\n
											      Values: bit 4..0 corresponds to supervised voltage 4..0\n
												  Default: -\n*/
#define F14BC_ERRCNT_WDOG	M_DEV_OF+0x07   /**< G: 8-bit error counter "watchdog reset"\n
											      Values: 0x00..0xff\n
												  Default: 0\n*/
#define F14BC_ERRCNT_BIOS_1	M_DEV_OF+0x08   /**< G: 8-bit error counter "no BIOS start state 1"\n
											      Values: 0x00..0xff\n
												  Default: 0\n*/
#define F14BC_ERRCNT_BIOS_2	M_DEV_OF+0x09   /**< G: 8-bit error counter "no BIOS start state 2"\n
											      Values: 0x00..0xff\n
												  Default: 0\n*/
#define F14BC_ERRCNT_SLP_S5	M_DEV_OF+0x0a   /**< G: 8-bit error counter "signal SLP_S5 not high"\n
											      Values: 0x00..0xff\n
												  Default: 0\n*/
#define F14BC_ERRCNT_RST	M_DEV_OF+0x0b   /**< G: 8-bit error counter "signal PLT_RST not deasserted"\n
											      Values: 0x00..0xff\n
												  Default: 0\n*/
#define F14BC_ERRCNT_TEMP	M_DEV_OF+0x0c   /**< G: 8-bit error counter "temperature emergency cutout"\n
											      Values: 0x00..0xff\n
												  Default: 0\n*/
#define F14BC_ERRCNT_PROCHOT M_DEV_OF+0x0d  /**< G: 8-bit error counter "processor hot"\n
											      Values: 0x00..0xff\n
												  Default: 0\n*/
#define F14BC_ERRCNT_CLR	M_DEV_OF+0x0e   /**< S: clear all error counters\n
											      Values: -\n
												  Default: -\n*/


/* F14BC specific Getstat/Setstat block codes */
#define F14BC_BLK_FWREV		M_DEV_BLK_OF+0x00    /**< S: firmware revision\n
											      Values: six ascii characters\n
												  Default: -\n*/
/**@}*/

#ifndef  F14BC_VARIANT
# define F14BC_VARIANT F14BC
#endif

# define _F14BC_GLOBNAME(var,name) var##_##name
#ifndef _ONE_NAMESPACE_PER_DRIVER_
# define F14BC_GLOBNAME(var,name) _F14BC_GLOBNAME(var,name)
#else
# define F14BC_GLOBNAME(var,name) _F14BC_GLOBNAME(F14BC,name)
#endif

#define __F14BC_GetEntry    F14BC_GLOBNAME(F14BC_VARIANT,GetEntry)

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
#ifdef _LL_DRV_
#ifndef _ONE_NAMESPACE_PER_DRIVER_
	extern void __F14BC_GetEntry(LL_ENTRY* drvP);
#endif
#endif /* _LL_DRV_ */

/*-----------------------------------------+
|  BACKWARD COMPATIBILITY TO MDIS4         |
+-----------------------------------------*/
#ifndef U_INT32_OR_64
 /* we have an MDIS4 men_types.h and mdis_api.h included */
 /* only 32bit compatibility needed!                     */
 #define INT32_OR_64  int32
 #define U_INT32_OR_64 u_int32
 typedef INT32_OR_64  MDIS_PATH;
#endif /* U_INT32_OR_64 */


#ifdef __cplusplus
      }
#endif

#endif /* _F14BC_DRV_H */
