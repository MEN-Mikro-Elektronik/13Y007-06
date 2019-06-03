/****************************************************************************
 ************                                                    ************
 ************                    F14BC_CTRL                        ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file f14bc_ctrl.c
 *       \author dieter.pfeuffer@men.de
 *
 *        \brief Tool to control F14BC PIC
 *
 *     Required: libraries: mdis_api, usr_oss, usr_utl
 *     \switches (none)
 */
 /*
 *---------------------------------------------------------------------------
 * Copyright (c) 2006-2019, MEN Mikro Elektronik GmbH
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

#include <stdio.h>
#include <stdlib.h>
#include <MEN/men_typs.h>
#include <MEN/usr_oss.h>
#include <MEN/usr_utl.h>
#include <MEN/mdis_api.h>
#include <MEN/f14bc_drv.h>
#include <MEN/f14bci.h>

static const char IdentString[]=MENT_XSTR(MAK_REVISION);

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
#define NONE			0
#define	IO_MAX_NBR		8
#define MAX_VOLT_CH		8

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/
static MDIS_PATH  G_Path;

static struct {
	int32	nom;
	int32	dev;
} G_VoltCh[MAX_VOLT_CH];	/* volt channel values */

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void usage(void);
static void PrintMdisError(char *info);
static void PrintUosError(char *info);

/********************************* usage ***********************************/
/**  Print program usage
 */
static void usage(void)
{
	printf("Usage: f14bc_ctrl [<opts>] <device> [<opts>]\n");
	printf("Function: Control F14BC PIC\n");
	printf("Options:\n");
	printf("    device       device name                               \n");
	printf("    -v           show voltage values                       \n");
	printf("    -s           do voltage supervision (requires option -v)\n");
	printf("    -r=0xdead    perform SW reset (!!! danger command !!!) \n");
	printf("    -e           show error counters                       \n");
	printf("    -c           clear error counters                      \n");
	printf("    -f           show firmware revision                    \n");
	printf("Note: Only option -r supports PIC firmware with rev. < 2.0\n");
	printf("\n");
	printf("Copyright (c) 2006-2019, MEN Mikro Elektronik GmbH\n%s\n", IdentString);
}

/********************************* main ************************************/
/** Program main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 *
 *  \return	          success (0) or error (1)
 */
