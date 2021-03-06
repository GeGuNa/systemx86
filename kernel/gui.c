/**
	@File:			gui.c
	@Author:		Ihsoh
	@Date:			2015-10-05
	@Description:
		用户模式GUI功能。
*/

#include "gui.h"
#include "types.h"
#include "tasks.h"
#include "memory.h"
#include "screen.h"
#include "vesa.h"
#include "lock.h"
#include "kernel.h"
#include "mouse.h"
#include "keyboard.h"
#include "window.h"
#include "image.h"
#include "enfont.h"

#include "window/button.h"
#include "window/label.h"
#include "window/list.h"
#include "window/edit.h"
#include "window/scroll.h"
#include "window/vscroll.h"
#include "window/driver_list.h"
#include "window/file_list.h"

#include <dslib/list.h>
#include <dslib/linked_list.h>
#include <dslib/value.h>

#include <ilib/string.h>

DEFINE_LOCK_IMPL(gui)

typedef struct
{
	int32				tid;
	WindowPtr			window;
	DSLLinkedListPtr	messages;
	DSLListPtr			controls;	
} WindowInstance, * WindowInstancePtr;

static DSLListPtr		_winstances = NULL;

/**
	@Function:		GuiInit
	@Access:		Public
	@Description:
		初始化用户GUI模块。
	@Parameters:
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiInit(void)
{
	_winstances = dsl_lst_new();
	if(_winstances == NULL)
		return FALSE;
	return TRUE;
}

/**
	@Function:		GuiIsEnabled
	@Access:		Public
	@Description:
		检测用户GUI是否可用。
	@Parameters:
	@Return:
		BOOL
			返回TRUE则可用，否则不可用。
*/
BOOL
GuiIsEnabled(void)
{
	if(VesaIsEnabled())
		return TRUE;
	else
		return FALSE;
}

/**
	@Function:		_GuiWindowEvent
	@Access:		Private
	@Description:
		用户GUI的窗体的事件函数。
	@Parameters:
		window, struct Window *, IN
			指向窗体对象的指针。
		params, struct WindowEventParams *, IN
			指向窗体事件参数对象的指针。
	@Return:
*/
static
void
_GuiWindowEvent(IN struct Window * window,
				IN struct WindowEventParams * params)
{
	BOOL top = ScrGetTopWindow() == window;
	switch(params->event_type)
	{
		case WINDOW_EVENT_PAINT:
		{
			int32 wid = window->uwid;
			DSLValuePtr v = dsl_lst_get(_winstances, wid);
			if(v != NULL)
			{
				WindowInstancePtr winstance = (WindowInstancePtr)DSL_OBJECTVAL(v);
				uint32 ui;
				for(ui = 0; ui < winstance->controls->count; ui++)
				{
					v = dsl_lst_get(winstance->controls, ui);
					ControlPtr control = (ControlPtr)DSL_OBJECTVAL(v);
					switch(control->type)
					{
						case CONTROL_BUTTON:
						{
							BUTTON(control, &window->workspace, params, top);
							break;
						}
						case CONTROL_LABEL:
						{
							LABEL(control, &window->workspace, params, top);
							break;
						}
						case CONTROL_LIST:
						{
							LIST(control, &window->workspace, params, top);
							break;
						}
						case CONTROL_EDIT:
						{
							NMLEDIT(control, &window->workspace, params, top);
							break;
						}
						case CONTROL_SCROLL:
						{
							SCROLL(control, &window->workspace, params, top);
							break;
						}
						case CONTROL_VSCROLL:
						{
							VSCROLL(control, &window->workspace, params, top);
							break;
						}
						case CONTROL_DRIVER_LIST:
						{
							DRIVER_LIST(control, &window->workspace, params, top);
							break;
						}
						case CONTROL_FILE_LIST:
						{
							CtrlFileListUpdate(control, &window->workspace, params, top);
							break;
						}
					}
				}
			}
			break;
		}
		case WINDOW_EVENT_WILL_CLOSE:
		{
			DSLValuePtr v = dsl_lst_get(_winstances, window->uwid);
			if(v == NULL)
				return;
			WindowInstancePtr winstance = (WindowInstancePtr)DSL_OBJECTVAL(v);
			if(winstance == NULL)
				return;
			GuiClearMessages(winstance->tid, window->uwid);
			GuiPushMessage(	winstance->tid,
								window->uwid,
								-1,
								WINDOW_WILL_CLOSE,
								NULL,
								NULL);
			break;
		}
	}
}

