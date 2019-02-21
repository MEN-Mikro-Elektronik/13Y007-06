/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  f14bc_drv.c
 *
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2009/09/04 11:32:21 $
 *    $Revision: 1.4 $
 *
 *      \brief   Low-level driver for F14BC device on SMBus
 *
 *     Required: OSS, DESC, DBG, libraries
 *
 *     \switches _ONE_NAMESPACE_PER_DRIVER_, F14BC_SW
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

#define _NO_LL_HANDLE		/* ll_defs.h: don't define LL_HANDLE struct */

#ifdef F14BC_SW				/* swapped variant */
#	define MAC_MEM_MAPPED
#	define ID_SW
#endif

#include <MEN/men_typs.h>   /* system dependent definitions */
#include <MEN/maccess.h>    /* hw access macros and types */
#include <MEN/dbg.h>        /* debug functions */
#include <MEN/oss.h>        /* oss functions */
#include <MEN/desc.h>       /* descriptor functions */
#include <MEN/mdis_api.h>   /* MDIS global defs */
#include <MEN/mdis_com.h>   /* MDIS common defs */
#include <MEN/mdis_err.h>   /* MDIS error codes */
#include <MEN/ll_defs.h>    /* low-level driver definitions */
#include <MEN/smb2.h>		/* SMB2 definitions */
#include <MEN/f14bci.h>		/* F14BC definitions	*/
#include <MEN/wdog.h>		/* watchdog specific definitions */

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/* general defines */
#define CH_NUMBER			F14BCI_VOLT_NBR	/**< Number of device channels */
#define CH_BYTES			4			/**< Number of bytes per channel */
#define USE_IRQ				FALSE		/**< Interrupt required  */
#define ADDRSPACE_COUNT		0			/**< Number of required address spaces */
#define ADDRSPACE_SIZE		0			/**< Size of address space */

/* debug defines */
#define DBG_MYLEVEL			llHdl->dbgLevel   /**< Debug level */
#define DBH					llHdl->dbgHdl     /**< Debug handle */

/* macro to map SMB2 error code into device specific error code range */
#define SMB2_ERRMAP(err)	(((err) && ((err) < (SMB_ERR_LAST))) ? \
								((err) += (ERR_DEV)) : (err))

/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/** low-level handle */
typedef struct {
	/* general */
    int32           memAlloc;		/**< Size allocated for the handle */
    OSS_HANDLE      *osHdl;         /**< OSS handle */
    OSS_IRQ_HANDLE  *irqHdl;        /**< IRQ handle */
    DESC_HANDLE     *descHdl;       /**< DESC handle */
	MDIS_IDENT_FUNCT_TBL idFuncTbl;	/**< ID function table */
	/* debug */
    u_int32         dbgLevel;		/**< Debug level */
	DBG_HANDLE      *dbgHdl;        /**< Debug handle */
	/* f14bc specific */
	SMB_HANDLE		*smbH;			/**< ptr to SMB_HANDLE struct */
	u_int16			smbAddr;		/**< SMB address of F14BC */
	u_int8 fwRev[F14BCI_REV_BYTESIZE]; /**< PIC firmware revision */
	u_int8			fwRevFirst;		/**< 1: first release of PIC firmware */
	u_int8			wdStatus;		/**< Watchdog status */
    OSS_SIG_HANDLE  *sigHdl;        /**< signal handle */
    OSS_ALARM_HANDLE *alarmHdl;		/**< alarm handle */
	u_int32			pollPeriod;		/**< poll period for alarm [ms] */
	u_int8			voltSvState;	/**< last voltage supervision state */
} LL_HANDLE;

/* include files which need LL_HANDLE */
#include <MEN/ll_entry.h>   /* low-level driver jump table  */
#include <MEN/f14bc_drv.h>	/* F14BC driver header file */

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
static int32 F14BC_Init(DESC_SPEC *descSpec, OSS_HANDLE *osHdl,
					   MACCESS *ma, OSS_SEM_HANDLE *devSemHdl,
					   OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP);
static int32 F14BC_Exit(LL_HANDLE **llHdlP );
static int32 F14BC_Read(LL_HANDLE *llHdl, int32 ch, int32 *value);
static int32 F14BC_Write(LL_HANDLE *llHdl, int32 ch, int32 value);
static int32 F14BC_SetStat(LL_HANDLE *llHdl,int32 ch, int32 code,
							INT32_OR_64 value32_or_64 );
static int32 F14BC_GetStat(LL_HANDLE *llHdl, int32 ch, int32 code,
							INT32_OR_64 *value32_or_64P );
static int32 F14BC_BlockRead(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
							int32 *nbrRdBytesP);
