/**
	@File:			enfont.c
	@Author:		Ihsoh
	@Date:			2014-7-24
	@Description:
		英文字体。
*/

#include "enfont.h"
#include "types.h"
#include "system.h"
#include "die.h"
#include "vesa.h"

#include "fs/ifs1/fs.h"

#define	__ISYS_KNL_BUILTIN_ENFONT__

#define	_ENFONT_WIDTH 	8
#define	_ENFONT_HEIGHT 	16

#define	_ENFONTX_WIDTH	8
#define	_ENFONTX_HEIGHT	16

#define	ENFONT_BUFFER_SIZE	(6 + 256 * _ENFONT_HEIGHT)
#define	ENFONTX_BUFFER_SIZE	(6 + 256 * _ENFONTX_WIDTH * _ENFONTX_HEIGHT)

uint32 __enfont_width	= _ENFONT_WIDTH;
uint32 __enfont_height	= _ENFONT_HEIGHT;

static ASCCHAR	enfont_file[1024];
#ifdef __ISYS_KNL_BUILTIN_ENFONT__
static BOOL		enfont_builtin = TRUE;
#else
static BOOL		enfont_builtin = FALSE;
#endif
static ASCCHAR	enfontx_file[1024];
static BOOL		_enfontx_enabled = FALSE;
static uint8 *	enfontx = NULL;

#ifdef __ISYS_KNL_BUILTIN_ENFONT__