/**
	@Function:		GuiCreateWindow
	@Access:		Public
	@Description:
		创建用户GUI的窗体。
	@Parameters:
		tid, int32, IN
			用户任务的ID。
		width, uint32, IN
			窗体宽度。
		height, uint32, IN
			窗体高度。
		bgcolor, uint32, IN
			窗体背景色。
		style, uint32, IN
			窗体样式。
		title, CASCTEXT, IN
			窗体标题。
	@Return:
		int32
			用户GUI的窗体的ID。
*/
int32
GuiCreateWindow(IN int32		tid,
				IN uint32		width,
				IN uint32		height,
				IN uint32		bgcolor,
				IN uint32		style,
				IN CASCTEXT		title)
{
	lock();
	WindowPtr window = NULL;
	DSLLinkedListPtr messages = NULL;
	DSLListPtr controls = NULL;
	WindowInstancePtr winstance = NULL;
	window = ScrCreateWindow(	width,
							height,
							bgcolor,
							style,
							title,
							_GuiWindowEvent);
	if(window == NULL)
		goto err;
	messages = dsl_lnklst_new();
	if(messages == NULL)
		goto err;
	controls = dsl_lst_new();
	if(controls == NULL)
		goto err;
	winstance = (WindowInstancePtr)MemAlloc(sizeof(WindowInstance));
	if(winstance == NULL)
		goto err;
	winstance->tid = tid;
	winstance->window = window;
	winstance->messages = messages;
	winstance->controls = controls;
	DSLValuePtr v = dsl_val_object(winstance);
	if(v == NULL)
		goto err;
	int32 wid = dsl_lst_find_value(_winstances, NULL);
	if(wid == -1)
	{
		if(!dsl_lst_add_value(_winstances, v))
			goto err;
		wid = dsl_lst_find_value(_winstances, v);
	}
	else
		dsl_lst_set(_winstances, wid, v);
	window->uwid = wid;
	unlock();
	return wid;
err:
	if(window != NULL)
		MemFree(window);
	if(messages != NULL)
		dsl_lnklst_free(messages);
	if(controls != NULL)
		dsl_lst_free(controls);
	if(winstance != NULL)
		MemFree(winstance);
	unlock();
	return -1;
}

#define _WINSTANCE_FALSE	\
	DSLValuePtr __v = dsl_lst_get(_winstances, wid);	\
	if(__v == NULL)	\
		return FALSE;	\
	WindowInstancePtr winstance = (WindowInstancePtr)DSL_OBJECTVAL(__v);	\
	if(winstance == NULL || winstance->tid != tid)	\
		return FALSE;

#define _CONTROL_FALSE		\
	__v = dsl_lst_get(winstance->controls, cid);	\
	ControlPtr control = (ControlPtr)DSL_OBJECTVAL(__v);

/**
	@Function:		GuiClearMessages
	@Access:		Public
	@Description:
		清除指定窗体的消息队列。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiClearMessages(	IN int32 tid,
					IN int32 wid)
{
	_WINSTANCE_FALSE
	lock();
	void * phyaddr_data = NULL;
	while(GuiPopMessage(tid, wid, NULL, NULL, NULL, &phyaddr_data))
		if(phyaddr_data != NULL)
			MemFree(phyaddr_data);
	unlock();
	return TRUE;
}

/**
	@Function:		GuiCloseWindow
	@Access:		Public
	@Description:
		关闭指定窗体。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiCloseWindow(	IN int32 tid,
				IN int32 wid)
{
	_WINSTANCE_FALSE
	lock();
	ScrDestroyWindow(winstance->window);
	GuiClearMessages(tid, wid);
	dsl_lnklst_free(winstance->messages);
	dsl_lst_delete_all_object_value(winstance->controls);
	dsl_lst_free(winstance->controls);
	dsl_lst_set(_winstances, wid, NULL);
	MemFree(winstance);
	MemFree(__v);
	unlock();
	return TRUE;
}

/**
	@Function:		GuiCloseWindows
	@Access:		Public
	@Description:
		关闭指定任务的所有窗体。
	@Parameters:
		tid, int32, IN
			任务ID。
	@Return:
*/
void
GuiCloseWindows(IN int32 tid)
{
	lock();
	uint32 wid = 0;
	uint32 count = _winstances->count;
	for(wid = 0; wid < count; wid++)
	{
		DSLValuePtr v = dsl_lst_get(_winstances, wid);
		if(v != NULL)
		{
			WindowInstancePtr winstance = (WindowInstancePtr)DSL_OBJECTVAL(v);
			if(winstance->tid == tid)
				GuiCloseWindow(tid, wid);
		}
	}
	unlock();
}

