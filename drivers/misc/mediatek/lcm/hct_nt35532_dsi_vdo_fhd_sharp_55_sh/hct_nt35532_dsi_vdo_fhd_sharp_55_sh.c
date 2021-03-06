
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
//	#include <mach/mt_gpio.h>
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(1080)
#define FRAME_HEIGHT 										(1920)
#define LCM_ID_NT35532 										(0x32)
#define REGFLAG_DELAY             							0xAB
#define REGFLAG_END_OF_TABLE      							0xAA   // END OF REGISTERS MARKER

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test
#define   LCM_DSI_CMD_MODE							0

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)										lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    

#define changcmd(a,b,c,d) ((d<<24)|(c<<16)|(b<<8)|a)

#define   LCM_DSI_CMD_MODE									0

static void NT35596_DCS_write_1A_1P(unsigned char cmd, unsigned char para)
{
	unsigned int data_array[16];

	data_array[0] = (0x00023902);
	data_array[1] = (0x00000000 | (para << 8) | (cmd));
	dsi_set_cmdq(data_array, 2, 1);

}
#if 0
static void NT35596_DCS_write_1A_0P(unsigned char cmd)
{
	unsigned int data_array[16];

	data_array[0]=(0x00000500 | (cmd<<16));
	dsi_set_cmdq(data_array, 1, 1);

}
#endif

