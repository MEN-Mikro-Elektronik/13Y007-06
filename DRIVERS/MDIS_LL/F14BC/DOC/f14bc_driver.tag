<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>
<tagfile>
  <compound kind="page">
    <filename>index</filename>
    <title></title>
    <name>index</name>
    <docanchor>codes</docanchor>
    <docanchor>f14bc_min</docanchor>
    <docanchor>Variants</docanchor>
    <docanchor>wdog_simp</docanchor>
    <docanchor>wdog_test</docanchor>
    <docanchor>f14bc_ctrl</docanchor>
    <docanchor>General</docanchor>
    <docanchor>programs</docanchor>
    <docanchor>api_functions</docanchor>
    <docanchor>FuncDesc</docanchor>
    <docanchor>f14bc_max</docanchor>
    <docanchor>channels</docanchor>
    <docanchor>descriptor_entries</docanchor>
  </compound>
  <compound kind="file">
    <name>f14bc_ctrl.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/F14BC/TOOLS/F14BC_CTRL/COM/</path>
    <filename>f14bc__ctrl_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>NONE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IO_MAX_NBR</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_VOLT_CH</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>usage</name>
      <anchor>a8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintMdisError</name>
      <anchor>a9</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintUosError</name>
      <anchor>a10</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a11</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a3</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>MDIS_PATH</type>
      <name>G_Path</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>@0</type>
      <name>G_VoltCh</name>
      <anchor>a7</anchor>
      <arglist>[MAX_VOLT_CH]</arglist>
    </member>
    <member kind="variable">
      <type>int32</type>
      <name>nom</name>
      <anchor>m0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32</type>
      <name>dev</name>
      <anchor>m1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>f14bc_doc.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/F14BC/DRIVER/COM/</path>
    <filename>f14bc__doc_8c</filename>
  </compound>
  <compound kind="file">
    <name>f14bc_drv.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/F14BC/DRIVER/COM/</path>
    <filename>f14bc__drv_8c</filename>
    <class kind="struct">LL_HANDLE</class>
    <member kind="define">
      <type>#define</type>
      <name>_NO_LL_HANDLE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CH_NUMBER</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CH_BYTES</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_IRQ</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_COUNT</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_SIZE</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBG_MYLEVEL</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBH</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMB2_ERRMAP</name>
      <anchor>a8</anchor>
      <arglist>(err)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_Init</name>
      <anchor>a10</anchor>
      <arglist>(DESC_SPEC *descSpec, OSS_HANDLE *osHdl, MACCESS *ma, OSS_SEM_HANDLE *devSemHdl, OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_Exit</name>
      <anchor>a11</anchor>
      <arglist>(LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_Read</name>
      <anchor>a12</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 *value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_Write</name>
      <anchor>a13</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_SetStat</name>
      <anchor>a14</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 value32_or_64)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_GetStat</name>
      <anchor>a15</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 *value32_or_64P)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_BlockRead</name>
      <anchor>a16</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrRdBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_BlockWrite</name>
      <anchor>a17</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrWrBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_Irq</name>
      <anchor>a18</anchor>
      <arglist>(LL_HANDLE *llHdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>F14BC_Info</name>
      <anchor>a19</anchor>
      <arglist>(int32 infoType,...)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>char *</type>
      <name>Ident</name>
      <anchor>a20</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Cleanup</name>
      <anchor>a21</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 retCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>AlarmHandler</name>
      <anchor>a22</anchor>
      <arglist>(void *arg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>__F14BC_GetEntry</name>
      <anchor>a23</anchor>
      <arglist>(LL_ENTRY *drvP)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a9</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>f14bc_drv.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>f14bc__drv_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_SWRESET</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_VOLT_IN</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_VOLT_NOM</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_VOLT_DEV</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_VOLT_SIGSET</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_VOLT_SIGCLR</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_VOLT_SVSTATE</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_ERRCNT_WDOG</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_ERRCNT_BIOS_1</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_ERRCNT_BIOS_2</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_ERRCNT_SLP_S5</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_ERRCNT_RST</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_ERRCNT_TEMP</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_ERRCNT_PROCHOT</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_ERRCNT_CLR</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_BLK_FWREV</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_VARIANT</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_F14BC_GLOBNAME</name>
      <anchor>a17</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F14BC_GLOBNAME</name>
      <anchor>a18</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__F14BC_GetEntry</name>
      <anchor>a19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_OR_64</name>
      <anchor>a20</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>U_INT32_OR_64</name>
      <anchor>a21</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>INT32_OR_64</type>
      <name>MDIS_PATH</name>
      <anchor>a22</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wdog.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>wdog_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_START</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TRIG</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_STOP</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TIME</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_STATUS</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_SHOT</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_RESET_CTRL</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TRIG_PAT</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TIME_MIN</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TIME_MAX</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TIME_IRQ</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_OUT_PIN</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_OUT_REASON</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_IRQ_PIN</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_IRQ_SIGSET</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_IRQ_SIGCLR</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_IRQ_REASON</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_ERR_PIN</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDOG_TRIGPAT</name>
      <anchor>a19</anchor>
      <arglist>(n)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_CH_PROFILE_WDOG</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wdog_simp.c</name>
    <path>/opt/menlinux/TOOLS/WDOG/WDOG_SIMP/COM/</path>
    <filename>wdog__simp_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>WATCHDOG_TIME</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintError</name>
      <anchor>a1</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a2</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wdog_test.c</name>
    <path>/opt/menlinux/TOOLS/WDOG/WDOG_TEST/COM/</path>
    <filename>wdog__test_8c</filename>
    <member kind="function" static="yes">
      <type>void</type>
      <name>usage</name>
      <anchor>a1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintMdisError</name>
      <anchor>a2</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a3</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a0</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LL_HANDLE</name>
    <filename>structLL__HANDLE.html</filename>
    <member kind="variable">
      <type>int32</type>
      <name>memAlloc</name>
      <anchor>m0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_HANDLE *</type>
      <name>osHdl</name>
      <anchor>m1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_IRQ_HANDLE *</type>
      <name>irqHdl</name>
      <anchor>m2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DESC_HANDLE *</type>
      <name>descHdl</name>
      <anchor>m3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MDIS_IDENT_FUNCT_TBL</type>
      <name>idFuncTbl</name>
      <anchor>m4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>dbgLevel</name>
      <anchor>m5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBG_HANDLE *</type>
      <name>dbgHdl</name>
      <anchor>m6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SMB_HANDLE *</type>
      <name>smbH</name>
      <anchor>m7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int16</type>
      <name>smbAddr</name>
      <anchor>m8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int8</type>
      <name>fwRev</name>
      <anchor>m9</anchor>
      <arglist>[F14BCI_REV_BYTESIZE]</arglist>
    </member>
    <member kind="variable">
      <type>u_int8</type>
      <name>fwRevFirst</name>
      <anchor>m10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int8</type>
      <name>wdStatus</name>
      <anchor>m11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_SIG_HANDLE *</type>
      <name>sigHdl</name>
      <anchor>m12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_ALARM_HANDLE *</type>
      <name>alarmHdl</name>
      <anchor>m13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>pollPeriod</name>
      <anchor>m14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int8</type>
      <name>voltSvState</name>
      <anchor>m15</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>f14bcdummy</name>
    <title>MEN logo</title>
    <filename>f14bcdummy</filename>
  </compound>
</tagfile>
