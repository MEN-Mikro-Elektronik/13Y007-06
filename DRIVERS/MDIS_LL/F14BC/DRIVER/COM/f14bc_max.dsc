#************************** MDIS4 device descriptor *************************
#
# f14bc_max.dsc: Descriptor for f14bc
# 2006-10-13
#
#****************************************************************************

f14bc_1 {

    # ------------------------------------------------------------------------
    #        general parameters (don't modify)
    # ------------------------------------------------------------------------
    DESC_TYPE = U_INT32 0x1
    HW_TYPE = STRING f14bc

    # ------------------------------------------------------------------------
    #        reference to base board
    # ------------------------------------------------------------------------
    BOARD_NAME = STRING smbpci_ich
    DEVICE_SLOT = U_INT32 0x0

    # ------------------------------------------------------------------------
    #        device parameters
    # ------------------------------------------------------------------------

    # SMBus bus number
    SMB_BUSNBR = U_INT32 0

    # SMBus address of f14bc
    SMB_DEVADDR = U_INT32 0x9a

    # poll period for voltage supervision
    # Range: 50..5000
    POLL_PERIOD = U_INT32 500

    # ------------------------------------------------------------------------
    #        debug levels (optional)
    #        these keys have only effect on debug drivers
    # ------------------------------------------------------------------------
    DEBUG_LEVEL = U_INT32 0xc0008000
    DEBUG_LEVEL_MK = U_INT32 0xc0008000
    DEBUG_LEVEL_OSS = U_INT32 0xc0008000
    DEBUG_LEVEL_DESC = U_INT32 0xc0008000
}
# EOF