int main( int argc, char *argv[])
{
	char	*device,*str,*errstr,buf[40];
	int32	volt, superv, swr, err, clear, frev;
	int32	n, val, ch;
	int32	chNbr, in;

	/*--------------------+
    |  check arguments    |
    +--------------------*/
	if( (errstr = UTL_ILLIOPT("vsr=ecf?", buf)) ){	/* check args */
		printf("*** %s\n", errstr);
		return(1);
	}

	if( UTL_TSTOPT("?") ){						/* help requested ? */
		usage();
		return(1);
	}

	/*--------------------+
    |  get arguments      |
    +--------------------*/
	for (device=NULL, n=1; n<argc; n++)
		if( *argv[n] != '-' ){
			device = argv[n];
			break;
		}

	if( !device || argc<3 ) {
		usage();
		return(1);
	}

	volt = (UTL_TSTOPT("v") ? 1 : NONE);
	superv = (UTL_TSTOPT("s") ? 1 : NONE);
	if( superv && !volt ){
		printf("*** option -s requires option -v\n");
		return(1);
	}

	if( (str = UTL_TSTOPT("r=")) )
		sscanf( str, "%x", &swr );
	else
		swr = NONE;

	err = (UTL_TSTOPT("e") ? 1 : NONE);
	clear = (UTL_TSTOPT("c") ? 1 : NONE);
	frev = (UTL_TSTOPT("f") ? 1 : NONE);

	/*--------------------+
    |  open path          |
    +--------------------*/
	if( (G_Path = M_open(device)) < 0) {
		PrintMdisError("open");
		return(1);
	}

	/*----------------------+
    |  show voltage values  |
    +----------------------*/
	if( volt != NONE ){
		printf("ch   in[mV]   nominal[mV]   deviation(range)[mV]\n");

		if ((M_getstat(G_Path, M_LL_CH_NUMBER, &chNbr)) < 0) {
			PrintMdisError("getstat M_LL_CH_NUMBER");
			goto abort;
		}

		if( chNbr > MAX_VOLT_CH ){
			printf("F14BC_CTRL supports only %d volt-ch (drv. reports %d)",
				MAX_VOLT_CH, chNbr);
			chNbr = MAX_VOLT_CH;
		}

		for( ch=0; ch<chNbr; ch++ ){

			/* select channel */
			if( (M_setstat(G_Path, M_MK_CH_CURRENT, ch)) < 0) {
				PrintMdisError("setstat M_MK_CH_CURRENT");
				goto abort;
			}

			/* get in voltage */
			if ((M_getstat(G_Path, F14BC_VOLT_IN, &in)) < 0) {
				PrintMdisError("getstat F14BC_VOLT_IN");
				goto abort;
			}

			/* get nominal voltage */
			if ((M_getstat(G_Path, F14BC_VOLT_NOM, &G_VoltCh[ch].nom)) < 0) {
				PrintMdisError("getstat F14BC_VOLT_NOM");
				goto abort;
			}

			/* get deviation voltage */
			if ((M_getstat(G_Path, F14BC_VOLT_DEV, &G_VoltCh[ch].dev)) < 0) {
				PrintMdisError("getstat F14BC_VOLT_DEV");
				goto abort;
			}

			printf(" %d     %4d          ", ch, in);

			if( G_VoltCh[ch].nom )
				printf("%4d      ", G_VoltCh[ch].nom);
			else
				printf("   -         ");

			if( G_VoltCh[ch].dev )
				printf("%4d (%4d..%4d)\n",
					G_VoltCh[ch].dev,
					G_VoltCh[ch].nom - G_VoltCh[ch].dev,
					G_VoltCh[ch].nom + G_VoltCh[ch].dev);
			else
				printf("-\n");
		}
	}

	/*----------------------+
    |  voltage supervision  |
    +----------------------*/
	if( superv != NONE ){
		u_int32 rcvSig;
		int32	lastSvState;

		printf("Voltage supervision:\n");
		printf("The driver polls the voltage supervision state and sends\n");
		printf("a signal to the app. if a voltage over-/underrun occures.\n");
		printf("Then, the app. gets the last polled voltage supervision state.\n");

		/* init signal handling */
		if( UOS_SigInit(NULL) ){
			PrintUosError("SigInit");
			return(1);
		}

		/* install signal */
		if( UOS_SigInstall(UOS_SIG_USR1) ){
			PrintUosError("SigInstall");
			goto abort;
		}

		UOS_SigMask();

		/* install signal for voltage supervision */
		if( (M_setstat(G_Path, F14BC_VOLT_SIGSET, (int32)UOS_SIG_USR1) < 0)) {
			PrintMdisError("setstat F14BC_VOLT_SIGSET");
			goto abort;
		}

		printf("--- press any key to abort ---\n");

		do{
			UOS_SigWait( 100, &rcvSig );

			/* our signal? */
			if( rcvSig == UOS_SIG_USR1 ){

				/* get last supervision error state */
				if ((M_getstat(G_Path, F14BC_VOLT_SVSTATE, &lastSvState)) < 0) {
					PrintMdisError("getstat F14BC_VOLT_SVSTATE");
					goto abort;
				}

				for( ch=0; ch<MAX_VOLT_CH; ch++ ){

					if( !G_VoltCh[ch].nom )
						continue;

					/* voltage out of range? */
					if( 0x01 & (lastSvState >> ch) ){

						printf(">>> volt-ch %d (nominal=%dmV) out of range (%dmV..%dmV)\n",
							ch, G_VoltCh[ch].nom,
							G_VoltCh[ch].nom - G_VoltCh[ch].dev,
							G_VoltCh[ch].nom + G_VoltCh[ch].dev);
					}
				}
			}
		} while( (UOS_KeyPressed() == -1) );
	}

	/*--------------------+
    |  sw reset           |
    +--------------------*/
	/* set shutdown delay */
	if( swr != NONE ){
		if( (M_setstat(G_Path, F14BC_SWRESET, swr)) < 0) {
			PrintMdisError("setstat F14BC_SWRESET");
			goto abort;
		}
	}

	/*----------------------+
    |  show error counters  |
    +----------------------*/
	if( err != NONE ){
		printf("Error Counters:\n");

		/* watchdog reset */
		if ((M_getstat(G_Path, F14BC_ERRCNT_WDOG, &val)) < 0) {
			PrintMdisError("getstat F14BC_ERRCNT_WDOG");
			goto abort;
		}
		printf("- watchdog reset                : 0x%02x\n", (u_int8)val );

		/* no BIOS start state 1 */
		if ((M_getstat(G_Path, F14BC_ERRCNT_BIOS_1, &val)) < 0) {
			PrintMdisError("getstat F14BC_ERRCNT_BIOS_1");
			goto abort;
		}
		printf("- no BIOS start state 1         : 0x%02x\n", (u_int8)val );

		/* no BIOS start state 2 */
		if ((M_getstat(G_Path, F14BC_ERRCNT_BIOS_2, &val)) < 0) {
			PrintMdisError("getstat F14BC_ERRCNT_BIOS_2");
			goto abort;
		}
		printf("- no BIOS start state 2         : 0x%02x\n", (u_int8)val );

		/* signal SLP_S5 not high */
		if ((M_getstat(G_Path, F14BC_ERRCNT_SLP_S5, &val)) < 0) {
			PrintMdisError("getstat F14BC_ERRCNT_SLP_S5");
			goto abort;
		}
		printf("- signal SLP_S5 not high        : 0x%02x\n", (u_int8)val );

		/* signal PLT_RST not deasserted */
		if ((M_getstat(G_Path, F14BC_ERRCNT_RST, &val)) < 0) {
			PrintMdisError("getstat F14BC_ERRCNT_RST");
			goto abort;
		}
		printf("- signal PLT_RST not deasserted : 0x%02x\n", (u_int8)val );

		/* temperature emergency cutout */
		if ((M_getstat(G_Path, F14BC_ERRCNT_TEMP, &val)) < 0) {
			PrintMdisError("getstat F14BC_ERRCNT_TEMP");
			goto abort;
		}
		printf("- temperature emergency cutout  : 0x%02x\n", (u_int8)val );

		/* processor hot */
		if ((M_getstat(G_Path, F14BC_ERRCNT_PROCHOT, &val)) < 0) {
			PrintMdisError("getstat F14BC_ERRCNT_PROCHOT");
			goto abort;
		}
		printf("- processor hot                 : 0x%02x\n", (u_int8)val );
	}

	/*--------------------+
    |  clr error counters |
    +--------------------*/
	if( clear != NONE ){
		printf("clearing error counters\n");
		if( (M_setstat(G_Path, F14BC_ERRCNT_CLR, 0)) < 0) {
			PrintMdisError("setstat F14BC_ERRCNT_CLR");
			goto abort;
		}
	}

	/*-------------------------------+
    |  show firmware revision        |
    +-------------------------------*/
	if( frev != NONE ){

		M_SG_BLOCK	blk;
		char		rev[6];

		blk.size = sizeof(rev);
		blk.data = (void*)&rev;

		if ((M_getstat(G_Path, F14BC_BLK_FWREV, (int32*)&blk)) < 0) {
			PrintMdisError("getstat F14BC_BLK_FWREV");
			goto abort;
		}
		printf("firmware rev.: %c%c.%c%c.%c%c\n",
			rev[0],rev[1],rev[2],rev[3],rev[4],rev[5]);
	}

	/*--------------------+
    |  cleanup            |
    +--------------------*/
	abort:

	if( superv != NONE ){
		UOS_SigExit();
	}

	if( M_close(G_Path) < 0 )
		PrintMdisError("close");

	return(0);
}

/********************************* PrintMdisError **************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
*/
static void PrintMdisError(char *info)
{
	printf("*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
}

/********************************* PrintUosError ***************************/
/** Print User OSS error message
 *
 *  \param info       \IN  info string
*/
static void PrintUosError(char *info)
{
	printf("*** can't %s: %s\n", info, UOS_ErrString(UOS_ErrnoGet()));
}