static uint8 enfont[6 + 256 * 16] = 
{
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00,
	 0x00, 0x48, 0x24, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x48, 0x48, 0x48, 0x7E, 0x24, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x24, 0x00, 0x00,
	 0x00, 0x00, 0x10, 0x3C, 0x52, 0x52, 0x12, 0x1C, 0x30, 0x50, 0x50, 0x52, 0x52, 0x3C, 0x10, 0x10,
	 0x00, 0x00, 0x00, 0x22, 0x25, 0x15, 0x15, 0x0D, 0x2A, 0x58, 0x54, 0x54, 0x52, 0x22, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x0C, 0x12, 0x12, 0x12, 0x0A, 0x76, 0x25, 0x29, 0x19, 0x91, 0x6E, 0x00, 0x00,
	 0x00, 0x06, 0x04, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x00,
	 0x00, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x1C, 0x1C, 0x6B, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0xFE, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x04, 0x04, 0x02,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00,
	 0x00, 0x00, 0x40, 0x20, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00,
	 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x10, 0x1C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x40, 0x20, 0x10, 0x08, 0x04, 0x42, 0x7E, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x20, 0x18, 0x20, 0x40, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x20, 0x30, 0x30, 0x28, 0x24, 0x24, 0x22, 0xFE, 0x20, 0x20, 0xF8, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x02, 0x1E, 0x22, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x18, 0x24, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x44, 0x38, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x7E, 0x42, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x1C, 0x22, 0x42, 0x42, 0x42, 0x62, 0x5C, 0x40, 0x40, 0x24, 0x18, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08,
	 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x46, 0x20, 0x10, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x1C, 0x22, 0x5A, 0x55, 0x55, 0x55, 0x55, 0x55, 0x3A, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x42, 0x42, 0xE7, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x1E, 0x22, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x01, 0x01, 0x01, 0x01, 0x01, 0x42, 0x22, 0x1C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x1F, 0x22, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x42, 0x42, 0x3F, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x01, 0x01, 0x01, 0x71, 0x21, 0x22, 0x22, 0x1C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0F,
	 0x00, 0x00, 0x00, 0x77, 0x22, 0x12, 0x0A, 0x0E, 0x0A, 0x12, 0x12, 0x22, 0x22, 0x77, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x77, 0x36, 0x36, 0x36, 0x36, 0x36, 0x2A, 0x2A, 0x2A, 0x2A, 0x6B, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0xE3, 0x46, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x52, 0x62, 0x62, 0x47, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x4D, 0x32, 0x1C, 0x60, 0x00,
	 0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x3E, 0x12, 0x12, 0x22, 0x22, 0x42, 0xC7, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x02, 0x04, 0x18, 0x20, 0x40, 0x42, 0x42, 0x3E, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x7F, 0x49, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x22, 0x24, 0x24, 0x14, 0x14, 0x18, 0x08, 0x08, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x6B, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x36, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x77, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x7E, 0x21, 0x20, 0x10, 0x10, 0x08, 0x04, 0x04, 0x42, 0x42, 0x3F, 0x00, 0x00,
	 0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00,
	 0x00, 0x00, 0x02, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40,
	 0x00, 0x1E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x00,
	 0x00, 0x18, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
	 0x00, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x30, 0x2C, 0x22, 0x32, 0x6C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x02, 0x02, 0x02, 0x44, 0x38, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x62, 0xDC, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x02, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x22, 0x22, 0x1C, 0x02, 0x3C, 0x42, 0x42, 0x3C,
	 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1E,
	 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x72, 0x12, 0x0A, 0x0E, 0x12, 0x22, 0x77, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x08, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x92, 0x92, 0x92, 0x92, 0x92, 0xB7, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x02, 0x07,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x64, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0xE0,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x4C, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x02, 0x3C, 0x40, 0x42, 0x3E, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x48, 0x30, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x42, 0x42, 0x42, 0x42, 0x62, 0xDC, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x91, 0x52, 0x5A, 0x2A, 0x24, 0x24, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x24, 0x18, 0x18, 0x18, 0x24, 0x76, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x08, 0x08, 0x06,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x22, 0x10, 0x08, 0x08, 0x44, 0x7E, 0x00, 0x00,
	 0x00, 0xC0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00,
	 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
	 0x00, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x00,
	 0x04, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x02,
	 0x00, 0x00, 0x00, 0x60, 0xD0, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x05,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x1B, 0x12,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x08, 0x1C, 0x08, 0x2A, 0x7F, 0x2A, 0x08, 0x08, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08,
	 0x00, 0x00, 0x00, 0x08, 0x1C, 0x2A, 0x7F, 0x2A, 0x08, 0x1C, 0x08, 0x08, 0x2A, 0x7F, 0x2A, 0x1C,
	 0x00, 0x0C, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x44, 0x7A, 0x29, 0x29, 0x19, 0x55, 0xAA, 0x98, 0x94, 0x94, 0x52, 0x22, 0x00,
	 0x00, 0x24, 0x18, 0x00, 0x5C, 0x62, 0x42, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x42, 0x46, 0x3A,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0xF8, 0x84, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x84, 0xF8, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x04, 0x02, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x06, 0x06, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x24, 0x12, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x36, 0x36, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x16, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0xFE, 0xAA, 0x88, 0x88, 0x88, 0x88, 0x88, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x0C, 0x00, 0x1C, 0x12, 0x02, 0x04, 0x08, 0x10, 0x12, 0x0E,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x91, 0xF1, 0x11, 0x11, 0x11, 0x91, 0x6E, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x88, 0x88, 0x00, 0x8E, 0x04, 0x88, 0x88, 0x50, 0x50, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0C, 0x04,
	 0x00, 0x00, 0x40, 0x20, 0x20, 0x18, 0x74, 0x72, 0x12, 0x0A, 0x0A, 0x44, 0x3C, 0x04, 0x02, 0x02,
	 0x00, 0x00, 0x00, 0x78, 0x48, 0x04, 0x04, 0x04, 0x3E, 0x04, 0x04, 0x86, 0x5D, 0x32, 0x00, 0x00,
	 0x00, 0x02, 0x04, 0xE8, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0xE8, 0x04, 0x02, 0x00,
	 0x00, 0x00, 0x00, 0x46, 0x44, 0x24, 0x28, 0x18, 0xFF, 0x10, 0xFF, 0x10, 0x10, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	 0xC0, 0x20, 0x20, 0x20, 0x40, 0xA0, 0x90, 0x10, 0x10, 0x20, 0xA0, 0x40, 0x80, 0x00, 0x10, 0xE0,
	 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x18, 0x66, 0xBD, 0x83, 0x83, 0x83, 0xBD, 0x62, 0x1C, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x07, 0x0D, 0x0B, 0x0D, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x48, 0x24, 0x22, 0x32, 0x24, 0x44, 0x88, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x38, 0x46, 0x9D, 0xA5, 0x9D, 0x95, 0xA5, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00,
	 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x00, 0xF8, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x0E, 0x09, 0x08, 0x04, 0x12, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x0E, 0x08, 0x04, 0x08, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0xFC, 0x02, 0x02, 0x02, 0x00, 0x00,
	 0x00, 0x00, 0x7C, 0x2F, 0x2F, 0x2F, 0x2F, 0x2E, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x06,
	 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x0E, 0x19, 0x11, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x12, 0x24, 0x44, 0x4C, 0x24, 0x22, 0x11, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x42, 0x22, 0x22, 0x12, 0x12, 0x12, 0x48, 0xA8, 0xA4, 0xE4, 0x02, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x42, 0x22, 0x22, 0x12, 0x12, 0x4B, 0xA8, 0x84, 0x44, 0x44, 0xE2, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x46, 0x24, 0x24, 0x24, 0x14, 0x12, 0x48, 0x48, 0xA4, 0xE4, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x08, 0x04, 0x04, 0x22, 0x22, 0x3C, 0x00, 0x00,
	 0x08, 0x08, 0x10, 0x08, 0x18, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x22, 0x42, 0xE3, 0x00, 0x00,
	 0x10, 0x08, 0x08, 0x08, 0x18, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x22, 0x42, 0xE3, 0x00, 0x00,
	 0x18, 0x14, 0x20, 0x08, 0x18, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x22, 0x42, 0xE3, 0x00, 0x00,
	 0x0C, 0x30, 0x00, 0x08, 0x18, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x22, 0x42, 0xE3, 0x00, 0x00,
	 0x24, 0x3C, 0x00, 0x08, 0x18, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x22, 0x42, 0xE3, 0x00, 0x00,
	 0x18, 0x14, 0x18, 0x08, 0x18, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x22, 0x42, 0xE3, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x78, 0x18, 0x18, 0x14, 0x54, 0x74, 0x1A, 0x12, 0x12, 0x91, 0x7B, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x78, 0x84, 0x82, 0x02, 0x02, 0x02, 0x02, 0x02, 0x86, 0x4C, 0x30, 0x10, 0x10,
	 0x0C, 0x08, 0x10, 0x6E, 0x42, 0x02, 0x22, 0x3E, 0x22, 0x22, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00,
	 0x30, 0x10, 0x08, 0x76, 0x42, 0x02, 0x22, 0x3E, 0x22, 0x22, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00,
	 0x18, 0x14, 0x22, 0x7E, 0x42, 0x02, 0x22, 0x3E, 0x22, 0x22, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00,
	 0x34, 0x34, 0x00, 0x7E, 0x42, 0x02, 0x22, 0x3E, 0x22, 0x22, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00,
	 0x0C, 0x08, 0x10, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	 0x30, 0x10, 0x08, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	 0x18, 0x14, 0x00, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	 0x24, 0x34, 0x00, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3C, 0x44, 0x44, 0x84, 0x84, 0x8C, 0x84, 0xC4, 0x44, 0x64, 0x1E, 0x00, 0x00,
	 0x0C, 0x30, 0x00, 0x42, 0x46, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x62, 0x62, 0x42, 0x47, 0x00, 0x00,
	 0x0C, 0x08, 0x10, 0x2C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	 0x10, 0x10, 0x08, 0x34, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	 0x18, 0x14, 0x24, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	 0x0C, 0x30, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	 0x24, 0x24, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x3C, 0x42, 0x62, 0x62, 0x52, 0xCA, 0x4A, 0x46, 0x46, 0x46, 0x38, 0x00, 0x00,
	 0x0C, 0x08, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x10, 0x10, 0x08, 0x46, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x18, 0x24, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x24, 0x24, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x10, 0x10, 0x08, 0x46, 0x22, 0x24, 0x24, 0x18, 0x18, 0x08, 0x08, 0x08, 0x08, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x06, 0x04, 0x3C, 0x44, 0x84, 0x84, 0x84, 0x7C, 0x04, 0x04, 0x0F, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x34, 0x44, 0x44, 0x44, 0x44, 0x5C, 0x36, 0x00, 0x00,
	 0x02, 0x04, 0x08, 0x00, 0x00, 0x2E, 0x31, 0x21, 0x21, 0x21, 0x31, 0x6E, 0x00, 0x00, 0x00, 0x00,
	 0x10, 0x08, 0x04, 0x00, 0x00, 0x2E, 0x31, 0x21, 0x21, 0x21, 0x31, 0x6E, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x0C, 0x14, 0x22, 0x00, 0x1C, 0x22, 0x22, 0x38, 0x26, 0x22, 0xA2, 0x5C, 0x00, 0x00,
	 0x00, 0x00, 0x04, 0x3A, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x38, 0x26, 0x22, 0xA2, 0x5C, 0x00, 0x00,
	 0x00, 0x00, 0x14, 0x36, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x38, 0x26, 0x22, 0xA2, 0x5C, 0x00, 0x00,
	 0x00, 0x00, 0x1C, 0x14, 0x08, 0x00, 0x1C, 0x22, 0x22, 0x38, 0x26, 0x22, 0xA2, 0x5C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x99, 0x89, 0xFC, 0x0A, 0x09, 0x99, 0x76, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x26, 0x22, 0x02, 0x02, 0x42, 0x22, 0x1C, 0x04, 0x08, 0x06,
	 0x02, 0x04, 0x08, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x7F, 0x01, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00,
	 0x10, 0x08, 0x04, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x7F, 0x01, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00,
	 0x08, 0x14, 0x22, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x7F, 0x01, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x34, 0x24, 0x00, 0x18, 0x24, 0x42, 0x42, 0x3E, 0x02, 0x44, 0x38, 0x00, 0x00,
	 0x04, 0x08, 0x10, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
	 0x10, 0x08, 0x04, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x08, 0x14, 0x22, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x34, 0x36, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x24, 0x18, 0x16, 0x38, 0x64, 0x42, 0x42, 0x42, 0x42, 0x46, 0x38, 0x00, 0x00,
	 0x00, 0x00, 0x0C, 0x38, 0x00, 0x00, 0x10, 0x2E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x67, 0x00, 0x00,
	 0x04, 0x08, 0x10, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00,
	 0x10, 0x08, 0x04, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x18, 0x14, 0x20, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x0C, 0x38, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x34, 0x34, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0xFC, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x64, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x3D, 0x00, 0x00,
	 0x04, 0x08, 0x10, 0x00, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x31, 0x6E, 0x00, 0x00, 0x00, 0x00,
	 0x10, 0x08, 0x04, 0x00, 0x00, 0x21, 0x21, 0x21, 0x21, 0x21, 0x31, 0x6E, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x18, 0x14, 0x22, 0x00, 0x40, 0x42, 0x42, 0x42, 0x42, 0x42, 0x62, 0x1C, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x21, 0x21, 0x21, 0x21, 0x31, 0x6E, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x30, 0x10, 0x08, 0x00, 0x00, 0x24, 0x24, 0x24, 0x18, 0x18, 0x10, 0x08, 0x08, 0x06,
	 0x00, 0x00, 0x07, 0x06, 0x06, 0x06, 0x7E, 0x46, 0x46, 0x46, 0x46, 0x46, 0x3E, 0x06, 0x06, 0x06
};