/**
	@Function:		GuiSetWindowStatus
	@Access:		Public
	@Description:
		设置窗体的状态。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		state, uint32, IN
			状态。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiSetWindowStatus(	IN int32 tid,
					IN int32 wid,
					IN uint32 state)
{
	_WINSTANCE_FALSE
	winstance->window->state = state;
	return TRUE;
}

/**
	@Function:		GuiGetWindowStatus
	@Access:		Public
	@Description:
		获取窗体的状态。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		state, uint32 *, OUT
			状态。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiGetWindowStatus(	IN int32 tid,
					IN int32 wid,
					OUT uint32 * state)
{
	_WINSTANCE_FALSE
	if(state == NULL)
		return FALSE;
	*state = winstance->window->state;
	return TRUE;
}

/**
	@Function:		GuiSetWindowPosition
	@Access:		Public
	@Description:
		设置窗体的位置。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			窗体的X坐标。
		y, int32, IN
			窗体的Y坐标。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiSetWindowPosition(	IN int32 tid,
						IN int32 wid,
						IN int32 x,
						IN int32 y)
{
	_WINSTANCE_FALSE
	winstance->window->x = x;
	winstance->window->y = y;
	return TRUE;
}

/**
	@Function:		GuiGetWindowPosition
	@Access:		Public
	@Description:
		获取窗体的位置。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32 *, OUT
			指向储存窗体的X坐标的缓冲区的指针。
		y, int32 *, OUT
			指向储存窗体的Y坐标的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiGetWindowPosition(	IN int32 tid,
						IN int32 wid,
						OUT int32 * x,
						OUT int32 * y)
{
	_WINSTANCE_FALSE
	if(x == NULL || y == NULL)
		return FALSE;
	*x = winstance->window->x;
	*y = winstance->window->y;
	return TRUE;
}

/**
	@Function:		GuiSetWindowSize
	@Access:		Public
	@Description:
		设置窗体的大小。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		width, uint32, IN
			宽度。
		height, uint32, IN
			高度。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiSetWindowSize(	IN int32 tid,
					IN int32 wid,
					IN uint32 width,
					IN uint32 height)
{
	_WINSTANCE_FALSE

	// 未实现！！！
	return FALSE;

	return TRUE;
}

/**
	@Function:		GuiGetWindowSize
	@Access:		Public
	@Description:
		获取窗体的大小。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		width, uint32 *, OUT
			指向储存宽度的缓冲区的指针。
		height, uint32 *, OUT
			指向储存高度的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiGetWindowSize(	IN int32 tid,
					IN int32 wid,
					OUT uint32 * width,
					OUT uint32 * height)
{
	_WINSTANCE_FALSE
	if(width == NULL || height == NULL)
		return FALSE;
	*width = winstance->window->width;
	*height = winstance->window->height;
	return TRUE;
}

/**
	@Function:		GuiSetWindowTitle
	@Access:		Public
	@Description:
		设置窗体标题。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		title, CASCTEXT, IN
			标题文本。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiSetWindowTitle(	IN int32 tid,
					IN int32 wid,
					IN CASCTEXT title)
{
	_WINSTANCE_FALSE
	if(strlen(title) >= sizeof(winstance->window->title))
		return FALSE;
	if(title == NULL)
		return FALSE;
	UtlCopyString(winstance->window->title, sizeof(winstance->window->title), title);
	return TRUE;
}

/**
	@Function:		GuiGetWindowTitle
	@Access:		Public
	@Description:
		获取窗体标题。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		title, ASCTEXT, OUT
			指向用于储存窗体标题的缓冲区。
			该缓冲区大小必须大于或等于WINDOW_TITLE_SIZE。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiGetWindowTitle(	IN int32 tid,
					IN int32 wid,
					OUT ASCTEXT title)
{
	_WINSTANCE_FALSE
	if(title == NULL)
		return FALSE;
	UtlCopyString(title, WINDOW_TITLE_SIZE, winstance->window->title);
	return TRUE;
}

/**
	@Function:		GuiHasKey
	@Access:		Public
	@Description:
		检测窗体按键队列里是否包含按键。
		这个操作不会弹出按键队列中的按键。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		key, uint32 *, OUT
			指向用于储存按键值的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则代表按键队列有按键。
*/
BOOL
GuiHasKey(	IN int32 tid,
			IN int32 wid,
			OUT uint32 * key)
{
	_WINSTANCE_FALSE
	int32 count = WINMGR_HAS_KEY(winstance->window);
	if(count > 0)
	{
		if(key != NULL)
			*key = WinmgrPeekKey(winstance->window);
		return TRUE;
	}
	else
		return FALSE;
}

/**
	@Function:		GuiGetKey
	@Access:		Public
	@Description:
		获取窗体按键队列里的按键。
		这个操作会弹出按键队列中的按键。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		key, uint32 *, OUT
			指向用于储存按键值的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则代表按键队列有按键。
*/
BOOL
GuiGetKey(	IN int32 tid,
			IN int32 wid,
			OUT uint32 * key)
{
	_WINSTANCE_FALSE
	if(key == NULL)
		return FALSE;
	int32 count = WINMGR_HAS_KEY(winstance->window);
	if(count > 0)
	{
		*key = WinmgrGetKey(winstance->window);
		return TRUE;
	}
	else
		return FALSE;
}

/**
	@Function:		GuiGetMouse
	@Access:		Public
	@Description:
		获取鼠标信息。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32 *, OUT
			指向用于储存光标在窗体内的X坐标的缓冲区的指针。
		y, int32 *, OUT
			指向用于储存光标在窗体内的Y坐标的缓冲区的指针。
		button, uint32 *, OUT
			指向用于储存鼠标当前状态的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则代表按键队列有按键。
*/
BOOL
GuiGetMouse(	IN int32 tid,
				IN int32 wid,
				OUT int32 * x,
				OUT int32 * y,
				OUT uint32 * button)
{
	_WINSTANCE_FALSE
	if(x == NULL || y == NULL || button == NULL)
		return FALSE;
	WindowPtr window = winstance->window;
	uint32 wstyle = window->style;
	BOOL has_title_bar = !(wstyle & WINDOW_STYLE_NO_TITLE);
	KnlGetMousePosition(x, y);
	*x = *x - window->x;
	if(has_title_bar)
		*y = *y - window->y - TITLE_BAR_HEIGHT;
	else
		*y = *y - window->y;
	*button = MOUSE_BUTTON_NONE;
	if(KnlIsMouseLeftButtonDown())
		*button |= MOUSE_BUTTON_LEFT;
	if(KnlIsMouseRightButtonDown())
		*button |= MOUSE_BUTTON_RIGHT;
	return TRUE;
}

