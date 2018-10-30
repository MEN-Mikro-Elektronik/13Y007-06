/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  f14bc_doc.c
 *
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2009/08/31 14:14:19 $
 *    $Revision: 1.3 $
 *
 *      \brief   User documentation for F14BC device driver
 *
 *     Required: -
 *
 *     \switches -
 */
 /*
 *---------------------------------------------------------------------------
 * (c) Copyright 2006 by MEN Mikro Elektronik GmbH, Nuernberg, Germany
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

/*! \mainpage
    This is the documentation of the MDIS5 low-level driver for the
	F14/F15 board-controller (F14BC).

    The F14BC is a PIC microcontroller that resides on the SMBus of the F14/F15
	CPU board. The F14BC with it's firmware controls the board power sequencing,
	performs voltage and temperature supervision, provides system watchdog and
	SW-reset functionality and error state logging.

	Note: MDIS5 32bit drivers are compatible to the MDIS4 drivers but must not
          be mixed with MDIS4 drivers at one target system.

    <b>Common Notes:</b>
	- The driver requires MDIS with SMB2 support

    \n
    \section Variants Variants
    The F14BC driver can be built in the following variants at compilation time:

    \code
    Driver              Variant Description
    --------            --------------------------------
    Standard            F14BC standard driver
    \endcode

    \n \section FuncDesc Functional Description

    \n \subsection General General
	The driver supports for PIC firmware revision 1.x only a restricted	watchdog
	functionality and a SW-reset. For PIC firmware revision 2.0 and later,
	all driver functionality is available.

	All driver functionality is provided via M_setstat() and M_getstat(). The
	Getstat/Setstat codes that are possible with PIC firmware revision 1.x
	are appropriate marked.

	When the first path is opened to an F14BC device, the HW and the driver are being
	initialized with default values	(see section about \ref descriptor_entries).

    \n \subsection channels Logical channels
	The driver provides seven logical channels that relates to the supervised
	voltages of the F14BC. All other functionality is channel independent.\n

    \n \section api_functions Supported API Functions

    <table border="0">
    <tr>
        <td><b>API Function</b></td>
        <td><b>Functionality</b></td>
        <td><b>Corresponding Low-Level Function</b></td></tr>

    <tr><td>M_open()      </td><td>Open device              </td>
	<td>F14BC_Init()</td></tr>
    <tr><td>M_close()     </td><td>Close device             </td>
    <td>F14BC_Exit())</td></tr>
    <tr><td>M_setstat()   </td><td>Set device parameter     </td>
    <td>F14BC_SetStat()</td></tr>
    <tr><td>M_getstat()   </td><td>Get device parameter     </td>
    <td>F14BC_GetStat()</td></tr>
    <tr><td>M_errstringTs() </td><td>Generate error message </td>
    <td>-</td></tr>
    </table>

    \n \section descriptor_entries F14BC Descriptor Entries

    The low-level driver initialization routine decodes the following entries
    ("keys") in addition to the general descriptor keys:

    <table border="0">
    <tr><td><b>Descriptor entry</b></td>
        <td><b>Description</b></td>
        <td><b>Values</b></td>
    </tr>
    <tr><td>SMB_BUSNBR</td>
        <td>SMBus bus number</td>
        <td>0, 1, 2, ...\n
			Default: 0</td>
    </tr>
    <tr><td>SMB_DEVADDR</td>
        <td>SMBus address of F14BC</td>
        <td>0x00, 0xff\n
			Default: 0x9a</td>
    </tr>
     <tr><td>POLL_PERIOD</td>
        <td>Poll period for voltage supervision\n
		    (only for FW-Rev. 2.0 and later)</td>
        <td>50..5000ms\n
			Default: 500ms</td>
    </tr>
    </tr>
    </table>

    \n \subsection f14bc_min   Minimum descriptor
    f14bc_min.dsc (see Examples section)\n
    Demonstrates the minimum set of options necessary for using the driver.

    \n \subsection f14bc_max   Maximum descriptor
    f14bc_max.dsc (see Examples section)\n
    Shows all possible configuration options for this driver.

    \n \section codes F14BC specific Getstat/Setstat codes
	For the watchdog functionality, the driver supports a subset of the WDOG
	Getstat/Setstat codes (defined in wdog.h):
	- #WDOG_START (*)
	- #WDOG_STOP (*)
	- #WDOG_TRIG (*)
	- #WDOG_TIME (M_setstat*)
	- #WDOG_STATUS\n
	(*) : possible with FW-Rev. 1.x

	The other driver functionality is supported by the F14_MON Getstat/Setstat
	codes.
	See \ref getstat_setstat_codes "section about F14_MON Getstat/Setstat codes".

    \n \section programs Overview of provided programs

    \subsection f14bc_ctrl  Tool that controls the F14BC PIC (example of using the F14BC driver)
    f14bc_ctrl.c (see Examples section)

    \subsection wdog_test  Tool to service and test the watchdog
    wdog_test.c (see example section)

    \subsection wdog_simp  Simple example program for watchdog drivers
    wdog_simp.c (see example section)
*/

/** \example f14bc_ctrl.c */
/** \example wdog_test.c */
/** \example wdog_simp.c */
/** \example f14bc_min.dsc */
/** \example f14bc_max.dsc */

/*! \page dummy
  \menimages
*/