static void init_lcm_registers(void)
{
//NT35532_SHARP5.5_2015.08.13


NT35596_DCS_write_1A_1P(0xFF,0x00); 
NT35596_DCS_write_1A_1P(0xBA,0x03); 
NT35596_DCS_write_1A_1P(0x36,0x00); 
NT35596_DCS_write_1A_1P(0xB0,0x00); 
NT35596_DCS_write_1A_1P(0xD3,0x15); //08
NT35596_DCS_write_1A_1P(0xD4,0x10); //0e
NT35596_DCS_write_1A_1P(0xD5,0x0F); 
NT35596_DCS_write_1A_1P(0xD6,0x48); 
NT35596_DCS_write_1A_1P(0xD7,0x00); 
NT35596_DCS_write_1A_1P(0xD9,0x00); 
NT35596_DCS_write_1A_1P(0xFB,0x01); 
NT35596_DCS_write_1A_1P(0xFF,0xEE); 
NT35596_DCS_write_1A_1P(0x40,0x00); 
NT35596_DCS_write_1A_1P(0x41,0x00); 
NT35596_DCS_write_1A_1P(0x42,0x00); 
NT35596_DCS_write_1A_1P(0xFB,0x01); 
NT35596_DCS_write_1A_1P(0xFF,0x01); 
NT35596_DCS_write_1A_1P(0xFB,0x01); 
NT35596_DCS_write_1A_1P(0x01,0x55); 
NT35596_DCS_write_1A_1P(0x04,0x0C); 
NT35596_DCS_write_1A_1P(0x05,0x3A); 
NT35596_DCS_write_1A_1P(0x06,0x50);
NT35596_DCS_write_1A_1P(0x07,0xD0); 
NT35596_DCS_write_1A_1P(0x0A,0x0F); 
NT35596_DCS_write_1A_1P(0x0D,0x7F); 
NT35596_DCS_write_1A_1P(0x0E,0x7F); 
NT35596_DCS_write_1A_1P(0x0F,0x70); 
NT35596_DCS_write_1A_1P(0x10,0x63); 
NT35596_DCS_write_1A_1P(0x11,0x3C); 
NT35596_DCS_write_1A_1P(0x12,0x5C);   
NT35596_DCS_write_1A_1P(0x13,0x55); 
NT35596_DCS_write_1A_1P(0x14,0x55); 
NT35596_DCS_write_1A_1P(0x15,0x60); 
NT35596_DCS_write_1A_1P(0x16,0x11); 
NT35596_DCS_write_1A_1P(0x17,0x11); 
NT35596_DCS_write_1A_1P(0x5B,0xCA); 
NT35596_DCS_write_1A_1P(0x5C,0x00); 
NT35596_DCS_write_1A_1P(0x5D,0x00); 
NT35596_DCS_write_1A_1P(0x5F,0x1B); 
NT35596_DCS_write_1A_1P(0x60,0xD5); 
NT35596_DCS_write_1A_1P(0x61,0xF7); 
NT35596_DCS_write_1A_1P(0x6C,0xAB); 
NT35596_DCS_write_1A_1P(0x6D,0x44); 
NT35596_DCS_write_1A_1P(0x6E,0x00); 

//3Gamma2.5                   
NT35596_DCS_write_1A_1P(0xFF,0x01); 
NT35596_DCS_write_1A_1P(0xFB,0x01); 
NT35596_DCS_write_1A_1P(0x75,0x00); 
NT35596_DCS_write_1A_1P(0x76,0x08); 
NT35596_DCS_write_1A_1P(0x77,0x00); 
NT35596_DCS_write_1A_1P(0x78,0x49); 
NT35596_DCS_write_1A_1P(0x79,0x00); 
NT35596_DCS_write_1A_1P(0x7A,0x7A); 
NT35596_DCS_write_1A_1P(0x7B,0x00); 
NT35596_DCS_write_1A_1P(0x7C,0x98); 
NT35596_DCS_write_1A_1P(0x7D,0x00); 
NT35596_DCS_write_1A_1P(0x7E,0xAF); 
NT35596_DCS_write_1A_1P(0x7F,0x00); 
NT35596_DCS_write_1A_1P(0x80,0xC3); 
NT35596_DCS_write_1A_1P(0x81,0x00); 
NT35596_DCS_write_1A_1P(0x82,0xD7); 
NT35596_DCS_write_1A_1P(0x83,0x00); 
NT35596_DCS_write_1A_1P(0x84,0xE4); 
NT35596_DCS_write_1A_1P(0x85,0x00); 
NT35596_DCS_write_1A_1P(0x86,0xF3); 
NT35596_DCS_write_1A_1P(0x87,0x01); 
NT35596_DCS_write_1A_1P(0x88,0x23); 
NT35596_DCS_write_1A_1P(0x89,0x01); 
NT35596_DCS_write_1A_1P(0x8A,0x48); 
NT35596_DCS_write_1A_1P(0x8B,0x01); 
NT35596_DCS_write_1A_1P(0x8C,0x85); 
NT35596_DCS_write_1A_1P(0x8D,0x01); 
NT35596_DCS_write_1A_1P(0x8E,0xB4); 
NT35596_DCS_write_1A_1P(0x8F,0x01); 
NT35596_DCS_write_1A_1P(0x90,0xFD); 
NT35596_DCS_write_1A_1P(0x91,0x02); 
NT35596_DCS_write_1A_1P(0x92,0x3A); 
NT35596_DCS_write_1A_1P(0x93,0x02); 
NT35596_DCS_write_1A_1P(0x94,0x3B); 
NT35596_DCS_write_1A_1P(0x95,0x02); 
NT35596_DCS_write_1A_1P(0x96,0x6F); 
NT35596_DCS_write_1A_1P(0x97,0x02); 
NT35596_DCS_write_1A_1P(0x98,0xAE); 
NT35596_DCS_write_1A_1P(0x99,0x02); 
NT35596_DCS_write_1A_1P(0x9A,0xD3); 
NT35596_DCS_write_1A_1P(0x9B,0x03); 
NT35596_DCS_write_1A_1P(0x9C,0x08); 
NT35596_DCS_write_1A_1P(0x9D,0x03); 
NT35596_DCS_write_1A_1P(0x9E,0x29); 
NT35596_DCS_write_1A_1P(0x9F,0x03); 
NT35596_DCS_write_1A_1P(0xA0,0x5C); 
NT35596_DCS_write_1A_1P(0xA2,0x03); 
NT35596_DCS_write_1A_1P(0xA3,0x67); 
NT35596_DCS_write_1A_1P(0xA4,0x03); 
NT35596_DCS_write_1A_1P(0xA5,0x6F); 
NT35596_DCS_write_1A_1P(0xA6,0x03); 
NT35596_DCS_write_1A_1P(0xA7,0x89); 
NT35596_DCS_write_1A_1P(0xA9,0x03); 
NT35596_DCS_write_1A_1P(0xAA,0xA3); 
NT35596_DCS_write_1A_1P(0xAB,0x03); 
NT35596_DCS_write_1A_1P(0xAC,0xBD); 
//NT35596_DCS_write_1A_1P(0xAD,0x03); 
//NT35596_DCS_write_1A_1P(0xAE,0xC0); 
NT35596_DCS_write_1A_1P(0xAF,0x03); 
NT35596_DCS_write_1A_1P(0xB0,0xE5); 
NT35596_DCS_write_1A_1P(0xB1,0x03); 
NT35596_DCS_write_1A_1P(0xB2,0xF5); 
NT35596_DCS_write_1A_1P(0xB3,0x00); 
NT35596_DCS_write_1A_1P(0xB4,0x08); 
NT35596_DCS_write_1A_1P(0xB5,0x00); 
NT35596_DCS_write_1A_1P(0xB6,0x49); 
NT35596_DCS_write_1A_1P(0xB7,0x00); 
NT35596_DCS_write_1A_1P(0xB8,0x7A); 
NT35596_DCS_write_1A_1P(0xB9,0x00); 
NT35596_DCS_write_1A_1P(0xBA,0x98); 
NT35596_DCS_write_1A_1P(0xBB,0x00); 
NT35596_DCS_write_1A_1P(0xBC,0xAF); 
NT35596_DCS_write_1A_1P(0xBD,0x00); 
NT35596_DCS_write_1A_1P(0xBE,0xC3); 
NT35596_DCS_write_1A_1P(0xBF,0x00); 
NT35596_DCS_write_1A_1P(0xC0,0xD7); 
NT35596_DCS_write_1A_1P(0xC1,0x00); 
NT35596_DCS_write_1A_1P(0xC2,0xE4); 
NT35596_DCS_write_1A_1P(0xC3,0x00); 
NT35596_DCS_write_1A_1P(0xC4,0xF3); 
NT35596_DCS_write_1A_1P(0xC5,0x01); 
NT35596_DCS_write_1A_1P(0xC6,0x23); 
NT35596_DCS_write_1A_1P(0xC7,0x01); 
NT35596_DCS_write_1A_1P(0xC8,0x48); 
NT35596_DCS_write_1A_1P(0xC9,0x01); 
NT35596_DCS_write_1A_1P(0xCA,0x85); 
NT35596_DCS_write_1A_1P(0xCB,0x01); 
NT35596_DCS_write_1A_1P(0xCC,0xB4); 
NT35596_DCS_write_1A_1P(0xCD,0x01); 
NT35596_DCS_write_1A_1P(0xCE,0xFD); 
NT35596_DCS_write_1A_1P(0xCF,0x02); 
NT35596_DCS_write_1A_1P(0xD0,0x3A); 
NT35596_DCS_write_1A_1P(0xD1,0x02); 
NT35596_DCS_write_1A_1P(0xD2,0x3B); 
NT35596_DCS_write_1A_1P(0xD3,0x02); 
NT35596_DCS_write_1A_1P(0xD4,0x6F); 
NT35596_DCS_write_1A_1P(0xD5,0x02); 
NT35596_DCS_write_1A_1P(0xD6,0xAE); 
NT35596_DCS_write_1A_1P(0xD7,0x02); 
NT35596_DCS_write_1A_1P(0xD8,0xD3); 
NT35596_DCS_write_1A_1P(0xD9,0x03); 
NT35596_DCS_write_1A_1P(0xDA,0x08); 
NT35596_DCS_write_1A_1P(0xDB,0x03); 
NT35596_DCS_write_1A_1P(0xDC,0x29); 
NT35596_DCS_write_1A_1P(0xDD,0x03); 
NT35596_DCS_write_1A_1P(0xDE,0x5C); 
NT35596_DCS_write_1A_1P(0xDF,0x03); 
NT35596_DCS_write_1A_1P(0xE0,0x67); 
NT35596_DCS_write_1A_1P(0xE1,0x03); 
NT35596_DCS_write_1A_1P(0xE2,0x6F); 
NT35596_DCS_write_1A_1P(0xE3,0x03); 
NT35596_DCS_write_1A_1P(0xE4,0x89); 
NT35596_DCS_write_1A_1P(0xE5,0x03); 
NT35596_DCS_write_1A_1P(0xE6,0xA3); 
NT35596_DCS_write_1A_1P(0xE7,0x03); 
NT35596_DCS_write_1A_1P(0xE8,0xBD); 
//NT35596_DCS_write_1A_1P(0xE9,0x03); 
//NT35596_DCS_write_1A_1P(0xEA,0xC0); 
NT35596_DCS_write_1A_1P(0xEB,0x03); 
NT35596_DCS_write_1A_1P(0xEC,0xE5); 
NT35596_DCS_write_1A_1P(0xED,0x03); 
NT35596_DCS_write_1A_1P(0xEE,0xF5); 
NT35596_DCS_write_1A_1P(0xEF,0x00); 
NT35596_DCS_write_1A_1P(0xF0,0x08); 
NT35596_DCS_write_1A_1P(0xF1,0x00); 
NT35596_DCS_write_1A_1P(0xF2,0x44); 
NT35596_DCS_write_1A_1P(0xF3,0x00); 
NT35596_DCS_write_1A_1P(0xF4,0x76); 
NT35596_DCS_write_1A_1P(0xF5,0x00); 
NT35596_DCS_write_1A_1P(0xF6,0x96); 
NT35596_DCS_write_1A_1P(0xF7,0x00); 
NT35596_DCS_write_1A_1P(0xF8,0xAE); 
NT35596_DCS_write_1A_1P(0xF9,0x00); 
NT35596_DCS_write_1A_1P(0xFA,0xC3); 
NT35596_DCS_write_1A_1P(0xFF,0x02); 
NT35596_DCS_write_1A_1P(0xFB,0x01); 
NT35596_DCS_write_1A_1P(0x00,0x00); 
NT35596_DCS_write_1A_1P(0x01,0xD4); 
NT35596_DCS_write_1A_1P(0x02,0x00); 
NT35596_DCS_write_1A_1P(0x03,0xE4); 
NT35596_DCS_write_1A_1P(0x04,0x00); 
NT35596_DCS_write_1A_1P(0x05,0xF3); 
NT35596_DCS_write_1A_1P(0x06,0x01); 
NT35596_DCS_write_1A_1P(0x07,0x23); 
NT35596_DCS_write_1A_1P(0x08,0x01); 
NT35596_DCS_write_1A_1P(0x09,0x48); 
NT35596_DCS_write_1A_1P(0x0A,0x01); 
NT35596_DCS_write_1A_1P(0x0B,0x84); 
NT35596_DCS_write_1A_1P(0x0C,0x01); 
NT35596_DCS_write_1A_1P(0x0D,0xB4); 
NT35596_DCS_write_1A_1P(0x0E,0x01); 
NT35596_DCS_write_1A_1P(0x0F,0xFE); 
NT35596_DCS_write_1A_1P(0x10,0x02); 
NT35596_DCS_write_1A_1P(0x11,0x38); 
NT35596_DCS_write_1A_1P(0x12,0x02); 
NT35596_DCS_write_1A_1P(0x13,0x3A); 
NT35596_DCS_write_1A_1P(0x14,0x02); 
NT35596_DCS_write_1A_1P(0x15,0x70); 
NT35596_DCS_write_1A_1P(0x16,0x02); 
NT35596_DCS_write_1A_1P(0x17,0xAB); 
NT35596_DCS_write_1A_1P(0x18,0x02); 
NT35596_DCS_write_1A_1P(0x19,0xD3); 
NT35596_DCS_write_1A_1P(0x1A,0x03); 
NT35596_DCS_write_1A_1P(0x1B,0x08); 
NT35596_DCS_write_1A_1P(0x1C,0x03); 
NT35596_DCS_write_1A_1P(0x1D,0x2C); 
NT35596_DCS_write_1A_1P(0x1E,0x03); 
NT35596_DCS_write_1A_1P(0x1F,0x59); 
NT35596_DCS_write_1A_1P(0x20,0x03); 
NT35596_DCS_write_1A_1P(0x21,0x68); 
NT35596_DCS_write_1A_1P(0x22,0x03); 
NT35596_DCS_write_1A_1P(0x23,0x71); 
NT35596_DCS_write_1A_1P(0x24,0x03); 
NT35596_DCS_write_1A_1P(0x25,0x8A); 
NT35596_DCS_write_1A_1P(0x26,0x03); 
NT35596_DCS_write_1A_1P(0x27,0xA4); 
NT35596_DCS_write_1A_1P(0x28,0x03); 
NT35596_DCS_write_1A_1P(0x29,0xBE); 
//NT35596_DCS_write_1A_1P(0x2A,0x03); 
//NT35596_DCS_write_1A_1P(0x2B,0xE4); 
NT35596_DCS_write_1A_1P(0x2D,0x03); 
NT35596_DCS_write_1A_1P(0x2F,0xF2); 
NT35596_DCS_write_1A_1P(0x30,0x03); 
NT35596_DCS_write_1A_1P(0x31,0xFB); 
NT35596_DCS_write_1A_1P(0x32,0x00); 
NT35596_DCS_write_1A_1P(0x33,0x08); 
NT35596_DCS_write_1A_1P(0x34,0x00); 
NT35596_DCS_write_1A_1P(0x35,0x44); 
NT35596_DCS_write_1A_1P(0x36,0x00); 
NT35596_DCS_write_1A_1P(0x37,0x76); 
NT35596_DCS_write_1A_1P(0x38,0x00); 
NT35596_DCS_write_1A_1P(0x39,0x96); 
NT35596_DCS_write_1A_1P(0x3A,0x00); 
NT35596_DCS_write_1A_1P(0x3B,0xAE); 
NT35596_DCS_write_1A_1P(0x3D,0x00); 
NT35596_DCS_write_1A_1P(0x3F,0xC3); 
NT35596_DCS_write_1A_1P(0x40,0x00); 
NT35596_DCS_write_1A_1P(0x41,0xD4); 
NT35596_DCS_write_1A_1P(0x42,0x00); 
NT35596_DCS_write_1A_1P(0x43,0xE4); 
NT35596_DCS_write_1A_1P(0x44,0x00); 
NT35596_DCS_write_1A_1P(0x45,0xF3); 
NT35596_DCS_write_1A_1P(0x46,0x01); 
NT35596_DCS_write_1A_1P(0x47,0x23); 
NT35596_DCS_write_1A_1P(0x48,0x01); 
NT35596_DCS_write_1A_1P(0x49,0x48); 
NT35596_DCS_write_1A_1P(0x4A,0x01); 
NT35596_DCS_write_1A_1P(0x4B,0x84); 
NT35596_DCS_write_1A_1P(0x4C,0x01); 
NT35596_DCS_write_1A_1P(0x4D,0xB4); 
NT35596_DCS_write_1A_1P(0x4E,0x01); 
NT35596_DCS_write_1A_1P(0x4F,0xFE); 
NT35596_DCS_write_1A_1P(0x50,0x02); 
NT35596_DCS_write_1A_1P(0x51,0x38); 
NT35596_DCS_write_1A_1P(0x52,0x02); 
NT35596_DCS_write_1A_1P(0x53,0x3A); 
NT35596_DCS_write_1A_1P(0x54,0x02); 
NT35596_DCS_write_1A_1P(0x55,0x70); 
NT35596_DCS_write_1A_1P(0x56,0x02); 
NT35596_DCS_write_1A_1P(0x58,0xAB); 
NT35596_DCS_write_1A_1P(0x59,0x02); 
NT35596_DCS_write_1A_1P(0x5A,0xD3); 
NT35596_DCS_write_1A_1P(0x5B,0x03); 
NT35596_DCS_write_1A_1P(0x5C,0x08); 
NT35596_DCS_write_1A_1P(0x5D,0x03); 
NT35596_DCS_write_1A_1P(0x5E,0x2C); 
NT35596_DCS_write_1A_1P(0x5F,0x03); 
NT35596_DCS_write_1A_1P(0x60,0x59); 
NT35596_DCS_write_1A_1P(0x61,0x03); 
NT35596_DCS_write_1A_1P(0x62,0x68); 
NT35596_DCS_write_1A_1P(0x63,0x03); 
NT35596_DCS_write_1A_1P(0x64,0x71); 
NT35596_DCS_write_1A_1P(0x65,0x03); 
NT35596_DCS_write_1A_1P(0x66,0x8A); 
NT35596_DCS_write_1A_1P(0x67,0x03); 
NT35596_DCS_write_1A_1P(0x68,0xA4); 
NT35596_DCS_write_1A_1P(0x69,0x03); 
NT35596_DCS_write_1A_1P(0x6A,0xBE); 
//NT35596_DCS_write_1A_1P(0x6B,0x03); 
//NT35596_DCS_write_1A_1P(0x6C,0xE4); 
NT35596_DCS_write_1A_1P(0x6D,0x03); 
NT35596_DCS_write_1A_1P(0x6E,0xF2); 
NT35596_DCS_write_1A_1P(0x6F,0x03); 
NT35596_DCS_write_1A_1P(0x70,0xFB); 
NT35596_DCS_write_1A_1P(0x71,0x00); 
NT35596_DCS_write_1A_1P(0x72,0x02); 
NT35596_DCS_write_1A_1P(0x73,0x00); 
NT35596_DCS_write_1A_1P(0x74,0x34); 
NT35596_DCS_write_1A_1P(0x75,0x00); 
NT35596_DCS_write_1A_1P(0x76,0x64); 
NT35596_DCS_write_1A_1P(0x77,0x00); 
NT35596_DCS_write_1A_1P(0x78,0x85); 
NT35596_DCS_write_1A_1P(0x79,0x00); 
NT35596_DCS_write_1A_1P(0x7A,0x9D); 
NT35596_DCS_write_1A_1P(0x7B,0x00); 
NT35596_DCS_write_1A_1P(0x7C,0xB2); 
NT35596_DCS_write_1A_1P(0x7D,0x00); 
NT35596_DCS_write_1A_1P(0x7E,0xC4); 
NT35596_DCS_write_1A_1P(0x7F,0x00); 
NT35596_DCS_write_1A_1P(0x80,0xD4); 
NT35596_DCS_write_1A_1P(0x81,0x00); 
NT35596_DCS_write_1A_1P(0x82,0xE3); 
NT35596_DCS_write_1A_1P(0x83,0x01); 
NT35596_DCS_write_1A_1P(0x84,0x15); 
NT35596_DCS_write_1A_1P(0x85,0x01); 
NT35596_DCS_write_1A_1P(0x86,0x3C); 
NT35596_DCS_write_1A_1P(0x87,0x01); 
NT35596_DCS_write_1A_1P(0x88,0x7A); 
NT35596_DCS_write_1A_1P(0x89,0x01); 
NT35596_DCS_write_1A_1P(0x8A,0xAB); 
NT35596_DCS_write_1A_1P(0x8B,0x01); 
NT35596_DCS_write_1A_1P(0x8C,0xF8); 
NT35596_DCS_write_1A_1P(0x8D,0x02); 
NT35596_DCS_write_1A_1P(0x8E,0x35); 
NT35596_DCS_write_1A_1P(0x8F,0x02); 
NT35596_DCS_write_1A_1P(0x90,0x37); 
NT35596_DCS_write_1A_1P(0x91,0x02); 
NT35596_DCS_write_1A_1P(0x92,0x6C); 
NT35596_DCS_write_1A_1P(0x93,0x02); 
NT35596_DCS_write_1A_1P(0x94,0xAA); 
NT35596_DCS_write_1A_1P(0x95,0x02); 
NT35596_DCS_write_1A_1P(0x96,0xD2); 
NT35596_DCS_write_1A_1P(0x97,0x03); 
NT35596_DCS_write_1A_1P(0x98,0x0B); 
NT35596_DCS_write_1A_1P(0x99,0x03); 
NT35596_DCS_write_1A_1P(0x9A,0x36); 
NT35596_DCS_write_1A_1P(0x9B,0x03); 
NT35596_DCS_write_1A_1P(0x9C,0x7D); 
NT35596_DCS_write_1A_1P(0x9D,0x03); 
NT35596_DCS_write_1A_1P(0x9E,0xA7); 
NT35596_DCS_write_1A_1P(0x9F,0x03); 
NT35596_DCS_write_1A_1P(0xA0,0xFB); 
NT35596_DCS_write_1A_1P(0xA2,0x03); 
NT35596_DCS_write_1A_1P(0xA3,0xFB); 
NT35596_DCS_write_1A_1P(0xA4,0x03); 
NT35596_DCS_write_1A_1P(0xA5,0xFB); 
NT35596_DCS_write_1A_1P(0xA6,0x03); 
NT35596_DCS_write_1A_1P(0xA7,0xFC); 
//NT35596_DCS_write_1A_1P(0xA9,0x03); 
//NT35596_DCS_write_1A_1P(0xAA,0xFE); 
NT35596_DCS_write_1A_1P(0xAB,0x03); 
NT35596_DCS_write_1A_1P(0xAC,0xFE); 
NT35596_DCS_write_1A_1P(0xAD,0x03); 
NT35596_DCS_write_1A_1P(0xAE,0xFE); 
NT35596_DCS_write_1A_1P(0xAF,0x00); 
NT35596_DCS_write_1A_1P(0xB0,0x02); 
NT35596_DCS_write_1A_1P(0xB1,0x00); 
NT35596_DCS_write_1A_1P(0xB2,0x34); 
NT35596_DCS_write_1A_1P(0xB3,0x00); 
NT35596_DCS_write_1A_1P(0xB4,0x64); 
NT35596_DCS_write_1A_1P(0xB5,0x00); 
NT35596_DCS_write_1A_1P(0xB6,0x85); 
NT35596_DCS_write_1A_1P(0xB7,0x00); 
NT35596_DCS_write_1A_1P(0xB8,0x9D); 
NT35596_DCS_write_1A_1P(0xB9,0x00); 
NT35596_DCS_write_1A_1P(0xBA,0xB2); 
NT35596_DCS_write_1A_1P(0xBB,0x00); 
NT35596_DCS_write_1A_1P(0xBC,0xC4); 
NT35596_DCS_write_1A_1P(0xBD,0x00); 
NT35596_DCS_write_1A_1P(0xBE,0xD4); 
NT35596_DCS_write_1A_1P(0xBF,0x00); 
NT35596_DCS_write_1A_1P(0xC0,0xE3); 
NT35596_DCS_write_1A_1P(0xC1,0x01); 
NT35596_DCS_write_1A_1P(0xC2,0x15); 
NT35596_DCS_write_1A_1P(0xC3,0x01); 
NT35596_DCS_write_1A_1P(0xC4,0x3C); 
NT35596_DCS_write_1A_1P(0xC5,0x01); 
NT35596_DCS_write_1A_1P(0xC6,0x7A); 
NT35596_DCS_write_1A_1P(0xC7,0x01); 
NT35596_DCS_write_1A_1P(0xC8,0xAB); 
NT35596_DCS_write_1A_1P(0xC9,0x01); 
NT35596_DCS_write_1A_1P(0xCA,0xF8); 
NT35596_DCS_write_1A_1P(0xCB,0x02); 
NT35596_DCS_write_1A_1P(0xCC,0x35); 
NT35596_DCS_write_1A_1P(0xCD,0x02); 
NT35596_DCS_write_1A_1P(0xCE,0x37); 
NT35596_DCS_write_1A_1P(0xCF,0x02); 
NT35596_DCS_write_1A_1P(0xD0,0x6C); 
NT35596_DCS_write_1A_1P(0xD1,0x02); 
NT35596_DCS_write_1A_1P(0xD2,0xAA); 
NT35596_DCS_write_1A_1P(0xD3,0x02); 
NT35596_DCS_write_1A_1P(0xD4,0xD2); 
NT35596_DCS_write_1A_1P(0xD5,0x03); 
NT35596_DCS_write_1A_1P(0xD6,0x0B); 
NT35596_DCS_write_1A_1P(0xD7,0x03); 
NT35596_DCS_write_1A_1P(0xD8,0x36); 
NT35596_DCS_write_1A_1P(0xD9,0x03); 
NT35596_DCS_write_1A_1P(0xDA,0x7D); 
NT35596_DCS_write_1A_1P(0xDB,0x03); 
NT35596_DCS_write_1A_1P(0xDC,0xA7); 
NT35596_DCS_write_1A_1P(0xDD,0x03); 
NT35596_DCS_write_1A_1P(0xDE,0xFB); 
NT35596_DCS_write_1A_1P(0xDF,0x03); 
NT35596_DCS_write_1A_1P(0xE0,0xFB); 
NT35596_DCS_write_1A_1P(0xE1,0x03); 
NT35596_DCS_write_1A_1P(0xE2,0xFB); 
NT35596_DCS_write_1A_1P(0xE3,0x03); 
NT35596_DCS_write_1A_1P(0xE4,0xFC); 
//NT35596_DCS_write_1A_1P(0xE5,0x03); 
//NT35596_DCS_write_1A_1P(0xE6,0xFE); 
NT35596_DCS_write_1A_1P(0xE7,0x03); 
NT35596_DCS_write_1A_1P(0xE8,0xFE); 
NT35596_DCS_write_1A_1P(0xE9,0x03); 
NT35596_DCS_write_1A_1P(0xEA,0xFE); 
NT35596_DCS_write_1A_1P(0xFF,0x05); 
NT35596_DCS_write_1A_1P(0xFB,0x01); 
NT35596_DCS_write_1A_1P(0x00,0x3F); 
NT35596_DCS_write_1A_1P(0x01,0x3F); 
NT35596_DCS_write_1A_1P(0x02,0x3F); 
NT35596_DCS_write_1A_1P(0x03,0x3F); 
NT35596_DCS_write_1A_1P(0x04,0x38); 
NT35596_DCS_write_1A_1P(0x05,0x3F); 
NT35596_DCS_write_1A_1P(0x06,0x3F); 
NT35596_DCS_write_1A_1P(0x07,0x19); 
NT35596_DCS_write_1A_1P(0x08,0x1D); 
NT35596_DCS_write_1A_1P(0x09,0x3F); 
NT35596_DCS_write_1A_1P(0x0A,0x3F); 
NT35596_DCS_write_1A_1P(0x0B,0x1B); 
NT35596_DCS_write_1A_1P(0x0C,0x17); 
NT35596_DCS_write_1A_1P(0x0D,0x3F); 
NT35596_DCS_write_1A_1P(0x0E,0x3F); 
NT35596_DCS_write_1A_1P(0x0F,0x08); 
NT35596_DCS_write_1A_1P(0x10,0x3F); 
NT35596_DCS_write_1A_1P(0x11,0x10); 
NT35596_DCS_write_1A_1P(0x12,0x3F); 
NT35596_DCS_write_1A_1P(0x13,0x3F); 
NT35596_DCS_write_1A_1P(0x14,0x3F); 
NT35596_DCS_write_1A_1P(0x15,0x3F); 
NT35596_DCS_write_1A_1P(0x16,0x3F); 
NT35596_DCS_write_1A_1P(0x17,0x3F); 
NT35596_DCS_write_1A_1P(0x18,0x38); 
NT35596_DCS_write_1A_1P(0x19,0x18); 
NT35596_DCS_write_1A_1P(0x1A,0x1C); 
NT35596_DCS_write_1A_1P(0x1B,0x3F); 
NT35596_DCS_write_1A_1P(0x1C,0x3F); 
NT35596_DCS_write_1A_1P(0x1D,0x1A); 
NT35596_DCS_write_1A_1P(0x1E,0x16); 
NT35596_DCS_write_1A_1P(0x1F,0x3F); 
NT35596_DCS_write_1A_1P(0x20,0x3F); 
NT35596_DCS_write_1A_1P(0x21,0x3F); 
NT35596_DCS_write_1A_1P(0x22,0x3F); 
NT35596_DCS_write_1A_1P(0x23,0x06); 
NT35596_DCS_write_1A_1P(0x24,0x3F); 
NT35596_DCS_write_1A_1P(0x25,0x0E); 
NT35596_DCS_write_1A_1P(0x26,0x3F); 
NT35596_DCS_write_1A_1P(0x27,0x3F); 
NT35596_DCS_write_1A_1P(0x54,0x06); 
NT35596_DCS_write_1A_1P(0x55,0x05); 
NT35596_DCS_write_1A_1P(0x56,0x04); 
NT35596_DCS_write_1A_1P(0x58,0x03); 
NT35596_DCS_write_1A_1P(0x59,0x1B); 
NT35596_DCS_write_1A_1P(0x5A,0x1B); 
NT35596_DCS_write_1A_1P(0x5B,0x01); 
NT35596_DCS_write_1A_1P(0x5C,0x32); 
NT35596_DCS_write_1A_1P(0x5E,0x18); 
NT35596_DCS_write_1A_1P(0x5F,0x20); 
NT35596_DCS_write_1A_1P(0x60,0x2B); 
NT35596_DCS_write_1A_1P(0x61,0x2C); 
NT35596_DCS_write_1A_1P(0x62,0x18); 
NT35596_DCS_write_1A_1P(0x63,0x01); 
NT35596_DCS_write_1A_1P(0x64,0x32); 
NT35596_DCS_write_1A_1P(0x65,0x00); 
NT35596_DCS_write_1A_1P(0x66,0x44); 
NT35596_DCS_write_1A_1P(0x67,0x11); 
NT35596_DCS_write_1A_1P(0x68,0x01); 
NT35596_DCS_write_1A_1P(0x69,0x01); 
NT35596_DCS_write_1A_1P(0x6A,0x04); 
NT35596_DCS_write_1A_1P(0x6B,0x2C); 
NT35596_DCS_write_1A_1P(0x6C,0x08); 
NT35596_DCS_write_1A_1P(0x6D,0x08); 
NT35596_DCS_write_1A_1P(0x78,0x00); 
NT35596_DCS_write_1A_1P(0x79,0x00); 
NT35596_DCS_write_1A_1P(0x7E,0x00); 
NT35596_DCS_write_1A_1P(0x7F,0x00); 
NT35596_DCS_write_1A_1P(0x80,0x00); 
NT35596_DCS_write_1A_1P(0x81,0x00); 
NT35596_DCS_write_1A_1P(0x8D,0x00); 
NT35596_DCS_write_1A_1P(0x8E,0x00); 
NT35596_DCS_write_1A_1P(0x8F,0xC0); 
NT35596_DCS_write_1A_1P(0x90,0x73); 
NT35596_DCS_write_1A_1P(0x91,0x10); 
NT35596_DCS_write_1A_1P(0x92,0x07); 
NT35596_DCS_write_1A_1P(0x96,0x11); 
NT35596_DCS_write_1A_1P(0x97,0x14); 
NT35596_DCS_write_1A_1P(0x98,0x00); 
NT35596_DCS_write_1A_1P(0x99,0x00); 
NT35596_DCS_write_1A_1P(0x9A,0x00); 
NT35596_DCS_write_1A_1P(0x9B,0x61); 
NT35596_DCS_write_1A_1P(0x9C,0x15); 
NT35596_DCS_write_1A_1P(0x9D,0x30); 
NT35596_DCS_write_1A_1P(0x9F,0x0F); 
NT35596_DCS_write_1A_1P(0xA2,0xB0); 
NT35596_DCS_write_1A_1P(0xA7,0x0A); 
NT35596_DCS_write_1A_1P(0xA9,0x00); 
NT35596_DCS_write_1A_1P(0xAA,0x70); 
NT35596_DCS_write_1A_1P(0xAB,0xDA); 
NT35596_DCS_write_1A_1P(0xAC,0xFF); 
NT35596_DCS_write_1A_1P(0xAE,0xF4); 
NT35596_DCS_write_1A_1P(0xAF,0x40); 
NT35596_DCS_write_1A_1P(0xB0,0x7F); 
NT35596_DCS_write_1A_1P(0xB1,0x16); 
NT35596_DCS_write_1A_1P(0xB2,0x53); 
NT35596_DCS_write_1A_1P(0xB3,0x00); 
NT35596_DCS_write_1A_1P(0xB4,0x2A); 
NT35596_DCS_write_1A_1P(0xB5,0x3A); 
NT35596_DCS_write_1A_1P(0xB6,0xF0); 
NT35596_DCS_write_1A_1P(0xBC,0x85); 
NT35596_DCS_write_1A_1P(0xBD,0xF4); 
NT35596_DCS_write_1A_1P(0xBE,0x33); 
NT35596_DCS_write_1A_1P(0xBF,0x13); 
NT35596_DCS_write_1A_1P(0xC0,0x77); 
NT35596_DCS_write_1A_1P(0xC1,0x77); 
NT35596_DCS_write_1A_1P(0xC2,0x77); 
NT35596_DCS_write_1A_1P(0xC3,0x77); 
NT35596_DCS_write_1A_1P(0xC4,0x77); 
NT35596_DCS_write_1A_1P(0xC5,0x77); 
NT35596_DCS_write_1A_1P(0xC6,0x77); 
NT35596_DCS_write_1A_1P(0xC7,0x77); 
NT35596_DCS_write_1A_1P(0xC8,0xAA); 
NT35596_DCS_write_1A_1P(0xC9,0x2A); 
NT35596_DCS_write_1A_1P(0xCA,0x00); 
NT35596_DCS_write_1A_1P(0xCB,0xAA); 
NT35596_DCS_write_1A_1P(0xCC,0x92); 
NT35596_DCS_write_1A_1P(0xCD,0x00); 
NT35596_DCS_write_1A_1P(0xCE,0x18); 
NT35596_DCS_write_1A_1P(0xCF,0x88); 
NT35596_DCS_write_1A_1P(0xD0,0xAA); 
NT35596_DCS_write_1A_1P(0xD1,0x00); 
NT35596_DCS_write_1A_1P(0xD2,0x00); 
NT35596_DCS_write_1A_1P(0xD3,0x00); 
NT35596_DCS_write_1A_1P(0xD6,0x02); 
NT35596_DCS_write_1A_1P(0xD7,0x31); 
NT35596_DCS_write_1A_1P(0xD8,0x7E); 
NT35596_DCS_write_1A_1P(0xED,0x00); 
NT35596_DCS_write_1A_1P(0xEE,0x00); 
NT35596_DCS_write_1A_1P(0xEF,0x70); 
NT35596_DCS_write_1A_1P(0xFA,0x03); 
NT35596_DCS_write_1A_1P(0xFF,0x00); 


NT35596_DCS_write_1A_1P(0x35,0x00); 
//MDELAY(120);
NT35596_DCS_write_1A_1P(0x11,0x00); 
MDELAY(120);	

NT35596_DCS_write_1A_1P(0x29,0x00); 
MDELAY(20);	


///{REGFLAG_END_OF_TABLE, 0x00, {}}
	// Note
	// Strongly recommend not to set Sleep out / Display On here. That will cause messed frame to be shown as later the backlight is on.
	// Setting ending by predefined flag
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));
    params->type   = LCM_TYPE_DSI;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;

	params->physical_width = 68;
	params->physical_height = 122;

	// enable tearing-free