/**
	@Function:		GuiIsFocused
	@Access:		Public
	@Description:
		检测窗体是否获得了焦点。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
	@Return:
		BOOL
			返回TRUE则获得了焦点。
*/
BOOL
GuiIsFocused(	IN int32 tid,
				IN int32 wid)
{
	_WINSTANCE_FALSE
	if(ScrGetTopWindow() == winstance->window)
		return TRUE;
	else
		return FALSE;
}

/**
	@Function:		GuiFocus
	@Access:		Public
	@Description:
		使窗体获得焦点。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiFocus(	IN int32 tid,
			IN int32 wid)
{
	_WINSTANCE_FALSE
	return ScrSetTopWindow(winstance->window);
}

/**
	@Function:		GuiSetPixel
	@Access:		Public
	@Description:
		使设置窗体工作区的像素。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		pixel, uint32, IN
			像素值。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiSetPixel(IN int32 tid,
			IN int32 wid,
			IN int32 x,
			IN int32 y,
			IN uint32 pixel)
{
	_WINSTANCE_FALSE
	return set_pixel_common_image(	&winstance->window->workspace,
									x,
									y,
									pixel);
}

/**
	@Function:		GuiGetPixel
	@Access:		Public
	@Description:
		获取窗体工作区的像素。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		pixel, uint32 *, OUT
			指向用于储存窗体工作区的像素的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiGetPixel(IN int32 tid,
			IN int32 wid,
			IN int32 x,
			IN int32 y,
			OUT uint32 * pixel)
{
	_WINSTANCE_FALSE
	if(pixel == NULL)
		return FALSE;
	*pixel = get_pixel_common_image(&winstance->window->workspace,
									x,
									y);
	return TRUE;
}

/**
	@Function:		GuiDrawRect
	@Access:		Public
	@Description:
		在工作区内绘制矩形。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		width, int32, IN
			宽度。
		height, int32, IN
			高度。
		pixel, uint32, IN
			矩形填充的像素值。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiDrawRect(IN int32 tid,
			IN int32 wid,
			IN int32 x,
			IN int32 y,
			IN int32 width,
			IN int32 height,
			IN uint32 pixel)
{
	_WINSTANCE_FALSE
	return rect_common_image(	&winstance->window->workspace,
								x,
								y,
								width,
								height,
								pixel);
}

/**
	@Function:		GuiDrawImage
	@Access:		Public
	@Description:
		在工作区内绘制图片。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		width, int32, IN
			宽度。
		height, int32, IN
			高度。
		image, ImagePtr, IN
			指向图片对象的指针。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiDrawImage(	IN int32 tid,
				IN int32 wid,
				IN int32 x,
				IN int32 y,
				IN int32 width,
				IN int32 height,
				IN ImagePtr image)
{
	_WINSTANCE_FALSE
	if(image == NULL)
		return FALSE;
	return draw_common_image(	&winstance->window->workspace,
								image,
								x,
								y,
								width,
								height);
}

/**
	@Function:		GuiDrawText
	@Access:		Public
	@Description:
		在工作区内绘制文本。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		text, CASCTEXT, IN
			文本。
		color, uint32, IN
			文本颜色。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiDrawText(IN int32 tid,
			IN int32 wid,
			IN int32 x,
			IN int32 y,
			IN CASCTEXT text,
			IN uint32 color)
{
	_WINSTANCE_FALSE
	if(text == NULL)
		return FALSE;
	return text_common_image_ml(&winstance->window->workspace,
								x,
								y,
								EnfntGetFontDataPtr(),
								text,
								strlen(text),
								color);
}

/**
	@Function:		GuiDrawLine
	@Access:		Public
	@Description:
		在工作区内绘制一条线。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		startx, int32, IN
			起始点的X坐标。
		starty, int32, IN
			起始点的Y坐标。
		endx, int32, IN
			结束点的X坐标。
		endy, int32, IN
			结束点的Y坐标。
		color, uint32, IN
			线条的颜色。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiDrawLine(IN int32 tid,
			IN int32 wid,
			IN int32 startx,
			IN int32 starty,
			IN int32 endx,
			IN int32 endy,
			IN uint32 color)
{
	_WINSTANCE_FALSE
	return line_common_image(	&winstance->window->workspace,
								startx,
								starty,
								endx,
								endy,
								color);
}

/**
	@Function:		GuiPushMessage
	@Access:		Public
	@Description:
		往指定窗口的消息队列里压入一条消息。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
		type, uint32, IN
			事件类型。
		data, void *, IN
			指向事件的附加数据的指针，用户态时可访问。
		phyaddr_data, void *, IN
			指向事件的附加数据的指针，内核态时可访问。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiPushMessage(	IN int32 tid,
				IN int32 wid,
				IN int32 cid,
				IN uint32 type,
				IN void * data,
				IN void * phyaddr_data)
{
	_WINSTANCE_FALSE
	if(winstance->messages->count >= GUI_MAX_MESSAGE_COUNT)
		return FALSE;
	lock();
	WindowMessagePtr msg = NULL;
	DSLLinkedListNodePtr node = NULL;
	msg = NEW(WindowMessage);
	if(msg == NULL)
		goto err;
	msg->wid = wid;
	msg->cid = cid;
	msg->type = type;
	msg->data = data;
	msg->phyaddr_data = phyaddr_data;
	node = dsl_lnklst_new_object_node(msg);
	if(node == NULL)
		goto err;
	dsl_lnklst_add_node(winstance->messages, node);
	unlock();
	return TRUE;
err:
	if(msg != NULL)
		DELETE(msg);
	if(node != NULL)
		DELETE(node);
	unlock();
	return FALSE;
}

/**
	@Function:		GuiPopMessage
	@Access:		Public
	@Description:
		从指定窗口的消息队列里弹出一条消息。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32 *, OUT
			指向用于储存控件ID的缓冲区的指针。
		type, uint32 *, OUT
			指向用于储存事件类型的缓冲区的指针。
		data, void **, OUT
			指向用于储存“事件的附加数据的指针（用户态）”的缓冲区的指针。
		phyaddr_data, void **, OUT
			指向用于储存“事件的附加数据的指针（内核态）”的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiPopMessage(	IN int32 tid,
				IN int32 wid,
				OUT int32 * cid,
				OUT uint32 * type,
				OUT void ** data,
				OUT void ** phyaddr_data)
{
	_WINSTANCE_FALSE
	if(winstance->messages->count == 0)
		return FALSE;
	DSLLinkedListNodePtr node = dsl_lnklst_shift_node(winstance->messages);
	if(node == NULL)
		return FALSE;
	WindowMessagePtr msg = node->value.value.object_value;
	if(cid != NULL)
		*cid = msg->cid;
	if(type != NULL)
		*type = msg->type;
	if(data != NULL)
		*data = msg->data;
	if(phyaddr_data != NULL)
		*phyaddr_data = msg->phyaddr_data;
	DELETE(msg);
	DELETE(node);
	return TRUE;
}

/**
	@Function:		GuiSetText
	@Access:		Public
	@Description:
		设置控件的文本。
		该函数支持的控件为：
		* BUTTON
		* TEXT
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
		text, CASCTEXT, IN
			文本。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiSetText(	IN int32 tid,
			IN int32 wid,
			IN int32 cid,
			IN CASCTEXT text)
{
	_WINSTANCE_FALSE
	if(text == NULL)
		return FALSE;
	_CONTROL_FALSE
	switch(control->type)
	{
		case CONTROL_BUTTON:
		{
			if(strlen(text) > MAX_BUTTON_TEXT_LEN)
				return FALSE;
			ButtonPtr button = (ButtonPtr)control;
			SET_BUTTON_TEXT(button, text);
			break;
		}
		case CONTROL_LABEL:
		{
			if(strlen(text) > MAX_LABEL_TEXT_LEN)
				return FALSE;
			LabelPtr label = (LabelPtr)control;
			SET_LABEL_TEXT(label, text);
			break;
		}
		case CONTROL_EDIT:
		{
			EditPtr edit = (EditPtr)control;
			SET_EDIT_TEXT(edit, text);
			break;
		}
		default:
			return FALSE;
	}
	return TRUE;
}

/**
	@Function:		GuiGetText
	@Access:		Public
	@Description:
		获取控件的文本。
		该函数支持的控件为：
		* BUTTON
		* TEXT
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
		text, ASCTEXT, OUT
			指向用于储存文本的缓冲区的指针。
		bufsz, uint32, IN
			缓冲区大小。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiGetText(	IN int32 tid,
			IN int32 wid,
			IN int32 cid,
			OUT ASCTEXT text,
			IN uint32 bufsz)
{
	_WINSTANCE_FALSE
	if(text == NULL)
		return FALSE;
	_CONTROL_FALSE
	CASCTEXT ctl_text = NULL;
	switch(control->type)
	{
		case CONTROL_BUTTON:
		{
			ButtonPtr button = (ButtonPtr)control;
			ctl_text = button->text;
			break;
		}
		case CONTROL_LABEL:
		{
			LabelPtr label = (LabelPtr)control;
			ctl_text = label->text;
			break;
		}
		case CONTROL_EDIT:
		{
			EditPtr edit = (EditPtr)control;
			return GET_EDIT_TEXT(edit, text, bufsz);
		}
		default:
			return FALSE;
	}
	if(ctl_text != NULL)
		if(strlen(ctl_text) < bufsz)
			UtlCopyString(text, bufsz, ctl_text);
		else
		{
			UtlCopyMemory(text, bufsz, ctl_text, bufsz - 1);
			text[bufsz - 1] = '\0';
		}
	else
		return FALSE;
	return TRUE;
}

/**
	@Function:		GuiUpdate
	@Access:		Public
	@Description:
		向窗体发送绘制事件。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiUpdate(	IN int32 tid,
			IN int32 wid)
{
	_WINSTANCE_FALSE
	WinmgrDispatchEvent(winstance->window, WINDOW_EVENT_PAINT, NULL);
	return TRUE;
}

/**
	@Function:		GuiRenderTextBuffer
	@Access:		Public
	@Description:
		渲染文本缓冲区。
		把指定的文本缓冲区的内容显示在指定窗体上。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		txtbuf, uint8 *, IN
			指向文本缓冲区的指针。
		row, uint32, IN
			行。
		column, uint32, IN
			列。
		curx, uint32, IN
			光标当前X坐标。
		cury, uint32, IN
			光标当前Y坐标。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiRenderTextBuffer(IN int32 tid,
					IN int32 wid,
					IN uint8 * txtbuf,
					IN uint32 row,
					IN uint32 column,
					IN uint32 curx,
					IN uint32 cury)
{
	_WINSTANCE_FALSE
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	// 这里渲染文本缓冲区不lock()/unlock()的话会，窗体工作区闪烁。
	// 即使设定了渲染期间不发送WINDOW_EVENT_PAINT，但是不起作用，原因不明。
	// 
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	lock();
	winstance->window->locked = TRUE;
	ScrRenderTextBuffer(	&winstance->window->workspace,
						txtbuf,
						row,
						column,
						curx,
						cury);
	winstance->window->locked = FALSE;
	unlock();
	return TRUE;
}

/**
	@Function:		GuiFreeMessageData
	@Access:		Public
	@Description:
		释放事件附加数据。
	@Parameters:
		tid, int32, IN
			任务ID。
		data, void *, IN
			指向事件的附加数据的指针，用户态时可访问。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
void
GuiFreeMessageData(	IN int32 tid,
					IN void * data)
{
	if(data == NULL)
		return;
	void * phyaddr_data = TaskmgrConvertLAddrToPAddr(tid, data);
	if(phyaddr_data == NULL)
		return;
	MemFree(phyaddr_data);
}

/**
	@Function:		GuiEnableControl
	@Access:		Public
	@Description:
		启用控件。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiEnableControl(	IN int32 tid,
					IN int32 wid,
					IN int32 cid)
{
	_WINSTANCE_FALSE
	_CONTROL_FALSE
	switch(control->type)
	{
		case CONTROL_BUTTON:
			ENABLE_BUTTON((ButtonPtr)control);
			break;
		case CONTROL_LABEL:
			ENABLE_LABEL((LabelPtr)control);
			break;
		case CONTROL_LIST:
			ENABLE_LIST((ListPtr)control);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

/**
	@Function:		GuiDisableControl
	@Access:		Public
	@Description:
		禁用控件。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiDisableControl(	IN int32 tid,
					IN int32 wid,
					IN int32 cid)
{
	_WINSTANCE_FALSE
	_CONTROL_FALSE
	switch(control->type)
	{
		case CONTROL_BUTTON:
			DISABLE_BUTTON((ButtonPtr)control);
			break;
		case CONTROL_LABEL:
			DISABLE_LABEL((LabelPtr)control);
			break;
		case CONTROL_LIST:
			DISABLE_LIST((ListPtr)control);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

/**
	@Function:		GuiGetWidth
	@Access:		Public
	@Description:
		获取控件宽度。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
		width, uint32 *, OUT
			指向用于储存宽度的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiGetWidth(IN int32 tid,
			IN int32 wid,
			IN int32 cid,
			OUT uint32 * width)
{
	if(width == NULL)
		return FALSE;
	_WINSTANCE_FALSE
	_CONTROL_FALSE
	*width = 0;
	*width = GET_CONTROL_WIDTH(control);
	return TRUE;
}

/**
	@Function:		GuiGetHeight
	@Access:		Public
	@Description:
		获取控件高度。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
		height, uint32 *, OUT
			指向用于储存高度的缓冲区的指针。
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
GuiGetHeight(	IN int32 tid,
				IN int32 wid,
				IN int32 cid,
				OUT uint32 * height)
{
	if(height == NULL)
		return FALSE;
	_WINSTANCE_FALSE
	_CONTROL_FALSE
	*height = 0;
	*height = GET_CONTROL_HEIGHT(control);
	return TRUE;
}

/**
	@Function:		_GuiNoDataEvent
	@Access:		Public
	@Description:
		无附加数据的控件事件。
		使用了该控件事件函数的控件有：
		* BUTTON
		* LABEL
	@Parameters:
		id, uint32, IN
			引发事件的控件的ID。
		type, uint32, IN
			事件类型。
		param, void *, IN
			指向事件附加参数的指针。
			理论上永远为NULL。
	@Return:
*/
static
void
_GuiNoDataEvent(IN uint32 id,
				IN uint32 type,
				IN void * param)
{
	ControlPtr ctrl = (ControlPtr)id;
	if(	ctrl->type != CONTROL_BUTTON
		&& ctrl->type != CONTROL_LABEL)
		return;
	int32 wid = ctrl->uwid;
	int32 cid = ctrl->uwcid;
	DSLValuePtr v = dsl_lst_get(_winstances, wid);
	WindowInstancePtr winstance = (WindowInstancePtr)DSL_OBJECTVAL(v);
	int32 tid = winstance->tid;
	GuiPushMessage(tid, wid, cid, type, NULL, NULL);
}