#else	//	#ifdef __ISYS_KNL_BUILTIN_ENFONT__

static uint8 * enfont = NULL;

#endif	//	#ifdef __ISYS_KNL_BUILTIN_ENFONT__

/**
	@Function:		get_enfont_ptr
	@Access:		Public
	@Description:
		获取英文字体的起始指针。
	@Parameters:
	@Return:
		uint8 *
			英文字体的起始指针。		
*/
uint8 *
get_enfont_ptr(void)
{
	if(_enfontx_enabled)
		return enfontx;
	else
		return enfont;
}

/**
	@Function:		get_enfontx_ptr
	@Access:		Public
	@Description:
		获取ENFontX的起始指针。
	@Parameters:
	@Return:
		uint8 *
			ENFontX的起始指针。		
*/
uint8 *
get_enfontx_ptr(void)
{
	return enfontx;
}

/**
	@Function:		get_enfont
	@Access:		Public
	@Description:
		获取某个字符的点阵信息。
	@Parameters:
		chr, uint8, IN
			字符。
	@Return:
		uint8 *
			点阵信息。		
*/
uint8 *
get_enfont(IN uint8 chr)
{
	return enfont + 6 + chr * 16;
}

/**
	@Function:		enfont_die
	@Access:		Private
	@Description:
		ENFont出现致命错误，调用该函数停机。
	@Parameters:
	@Return:		
*/
static
void
enfont_die(void)
{
	struct die_info info;
	fill_info(info, DC_INIT_ENFONT, DI_INIT_ENFONT);
	die(&info);
}