#if (LCM_DSI_CMD_MODE)
	params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
	params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
#endif

#if (LCM_DSI_CMD_MODE)
	params->dsi.mode   = CMD_MODE;
#else
    params->dsi.mode = SYNC_EVENT_VDO_MODE;
#endif
	
	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM				= LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

	// Highly depends on LCD driver capability.
	// Not support in MT6573
	params->dsi.packet_size=256;

	// Video mode setting		
	params->dsi.intermediat_buffer_num = 0;// 0;

	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count=FRAME_WIDTH*3;	
		
	params->dsi.vertical_sync_active				= 5;
	params->dsi.vertical_backporch					= 16;
	params->dsi.vertical_frontporch					= 16;
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active				= 10;
	params->dsi.horizontal_backporch				= 80;
	params->dsi.horizontal_frontporch				= 80;
	params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
   // params->dsi.TA_GO =5;
	//params->dsi.compatibility_for_nvk = 1;

	params->dsi.clk_lp_per_line_enable = 1; // 1 clk line by line return LP11 0 clk frame by frame return LP11
    	params->dsi.esd_check_enable = 1;
    	params->dsi.customization_esd_check_enable = 0; // 0 ESD check via TE 1 ESD check via register
    	params->dsi.lcm_esd_check_table[0].cmd = 0x0A;
    	params->dsi.lcm_esd_check_table[0].count = 1;
    	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;
    	params->dsi.noncont_clock = 1; // 1 clk noncontinue 非连续 0 continue 连续
    	params->dsi.noncont_clock_period = 2;

      
	// Bit rate calculation
	params->dsi.PLL_CLOCK = 470;//500;

	
}