static int32 F14BC_BlockWrite(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
							 int32 *nbrWrBytesP);
static int32 F14BC_Irq(LL_HANDLE *llHdl );
static int32 F14BC_Info(int32 infoType, ... );

static char* Ident( void );
static int32 Cleanup(LL_HANDLE *llHdl, int32 retCode);

/* F14BC specific helper functions */
static void AlarmHandler(void *arg);

/****************************** F14BC_GetEntry ********************************/
/** Initialize driver's jump table
 *
 *  \param drvP     \OUT Pointer to the initialized jump table structure
 */
#ifdef _ONE_NAMESPACE_PER_DRIVER_
    void LL_GetEntry( LL_ENTRY* drvP )
#else
    void __F14BC_GetEntry( LL_ENTRY* drvP )
#endif /* _ONE_NAMESPACE_PER_DRIVER_ */
{
    drvP->init        = F14BC_Init;
    drvP->exit        = F14BC_Exit;
    drvP->read        = F14BC_Read;
    drvP->write       = F14BC_Write;
    drvP->blockRead   = F14BC_BlockRead;
    drvP->blockWrite  = F14BC_BlockWrite;
    drvP->setStat     = F14BC_SetStat;
    drvP->getStat     = F14BC_GetStat;
    drvP->irq         = F14BC_Irq;
    drvP->info        = F14BC_Info;
}

/******************************** F14BC_Init **********************************/
/** Allocate and return low-level handle, initialize hardware
 *
 * The function initializes the F14BC device with the definitions made
 * in the descriptor.
 *
 * The function decodes \ref descriptor_entries "these descriptor entries"
 * in addition to the general descriptor keys.
 *
 *  \param descP      \IN  Pointer to descriptor data
 *  \param osHdl      \IN  OSS handle
 *  \param ma         \IN  HW access handle
 *  \param devSemHdl  \IN  Device semaphore handle
 *  \param irqHdl     \IN  IRQ handle
 *  \param llHdlP     \OUT Pointer to low-level driver handle
 *
 *  \return           \c 0 On success or error code
 */