/**
	@Function:		init_enfont
	@Access:		Public
	@Description:
		初始化英文字体。
	@Parameters:
	@Return:	
*/
void 
init_enfont(void)
{
	if(vesa_is_valid())
	{
		#ifndef __ISYS_KNL_BUILTIN_ENFONT__
		if(enfont == NULL)
		{
			enfont = (uint8 *)alloc_memory(ENFONT_BUFFER_SIZE);
			if(enfont == NULL)
				enfont_die();
		}
		#endif

		if(	!config_gui_get_string("ENFont", enfont_file, sizeof(enfont_file))
			|| !config_gui_get_string("ENFontX", enfontx_file, sizeof(enfontx_file))
			|| !config_gui_get_bool("EnableENFontX", &_enfontx_enabled))
			enfont_die();
		if(_enfontx_enabled)
		{
			__enfont_width = _ENFONTX_WIDTH;
			__enfont_height = _ENFONTX_HEIGHT;
			enfontx = (uint8 *)alloc_memory(ENFONTX_BUFFER_SIZE);
			if(enfontx == NULL)
				enfont_die();
			FileObject * fptr = Ifs1OpenFile(enfontx_file, FILE_MODE_READ);
			if(fptr == NULL)
				enfont_die();
			if(Ifs1ReadFile(fptr, enfontx, ENFONTX_BUFFER_SIZE) != ENFONTX_BUFFER_SIZE)
				enfont_die();
			Ifs1CloseFile(fptr);
		}
		else
		{
			if(strcmp(enfont_file, "") == 0 && !enfont_builtin)
				enfont_die();
			if(strcmp(enfont_file, "") != 0)
			{
				FileObject * fptr = Ifs1OpenFile(enfont_file, FILE_MODE_READ);
				if(fptr == NULL && !enfont_builtin)
					enfont_die();
				if(Ifs1ReadFile(fptr, enfont, ENFONT_BUFFER_SIZE) != ENFONT_BUFFER_SIZE)
					enfont_die();
				if(strncmp(enfont, "ENFONT", 6) != 0)
					enfont_die();
				Ifs1CloseFile(fptr);
			}
		}
	}
}

/**
	@Function:		enfontx_enabled
	@Access:		Public
	@Description:
		检测ENFontX是否可用。
	@Parameters:
	@Return:
		BOOL
			返回TRUE则可用，否则不可用。
*/
BOOL
enfontx_enabled(void)
{
	return _enfontx_enabled;
}
