/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  wdog.h
 *
 *      \author  dp
 *        $Date: 2006/05/26 15:08:34 $
 *    $Revision: 2.6 $
 *
 *       \brief  Header file for Watchdog drivers
 *               - Watchdog specific status codes
 *
 *    \switches  ---
 */
 /*
 *---------------------------------------------------------------------------
 * Copyright (c) 1999-2019, MEN Mikro Elektronik GmbH
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

#ifndef _WDOG_H
#define _WDOG_H

#ifdef __cplusplus
      extern "C" {
#endif

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
#define M_CH_PROFILE_WDOG	200		/**< profile type (returned in M_LL_CH_TYP */

/** \name Watchdog specific Getstat/Setstat standard codes 
 *  \anchor wdog_getstat_setstat_codes
 */
/**@{*/
#define WDOG_START		M_DEV_OF+0xe0		/**<  S: Start Watchdog					*/
#define WDOG_TRIG		M_DEV_OF+0xe1		/**<  S: Trigger Watchdog				*/
#define WDOG_STOP		M_DEV_OF+0xe2		/**<  S: Stop Watchdog					*/
#define WDOG_TIME		M_DEV_OF+0xe3		/**<G,S: Get-/Set Watchdog Time (ms)	    */
#define WDOG_STATUS		M_DEV_OF+0xe4		/**<G  : Get Status (0:deaktiv, 1:aktiv) */
#define WDOG_SHOT		M_DEV_OF+0xe5		/**<G  : 0: no wdog shot, 1: wdog shot, 
                                                     255 = not identifiable */ 
/**@}*/

#ifdef __cplusplus
      }
#endif

#endif /* WDOG_H */




