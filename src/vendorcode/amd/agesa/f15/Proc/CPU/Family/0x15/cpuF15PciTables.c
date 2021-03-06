/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15  PCI tables with values as defined in BKDG
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15
 * @e \$Revision: 59440 $   @e \$Date: 2011-09-22 19:44:44 -0600 (Thu, 22 Sep 2011) $
 *
 */
/*
 ******************************************************************************
 *
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Advanced Micro Devices, Inc. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "Table.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CPUF15PCITABLES_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

//  P C I    T a b l e s
// ----------------------

STATIC CONST TABLE_ENTRY_FIELDS ROMDATA F15PciRegisters[] =
{
// F2x1B0 - Extended Memory Controller Configuration Low
// bits[10:8], CohPrefPrbLmt = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15,                      // CpuFamily
      AMD_F15_ALL                         // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_2, 0x1B0),  // Address
      0x00000100,                           // regData
      0x00000700,                           // regMask
    }}
  },

// Function 3 - Misc. Control

// F3x6C - Data Buffer Count
// bits[30:28] IsocRspDBC = 1
// bits[18:16] UpRspDBC = 1
// bits[7:6]   DnRspDBC = 1
// bits[5:4]   DnReqDBC = 1
// bits[2:0]   UpReqDBC = 2
  {
    PciRegister,
    {
      AMD_FAMILY_15,                      // CpuFamily
      AMD_F15_ALL                         // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x6C),  // Address
      0x10010052,                           // regData
      0x700700F7,                           // regMask
    }}
  },
// F3xA0 - Power Control Miscellaneous
// bits[13:11] PllLockTime = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15,                      // CpuFamily
      AMD_F15_ALL                         // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xA0),  // Address
      0x00000800,                           // regData
      0x00003800,                           // regMask
    }}
  },
// F3xA4 - Reported Temperature Control
// bits[12:8] PerStepTimeDn = 0x0F
// bits[7] TmpSlewDnEn = 1
// bits[6:5] TmpMaxDiffUp = 3
// bits[4:0] PerStepTimeUp = 0x0F
  {
    PciRegister,
    {
      AMD_FAMILY_15,                      // CpuFamily
      AMD_F15_ALL                         // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xA4),  // Address
      0x00000FEF,                           // regData
      0x00001FFF,                           // regMask
    }}
  },
// F3xDC - Clock Power Timing Control 2
// bit [26]    IgnCpuPrbEn = 1
// bits[14:12] NbsynPtrAdj = 5
  {
    PciRegister,
    {
      AMD_FAMILY_15,                      // CpuFamily
      AMD_F15_ALL                         // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xDC),  // Address
      0x04005000,                           // regData
      0x04007000,                           // regMask
    }}
  },
// F3x1CC - IBS Control
// bits[8] LvtOffsetVal = 1
// bits[3:0] LvtOffset = 0
  {
    PciRegister,
    {
      AMD_FAMILY_15,                      // CpuFamily
      AMD_F15_ALL                         // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x1CC), // Address
      0x00000100,                           // regData
      0x0000010F,                           // regMask
    }}
  },
// F4x15C - Core Performance Boost Control
// bits[1:0] BoostSrc = 0
  {
    PciRegister,
    {
      AMD_FAMILY_15,                      // CpuFamily
      AMD_F15_ALL                         // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_4, 0x15C), // Address
      0x00000000,                           // regData
      0x00000003,                           // regMask
    }}
  },
};

CONST REGISTER_TABLE ROMDATA F15PciRegisterTable = {
  PrimaryCores,
  (sizeof (F15PciRegisters) / sizeof (TABLE_ENTRY_FIELDS)),
  F15PciRegisters,
};