/**
	@Function:		_GuiHasDataEvent
	@Access:		Public
	@Description:
		有附加数据的控件事件。
		使用了该控件事件函数的控件有：
		* LIST
	@Parameters:
		id, uint32, IN
			引发事件的控件的ID。
		type, uint32, IN
			事件类型。
		param, void *, IN
			指向事件附加参数的指针。
	@Return:
*/
static
void
_GuiHasDataEvent(	IN uint32 id,
					IN uint32 type,
					IN void * param)
{
	ControlPtr ctrl = (ControlPtr)id;
	if(ctrl->type != CONTROL_LIST)
		return;
	int32 wid = ctrl->uwid;
	int32 cid = ctrl->uwcid;
	DSLValuePtr v = dsl_lst_get(_winstances, wid);
	WindowInstancePtr winstance = (WindowInstancePtr)DSL_OBJECTVAL(v);
	int32 tid = winstance->tid;
	void * data = NULL;
	void * phyaddr_data = NULL;
	switch(ctrl->type)
	{
		case CONTROL_LIST:
		{
			uint32 index = *(uint32 *)param;
			data = TaskmgrAllocMemory(tid, sizeof(index), &phyaddr_data);
			memcpy(phyaddr_data, &index, sizeof(index));
			break;
		}
		default:
			return;
	}
	GuiPushMessage(tid, wid, cid, type, data, phyaddr_data);
}

