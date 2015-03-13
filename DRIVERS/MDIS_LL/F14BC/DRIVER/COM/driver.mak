#***************************  M a k e f i l e  *******************************
#
#         Author: dieter.pfeuffer@men.de
#          $Date: 2006/11/08 09:45:39 $
#      $Revision: 1.1 $
#
#    Description: Makefile definitions for the F14BC driver
#
#---------------------------------[ History ]---------------------------------
#
#   $Log: driver.mak,v $
#   Revision 1.1  2006/11/08 09:45:39  DPfeuffer
#   Initial Revision
#
#-----------------------------------------------------------------------------
#   (c) Copyright 2006 by MEN mikro elektronik GmbH, Nuernberg, Germany
#*****************************************************************************

MAK_NAME=f14bc

MAK_SWITCH=$(SW_PREFIX)MAC_MEM_MAPPED

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/desc$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/oss$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/dbg$(LIB_SUFFIX)	\

MAK_INCL=$(MEN_INC_DIR)/f14bc_drv.h	\
		 $(MEN_INC_DIR)/men_typs.h	\
		 $(MEN_INC_DIR)/oss.h		\
		 $(MEN_INC_DIR)/mdis_err.h	\
		 $(MEN_INC_DIR)/maccess.h	\
		 $(MEN_INC_DIR)/desc.h		\
		 $(MEN_INC_DIR)/mdis_api.h	\
		 $(MEN_INC_DIR)/mdis_com.h	\
		 $(MEN_INC_DIR)/ll_defs.h	\
		 $(MEN_INC_DIR)/ll_entry.h	\
		 $(MEN_INC_DIR)/dbg.h		\
 		 $(MEN_INC_DIR)/smb2.h		\
 		 $(MEN_INC_DIR)/f14bci.h	\

MAK_INP1=f14bc_drv$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)
