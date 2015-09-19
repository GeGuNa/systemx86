/**
	@File:			taskbar.c
	@Author:		Ihsoh
	@Date:			2015-09-19
	@Description:
		任务栏。
*/

#include "taskbar.h"
#include "../types.h"
#include "../screen.h"
#include "../cmos.h"
#include "../image.h"
#include "../enfont.h"
#include "../window.h"
#include "../vesa.h"
#include "../memory.h"

#include "start.h";

#include <ilib/string.h>

#include "../window/control.h"
#include "../window/button.h"
#include "../window/label.h"

#define	_HEIGHT	24

#define	_BTN_TIME_WIDTH		250

static WindowPtr _window	= NULL;

static ButtonPtr btn_start	= NULL;
static ButtonPtr btn_time	= NULL;

static int8 weeks[][10] 	= {	"Sunday", 
								"Monday", 
								"Tuesday",
								"Wednesday",
								"Thursday",
								"Friday",
								"Saturday"};

/**
	@Function:		_control_event
	@Access:		Private
	@Description:
		控件的事件函数。
	@Parameters:
		id, uint32, IN
			控件ID。
		type, uint32, IN
			事件类型。
		param, void *, IN
			事件附带的参数。
	@Return:
*/
static
void
_control_event(	IN uint32 id,
				IN uint32 type,
				IN void * param)
{
	if(id == btn_start->id)
	{
		if(type == BUTTON_LBUP)
		{
			start_window_show(_window->x, _window->y);
		}
	}
}

/**
	@Function:		_window_event
	@Access:		Private
	@Description:
		窗体的事件函数。
	@Parameters:
		window, struct Window *, IN
			指向窗体对象的指针。
		params, struct WindowEventParams *, IN
			指向窗体事件参数对象的指针。
	@Return:
*/
static
void
_window_event(	IN struct Window * window,
				IN struct WindowEventParams * params)
{
	struct CMOSDateTime dt;
	get_cmos_date_time(&dt);
	ASCCHAR buffer[1024] = "";
	ASCCHAR temp[10];
	strcat(buffer, itos(temp, dt.year));
	strcat(buffer, "-");
	strcat(buffer, itos(temp, dt.month));
	strcat(buffer, "-");
	strcat(buffer, itos(temp, dt.day));
	strcat(buffer, " ");
	strcat(buffer, weeks[dt.day_of_week]);
	strcat(buffer, " ");
	strcat(buffer, itos(temp, dt.hour));
	strcat(buffer, ":");
	strcat(buffer, itos(temp, dt.minute));
	strcat(buffer, ":");
	strcat(buffer, itos(temp, dt.second));
	SET_BUTTON_TEXT(btn_time, buffer);

	BOOL top = get_top_window() == window;
	BUTTON(btn_start, &window->workspace, params, top);
	BUTTON(btn_time, &window->workspace, params, top);
}

/**
	@Function:		taskbar_window_init
	@Access:		Public
	@Description:
		初始化任务栏。
	@Parameters:
	@Return:
		BOOL
			返回TRUE则成功，否则失败。
*/
BOOL
taskbar_window_init(void)
{
	uint32 w = vesa_get_width();
	uint32 h = vesa_get_height();
	_window = create_window(w, _HEIGHT,
							0xff000000,
							WINDOW_STYLE_NO_TITLE,
							"Taskbar",
							_window_event);
	rect_common_image(&_window->workspace, 0, 0, w, _HEIGHT, 0xff222222);
	_window->y = h - _HEIGHT;

	btn_start = NEW(Button);
	button_init(btn_start, 0,
				0, 0,
				"ISystem",
				0xffffffff, 0xff222222, 0xffffffff, 0xff444444,
				_control_event,
				0, _HEIGHT);
	btn_start->style = BUTTON_STYLE_REFRESH;
	btn_time = NEW(Button);
	button_init(btn_time, 0,
				w - _BTN_TIME_WIDTH, 0,
				"Time",
				0xffffffff, 0xff222222, 0xffffffff, 0xff444444,
				_control_event,
				_BTN_TIME_WIDTH, 0);
	btn_time->style = BUTTON_STYLE_REFRESH;

	_window->state = WINDOW_STATE_SHOW;
	return TRUE;
}