static void lcm_init(void)
{

   SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
 
#ifdef GPIO_LCM_LDO_1V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ONE);  //shm
#else   

    mt_dsi_pinctrl_set(LCM_POWER_DM_NO, 1);
#endif
    MDELAY(10);

#ifdef GPIO_LCM_LDO_2V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_OUT_ONE);
#else   
    mt_dsi_pinctrl_set(LCM_POWER_DP_NO, 1);
#endif
    MDELAY(10);
   
SET_RESET_PIN(1);
    MDELAY(10);


//	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
	init_lcm_registers();

}



static void lcm_suspend(void)
{	
	unsigned int data_array[16];
    
	data_array[0]=0x00280500; // Display Off
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(10);
	data_array[0] = 0x00100500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(50);


    NT35596_DCS_write_1A_1P(0xFF,0x05);
    NT35596_DCS_write_1A_1P(0xFB,0x01);
    NT35596_DCS_write_1A_1P(0xC5,0x00);
     MDELAY(10);

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10); 


#ifdef GPIO_LCM_LDO_2V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_OUT_ZERO);
#else   

    mt_dsi_pinctrl_set(LCM_POWER_DP_NO, 0);    // 2V8 off
#endif
    MDELAY(5);

#ifdef GPIO_LCM_LDO_1V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ZERO);  //shm
#else   
    mt_dsi_pinctrl_set(LCM_POWER_DM_NO, 0);