static int32 F14BC_Init(
    DESC_SPEC       *descP,
    OSS_HANDLE      *osHdl,
    MACCESS         *ma,
    OSS_SEM_HANDLE  *devSemHdl,
    OSS_IRQ_HANDLE  *irqHdl,
    LL_HANDLE       **llHdlP
)
{
    LL_HANDLE	*llHdl = NULL;
    u_int32		gotsize, smbBusNbr;
    int32		error;
    u_int32		value;
	u_int8		n, tmp;

    /*------------------------------+
    |  prepare the handle           |
    +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */

	/* alloc */
    if((llHdl = (LL_HANDLE*)OSS_MemGet(
    				osHdl, sizeof(LL_HANDLE), &gotsize)) == NULL)
       return(ERR_OSS_MEM_ALLOC);

	/* clear */
    OSS_MemFill(osHdl, gotsize, (char*)llHdl, 0x00);

	/* init */
    llHdl->memAlloc   = gotsize;
    llHdl->osHdl      = osHdl;
    llHdl->irqHdl     = irqHdl;

    /*------------------------------+
    |  init id function table       |
    +------------------------------*/
	/* driver's ident function */
	llHdl->idFuncTbl.idCall[0].identCall = Ident;
	/* library's ident functions */
	llHdl->idFuncTbl.idCall[1].identCall = DESC_Ident;
	llHdl->idFuncTbl.idCall[2].identCall = OSS_Ident;
	/* terminator */
	llHdl->idFuncTbl.idCall[3].identCall = NULL;

    /*------------------------------+
    |  prepare debugging            |
    +------------------------------*/
	DBG_MYLEVEL = OSS_DBG_DEFAULT;	/* set OS specific debug level */
	DBGINIT((NULL,&DBH));

    /*------------------------------+
    |  scan descriptor              |
    +------------------------------*/
	/* prepare access */
    if((error = DESC_Init(descP, osHdl, &llHdl->descHdl)))
		return( Cleanup(llHdl,error) );

    /* DEBUG_LEVEL_DESC */
    if((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
								&value, "DEBUG_LEVEL_DESC")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

	DESC_DbgLevelSet(llHdl->descHdl, value);	/* set level */

    /* DEBUG_LEVEL */
    if((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
								&llHdl->dbgLevel, "DEBUG_LEVEL")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

    DBGWRT_1((DBH, "LL - F14BC_Init\n"));

    /* SMB_BUSNBR (required) */
    if((error = DESC_GetUInt32(llHdl->descHdl, 0,
								&smbBusNbr, "SMB_BUSNBR")))
		return( Cleanup(llHdl,error) );

    /* SMB_DEVADDR (required) */
    if((error = DESC_GetUInt32(llHdl->descHdl, 0,
								&value, "SMB_DEVADDR")))
		return( Cleanup(llHdl,error) );
	llHdl->smbAddr = (u_int16)value;

    /* POLL_PERIOD [ms] */
    if((error = DESC_GetUInt32(llHdl->descHdl, 500,
								&llHdl->pollPeriod, "POLL_PERIOD")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );
	/* check range */
	if( (llHdl->pollPeriod < 50) ||
		(llHdl->pollPeriod > 5000))
		return( Cleanup(llHdl,ERR_LL_DESC_PARAM) );

    /*------------------------------+
    |  init alarm                   |
    +------------------------------*/
    if((error = OSS_AlarmCreate(llHdl->osHdl, AlarmHandler, llHdl,
								 &llHdl->alarmHdl)) )
		return( Cleanup(llHdl,error));

    /*------------------------------+
    |  get SMB handle               |
    +------------------------------*/
	if((error = OSS_GetSmbHdl( llHdl->osHdl, smbBusNbr, (void**)&llHdl->smbH) ))
		return( Cleanup(llHdl,error) );

    /*------------------------------+
    |  init hardware                |
    +------------------------------*/
	/* get/save firmware rev. and check device access */
	tmp = 0;
	for( n=0; n<F14BCI_REV_BYTESIZE; n++ ){

		if( (error = llHdl->smbH->ReadByteData( llHdl->smbH, 0, llHdl->smbAddr,
									(u_int8)F14BCI_GETREV(n), &llHdl->fwRev[n] ) ))
			return( Cleanup(llHdl,error) );

		/* firmware rev 2.0 or later? */
		if( (llHdl->fwRev[n] >= '0') &&
			(llHdl->fwRev[n] <= '9') )
			tmp++;
	}

	DBGDMP_2((DBH, " PIC firmware revision (for rev. 2.0 and later)",
		(void*)&llHdl->fwRev[0], F14BCI_REV_BYTESIZE, 1));

	/* first firmware rev */
	if( tmp != F14BCI_REV_BYTESIZE ){
	    DBGWRT_2((DBH, " first PIC firmware revision -> restricted functionality\n"));
		llHdl->fwRevFirst = TRUE;
	}
	else{
		/* get WDOG state */
		if( (error = llHdl->smbH->ReadByteData( llHdl->smbH, 0, llHdl->smbAddr,
										F14BCI_WDSTATE, &llHdl->wdStatus ) ))
			return( Cleanup(llHdl,error) );
	}

	*llHdlP = llHdl;	/* set low-level driver handle */

	return(ERR_SUCCESS);
}

/****************************** F14BC_Exit ************************************/
/** De-initialize hardware and clean up memory
 *
 *  The function deinitializes the F14BC device.
 *
 *  \param llHdlP      \IN  Pointer to low-level driver handle
 *
 *  \return           \c 0 On success or error code
 */
static int32 F14BC_Exit(
   LL_HANDLE    **llHdlP
)
{
    LL_HANDLE *llHdl = *llHdlP;
	int32 error = 0;

    DBGWRT_1((DBH, "LL - F14BC_Exit\n"));

    /*------------------------------+
    |  de-init hardware             |
    +------------------------------*/

    /*------------------------------+
    |  clean up memory               |
    +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */
	error = Cleanup(llHdl,error);

	return(error);
}

/****************************** F14BC_Read ************************************/
/** Read a value from the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param ch         \IN  Current channel
 *  \param valueP     \OUT Read value
 *
 *  \return           \c 0 On success or error code
 */
static int32 F14BC_Read(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 *valueP
)
{
    DBGWRT_1((DBH, "LL - F14BC_Read: ch=%d\n",ch));

	return(ERR_LL_ILL_FUNC);
}

/****************************** F14BC_Write ***********************************/
/** Write a value to the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param ch         \IN  Current channel
 *  \param value      \IN  Read value
 *
 *  \return           \c ERR_LL_ILL_FUNC
 */
static int32 F14BC_Write(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 value
)
{
    DBGWRT_1((DBH, "LL - F14BC_Write: ch=%d\n",ch));

	return(ERR_LL_ILL_FUNC);
}

/****************************** F14BC_SetStat *********************************/
/** Set the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl  	  \IN  Low-level handle
 *  \param code       \IN  \ref getstat_setstat_codes "status code"
 *  \param ch         \IN  Current channel
 *  \param value32_or_64  \IN  Data or pointer to block data structure (M_SG_BLOCK)
 *                             for block status codes
 *  \return           \c 0 On success or error code
 */
static int32 F14BC_SetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 value32_or_64
)
{
	int32	error = ERR_SUCCESS;
	u_int8	byte;
	int32	value  	= (int32)value32_or_64;	/* 32bit value */

    DBGWRT_1((DBH, "LL - F14BC_SetStat: ch=%d code=0x%04x value=0x%x\n",
			  ch,code,value));

	switch(code) {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            llHdl->dbgLevel = value;
            break;
        /*--------------------------+
        |  channel direction        |
        +--------------------------*/
        case M_LL_CH_DIR:
			if( value != M_CH_IN )
				error = ERR_LL_ILL_DIR;
            break;
        /*--------------------------+
        |  start WDOG               |
        +--------------------------*/
        case WDOG_START:
			if( (error = llHdl->smbH->WriteByte( llHdl->smbH, 0,
						llHdl->smbAddr, F14BCI_WDON )) )
				break;

			/* firmware rev. 2.0 and later */
			if( !llHdl->fwRevFirst ){
				/* verify the WDOG start */
				if( (error = llHdl->smbH->ReadByteData( llHdl->smbH, 0,
							llHdl->smbAddr, F14BCI_WDSTATE, &byte)) )
					break;
				if( !byte ){
					error = ERR_LL_DEV_NOTRDY;
					break;
				}
			}

			llHdl->wdStatus = 1;
            break;
        /*--------------------------+
        |  stop WDOG                |
        +--------------------------*/
        case WDOG_STOP:
			/* firmware rev. 1.x */
			if( llHdl->fwRevFirst ){
				if( (error = llHdl->smbH->WriteByte( llHdl->smbH, 0,
							llHdl->smbAddr, F14BCI_WDOFF )) )
					break;
			}
			/* firmware rev. 2.0 and later */
			else{
				if( (error = llHdl->smbH->WriteByteData( llHdl->smbH, 0,
							llHdl->smbAddr, F14BCI_WDOFF, F14BCI_WDOFF_BYTE )) )
					break;

				/* verify the WDOG stop */
				if( (error = llHdl->smbH->ReadByteData( llHdl->smbH, 0,
							llHdl->smbAddr, F14BCI_WDSTATE, &byte)) )
					break;
				if( byte ){
					error = ERR_LL_DEV_NOTRDY;
					break;
				}
			}

			llHdl->wdStatus = 0;
            break;
        /*--------------------------+
        |  trigger WDOG             |
        +--------------------------*/
        case WDOG_TRIG:
			/* watchdog not enabled? */
			if ( llHdl->wdStatus == 0 ) {
				error = ERR_LL_DEV_NOTRDY;
				break;
			}
			error = llHdl->smbH->WriteByte( llHdl->smbH, 0,
						llHdl->smbAddr, F14BCI_WDTRIG );
            break;
        /*--------------------------+
        |  set WDOG time            |
        +--------------------------*/
        case WDOG_TIME:
				/* out of range? */
			if( ((value % F14BCI_WDTIME_MSUNIT) != 0) || /* wrong step? */
				 (value < F14BCI_WDTIME_MSUNIT)       || /* too short? */
				 (value > F14BCI_WDTIME_MSUNIT * F14BCI_WDTIME_MAX)){ /* too long? */
				error = ERR_LL_ILL_PARAM;
				break;
			}
			error = llHdl->smbH->WriteWordData( llHdl->smbH, 0,
									llHdl->smbAddr, F14BCI_WDTIME,
									(u_int16)(value/F14BCI_WDTIME_MSUNIT) );
            break;
		/*--------------------------+
        |  software reset           |
        +--------------------------*/
        case F14BC_SWRESET:
			/* right keyword? */
			if ( value != 0xdead ) {
				error = ERR_LL_ILL_PARAM;
				break;
			}
			/* firmware rev. 1.x */
			if( llHdl->fwRevFirst )
				error = llHdl->smbH->WriteByte( llHdl->smbH, 0,
							llHdl->smbAddr, F14BCI_SWRESET );
			else
				/* firmware rev. 2.0 and later */
				error = llHdl->smbH->WriteWordData( llHdl->smbH, 0,
							llHdl->smbAddr, F14BCI_SWRESET,
							F14BCI_SWRESET_WORD );
            break;
        /*--------------------------+
        |  unknown                  |
        +--------------------------*/
        default:
			error = ERR_LL_UNK_CODE;
    }

	/*
	 * additional functionality for
	 *  firmware rev. 2.0 and later
	 */
	if( (llHdl->fwRevFirst == FALSE) &&
		(error == ERR_LL_UNK_CODE) ){

		error = ERR_SUCCESS;

		switch(code)
		{
		   /*------------------------------------------+
			|  install signal for voltage supervision  |
			+-----------------------------------------*/
			case F14BC_VOLT_SIGSET:
			{
				u_int32				realMsec;

				/* illegal signal code ? */
				if( value == 0 ){
					DBGWRT_ERR((DBH, " *** F14BC_SetStat: illegal signal code=0x%x",
						value));
					error = ERR_LL_ILL_PARAM;
					break;
				}

				/* already defined ? */
				if( llHdl->sigHdl != NULL ){
					DBGWRT_ERR((DBH, " *** F14BC_SetStat: signal already installed"));
					error = ERR_OSS_SIG_SET;
					break;
				}

				/* install signal+alarm */
				if( (error = OSS_SigCreate(llHdl->osHdl, value, &llHdl->sigHdl)) )
					break;

				if( (error = OSS_AlarmSet(llHdl->osHdl, llHdl->alarmHdl,
										  llHdl->pollPeriod, 1, &realMsec)) )
					break;
				break;
			}
			/*---------------------------------------+
			|  deinstall signal for shutdown event   |
			+---------------------------------------*/
			case F14BC_VOLT_SIGCLR:
				/* not defined ? */
				if( llHdl->sigHdl == NULL ){
					DBGWRT_ERR((DBH, " *** F14BC_SetStat: signal not installed"));
					error = ERR_OSS_SIG_CLR;
					break;
				}

  				/* remove signal+alarm */
				if( (error = OSS_SigRemove(llHdl->osHdl, &llHdl->sigHdl)) )
					break;

				if( (error = OSS_AlarmClear(llHdl->osHdl, llHdl->alarmHdl)) )
					break;

				break;
			/*--------------------------+
			|  clear all error counters |
			+--------------------------*/
			case F14BC_ERRCNT_CLR:
				error = llHdl->smbH->WriteByte( llHdl->smbH, 0,
							llHdl->smbAddr, F14BCI_ERRCNT_CLR );
				break;
			/*--------------------------+
			|  unknown                  |
			+--------------------------*/
			default:
				error = ERR_LL_UNK_CODE;
		}
	}
	return(error);
}

/****************************** F14BC_GetStat *********************************/
/** Get the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param code       \IN  \ref getstat_setstat_codes "status code"
 *  \param ch         \IN  Current channel
 *  \param *value32_or_64P  \IN  Pointer to block data structure (M_SG_BLOCK) for
 *                               block status codes
 *  \param *value32_or_64P  \OUT Data pointer or pointer to block data structure
 *                               (M_SG_BLOCK) for block status codes
 *
 *  \return           \c 0 On success or error code
 */
static int32 F14BC_GetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 *value32_or_64P
)
{
	int32	error = ERR_SUCCESS;
	u_int16	word;
	u_int8	byte, c = (u_int8)ch;

	int32		*valueP	   = (int32*)value32_or_64P;		/* pointer to 32bit value  */
	INT32_OR_64	*value64P  = value32_or_64P;		 		/* stores 32/64bit pointer */

    DBGWRT_1((DBH, "LL - F14BC_GetStat: ch=%d code=0x%04x\n",
			  ch,code));

    switch(code)
    {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            *valueP = llHdl->dbgLevel;
            break;
        /*--------------------------+
        |  number of channels       |
        +--------------------------*/
        case M_LL_CH_NUMBER:
            *valueP = CH_NUMBER;
            break;
        /*--------------------------+
        |  channel direction        |
        +--------------------------*/
        case M_LL_CH_DIR:
            *valueP = M_CH_IN;
            break;
        /*--------------------------+
        |  channel length [bits]    |
        +--------------------------*/
        case M_LL_CH_LEN:
            *valueP = 32;
            break;
        /*--------------------------+
        |  channel type info        |
        +--------------------------*/
        case M_LL_CH_TYP:
            *valueP = M_CH_PROFILE_WDOG;
            break;
        /*--------------------------+
        |   ident table pointer     |
        |   (treat as non-block!)   |
        +--------------------------*/
        case M_MK_BLK_REV_ID:
           *value64P = (INT32_OR_64)&llHdl->idFuncTbl;
           break;
       /*--------------------------+
        |  unknown                  |
        +--------------------------*/
        default:
			error = ERR_LL_UNK_CODE;
    }

	/*
	 * additional functionality for
	 *  firmware rev. 2.0 and later
	 */
	if( (llHdl->fwRevFirst == FALSE) &&
		(error == ERR_LL_UNK_CODE) ){

		error = ERR_SUCCESS;

		switch(code)
		{
			/*--------------------------+
			|  get WDOG time            |
			+--------------------------*/
			case WDOG_TIME:
				error = llHdl->smbH->ReadWordData( llHdl->smbH, 0,
										llHdl->smbAddr, F14BCI_WDTIME, &word);
				*valueP = (u_int32)(word * F14BCI_WDTIME_MSUNIT);
				break;
			/*--------------------------+
			|  get WDOG state           |
			+--------------------------*/
			case WDOG_STATUS:
				error = llHdl->smbH->ReadByteData( llHdl->smbH, 0,
										llHdl->smbAddr, F14BCI_WDSTATE, &byte);
				*valueP = (u_int32)byte;
				break;
			/*--------------------------+
			|  get input voltage        |
			+--------------------------*/
			case F14BC_VOLT_IN:
				error = llHdl->smbH->ReadByteData( llHdl->smbH, 0,
										llHdl->smbAddr, F14BCI_VOLT_IN(c), &byte);
				*valueP = (u_int32)(byte * F14BCI_VOLT_MVUNIT);
				break;
			/*--------------------------+
			|  get nominal voltage      |
			+--------------------------*/
			case F14BC_VOLT_NOM:
				error = llHdl->smbH->ReadByteData( llHdl->smbH, 0,
										llHdl->smbAddr, F14BCI_VOLT_NOM(c), &byte);
				*valueP = (u_int32)(byte * F14BCI_VOLT_MVUNIT);
				break;
			/*--------------------------+
			|  get deviation voltage    |
			+--------------------------*/
			case F14BC_VOLT_DEV:
				error = llHdl->smbH->ReadByteData( llHdl->smbH, 0,
										llHdl->smbAddr, F14BCI_VOLT_DEV(c), &byte);
				*valueP = (u_int32)(byte * F14BCI_VOLT_MVUNIT);
				break;
			/*--------------------------+
			|  get supervision state    |
			+--------------------------*/
			case F14BC_VOLT_SVSTATE:
				*valueP = (int32)llHdl->voltSvState;
				break;
			/*--------------------------+
			|  get error count - WDOG   |
			+--------------------------*/
			case F14BC_ERRCNT_WDOG:
				error = llHdl->smbH->ReadByteData(
											llHdl->smbH, 0, llHdl->smbAddr,
											F14BCI_ERRCNT_NO_TRIG, &byte);
				*valueP = (u_int32)byte;
				break;
			/*--------------------------+
			|  get error count - BIOS_1 |
			+--------------------------*/
			case F14BC_ERRCNT_BIOS_1:
				error = llHdl->smbH->ReadByteData(
											llHdl->smbH, 0, llHdl->smbAddr,
											F14BCI_ERRCNT_BIOS_1, &byte);
				*valueP = (u_int32)byte;
				break;
			/*--------------------------+
			|  get error count - BIOS_2 |
			+--------------------------*/
			case F14BC_ERRCNT_BIOS_2:
				error = llHdl->smbH->ReadByteData(
											llHdl->smbH, 0, llHdl->smbAddr,
											F14BCI_ERRCNT_BIOS_2, &byte);
				*valueP = (u_int32)byte;
				break;
			/*--------------------------+
			|  get error count - SLP_S5 |
			+--------------------------*/
			case F14BC_ERRCNT_SLP_S5:
				error = llHdl->smbH->ReadByteData(
										llHdl->smbH, 0, llHdl->smbAddr,
										F14BCI_ERRCNT_NO_SLP_S5, &byte);
				*valueP = (u_int32)byte;
				break;
			/*--------------------------+
			|  get error count - RST    |
			+--------------------------*/
			case F14BC_ERRCNT_RST:
				error = llHdl->smbH->ReadByteData(
										llHdl->smbH, 0, llHdl->smbAddr,
										F14BCI_ERRCNT_RST_ACTIVE, &byte);
				*valueP = (u_int32)byte;
				break;
			/*--------------------------+
			|  get error count - TEMP   |
			+--------------------------*/
			case F14BC_ERRCNT_TEMP:
				error = llHdl->smbH->ReadByteData(
										llHdl->smbH, 0, llHdl->smbAddr,
										F14BCI_ERRCNT_TEMP, &byte);
				*valueP = (u_int32)byte;
				break;
			/*----------------------------+
			|  get error count - PROCHOT  |
			+----------------------------*/
			case F14BC_ERRCNT_PROCHOT:
				error = llHdl->smbH->ReadByteData(
										llHdl->smbH, 0, llHdl->smbAddr,
										F14BCI_ERRCNT_PROCHOT, &byte);
				*valueP = (u_int32)byte;
				break;
			/*--------------------------+
			|  get firmware revision    |
			+--------------------------*/
			case F14BC_BLK_FWREV:
			{
				M_SG_BLOCK *blk = (M_SG_BLOCK*)value32_or_64P; 	/* stores block struct pointer */

				/* check blk size */
				if( blk->size < F14BCI_REV_BYTESIZE ){
					error = ERR_LL_USERBUF;
					break;
				}

				OSS_MemCopy( llHdl->osHdl, F14BCI_REV_BYTESIZE,
					(char*)llHdl->fwRev, (char*)blk->data);
				break;
			}
			/*--------------------------+
			|  unknown                  |
			+--------------------------*/
			default:
				error = ERR_LL_UNK_CODE;
		}
	}
	return(error);
}

/******************************* F14BC_BlockRead ******************************/
/** Read a data block from the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl       \IN  Low-level handle
 *  \param ch          \IN  Current channel
 *  \param buf         \IN  Data buffer
 *  \param size        \IN  Data buffer size
 *  \param nbrRdBytesP \OUT Number of read bytes
 *
 *  \return            \c 0 On success or error code
 */
static int32 F14BC_BlockRead(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrRdBytesP
)
{
    DBGWRT_1((DBH, "LL - F14BC_BlockRead: ch=%d, size=%d\n",ch,size));

	/* return number of read bytes */
	*nbrRdBytesP = 0;

	return(ERR_LL_ILL_FUNC);
}

/****************************** F14BC_BlockWrite ******************************/
/** Write a data block from the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl  	   \IN  Low-level handle
 *  \param ch          \IN  Current channel
 *  \param buf         \IN  Data buffer
 *  \param size        \IN  Data buffer size
 *  \param nbrWrBytesP \OUT Number of written bytes
 *
 *  \return            \c ERR_LL_ILL_FUNC
 */
static int32 F14BC_BlockWrite(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrWrBytesP
)
{
    DBGWRT_1((DBH, "LL - F14BC_BlockWrite: ch=%d, size=%d\n",ch,size));

	/* return number of written bytes */
	*nbrWrBytesP = 0;

	return(ERR_LL_ILL_FUNC);
}


/****************************** F14BC_Irq ************************************/
/** Interrupt service routine - unused
 *
 *  If the driver can detect the interrupt's cause it returns
 *  LL_IRQ_DEVICE or LL_IRQ_DEV_NOT, otherwise LL_IRQ_UNKNOWN.
 *
 *  \param llHdl  	   \IN  Low-level handle
 *  \return LL_IRQ_DEVICE	IRQ caused by device
 *          LL_IRQ_DEV_NOT  IRQ not caused by device
 *          LL_IRQ_UNKNOWN  Unknown
 */
static int32 F14BC_Irq(
   LL_HANDLE *llHdl
)
{
	return(LL_IRQ_DEV_NOT);
}

/****************************** F14BC_Info ***********************************/
/** Get information about hardware and driver requirements
 *
 *  The following info codes are supported:
 *
 * \code
 *  Code                      Description
 *  ------------------------  -----------------------------
 *  LL_INFO_HW_CHARACTER      Hardware characteristics
 *  LL_INFO_ADDRSPACE_COUNT   Number of required address spaces
 *  LL_INFO_ADDRSPACE         Address space information
 *  LL_INFO_IRQ               Interrupt required
 *  LL_INFO_LOCKMODE          Process lock mode required
 * \endcode
 *
 *  The LL_INFO_HW_CHARACTER code returns all address and
 *  data modes (ORed) which are supported by the hardware
 *  (MDIS_MAxx, MDIS_MDxx).
 *
 *  The LL_INFO_ADDRSPACE_COUNT code returns the number
 *  of address spaces used by the driver.
 *
 *  The LL_INFO_ADDRSPACE code returns information about one
 *  specific address space (MDIS_MAxx, MDIS_MDxx). The returned
 *  data mode represents the widest hardware access used by
 *  the driver.
 *
 *  The LL_INFO_IRQ code returns whether the driver supports an
 *  interrupt routine (TRUE or FALSE).
 *
 *  The LL_INFO_LOCKMODE code returns which process locking
 *  mode the driver needs (LL_LOCK_xxx).
 *
 *  \param infoType	   \IN  Info code
 *  \param ...         \IN  Argument(s)
 *
 *  \return            \c 0 On success or error code
 */
static int32 F14BC_Info(
   int32  infoType,
   ...
)
{
    int32   error = ERR_SUCCESS;
    va_list argptr;

    va_start(argptr, infoType );

    switch(infoType) {
		/*-------------------------------+
        |  hardware characteristics      |
        |  (all addr/data modes ORed)   |
        +-------------------------------*/
        case LL_INFO_HW_CHARACTER:
		{
			u_int32 *addrModeP = va_arg(argptr, u_int32*);
			u_int32 *dataModeP = va_arg(argptr, u_int32*);

			*addrModeP = MDIS_MA08;
			*dataModeP = MDIS_MD08 | MDIS_MD16;
			break;
	    }
		/*-------------------------------+
        |  nr of required address spaces |
        |  (total spaces used)           |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE_COUNT:
		{
			u_int32 *nbrOfAddrSpaceP = va_arg(argptr, u_int32*);

			*nbrOfAddrSpaceP = ADDRSPACE_COUNT;
			break;
	    }
		/*-------------------------------+
        |  address space type            |
        |  (widest used data mode)       |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE:
		{
			u_int32 addrSpaceIndex = va_arg(argptr, u_int32);
			u_int32 *addrModeP = va_arg(argptr, u_int32*);
			u_int32 *dataModeP = va_arg(argptr, u_int32*);
			u_int32 *addrSizeP = va_arg(argptr, u_int32*);

			if((int32)addrSpaceIndex >= ADDRSPACE_COUNT)
				error = ERR_LL_ILL_PARAM;
			else {
				*addrModeP = MDIS_MA08;
				*dataModeP = MDIS_MD16;
				*addrSizeP = ADDRSPACE_SIZE;
			}

			break;
	    }
		/*-------------------------------+
        |   interrupt required           |
        +-------------------------------*/
        case LL_INFO_IRQ:
		{
			u_int32 *useIrqP = va_arg(argptr, u_int32*);

			*useIrqP = USE_IRQ;
			break;
	    }
		/*-------------------------------+
        |   process lock mode            |
        +-------------------------------*/
        case LL_INFO_LOCKMODE:
		{
			u_int32 *lockModeP = va_arg(argptr, u_int32*);

			*lockModeP = LL_LOCK_CALL;
			break;
	    }
		/*-------------------------------+
        |   (unknown)                    |
        +-------------------------------*/
        default:
          error = ERR_LL_ILL_PARAM;
    }

    va_end(argptr);
    return(error);
}

/*******************************  Ident  ***********************************/
/** Return ident string
 *
 *  \return            Pointer to ident string
 */
static char* Ident( void )
{
    return( "F14BC - F14BC low-level driver: $Id: f14bc_drv.c,v 1.4 2009/09/04 11:32:21 MRoth Exp $" );
}

/********************************* Cleanup *********************************/
/** Close all handles, free memory and return error code
 *
 *	\warning The low-level handle is invalid after this function is called.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param retCode    \IN  Return value
 *
 *  \return           \IN   retCode
 */
static int32 Cleanup(
   LL_HANDLE    *llHdl,
   int32        retCode
)
{
    /*------------------------------+
    |  close handles                |
    +------------------------------*/
	/* clean up desc */
	if(llHdl->descHdl)
		DESC_Exit(&llHdl->descHdl);

	/* clean up alarm */
	if (llHdl->alarmHdl)
		OSS_AlarmRemove(llHdl->osHdl, &llHdl->alarmHdl);

	/* clean up signal */
	if (llHdl->sigHdl)
		OSS_SigRemove(llHdl->osHdl, &llHdl->sigHdl);

	/* clean up debug */
	DBGEXIT((&DBH));

    /*------------------------------+
    |  free memory                  |
    +------------------------------*/
    /* free my handle */
    OSS_MemFree(llHdl->osHdl, (int8*)llHdl, llHdl->memAlloc);

    /*------------------------------+
    |  return error code            |
    +------------------------------*/
	return(retCode);
}

/******************************* AlarmHandler *******************************
 *
 *  Description: Handler for alarm
 *
 *---------------------------------------------------------------------------
 *  Input......: arg		ll handle
 *  Output.....: -
 *  Globals....: -
 ****************************************************************************/
static void AlarmHandler(void *arg)
{
	LL_HANDLE	*llHdl = (LL_HANDLE*)arg;
	int32		error;
	u_int8		state;

	DBGWRT_3((DBH,">>> LL - F14BC AlarmHandler:\n"));

	error = llHdl->smbH->ReadByteData( llHdl->smbH, 0, llHdl->smbAddr,
							F14BCI_VOLT_SVSTATE, &state);

	if( !error && state ){
		llHdl->voltSvState = state;
		DBGWRT_2((DBH, " voltSvState=0x%x --> send signal\n",
			llHdl->voltSvState));
		OSS_SigSend( llHdl->osHdl, llHdl->sigHdl );
	}
}

