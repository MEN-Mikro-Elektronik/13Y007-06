/***********************  I n c l u d e  -  F i l e  ************************/
/*!  
 *        \file  f14bci.h
 *
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2007/12/03 17:53:42 $
 *    $Revision: 3.3 $
 * 
 *  	 \brief  Header file for F14 board-controller firmware interface
 *                      
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: f14bci.h,v $
 * Revision 3.3  2007/12/03 17:53:42  SVogel
 * update, due to new build
 *
 * Revision 3.2  2007/06/08 15:13:30  SVogel
 * cosmetics
 *
 * Revision 3.1  2006/11/08 09:45:48  DPfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006 by MEN Mikro Elektronik GmbH, Nuremberg, Germany 
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

#ifndef _F14BCI_H
#define _F14BCI_H

#ifdef __cplusplus
	extern "C" {
#endif


/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
/**
 * @defgroup _SMBCMDS firmware SMB-Cmds
 * Note:\n
 *   - (C / B / W) indicates Cmd only/Cmd+Byte/Cmd+Word access
 *   - (r / w) indicates, that read/write access is supported
 *             by PIC firmware rev. 2.0 and later
 *   - (r* /w*) indicates, that the read/write access is also
 *              supported by PIC firmware rev. 1.x   
 *   - PIC firmware rev. 1.x only features are not considered here
 * \code
   Mode          :   0  | 1 | 2 |  3 | 4 |  5 |  6 |  7 |   8 |   9 |  10 |   11
   Timeout [sec] : none | 1 | 2 |  4 | 8 | 16 | 32 | 64 | 128 | 256 | 512 | 1024
   \endcode
 * @{ */
#define F14BCI_WDON				(0x11)	/**< watchdog ON (C*) */
#define F14BCI_WDOFF			(0x12)	/**< watchdog OFF (C* / Bw) */
#	define F14BCI_WDOFF_BYTE	(0x69)	/**< watchdog OFF keyword */
#define F14BCI_WDTRIG			(0x13)	/**< watchdog trigger (C*) */
#define F14BCI_WDTIME			(0x14)	/**< watchdog time (Wr / Ww*) */
#	define F14BCI_WDTIME_MSUNIT	    (100)		/**< unit of time in ms */
#	define F14BCI_WDTIME_MAX		(65500)	/**< max time in 100ms units */
#define F14BCI_WDSTATE			(0x17)	/**< get watchdog state, 0=OFF/1=ON (Br) */
#define F14BCI_SMB_ADDR         (0x35)	/**< overwrite default SMB address (Bw) */ 
#define F14BCI_SWRESET			(0x31)	/**< software reset (w*,Ww) */
#	define F14BCI_SWRESET_WORD		(0xdead)	/**< software reset keyword */

#define F14BCI_VOLT_SVSTATE		(0x3a)	/**< get state of supervised voltages (Br)
											 bit 6..0 reflects state of voltage 6..0
											 - 0=voltage in range or not supervised
											 - 1=voltage out of range */

#define F14BCI_VOLT_NOM(c)      (0x40+(c))	/**< nominal voltage in 25mV units 
											     or 0 if not supported (Br/w)*/
#define F14BCI_VOLT_DEV(c)      (0x50+(c))	/**< deviation voltage in 25mV units 
											     or 0 if not supported (Br/w)*/
#define F14BCI_VOLT_IN(c)       (0x60+(c))	/**< PIC input voltage in 25mV units (Br) */
#	define F14BCI_VOLT_NBR			(7)		/**< number of supported voltages
														Note: c=0..F14BCI_VOLT_NBR-1 */	
#	define F14BCI_VOLT_MVUNIT	    (25)		/**< unit of voltage in mV */

#define F14BCI_ERRCNT_NO_TRIG	(0x70)	/**< counter: no trigger occured, wdg enabled (Br) */
#define F14BCI_ERRCNT_BIOS_1	(0x71)	/**< counter: no BIOS start state 1 (Br) */
#define F14BCI_ERRCNT_BIOS_2	(0x72)	/**< counter: no BIOS start state 2 (Br) */
#define F14BCI_ERRCNT_NO_SLP_S5	(0x73)	/**< counter: signal SLP_S5 not high (Br) */
#define F14BCI_ERRCNT_RST_ACTIVE (0x74)	/**< counter: signal PLT_RST not deasserted (Br) */
#define F14BCI_ERRCNT_TEMP		(0x75)	/**< counter: temperature emergency cutout (Br) */
#define F14BCI_ERRCNT_PROCHOT	(0x76)	/**< counter: processor hot (Br) */
#define F14BCI_ERRCNT_CLR		(0x7a)	/**< clear all error counters (C) */

#define F14BCI_GETREV(b)		(0x80+(b))	/**< firmware revision ascii character 0..5 (r*)
											     Note: don't change these offset range! */
#	define F14BCI_REV_BYTESIZE		(6)			/**< byte size of firmware revision */
/**@}*/

#ifdef __cplusplus
	}
#endif

#endif	/* _F14BCI_H */