/**
	@Function:		GuiNewButton
	@Access:		Public
	@Description:
		新建一个BUTTON。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		text, CASCTEXT, IN
			文本。
		cid, int32 *, OUT
			指向用于储存控件ID的缓冲区的指针。
	@Return:
		返回TRUE则成功，否则失败。
*/
BOOL
GuiNewButton(	IN int32 tid,
				IN int32 wid,
				IN int32 x,
				IN int32 y,
				IN CASCTEXT text,
				OUT int32 * cid)
{
	_WINSTANCE_FALSE
	lock();
	ButtonPtr btn = NULL;
	DSLValuePtr val = NULL;
	if(cid == NULL)
		goto err;
	btn = NEW(Button);
	if(btn == NULL)
		goto err;
	INIT_BUTTON(btn, x, y, text, _GuiNoDataEvent);
	btn->uwid = wid;
	val = dsl_val_object(btn);
	if(val == NULL)
		goto err;
	int32 _cid = dsl_lst_find_value(winstance->controls, NULL);
	if(_cid == -1)
	{
		if(!dsl_lst_add_value(winstance->controls, val))
			goto err;
		_cid = dsl_lst_find_value(winstance->controls, val);
	}
	else
		dsl_lst_set(winstance->controls, _cid, val);
	btn->uwcid = _cid;
	*cid = _cid;
	unlock();
	return TRUE;
err:
	if(btn != NULL)
		DELETE(btn);
	if(val != NULL)
		DELETE(val);
	unlock();
	return FALSE;
}

