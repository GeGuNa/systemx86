/**
	@File:			file_list.h
	@Author:		Ihsoh
	@Date:			2020-05-04
	@Description:
		文件列表控件 - File List。
*/

#ifndef	_WINDOW_FILE_LIST_H_
#define	_WINDOW_FILE_LIST_H_

#include "control.h"
#include "list.h"
#include "button.h"

#include "../types.h"
#include "../image.h"
#include "../window.h"

#include "../fs/ifs1/fs.h"

#define	MAX_FILE_LIST_ITEM_COUNT		16

#define	FILE_LIST_ITEM_TYPE_UNKNOWN		0
#define	FILE_LIST_ITEM_TYPE_FILE		1
#define	FILE_LIST_ITEM_TYPE_DIR			2
#define	FILE_LIST_ITEM_TYPE_SLINK		3

typedef struct
{
	int8				name[256];
	int					type;
} FileListItem, * FileListItemPtr;

typedef	struct
{
	uint32				id;
	int32				type;
	int32				uwid;							// User Window ID。
	int32				uwcid;							// User Window Control ID。
	uint32				x;
	uint32				y;
	List				list;
	uint32				max_count;

	int8				path[MAX_PATH_BUFFER_LEN];
	
	FileListItemPtr		items;
	uint32				count;

	uint32				top;

	ControlEvent		event;
} FileList, * FileListPtr;

extern
BOOL
CtrlFileListInit(	OUT FileListPtr file_list,
					IN uint32 id,
					IN uint32 max_count,
					IN uint32 x,
					IN uint32 y,
					IN uint32 color,
					IN uint32 bgcolor,
					IN uint32 colorh,
					IN uint32 bgcolorh,
					IN ControlEvent event);

extern
BOOL
CtrlFileListRelease(IN OUT FileListPtr file_list);

extern
BOOL
CtrlFileListUpdate(	IN OUT FileListPtr file_list,
					OUT ImagePtr image,
					IN WindowEventParamsPtr params,
					BOOL top);

extern
BOOL
CtrlFileListSetPath(
	IN OUT FileListPtr file_list,
	IN int8 * path);

extern
BOOL
CtrlFileListSetTop(
	IN OUT FileListPtr file_list,
	IN uint32 top);



#endif