#endif
    MDELAY(5);

}


static void lcm_resume(void)
{  
 #ifdef GPIO_LCM_LDO_2V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_OUT_ZERO);
#else   

    mt_dsi_pinctrl_set(LCM_POWER_DP_NO, 0);    // 2V8 off
#endif
    MDELAY(5);

#ifdef GPIO_LCM_LDO_1V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ZERO);  //shm
#else   
    mt_dsi_pinctrl_set(LCM_POWER_DM_NO, 0);
#endif
    MDELAY(5);


    lcm_init();
}
         
#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0]= 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);

}
#endif

static unsigned int lcm_compare_id(void)
{
	unsigned int id=0;
	unsigned char buffer[2];
	unsigned int array[16];
	
#ifdef GPIO_LCM_LDO_1V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ONE);  //shm
#else   

    mt_dsi_pinctrl_set(LCM_POWER_DM_NO, 1);
#endif
    MDELAY(5);

#ifdef GPIO_LCM_LDO_2V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_OUT_ONE);
#else   
    mt_dsi_pinctrl_set(LCM_POWER_DP_NO, 1);
#endif
    MDELAY(5);

    SET_RESET_PIN(1);
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(20);
	array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);

		read_reg_v2(0xF4, buffer, 2);
		id = buffer[0]; //we only need ID
    	#ifdef BUILD_LK
			printf("%s, LK nt35596 debug: id = 0x%08x\n", __func__, id);
    	#else
			printk("%s, kernel nt35596 debug: id = 0x%08x\n", __func__, id);
    	#endif

    if(id == LCM_ID_NT35532)
    	return 1;
    else
        return 0;
}

#if 0
static unsigned int lcm_esd_check(void)
{
  #ifndef BUILD_LK
	char  buffer[3];
	int   array[4];

	if(lcm_esd_test)
	{
		lcm_esd_test = FALSE;
		return TRUE;
	}

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x36, buffer, 1);
	if(buffer[0]==0x90)
	{
		return FALSE;
	}
	else
	{			 
		return TRUE;
	}
 #endif

}


static unsigned int lcm_esd_recover(void)
{
	lcm_init();

	return TRUE;
}
#endif

LCM_DRIVER hct_nt35532_dsi_vdo_fhd_sharp_55_sh = 
{
    .name			= "hct_nt35532_dsi_vdo_fhd_sharp_55_sh",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,   
    .compare_id    = lcm_compare_id,    
#if (LCM_DSI_CMD_MODE)
    //.set_backlight    = lcm_setbacklight,
    .update         = lcm_update,
#endif  //wqtao
};