/**
	@Function:		GuiNewLabel
	@Access:		Public
	@Description:
		新建一个LABEL。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		text, CASCTEXT, IN
			文本。
		cid, int32 *, OUT
			指向用于储存控件ID的缓冲区的指针。
	@Return:
		返回TRUE则成功，否则失败。
*/
BOOL
GuiNewLabel(IN int32 tid,
			IN int32 wid,
			IN int32 x,
			IN int32 y,
			IN CASCTEXT text,
			OUT int32 * cid)
{
	_WINSTANCE_FALSE
	lock();
	LabelPtr lbl = NULL;
	DSLValuePtr val = NULL;
	if(cid == NULL)
		goto err;
	lbl = NEW(Label);
	if(lbl == NULL)
		goto err;
	INIT_LABEL(lbl, x, y, text, _GuiNoDataEvent);
	lbl->uwid = wid;
	val = dsl_val_object(lbl);
	if(val == NULL)
		goto err;
	int32 _cid = dsl_lst_find_value(winstance->controls, NULL);
	if(_cid == -1)
	{
		if(!dsl_lst_add_value(winstance->controls, val))
			goto err;
		_cid = dsl_lst_find_value(winstance->controls, val);
	}
	else
		dsl_lst_set(winstance->controls, _cid, val);
	lbl->uwcid = _cid;
	*cid = _cid;
	unlock();
	return TRUE;
err:
	if(lbl != NULL)
		DELETE(lbl);
	if(val != NULL)
		DELETE(val);
	unlock();
	return FALSE;
}

/**
	@Function:		GuiNewList
	@Access:		Public
	@Description:
		新建一个LIST。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		count, uint32, IN
			列表项的数量。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		text, CASCTEXT, IN
			所有列表项的初始化文本。
		cid, int32 *, OUT
			指向用于储存控件ID的缓冲区的指针。
	@Return:
		返回TRUE则成功，否则失败。
*/
BOOL
GuiNewList(	IN int32 tid,
			IN int32 wid,
			IN uint32 count,
			IN int32 x,
			IN int32 y,
			IN CASCTEXT text,
			OUT int32 * cid)
{
	_WINSTANCE_FALSE
	lock();
	ListPtr lst = NULL;
	DSLValuePtr val = NULL;
	if(cid == NULL)
		goto err;
	lst = NEW(List);
	if(lst == NULL)
		goto err;
	INIT_LIST(lst, count, x, y, text, _GuiHasDataEvent);
	lst->uwid = wid;
	val = dsl_val_object(lst);
	if(val == NULL)
		goto err;
	int32 _cid = dsl_lst_find_value(winstance->controls, NULL);
	if(_cid == -1)
	{
		if(!dsl_lst_add_value(winstance->controls, val))
			goto err;
		_cid = dsl_lst_find_value(winstance->controls, val);
	}
	else
		dsl_lst_set(winstance->controls, _cid, val);
	lst->uwcid = _cid;
	*cid = _cid;
	unlock();
	return TRUE;
err:
	if(lst != NULL)
		DELETE(lst);
	if(val != NULL)
		DELETE(val);
	unlock();
	return FALSE;
}

/**
	@Function:		GuiSetListText
	@Access:		Public
	@Description:
		设置LIST的列表项的文本。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
		index, uint32, IN
			列表项的索引。
		text, CASCTEXT, IN
			文本。
	@Return:
		返回TRUE则成功，否则失败。
*/
BOOL
GuiSetListText(	IN int32 tid,
				IN int32 wid,
				IN int32 cid,
				IN uint32 index,
				IN CASCTEXT text)
{
	_WINSTANCE_FALSE
	if(text == NULL)
		return FALSE;
	DSLValuePtr v = dsl_lst_get(winstance->controls, cid);
	ControlPtr control = (ControlPtr)DSL_OBJECTVAL(v);
	if(control->type != CONTROL_LIST)
		return FALSE;
	return SET_LIST_TEXT(control, index, text);
}

/**
	@Function:		GuiEnableListItem
	@Access:		Public
	@Description:
		启用列表项。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
		index, uint32, IN
			列表项的索引。
	@Return:
		返回TRUE则成功，否则失败。
*/
BOOL
GuiEnableListItem(	IN int32 tid,
					IN int32 wid,
					IN int32 cid,
					IN uint32 index)
{
	_WINSTANCE_FALSE
	_CONTROL_FALSE
	return CtrlListEnableItem((ListPtr)control, index);
}

/**
	@Function:		GuiDisableListItem
	@Access:		Public
	@Description:
		禁用列表项。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		cid, int32, IN
			控件ID。
		index, uint32, IN
			列表项的索引。
	@Return:
		返回TRUE则成功，否则失败。
*/
BOOL
GuiDisableListItem(	IN int32 tid,
					IN int32 wid,
					IN int32 cid,
					IN uint32 index)
{
	_WINSTANCE_FALSE
	_CONTROL_FALSE
	return CtrlListDisableItem((ListPtr)control, index);
}

/**
	@Function:		GuiNewEdit
	@Access:		Public
	@Description:
		新建一个EDIT。
	@Parameters:
		tid, int32, IN
			任务ID。
		wid, int32, IN
			窗体ID。
		x, int32, IN
			X坐标。
		y, int32, IN
			Y坐标。
		row, uint32, IN
			行数。
		column, uint32, IN
			列数。
		text, CASCTEXT, IN
			文本。
		style, uint32, IN
			样式。
		cid, int32 *, OUT
			指向用于储存控件ID的缓冲区的指针。
	@Return:
		返回TRUE则成功，否则失败。
*/
BOOL
GuiNewEdit(	IN int32 tid,
			IN int32 wid,
			IN int32 x,
			IN int32 y,
			IN uint32 row,
			IN uint32 column,
			IN CASCTEXT text,
			IN uint32 style,
			OUT int32 * cid)
{
	_WINSTANCE_FALSE
	lock();
	EditPtr edit = NULL;
	DSLValuePtr val = NULL;
	if(cid == NULL)
		goto err;
	edit = NEW(Edit);
	if(edit == NULL)
		goto err;
	INIT_EDIT(edit, x, y, row, column, style, _GuiNoDataEvent);
	SET_EDIT_TEXT(edit, text);
	edit->uwid = wid;
	val = dsl_val_object(edit);
	if(val == NULL)
		goto err;
	int32 _cid = dsl_lst_find_value(winstance->controls, NULL);
	if(_cid == -1)
	{
		if(!dsl_lst_add_value(winstance->controls, val))
			goto err;
		_cid = dsl_lst_find_value(winstance->controls, val);
	}
	else
		dsl_lst_set(winstance->controls, _cid, val);
	edit->uwcid = _cid;
	*cid = _cid;
	unlock();
	return TRUE;
err:
	if(edit != NULL)
		DELETE(edit);
	if(val != NULL)
		DELETE(val);
	unlock();
	return FALSE;
}

BOOL
GuiNewFileList(	IN int32 tid,
				IN int32 wid,
				IN uint32 x,
				IN uint32 y,
				IN uint32 width,
				IN uint32 height,
				IN uint32 vscroll_width,
				IN uint32 color,
				IN uint32 bgcolor,
				IN uint32 colorh,
				IN uint32 bgcolorh,
				OUT int32 * cid)
{
	_WINSTANCE_FALSE
	lock();
	FileListPtr file_list = NULL;
	DSLValuePtr val = NULL;
	if(cid == NULL)
		goto err;
	file_list = NEW(FileList);
	if(file_list == NULL)
		goto err;
	CtrlFileListInit(
		file_list,
		0,
		x, y,
		width, height,
		vscroll_width,
		color, bgcolor,
		colorh, bgcolorh,
		_GuiNoDataEvent
	);


	// TODO: 临时代码
	CtrlFileListSetPath(file_list, "SA:/");
	CtrlFileListSetTop(file_list, 0);


	file_list->uwid = wid;
	val = dsl_val_object(file_list);
	if(val == NULL)
		goto err;
	int32 _cid = dsl_lst_find_value(winstance->controls, NULL);
	if(_cid == -1)
	{
		if(!dsl_lst_add_value(winstance->controls, val))
			goto err;
		_cid = dsl_lst_find_value(winstance->controls, val);
	}
	else
		dsl_lst_set(winstance->controls, _cid, val);
	file_list->uwcid = _cid;
	*cid = _cid;	
	unlock();
	return TRUE;
err:
	if(file_list != NULL)
		DELETE(file_list);
	if(val != NULL)
		DELETE(val);
	unlock();
	return FALSE;
}
