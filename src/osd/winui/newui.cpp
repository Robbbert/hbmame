// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************
//============================================================
//
//  newui.c - This is the NEWUI Windows dropdown menu system
//
//============================================================

// Needed for better file dialog
//#ifdef _WIN32_WINNT
//#undef _WIN32_WINNT
//#endif // _WIN32_WINNT
//#define _WIN32_WINNT 0x500


// standard windows headers
#define WIN32_LEAN_AND_MEAN
#include "newui.h"

// stupid hack; not sure why this is needed
//#ifdef const
//#undef const
//#endif // const
#include <shellapi.h>


enum _win_file_dialog_type
{
	WIN_FILE_DIALOG_OPEN = 1,
	WIN_FILE_DIALOG_SAVE
};
typedef enum _win_file_dialog_type win_file_dialog_type;

typedef struct _win_open_file_name win_open_file_name;
struct _win_open_file_name
{
	win_file_dialog_type	type;			// type of file dialog
	HWND			owner;			// owner of the dialog
	HINSTANCE		instance;		// instance
	const char *		filter;			// pipe char ("|") delimited strings
	DWORD			filter_index;		// index into filter
	char			filename[MAX_PATH];	// filename buffer
	const char *		initial_directory;	// initial directory for dialog
	DWORD			flags;			// standard flags
	LPARAM			custom_data;		// custom data for dialog hook
	LPOFNHOOKPROC		hook;			// custom dialog hook
	LPCTSTR			template_name;		// custom dialog template
};


typedef struct _dialog_box dialog_box;

struct dialog_layout
{
	short label_width;
	short combo_width;
};

typedef void (*dialog_itemstoreval)(void *param, int val);
typedef void (*dialog_itemchangedproc)(dialog_box *dialog, HWND dlgitem, void *changed_param);
typedef void (*dialog_notification)(dialog_box *dialog, HWND dlgwnd, NMHDR *notification, void *param);

#ifdef UNICODE
#define win_dialog_tcsdup	win_dialog_wcsdup
#else
#define win_dialog_tcsdup	win_dialog_strdup
#endif

static const TCHAR guide_prop[] = TEXT("opcntrl_guide");
static const TCHAR spec_prop[] = TEXT("opcntrl_optspec");
static const TCHAR value_prop[] = TEXT("opcntrl_value");

#define SEQWM_SETFOCUS	(WM_APP + 0)
#define SEQWM_KILLFOCUS	(WM_APP + 1)

enum
{
	TRIGGER_INITDIALOG	= 1,
	TRIGGER_APPLY		= 2,
	TRIGGER_CHANGED		= 4
};

typedef LRESULT (*trigger_function)(dialog_box *dialog, HWND dlgwnd, UINT message, WPARAM wparam, LPARAM lparam);

struct dialog_info_trigger
{
	struct dialog_info_trigger *next;
	WORD dialog_item;
	WORD trigger_flags;
	UINT message;
	WPARAM wparam;
	LPARAM lparam;
	void (*storeval)(void *param, int val);
	void *storeval_param;
	trigger_function trigger_proc;
};

struct dialog_object_pool
{
	HGDIOBJ objects[16];
};

struct _dialog_box
{
	HGLOBAL handle;
	size_t handle_size;
	struct dialog_info_trigger *trigger_first;
	struct dialog_info_trigger *trigger_last;
	WORD item_count;
	WORD size_x, size_y;
	WORD pos_x, pos_y;
	WORD cursize_x, cursize_y;
	WORD home_y;
	DWORD style;
	int combo_string_count;
	int combo_default_value;
	//object_pool *mempool;
	struct dialog_object_pool *objpool;
	const struct dialog_layout *layout;

	// singular notification callback; hack
	UINT notify_code;
	dialog_notification notify_callback;
	void *notify_param;
};

enum _seqselect_state
{
	SEQSELECT_STATE_NOT_POLLING,
	SEQSELECT_STATE_POLLING,
	SEQSELECT_STATE_POLLING_COMPLETE
};
typedef enum _seqselect_state seqselect_state;

// this is the structure that gets associated with each input_seq edit box
typedef struct _seqselect_info seqselect_info;
struct _seqselect_info
{
	WNDPROC oldwndproc;
	ioport_field *field;		// pointer to the field
	ioport_field::user_settings settings;		// the new settings
	input_seq *code;				// the input_seq within settings
	WORD pos;
	BOOL is_analog;
	seqselect_state poll_state;
};



//============================================================
//  PARAMETERS
//============================================================

#define DIM_VERTICAL_SPACING	3
#define DIM_HORIZONTAL_SPACING	5
#define DIM_NORMAL_ROW_HEIGHT	10
#define DIM_COMBO_ROW_HEIGHT	12
#define DIM_SLIDER_ROW_HEIGHT	18
#define DIM_BUTTON_ROW_HEIGHT	12
#define DIM_EDIT_WIDTH		120
#define DIM_BUTTON_WIDTH	50
#define DIM_ADJUSTER_SCR_WIDTH	12
#define DIM_ADJUSTER_HEIGHT	12
#define DIM_SCROLLBAR_WIDTH	10
#define DIM_BOX_VERTSKEW	-3

#define DLGITEM_BUTTON		((const WCHAR *) dlgitem_button)
#define DLGITEM_EDIT		((const WCHAR *) dlgitem_edit)
#define DLGITEM_STATIC		((const WCHAR *) dlgitem_static)
#define DLGITEM_LISTBOX		((const WCHAR *) dlgitem_listbox)
#define DLGITEM_SCROLLBAR	((const WCHAR *) dlgitem_scrollbar)
#define DLGITEM_COMBOBOX	((const WCHAR *) dlgitem_combobox)

#define DLGTEXT_OK		"OK"
#define DLGTEXT_APPLY		"Apply"
#define DLGTEXT_CANCEL		"Cancel"

#define FONT_SIZE		8
#define FONT_FACE		L"Arial"

#define SCROLL_DELTA_LINE	10
#define SCROLL_DELTA_PAGE	100

#define LOG_WINMSGS		0
#define DIALOG_STYLE		WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION | DS_SETFONT
#define MAX_DIALOG_HEIGHT	200



//============================================================
//  LOCAL VARIABLES
//============================================================

static running_machine *Machine;         // HACK - please fix

static double pixels_to_xdlgunits;
static double pixels_to_ydlgunits;

static const struct dialog_layout default_layout = { 80, 140 };
static const WORD dlgitem_button[] =	{ 0xFFFF, 0x0080 };
static const WORD dlgitem_edit[] =	{ 0xFFFF, 0x0081 };
static const WORD dlgitem_static[] =	{ 0xFFFF, 0x0082 };
static const WORD dlgitem_listbox[] =	{ 0xFFFF, 0x0083 };
static const WORD dlgitem_scrollbar[] =	{ 0xFFFF, 0x0084 };
static const WORD dlgitem_combobox[] =	{ 0xFFFF, 0x0085 };


//============================================================
//  PROTOTYPES
//============================================================

static void dialog_prime(dialog_box *di);
static int dialog_write_item(dialog_box *di, DWORD style, short x, short y,
	 short width, short height, const char *str, const WCHAR *class_name, WORD *id);



//============================================================
//  PARAMETERS
//============================================================

#define ID_FRAMESKIP_0		10000
#define ID_DEVICE_0		11000
#define ID_JOYSTICK_0		12000
#define ID_INPUT_0		13000
#define ID_VIDEO_VIEW_0		14000

#define MAX_JOYSTICKS		(8)

enum
{
	DEVOPTION_OPEN,
	DEVOPTION_CREATE,
	DEVOPTION_CLOSE,
	DEVOPTION_CASSETTE_PLAYRECORD,
	DEVOPTION_CASSETTE_STOPPAUSE,
	DEVOPTION_CASSETTE_PLAY,
	DEVOPTION_CASSETTE_RECORD,
	DEVOPTION_CASSETTE_REWIND,
	DEVOPTION_CASSETTE_FASTFORWARD,
	DEVOPTION_MAX
};

#ifdef MAME_PROFILER
#define HAS_PROFILER	1
#else
#define HAS_PROFILER	0
#endif

//============================================================
//  LOCAL VARIABLES
//============================================================

static int joystick_menu_setup = 0;
static char state_filename[MAX_PATH];
static int add_filter_entry(char *dest, size_t dest_len, const char *description, const char *extensions);
static const char* software_dir;

struct file_dialog_params
{
	device_image_interface *dev;
	int *create_format;
	option_resolution **create_args;
};





//============================================================
//  win_get_file_name_dialog - sanitize all of the ugliness
//  in invoking GetOpenFileName() and GetSaveFileName()
//============================================================

static BOOL win_get_file_name_dialog(win_open_file_name *ofn)
{
	BOOL result = FALSE;
	BOOL dialog_result;
	OSVERSIONINFO vers;
	OPENFILENAME os_ofn;
	DWORD os_ofn_size;
	LPTSTR t_filter = NULL;
	LPTSTR t_file = NULL;
	DWORD t_file_size = 0;
	LPTSTR t_initial_directory = NULL;
	LPTSTR buffer;
	char *utf8_file;
	int i;

	// determine the version of Windows
	memset(&vers, 0, sizeof(vers));
	vers.dwOSVersionInfoSize = sizeof(vers);
	GetVersionEx(&vers);

	// based on the version of Windows, determine the correct struct size
	if (vers.dwMajorVersion >= 5)
		os_ofn_size = sizeof(os_ofn);
	else
#ifdef __GNUC__
		os_ofn_size = 76;	// MinGW does not define OPENFILENAME_NT4
#else
		os_ofn_size = sizeof(OPENFILENAME_NT4);
#endif

	// do we have to translate the filter?
	if (ofn->filter)
	{
		buffer = tstring_from_utf8(ofn->filter);
		if (!buffer)
			goto done;

		// convert a pipe-char delimited string into a NUL delimited string
		t_filter = (LPTSTR) alloca((_tcslen(buffer) + 2) * sizeof(*t_filter));
		for (i = 0; buffer[i] != '\0'; i++)
			t_filter[i] = (buffer[i] != '|') ? buffer[i] : '\0';
		t_filter[i++] = '\0';
		t_filter[i++] = '\0';
		osd_free(buffer);
	}

	// do we need to translate the file parameter?
	if (ofn->filename)
	{
		buffer = tstring_from_utf8(ofn->filename);
		if (!buffer)
			goto done;

		t_file_size = MAX(_tcslen(buffer) + 1, MAX_PATH);
		t_file = (LPTSTR) alloca(t_file_size * sizeof(*t_file));
		_tcscpy(t_file, buffer);
		osd_free(buffer);
	}

	// do we need to translate the initial directory?
	if (ofn->initial_directory)
	{
		t_initial_directory = tstring_from_utf8(ofn->initial_directory);
		if (t_initial_directory == NULL)
			goto done;
	}

	// translate our custom structure to a native Win32 structure
	memset(&os_ofn, 0, sizeof(os_ofn));
	os_ofn.lStructSize = os_ofn_size;
	os_ofn.hwndOwner = ofn->owner;
	os_ofn.hInstance = ofn->instance;
	os_ofn.lpstrFilter = t_filter;
	os_ofn.nFilterIndex = ofn->filter_index;
	os_ofn.lpstrFile = t_file;
	os_ofn.nMaxFile = t_file_size;
	os_ofn.lpstrInitialDir = t_initial_directory;
	os_ofn.Flags = ofn->flags;
	os_ofn.lCustData = ofn->custom_data;
	os_ofn.lpfnHook = ofn->hook;
	os_ofn.lpTemplateName = ofn->template_name;

	// invoke the correct Win32 call
	switch(ofn->type)
	{
		case WIN_FILE_DIALOG_OPEN:
			dialog_result = GetOpenFileName(&os_ofn);
			break;

		case WIN_FILE_DIALOG_SAVE:
			dialog_result = GetSaveFileName(&os_ofn);
			break;

		default:
			// should not reach here
			dialog_result = FALSE;
			break;
	}

	// copy data out
	ofn->filter_index = os_ofn.nFilterIndex;
	ofn->flags = os_ofn.Flags;

	// copy file back out into passed structure
	if (t_file)
	{
		utf8_file = utf8_from_tstring(t_file);
		if (!utf8_file)
			goto done;

		snprintf(ofn->filename, ARRAY_LENGTH(ofn->filename), "%s", utf8_file);
		osd_free(utf8_file);
	}

	// we've completed the process
	result = dialog_result;

done:
	if (t_initial_directory)
		osd_free(t_initial_directory);
	return result;
}



//============================================================
//  win_scroll_window
//============================================================

static void win_scroll_window(HWND window, WPARAM wparam, int scroll_bar, int scroll_delta_line)
{
	SCROLLINFO si;
	int scroll_pos;

	// retrieve vital info about the scroll bar
	memset(&si, 0, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	GetScrollInfo(window, scroll_bar, &si);

	scroll_pos = si.nPos;

	// change scroll_pos in accordance with this message
	switch(LOWORD(wparam))
	{
		case SB_BOTTOM:
			scroll_pos = si.nMax;
			break;
		case SB_LINEDOWN:
			scroll_pos += scroll_delta_line;
			break;
		case SB_LINEUP:
			scroll_pos -= scroll_delta_line;
			break;
		case SB_PAGEDOWN:
			scroll_pos += scroll_delta_line;
			break;
		case SB_PAGEUP:
			scroll_pos -= scroll_delta_line;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			scroll_pos = HIWORD(wparam);
			break;
		case SB_TOP:
			scroll_pos = si.nMin;
			break;
	}

	// max out the scroll bar value
	if (scroll_pos < si.nMin)
		scroll_pos = si.nMin;
	else if (scroll_pos > (si.nMax - si.nPage))
		scroll_pos = (si.nMax - si.nPage);

	// if the value changed, set the scroll position
	if (scroll_pos != si.nPos)
	{
		SetScrollPos(window, scroll_bar, scroll_pos, TRUE);
		ScrollWindowEx(window, 0, si.nPos - scroll_pos, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
	}
}



//============================================================
//  win_append_menu_utf8
//============================================================

static BOOL win_append_menu_utf8(HMENU menu, UINT flags, UINT_PTR id, const char *item)
{
	BOOL result;
	const TCHAR *t_item = (const TCHAR*)item;
	TCHAR *t_str = NULL;

	// only convert string when it's not a bitmap
	if (!(flags & MF_BITMAP) && item)
	{
		t_str = tstring_from_utf8(item);
		t_item = t_str;
	}

	result = AppendMenu(menu, flags, id, t_item);

	if (t_str)
		osd_free(t_str);

	return result;
}


static int get_option_count(const option_guide *guide, const char *optspec)
{
	struct OptionRange ranges[128];
	int count = 0, i;

	option_resolution_listranges(optspec, guide->parameter, ranges, ARRAY_LENGTH(ranges));

	for (i = 0; ranges[i].min >= 0; i++)
		count += ranges[i].max - ranges[i].min + 1;

	return count;
}



static BOOL prepare_combobox(HWND control, const option_guide *guide, const char *optspec)
{
	struct OptionRange ranges[128];
	int default_value, default_index, current_index, option_count;
	int i, j, k;
	BOOL has_option;
	TCHAR buf1[256];
	TCHAR buf2[256];
	LPTSTR tempstr;

	SendMessage(control, CB_GETLBTEXT, SendMessage(control, CB_GETCURSEL, 0, 0), (LPARAM) buf1);
	SendMessage(control, CB_RESETCONTENT, 0, 0);
	has_option = guide && optspec;

	if (has_option)
	{
		if ((guide->option_type != OPTIONTYPE_INT) && (guide->option_type != OPTIONTYPE_ENUM_BEGIN))
			goto unexpected;

		option_resolution_listranges(optspec, guide->parameter,
			ranges, ARRAY_LENGTH(ranges));
		option_resolution_getdefault(optspec, guide->parameter, &default_value);

		option_count = 0;
		default_index = -1;
		current_index = -1;

		for (i = 0; ranges[i].min >= 0; i++)
		{
			for (j = ranges[i].min; j <= ranges[i].max; j++)
			{
				if (guide->option_type == OPTIONTYPE_INT)
				{
					_sntprintf(buf2, ARRAY_LENGTH(buf2), TEXT("%d"), j);
					SendMessage(control, CB_ADDSTRING, 0, (LPARAM) buf2);
				}
				else
				if (guide->option_type == OPTIONTYPE_ENUM_BEGIN)
				{
					for (k = 1; guide[k].option_type == OPTIONTYPE_ENUM_VALUE; k++)
					{
						if (guide[k].parameter == j)
							break;
					}
					if (guide[k].option_type != OPTIONTYPE_ENUM_VALUE)
						goto unexpected;

					tempstr = tstring_from_utf8(guide[k].display_name);
					SendMessage(control, CB_ADDSTRING, 0, (LPARAM) tempstr);
					osd_free(tempstr);
				}
				else
					goto unexpected;

				SendMessage(control, CB_SETITEMDATA, option_count, j);

				if (j == default_value)
					default_index = option_count;
				if (!_tcscmp(buf1, buf2))
					current_index = option_count;
				option_count++;
			}
		}

		// if there is only one option, it is effectively disabled
		if (option_count <= 1)
			has_option = FALSE;

		if (current_index >= 0)
			SendMessage(control, CB_SETCURSEL, current_index, 0);
		else if (default_index >= 0)
			SendMessage(control, CB_SETCURSEL, default_index, 0);
	}
	else
	{
		// this item is non applicable
		SendMessage(control, CB_ADDSTRING, 0, (LPARAM) TEXT("N/A"));
		SendMessage(control, CB_SETCURSEL, 0, 0);
	}
	EnableWindow(control, has_option);
	return TRUE;

unexpected:
	printf("Unexpected result in prepare_combobox\n");
	assert(FALSE);
	return FALSE;
}



static BOOL prepare_editbox(HWND control, const option_guide *guide, const char *optspec)
{
	optreserr_t err = OPTIONRESOLUTION_ERROR_SUCCESS;
	char buf[32];
	int val, has_option, option_count;

	has_option = guide && optspec;
	buf[0] = '\0';

	if (has_option)
	{
		switch(guide->option_type)
		{
			case OPTIONTYPE_STRING:
				break;

			case OPTIONTYPE_INT:
				err = option_resolution_getdefault(optspec, guide->parameter, &val);
				if (err)
					goto done;
				_snprintf(buf, ARRAY_LENGTH(buf), "%d", val);
				break;

			default:
				err = OPTIONRESOLTUION_ERROR_INTERNAL;
				goto done;
		}
	}

	if (has_option)
	{
		option_count = get_option_count(guide, optspec);
		if (option_count <= 1)
			has_option = FALSE;
	}

done:
	if (err == OPTIONRESOLTUION_ERROR_INTERNAL)
		printf("Unexpected result in prepare_editbox\n");
	assert(err != OPTIONRESOLTUION_ERROR_INTERNAL);
	win_set_window_text_utf8(control, buf);
	EnableWindow(control, !err && has_option);
	return err == OPTIONRESOLUTION_ERROR_SUCCESS;
}



static BOOL win_prepare_option_control(HWND control, const option_guide *guide, const char *optspec)
{
	BOOL rc = FALSE;
	TCHAR class_name[32];

	SetProp(control, guide_prop, (HANDLE) guide);
	SetProp(control, spec_prop, (HANDLE) optspec);
	GetClassName(control, class_name, sizeof(class_name)
		/ sizeof(class_name[0]));

	if (!_tcsicmp(class_name, TEXT("ComboBox")))
		rc = prepare_combobox(control, guide, optspec);
	else
	if (!_tcsicmp(class_name, TEXT("Edit")))
		rc = prepare_editbox(control, guide, optspec);

	return rc;
}




//============================================================
//  call_windowproc
//============================================================

static LRESULT call_windowproc(WNDPROC wndproc, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT rc;
	if (IsWindowUnicode(hwnd))
		rc = CallWindowProcW(wndproc, hwnd, msg, wparam, lparam);
	else
		rc = CallWindowProcA(wndproc, hwnd, msg, wparam, lparam);
	return rc;
}



//============================================================
//  dialog_write
//============================================================

static int dialog_write(struct _dialog_box *di, const void *ptr, size_t sz, int align)
{
	HGLOBAL newhandle;
	size_t base;
	UINT8 *mem;
	UINT8 *mem2;

	if (!di->handle)
	{
		newhandle = GlobalAlloc(GMEM_ZEROINIT, sz);
		base = 0;
	}
	else
	{
		base = di->handle_size;
		base += align - 1;
		base -= base % align;
		newhandle = GlobalReAlloc(di->handle, base + sz, GMEM_ZEROINIT);
		if (!newhandle)
		{
			newhandle = GlobalAlloc(GMEM_ZEROINIT, base + sz);
			if (newhandle)
			{
				mem = (UINT8*)GlobalLock(di->handle);
				mem2 = (UINT8*)GlobalLock(newhandle);
				memcpy(mem2, mem, base);
				GlobalUnlock(di->handle);
				GlobalUnlock(newhandle);
				GlobalFree(di->handle);
			}
		}
	}
	if (!newhandle)
		return 1;

	mem = (UINT8*)GlobalLock(newhandle);
	memcpy(mem + base, ptr, sz);
	GlobalUnlock(newhandle);
	di->handle = newhandle;
	di->handle_size = base + sz;
	return 0;
}



//============================================================
//  dialog_write_string
//============================================================

static int dialog_write_string(dialog_box *di, const WCHAR *str)
{
	if (!str)
		str = L"";
	return dialog_write(di, str, (wcslen(str) + 1) * sizeof(WCHAR), 2);
}




//============================================================
//  win_dialog_exit
//============================================================

static void win_dialog_exit(dialog_box *dialog)
{
	int i;
	struct dialog_object_pool *objpool;

	if (!dialog)
		printf("No dialog passed to win_dialog_exit\n");
	assert(dialog);

	objpool = dialog->objpool;
	if (objpool)
	{
		for (i = 0; i < ARRAY_LENGTH(objpool->objects); i++)
			DeleteObject(objpool->objects[i]);
	}

	if (dialog->handle)
		GlobalFree(dialog->handle);

	//pool_free_lib(dialog->mempool);
	free(dialog);
}



//============================================================
//  win_dialog_init
//============================================================

dialog_box *win_dialog_init(const char *title, const struct dialog_layout *layout)
{
	struct _dialog_box *di;
	DLGTEMPLATE dlg_template;
	WORD w[2];
	WCHAR *w_title;
	int rc;

	// use default layout if not specified
	if (!layout)
		layout = &default_layout;

	// create the dialog structure
	di = (_dialog_box *)malloc(sizeof(struct _dialog_box));
	if (!di)
		goto error;
	memset(di, 0, sizeof(*di));

	di->layout = layout;
	//di->mempool = pool_alloc_lib(NULL);

	memset(&dlg_template, 0, sizeof(dlg_template));
	dlg_template.style = di->style = DIALOG_STYLE;
	dlg_template.x = 10;
	dlg_template.y = 10;
	if (dialog_write(di, &dlg_template, sizeof(dlg_template), 4))
		goto error;

	w[0] = 0;
	w[1] = 0;
	if (dialog_write(di, w, sizeof(w), 2))
		goto error;

	w_title = wstring_from_utf8(title);
	rc = dialog_write_string(di, w_title);
	osd_free(w_title);
	if (rc)
		goto error;

	// set the font, if necessary
	if (di->style & DS_SETFONT)
	{
		w[0] = FONT_SIZE;
		if (dialog_write(di, w, sizeof(w[0]), 2))
			goto error;
		if (dialog_write_string(di, FONT_FACE))
			goto error;
	}

	return di;

error:
	if (di)
		win_dialog_exit(di);
	return NULL;
}


//============================================================
//  compute_dlgunits_multiple
//============================================================

static void calc_dlgunits_multiple(void)
{
	dialog_box *dialog = NULL;
	short offset_x = 2048;
	short offset_y = 2048;
	const char *wnd_title = "Foo";
	WORD id;
	HWND dlg_window = NULL;
	HWND child_window;
	RECT r;

	if ((pixels_to_xdlgunits == 0) || (pixels_to_ydlgunits == 0))
	{
		// create a bogus dialog
		dialog = win_dialog_init(NULL, NULL);
		if (!dialog)
			goto done;

		if (dialog_write_item(dialog, WS_CHILD | WS_VISIBLE | SS_LEFT,
				0, 0, offset_x, offset_y, wnd_title, DLGITEM_STATIC, &id))
			goto done;

		dialog_prime(dialog);
		dlg_window = CreateDialogIndirectParam(NULL, (const DLGTEMPLATE*)dialog->handle, NULL, NULL, 0);
		child_window = GetDlgItem(dlg_window, id);

		GetWindowRect(child_window, &r);
		pixels_to_xdlgunits = (double)(r.right - r.left) / offset_x;
		pixels_to_ydlgunits = (double)(r.bottom - r.top) / offset_y;

done:
		if (dialog)
			win_dialog_exit(dialog);
		if (dlg_window)
			DestroyWindow(dlg_window);
	}
}



//============================================================
//  dialog_trigger
//============================================================

static void dialog_trigger(HWND dlgwnd, WORD trigger_flags)
{
	LRESULT result;
	HWND dialog_item;
	struct _dialog_box *di;
	struct dialog_info_trigger *trigger;
	LONG_PTR l;

	l = GetWindowLongPtr(dlgwnd, GWLP_USERDATA);
	di = (struct _dialog_box *) l;
	if (!di)
		printf("Unexpected result of di in dialog_trigger\n");
	assert(di);
	for (trigger = di->trigger_first; trigger; trigger = trigger->next)
	{
		if (trigger->trigger_flags & trigger_flags)
		{
			if (trigger->dialog_item)
				dialog_item = GetDlgItem(dlgwnd, trigger->dialog_item);
			else
				dialog_item = dlgwnd;
			if (!dialog_item)
				printf("Unexpected result of dialog_item in dialog_trigger\n");
			assert(dialog_item);
			result = 0;

			if (trigger->message)
				result = SendMessage(dialog_item, trigger->message, trigger->wparam, trigger->lparam);
			if (trigger->trigger_proc)
				result = trigger->trigger_proc(di, dialog_item, trigger->message, trigger->wparam, trigger->lparam);

			if (trigger->storeval)
				trigger->storeval(trigger->storeval_param, result);
		}
	}
}

//============================================================
//  dialog_proc
//============================================================

static INT_PTR CALLBACK dialog_proc(HWND dlgwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	INT_PTR handled = TRUE;
	CHAR buf[32];
	WORD command;

	//if (LOG_WINMSGS)
	//{
	//	logerror("dialog_proc(): dlgwnd=%p msg=0x%08x wparam=0x%08x lparam=0x%08x\n",
	//		dlgwnd, (unsigned int) msg, (unsigned int) wparam, (unsigned int) lparam);
	//}

	switch(msg)
	{
		case WM_INITDIALOG:
			SetWindowLongPtr(dlgwnd, GWLP_USERDATA, (LONG_PTR) lparam);
			dialog_trigger(dlgwnd, TRIGGER_INITDIALOG);
			break;

		case WM_COMMAND:
			command = LOWORD(wparam);

			win_get_window_text_utf8((HWND) lparam, buf, ARRAY_LENGTH(buf));
			if (!strcmp(buf, DLGTEXT_OK))
				command = IDOK;
			else if (!strcmp(buf, DLGTEXT_CANCEL))
				command = IDCANCEL;
			else
				command = 0;

			switch(command)
			{
				case IDOK:
					dialog_trigger(dlgwnd, TRIGGER_APPLY);
					// fall through

				case IDCANCEL:
					EndDialog(dlgwnd, 0);
					break;

				default:
					handled = FALSE;
					break;
			}
			break;

		case WM_SYSCOMMAND:
			if (wparam == SC_CLOSE)
				EndDialog(dlgwnd, 0);
			else
				handled = FALSE;

			break;

		case WM_VSCROLL:
			if (lparam)
				// this scroll message came from an actual scroll bar window;
				// pass it on
				SendMessage((HWND) lparam, msg, wparam, lparam);
			else
				// scroll the dialog
				win_scroll_window(dlgwnd, wparam, SB_VERT, SCROLL_DELTA_LINE);

			break;

		default:
			handled = FALSE;
			break;
	}
	return handled;
}



//============================================================
//  dialog_write_item
//============================================================

static int dialog_write_item(dialog_box *di, DWORD style, short x, short y,
	 short width, short height, const char *str, const WCHAR *class_name, WORD *id)
{
	DLGITEMTEMPLATE item_template;
	UINT class_name_length;
	WORD w;
	WCHAR *w_str;
	int rc;

	memset(&item_template, 0, sizeof(item_template));
	item_template.style = style;
	item_template.x = x;
	item_template.y = y;
	item_template.cx = width;
	item_template.cy = height;
	item_template.id = di->item_count + 1;

	if (dialog_write(di, &item_template, sizeof(item_template), 4))
		return 1;

	if (*class_name == 0xffff)
		class_name_length = 4;
	else
		class_name_length = (wcslen(class_name) + 1) * sizeof(WCHAR);
	if (dialog_write(di, class_name, class_name_length, 2))
		return 1;

	w_str = str ? wstring_from_utf8(str) : NULL;
	rc = dialog_write_string(di, w_str);
	if (w_str)
		osd_free(w_str);
	if (rc)
		return 1;

	w = 0;
	if (dialog_write(di, &w, sizeof(w), 2))
		return 1;

	di->item_count++;

	if (id)
		*id = di->item_count;
	return 0;
}



//============================================================
//  dialog_add_trigger
//============================================================

static int dialog_add_trigger(struct _dialog_box *di, WORD dialog_item,
	WORD trigger_flags, UINT message, trigger_function trigger_proc,
	WPARAM wparam, LPARAM lparam, void (*storeval)(void *param, int val), void *storeval_param)
{
	if (!di)
		printf("Unexpected result of di in dialog_add_trigger\n");
	assert(di);
	if (!trigger_flags)
		printf("Unexpected result of trigger_flags in dialog_add_trigger\n");
	assert(trigger_flags);

	dialog_info_trigger *trigger = new(dialog_info_trigger);
	//dialog_info_trigger *trigger;
	//trigger = global_alloc(dialog_info_trigger);

	trigger->next = NULL;
	trigger->trigger_flags = trigger_flags;
	trigger->dialog_item = dialog_item;
	trigger->message = message;
	trigger->trigger_proc = trigger_proc;
	trigger->wparam = wparam;
	trigger->lparam = lparam;
	trigger->storeval = storeval;
	trigger->storeval_param = storeval_param;

	if (di->trigger_last)
		di->trigger_last->next = trigger;
	else
		di->trigger_first = trigger;
	di->trigger_last = trigger;
	return 0;
}



//============================================================
//  dialog_scrollbar_init
//============================================================

static LRESULT dialog_scrollbar_init(dialog_box *dialog, HWND dlgwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	SCROLLINFO si;

	calc_dlgunits_multiple();

	memset(&si, 0, sizeof(si));
	si.cbSize = sizeof(si);
	si.nMin  = pixels_to_ydlgunits * 0;
	si.nMax  = pixels_to_ydlgunits * dialog->size_y;
	si.nPage = pixels_to_ydlgunits * MAX_DIALOG_HEIGHT;
	si.fMask = SIF_PAGE | SIF_RANGE;

	SetScrollInfo(dlgwnd, SB_VERT, &si, TRUE);
	return 0;
}



//============================================================
//  dialog_add_scrollbar
//============================================================

static int dialog_add_scrollbar(dialog_box *dialog)
{
	if (dialog_add_trigger(dialog, 0, TRIGGER_INITDIALOG, 0, dialog_scrollbar_init, 0, 0, NULL, NULL))
		return 1;

	dialog->style |= WS_VSCROLL;
	return 0;
}



//============================================================
//  dialog_prime
//============================================================

static void dialog_prime(dialog_box *di)
{
	DLGTEMPLATE *dlg_template;

	if (di->size_y > MAX_DIALOG_HEIGHT)
	{
		di->size_x += DIM_SCROLLBAR_WIDTH;
		dialog_add_scrollbar(di);
	}

	dlg_template = (DLGTEMPLATE *) GlobalLock(di->handle);
	dlg_template->cdit = di->item_count;
	dlg_template->cx = di->size_x;
	dlg_template->cy = (di->size_y > MAX_DIALOG_HEIGHT) ? MAX_DIALOG_HEIGHT : di->size_y;
	dlg_template->style = di->style;
	GlobalUnlock(di->handle);
}



//============================================================
//  dialog_get_combo_value
//============================================================

static LRESULT dialog_get_combo_value(dialog_box *dialog, HWND dialog_item, UINT message, WPARAM wparam, LPARAM lparam)
{
	int idx;
	idx = SendMessage(dialog_item, CB_GETCURSEL, 0, 0);
	if (idx == CB_ERR)
		return 0;
	return SendMessage(dialog_item, CB_GETITEMDATA, idx, 0);
}



//============================================================
//  dialog_get_adjuster_value
//============================================================

static LRESULT dialog_get_adjuster_value(dialog_box *dialog, HWND dialog_item, UINT message, WPARAM wparam, LPARAM lparam)
{
	TCHAR buf[32];
	GetWindowText(dialog_item, buf, ARRAY_LENGTH(buf));
	return _ttoi(buf);
}



//============================================================
//  dialog_new_control
//============================================================

static void dialog_new_control(struct _dialog_box *di, short *x, short *y)
{
	*x = di->pos_x + DIM_HORIZONTAL_SPACING;
	*y = di->pos_y + DIM_VERTICAL_SPACING;
}



//============================================================
//  dialog_finish_control
//============================================================

static void dialog_finish_control(struct _dialog_box *di, short x, short y)
{
	di->pos_y = y;

	// update dialog size
	if (x > di->size_x)
		di->size_x = x;
	if (y > di->size_y)
		di->size_y = y;
	if (x > di->cursize_x)
		di->cursize_x = x;
	if (y > di->cursize_y)
		di->cursize_y = y;
}



//============================================================
//  dialog_combo_changed
//============================================================

static LRESULT dialog_combo_changed(dialog_box *dialog, HWND dlgitem, UINT message, WPARAM wparam, LPARAM lparam)
{
	dialog_itemchangedproc changed = (dialog_itemchangedproc) wparam;
	changed(dialog, dlgitem, (void *) lparam);
	return 0;
}



//============================================================
//  win_dialog_wcsdup
//============================================================

static WCHAR *win_dialog_wcsdup(dialog_box *dialog, const WCHAR *s)
{
	WCHAR *result = global_alloc_array(WCHAR, wcslen(s) + 1);
	if (result)
		wcscpy(result, s);
	return result;
}



//============================================================
//  win_dialog_add_active_combobox
//============================================================

static int win_dialog_add_active_combobox(dialog_box *dialog, const char *item_label, int default_value,
	dialog_itemstoreval storeval, void *storeval_param, dialog_itemchangedproc changed, void *changed_param)
{
	int rc = 1;
	short x;
	short y;

	dialog_new_control(dialog, &x, &y);

	if (dialog_write_item(dialog, WS_CHILD | WS_VISIBLE | SS_LEFT,
			x, y, dialog->layout->label_width, DIM_COMBO_ROW_HEIGHT, item_label, DLGITEM_STATIC, NULL))
		goto done;

	y += DIM_BOX_VERTSKEW;

	x += dialog->layout->label_width + DIM_HORIZONTAL_SPACING;
	if (dialog_write_item(dialog, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | CBS_DROPDOWNLIST,
			x, y, dialog->layout->combo_width, DIM_COMBO_ROW_HEIGHT * 8, NULL, DLGITEM_COMBOBOX, NULL))
		goto done;
	dialog->combo_string_count = 0;
	dialog->combo_default_value = default_value;

	// add the trigger invoked when the apply button is pressed
	if (dialog_add_trigger(dialog, dialog->item_count, TRIGGER_APPLY, 0, dialog_get_combo_value, 0, 0, storeval, storeval_param))
		goto done;

	// if appropriate, add the optional changed trigger
	if (changed)
	{
		if (dialog_add_trigger(dialog, dialog->item_count, TRIGGER_INITDIALOG | TRIGGER_CHANGED, 0, dialog_combo_changed, (WPARAM) changed, (LPARAM) changed_param, NULL, NULL))
			goto done;
	}

	x += dialog->layout->combo_width + DIM_HORIZONTAL_SPACING;
	y += DIM_COMBO_ROW_HEIGHT + DIM_VERTICAL_SPACING * 2;

	dialog_finish_control(dialog, x, y);
	rc = 0;

done:
	return rc;
}



//============================================================
//  win_dialog_add_combobox
//============================================================

static int win_dialog_add_combobox(dialog_box *dialog, const char *item_label, int default_value,
	void (*storeval)(void *param, int val), void *storeval_param)
{
	return win_dialog_add_active_combobox(dialog, item_label, default_value,
		storeval, storeval_param, NULL, NULL);
}



//============================================================
//  win_dialog_add_combobox_item
//============================================================

static int win_dialog_add_combobox_item(dialog_box *dialog, const char *item_label, int item_data)
{
	// create our own copy of the string
	size_t newsize = strlen(item_label) + 1;
	wchar_t * t_item_label = new wchar_t[newsize];
	//size_t convertedChars = 0;
	//mbstowcs_s(&convertedChars, t_item_label, newsize, item_label, _TRUNCATE);
	mbstowcs(t_item_label, item_label, newsize);

	if (dialog_add_trigger(dialog, dialog->item_count, TRIGGER_INITDIALOG, CB_ADDSTRING, NULL, 0, (LPARAM) t_item_label, NULL, NULL))
		return 1;
	dialog->combo_string_count++;
	if (dialog_add_trigger(dialog, dialog->item_count, TRIGGER_INITDIALOG, CB_SETITEMDATA, NULL, dialog->combo_string_count-1, (LPARAM) item_data, NULL, NULL))
		return 1;
	if (item_data == dialog->combo_default_value)
	{
		if (dialog_add_trigger(dialog, dialog->item_count, TRIGGER_INITDIALOG, CB_SETCURSEL, NULL, dialog->combo_string_count-1, 0, NULL, NULL))
			return 1;
	}
	return 0;
}



//============================================================
//  adjuster_sb_wndproc
//============================================================

struct adjuster_sb_stuff
{
	WNDPROC oldwndproc;
	int min_value;
	int max_value;
};

static INT_PTR CALLBACK adjuster_sb_wndproc(HWND sbwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	INT_PTR result;
	struct adjuster_sb_stuff *stuff;
	char buf[64];
	HWND dlgwnd, editwnd;
	int value, id;
	LONG_PTR l;

	l = GetWindowLongPtr(sbwnd, GWLP_USERDATA);
	stuff = (struct adjuster_sb_stuff *) l;

	if (msg == WM_VSCROLL)
	{
		id = GetWindowLong(sbwnd, GWL_ID);
		dlgwnd = GetParent(sbwnd);
		editwnd = GetDlgItem(dlgwnd, id - 1);
		win_get_window_text_utf8(editwnd, buf, ARRAY_LENGTH(buf));
		value = atoi(buf);

		switch(wparam)
		{
			case SB_LINEDOWN:
			case SB_PAGEDOWN:
				value--;
				break;

			case SB_LINEUP:
			case SB_PAGEUP:
				value++;
				break;
		}

		if (value < stuff->min_value)
			value = stuff->min_value;
		else if (value > stuff->max_value)
			value = stuff->max_value;
		_snprintf(buf, ARRAY_LENGTH(buf), "%d", value);
		win_set_window_text_utf8(editwnd, buf);
		result = 0;
	}
	else
		result = call_windowproc(stuff->oldwndproc, sbwnd, msg, wparam, lparam);

	return result;
}


//============================================================
//  win_dialog_malloc
//============================================================

//static void *win_dialog_malloc(dialog_box *dialog, size_t size)
//{
//	return pool_malloc_lib(dialog->mempool, size);
//}



//============================================================
//  adjuster_sb_setup
//============================================================

static LRESULT adjuster_sb_setup(dialog_box *dialog, HWND sbwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	struct adjuster_sb_stuff *stuff;
	LONG_PTR l;

	stuff = global_alloc(adjuster_sb_stuff);
	if (!stuff)
		return 1;
	stuff->min_value = (WORD) (lparam >> 0);
	stuff->max_value = (WORD) (lparam >> 16);

	l = (LONG_PTR) stuff;
	SetWindowLongPtr(sbwnd, GWLP_USERDATA, l);
	l = (LONG_PTR) adjuster_sb_wndproc;
	l = SetWindowLongPtr(sbwnd, GWLP_WNDPROC, l);
	stuff->oldwndproc = (WNDPROC) l;
	return 0;
}



//============================================================
//  win_dialog_add_adjuster
//============================================================

static int win_dialog_add_adjuster(dialog_box *dialog, const char *item_label, int default_value,
	int min_value, int max_value, BOOL is_percentage, dialog_itemstoreval storeval, void *storeval_param)
{
	short x;
	short y;
	TCHAR buf[32];
	TCHAR *s;

	dialog_new_control(dialog, &x, &y);

	if (dialog_write_item(dialog, WS_CHILD | WS_VISIBLE | SS_LEFT,
			x, y, dialog->layout->label_width, DIM_ADJUSTER_HEIGHT, item_label, DLGITEM_STATIC, NULL))
		goto error;
	x += dialog->layout->label_width + DIM_HORIZONTAL_SPACING;

	y += DIM_BOX_VERTSKEW;

	if (dialog_write_item(dialog, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NUMBER,
			x, y, dialog->layout->combo_width - DIM_ADJUSTER_SCR_WIDTH, DIM_ADJUSTER_HEIGHT, NULL, DLGITEM_EDIT, NULL))
		goto error;
	x += dialog->layout->combo_width - DIM_ADJUSTER_SCR_WIDTH;

	_sntprintf(buf, ARRAY_LENGTH(buf),
		is_percentage ? TEXT("%d%%") : TEXT("%d"),
		default_value);
	s = win_dialog_tcsdup(dialog, buf);
	osd_free(buf);
	if (!s)
		return 1;
	if (dialog_add_trigger(dialog, dialog->item_count, TRIGGER_INITDIALOG, WM_SETTEXT, NULL,
			0, (LPARAM) s, NULL, NULL))
		goto error;

	// add the trigger invoked when the apply button is pressed
	if (dialog_add_trigger(dialog, dialog->item_count, TRIGGER_APPLY, 0, dialog_get_adjuster_value, 0, 0, storeval, storeval_param))
		goto error;

	if (dialog_write_item(dialog, WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
			x, y, DIM_ADJUSTER_SCR_WIDTH, DIM_ADJUSTER_HEIGHT, NULL, DLGITEM_SCROLLBAR, NULL))
		goto error;
	x += DIM_ADJUSTER_SCR_WIDTH + DIM_HORIZONTAL_SPACING;

	if (dialog_add_trigger(dialog, dialog->item_count, TRIGGER_INITDIALOG, 0, adjuster_sb_setup,
			0, MAKELONG(min_value, max_value), NULL, NULL))
		return 1;

	y += DIM_COMBO_ROW_HEIGHT + DIM_VERTICAL_SPACING * 2;

	dialog_finish_control(dialog, x, y);
	return 0;

error:
	return 1;
}




//============================================================
//  get_seqselect_info
//============================================================

static seqselect_info *get_seqselect_info(HWND editwnd)
{
	LONG_PTR lp;
	lp = GetWindowLongPtr(editwnd, GWLP_USERDATA);
	return (seqselect_info *) lp;
}



//============================================================
//  seqselect_settext
//============================================================
//#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

static void seqselect_settext(HWND editwnd)
{
	seqselect_info *stuff;
	std::string seqstring;
	char buffer[128];

	// the basics
	stuff = get_seqselect_info(editwnd);
	if (stuff == NULL)
		return;	// this should not happen - need to fix this

	// retrieve the seq name
	*Machine->input().seq_name(seqstring, *stuff->code);

	// change the text - avoid calls to SetWindowText() if we can
	win_get_window_text_utf8(editwnd, buffer, ARRAY_LENGTH(buffer));
	if (strcmp(buffer, seqstring.c_str())!=0)
		win_set_window_text_utf8(editwnd, seqstring.c_str());

	// reset the selection
	if (GetFocus() == editwnd)
	{
		DWORD start = 0, end = 0;
		SendMessage(editwnd, EM_GETSEL, (WPARAM) (LPDWORD) &start, (LPARAM) (LPDWORD) &end);
		if ((start != 0) || (end != strlen(buffer)))
			SendMessage(editwnd, EM_SETSEL, 0, -1);
	}
}
#pragma GCC diagnostic error "-Wunused-value"



//============================================================
//  seqselect_start_read_from_main_thread
//============================================================

static void seqselect_start_read_from_main_thread(void *param)
{
	seqselect_info *stuff;
	HWND editwnd;
	win_window_info fake_window_info(*Machine);
	win_window_info *old_window_list;
	int pause_count;

	// get the basics
	editwnd = (HWND) param;
	stuff = get_seqselect_info(editwnd);

	// are we currently polling?  if so bail out
	if (stuff->poll_state != SEQSELECT_STATE_NOT_POLLING)
		return;

	// change the state
	stuff->poll_state = SEQSELECT_STATE_POLLING;

	// the Win32 OSD code thinks that we are paused, we need to temporarily
	// unpause ourselves or else we will block
	pause_count = 0;
	while(Machine->paused() && !winwindow_ui_is_paused(*Machine))
	{
		winwindow_ui_pause_from_main_thread(*Machine, FALSE);
		pause_count++;
	}

	// butt ugly hack so that we accept focus
	old_window_list = win_window_list;
	memset(&fake_window_info, 0, sizeof(fake_window_info));
	fake_window_info.m_hwnd = GetFocus();
	win_window_list = &fake_window_info;

	// start the polling
	(*Machine).input().seq_poll_start(stuff->is_analog ? ITEM_CLASS_ABSOLUTE : ITEM_CLASS_SWITCH, NULL);

	while(stuff->poll_state == SEQSELECT_STATE_POLLING)
	{
		// poll
		if (Machine->input().seq_poll())
		{
			*stuff->code = Machine->input().seq_poll_final();
			seqselect_settext(editwnd);
		}
	}

	// clean up after hack
	win_window_list = old_window_list;

	// we are no longer polling
	stuff->poll_state = SEQSELECT_STATE_NOT_POLLING;

	// repause the OSD code
	while(pause_count--)
		winwindow_ui_pause_from_main_thread(*Machine, TRUE);
}



//============================================================
//  seqselect_stop_read_from_main_thread
//============================================================

static void seqselect_stop_read_from_main_thread(void *param)
{
	HWND editwnd;
	seqselect_info *stuff;

	// get the basics
	editwnd = (HWND) param;
	stuff = get_seqselect_info(editwnd);

	// stop the read
	if (stuff->poll_state == SEQSELECT_STATE_POLLING)
		stuff->poll_state = SEQSELECT_STATE_POLLING_COMPLETE;
}



//============================================================
//  seqselect_wndproc
//============================================================

static INT_PTR CALLBACK seqselect_wndproc(HWND editwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	seqselect_info *stuff;
	INT_PTR result = 0;
	BOOL call_baseclass = TRUE;

	stuff = get_seqselect_info(editwnd);

	switch(msg)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_CHAR:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			result = 1;
			call_baseclass = FALSE;
			break;

		case WM_SETFOCUS:
			PostMessage(editwnd, SEQWM_SETFOCUS, 0, 0);
			break;

		case WM_KILLFOCUS:
			PostMessage(editwnd, SEQWM_KILLFOCUS, 0, 0);
			break;

		case SEQWM_SETFOCUS:
			// if we receive the focus, we should start a polling loop
			winwindow_ui_exec_on_main_thread(seqselect_start_read_from_main_thread, (void *) editwnd);
			break;

		case SEQWM_KILLFOCUS:
			// when we abort the focus, end any current polling loop
			winwindow_ui_exec_on_main_thread(seqselect_stop_read_from_main_thread, (void *) editwnd);
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			SetFocus(editwnd);
			SendMessage(editwnd, EM_SETSEL, 0, -1);
			call_baseclass = FALSE;
			result = 0;
			break;
	}

	if (call_baseclass)
		result = call_windowproc(stuff->oldwndproc, editwnd, msg, wparam, lparam);
	return result;
}



//============================================================
//  seqselect_setup
//============================================================

static LRESULT seqselect_setup(dialog_box *dialog, HWND editwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	seqselect_info *stuff = (seqselect_info *) lparam;
	LONG_PTR lp;

	lp = SetWindowLongPtr(editwnd, GWLP_WNDPROC, (LONG_PTR) seqselect_wndproc);
	stuff->oldwndproc = (WNDPROC) lp;
	SetWindowLongPtr(editwnd, GWLP_USERDATA, lparam);
	seqselect_settext(editwnd);
	return 0;
}



//============================================================
//  seqselect_apply
//============================================================

static LRESULT seqselect_apply(dialog_box *dialog, HWND editwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	seqselect_info *stuff;
	stuff = get_seqselect_info(editwnd);

	// store the settings
	stuff->field->set_user_settings(stuff->settings);

	return 0;
}

//============================================================
//  dialog_add_single_seqselect
//============================================================

static int dialog_add_single_seqselect(struct _dialog_box *di, short x, short y,
	short cx, short cy, ioport_field *field, int is_analog, int seqtype)
{
	// write the dialog item
	if (dialog_write_item(di, WS_CHILD | WS_VISIBLE | SS_ENDELLIPSIS | ES_CENTER | SS_SUNKEN,
			x, y, cx, cy, NULL, DLGITEM_EDIT, NULL))
		return 1;

	// allocate a seqselect_info
	seqselect_info *stuff = new(seqselect_info);
	//seqselect_info *stuff;
	//stuff = global_alloc(seqselect_info);
	if (!stuff)
		return 1;

	// initialize the structure
	memset(stuff, 0, sizeof(*stuff));
	field->get_user_settings(stuff->settings);
	stuff->field = field;
	stuff->pos = di->item_count;
	stuff->is_analog = is_analog;

	// This next line is completely unsafe, but I do not know what to use *****************
	stuff->code = const_cast <input_seq*> (&field->seq( (input_seq_type) seqtype) );

	if (dialog_add_trigger(di, di->item_count, TRIGGER_INITDIALOG, 0, seqselect_setup, di->item_count, (LPARAM) stuff, NULL, NULL))
		return 1;
	if (dialog_add_trigger(di, di->item_count, TRIGGER_APPLY, 0, seqselect_apply, 0, 0, NULL, NULL))
		return 1;
	return 0;
}



//============================================================
//  win_dialog_add_seqselect
//============================================================

static int win_dialog_add_portselect(dialog_box *dialog, ioport_field *field)
{
	dialog_box *di = dialog;
	short x;
	short y;
	const char *port_name;
	const char *this_port_name;
	char *s;
	int seq;
	int seq_count = 0;
	const char *port_suffix[3];
	int seq_types[3];
	int is_analog[3];
	int len;

	port_name = field->name();
	if (!port_name)
		printf("Blank port_name encountered in win_dialog_add_portselect\n");
	assert(port_name);

	if (field->type() > IPT_ANALOG_FIRST && field->type() < IPT_ANALOG_LAST)
	{
		seq_types[seq_count] = SEQ_TYPE_STANDARD;
		port_suffix[seq_count] = " Analog";
		is_analog[seq_count] = TRUE;
		seq_count++;

		seq_types[seq_count] = SEQ_TYPE_DECREMENT;
		port_suffix[seq_count] = " Dec";
		is_analog[seq_count] = FALSE;
		seq_count++;

		seq_types[seq_count] = SEQ_TYPE_INCREMENT;
		port_suffix[seq_count] = " Inc";
		is_analog[seq_count] = FALSE;
		seq_count++;
	}
	else
	{
		seq_types[seq_count] = SEQ_TYPE_STANDARD;
		port_suffix[seq_count] = NULL;
		is_analog[seq_count] = FALSE;
		seq_count++;
	}

	for (seq = 0; seq < seq_count; seq++)
	{
		// create our local name for this entry; also convert from
		// MAME strings to wide strings
		len = strlen(port_name);
		s = (char *) alloca((len + (port_suffix[seq] ? strlen(port_suffix[seq])
			: 0) + 1) * sizeof(*s));
		strcpy(s, port_name);

		if (port_suffix[seq])
			strcpy(s + len, port_suffix[seq]);
		this_port_name = s;

		// no positions specified
		dialog_new_control(di, &x, &y);

		if (dialog_write_item(di, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX, x, y,
				dialog->layout->label_width, DIM_NORMAL_ROW_HEIGHT, this_port_name, DLGITEM_STATIC, NULL))
			return 1;
		x += dialog->layout->label_width + DIM_HORIZONTAL_SPACING;

		if (dialog_add_single_seqselect(di, x, y, DIM_EDIT_WIDTH, DIM_NORMAL_ROW_HEIGHT,
				field, is_analog[seq], seq_types[seq]))
			return 1;
		y += DIM_VERTICAL_SPACING + DIM_NORMAL_ROW_HEIGHT;
		x += DIM_EDIT_WIDTH + DIM_HORIZONTAL_SPACING;

		dialog_finish_control(di, x, y);
	}
	return 0;
}



//============================================================
//  win_dialog_add_notification
//============================================================

static int win_dialog_add_notification(dialog_box *dialog, UINT notification, dialog_notification callback, void *param)
{
	// hack
	if (!dialog->notify_callback)
		printf("Unexpected result in win_dialog_add_notification\n");
	assert(!dialog->notify_callback);
	dialog->notify_code = notification;
	dialog->notify_callback = callback;
	dialog->notify_param = param;
	return 0;
}



//============================================================
//  win_dialog_add_standard_buttons
//============================================================

static int win_dialog_add_standard_buttons(dialog_box *dialog)
{
	dialog_box *di = dialog;
	short x;
	short y;

	// Handle the case of an empty dialog box (size_x & size_y will be 0)
	if (!di->size_x)
		di->size_x = 3 * DIM_HORIZONTAL_SPACING + 2 * DIM_BUTTON_WIDTH;
	if (!di->size_y)
		di->size_y = DIM_VERTICAL_SPACING;

	// work out where cancel button goes
	x = di->size_x - DIM_HORIZONTAL_SPACING - DIM_BUTTON_WIDTH;
	y = di->size_y + DIM_VERTICAL_SPACING;

	// display cancel button
	if (dialog_write_item(di, WS_CHILD | WS_VISIBLE | SS_LEFT,
			x, y, DIM_BUTTON_WIDTH, DIM_BUTTON_ROW_HEIGHT, DLGTEXT_CANCEL, DLGITEM_BUTTON, NULL))
		return 1;

	// work out where OK button goes
	x -= DIM_HORIZONTAL_SPACING + DIM_BUTTON_WIDTH;

	// display OK button
	if (dialog_write_item(di, WS_CHILD | WS_VISIBLE | SS_LEFT,
			x, y, DIM_BUTTON_WIDTH, DIM_BUTTON_ROW_HEIGHT, DLGTEXT_OK, DLGITEM_BUTTON, NULL))
		return 1;

	di->size_y += DIM_BUTTON_ROW_HEIGHT + DIM_VERTICAL_SPACING * 2;

	return 0;
}



//============================================================
//  before_display_dialog
//============================================================

static void before_display_dialog(running_machine &machine)
{
	Machine = &machine;
	winwindow_ui_pause_from_window_thread(machine, TRUE);
}



//============================================================
//  after_display_dialog
//============================================================

static void after_display_dialog(running_machine &machine)
{
	winwindow_ui_pause_from_window_thread(machine, FALSE);
	Machine = NULL;
}



//============================================================
//  win_dialog_runmodal
//============================================================

static void win_dialog_runmodal(running_machine &machine, HWND wnd, dialog_box *dialog)
{
	if (!dialog)
		printf("Unexpected result in win_dialog_runmodal\n");
	assert(dialog);

	// finishing touches on the dialog
	dialog_prime(dialog);

	// show the dialog
	before_display_dialog(machine);
	DialogBoxIndirectParamW(NULL, (const DLGTEMPLATE*)dialog->handle, wnd, (DLGPROC) dialog_proc, (LPARAM) dialog);
	after_display_dialog(machine);
}



//============================================================
//  file_dialog_hook
//============================================================

static UINT_PTR CALLBACK file_dialog_hook(HWND dlgwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	OPENFILENAME *ofn;
	dialog_box *dialog;
	UINT_PTR rc = 0;
	LPNMHDR notify;
	LONG_PTR l;

	switch(message)
	{
	case WM_INITDIALOG:
		ofn = (OPENFILENAME *) lparam;
		dialog = (dialog_box *) ofn->lCustData;

		SetWindowLongPtr(dlgwnd, GWLP_USERDATA, (LONG_PTR) dialog);
		dialog_trigger(dlgwnd, TRIGGER_INITDIALOG);
		rc = 1;

		// hack
		if (dialog->notify_callback && (dialog->notify_code == CDN_TYPECHANGE))
			dialog->notify_callback(dialog, dlgwnd, NULL, dialog->notify_param);
		break;

	case WM_NOTIFY:
		notify = (LPNMHDR) lparam;
		switch(notify->code)
		{
		case CDN_FILEOK:
			dialog_trigger(dlgwnd, TRIGGER_APPLY);
			break;
		}

		// hack
		l = GetWindowLongPtr(dlgwnd, GWLP_USERDATA);
		dialog = (dialog_box *) l;
		if (dialog->notify_callback && (notify->code == dialog->notify_code))
			dialog->notify_callback(dialog, dlgwnd, notify, dialog->notify_param);
		break;

	case WM_COMMAND:
		switch(HIWORD(wparam))
		{
		case CBN_SELCHANGE:
			dialog_trigger(dlgwnd, TRIGGER_CHANGED);
			break;
		}
		break;
	}
	return rc;
}



//============================================================
//  win_file_dialog
//============================================================

static BOOL win_file_dialog(running_machine &machine,
	HWND parent, win_file_dialog_type dlgtype, dialog_box *custom_dialog, const char *filter,
	const char *initial_dir, char *filename, size_t filename_len)
{
	win_open_file_name ofn;
	BOOL result = FALSE;

	// set up the OPENFILENAME data structure
	memset(&ofn, 0, sizeof(ofn));
	ofn.type = dlgtype;
	ofn.owner = parent;
	ofn.flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	ofn.filter = filter;
	ofn.initial_directory = initial_dir;

	if (dlgtype == WIN_FILE_DIALOG_OPEN)
		ofn.flags |= OFN_FILEMUSTEXIST;

	if (custom_dialog)
	{
		custom_dialog->style = WS_CHILD | WS_CLIPSIBLINGS | DS_3DLOOK | DS_CONTROL | DS_SETFONT;
		dialog_prime(custom_dialog);

		ofn.flags |= OFN_ENABLETEMPLATEHANDLE | OFN_ENABLEHOOK;
		ofn.instance = (HINSTANCE)custom_dialog->handle;
		ofn.custom_data = (LPARAM) custom_dialog;
		ofn.hook = file_dialog_hook;
	}

	snprintf(ofn.filename, ARRAY_LENGTH(ofn.filename), "%s", filename);

	before_display_dialog(machine);
	result = win_get_file_name_dialog(&ofn);
	after_display_dialog(machine);

	snprintf(filename, filename_len, "%s", ofn.filename);
	return result;
}




//============================================================
//  input_item_from_serial_number
//============================================================

static int input_item_from_serial_number(running_machine &machine, int serial_number,
	ioport_port **port, ioport_field **field, ioport_setting **setting)
{
	ioport_port *this_port = NULL;
	ioport_field *this_field = NULL;
	ioport_setting *this_setting = NULL;

	int i = 0;
	for (this_port = machine.ioport().first_port(); (i != serial_number) && (this_port); this_port = this_port->next())
	{
		i++;
		for (this_field = this_port->first_field(); (i != serial_number) && (this_field); this_field = this_field->next())
		{
			i++;
			for (this_setting = this_field->first_setting(); (i != serial_number) && (this_setting); this_setting = this_setting->next())
				i++;
		}
	}

	if (this_field)
		this_port = &this_field->port();

	if (port)
		*port = this_port;
	if (field)
		*field = this_field;
	if (setting)
		*setting = this_setting;

	return (i == serial_number);
}



//============================================================
//  customise_input
//============================================================

static void customise_input(running_machine &machine, HWND wnd, const char *title, int player, int inputclass)
{
	dialog_box *dlg;
	ioport_port *port;
	ioport_field *field;
	int this_inputclass = 0;
	int this_player = 0, portslot_count = 0, i = 0;
	const int max_portslots = 256;

	struct
	{
		ioport_field *field;
	} portslots[max_portslots];

	/* create the dialog */
	dlg = win_dialog_init(title, NULL);
	if (!dlg)
		return;

	for (port = machine.ioport().first_port(); port; port = port->next())
	{
		for (field = port->first_field(); field; field = field->next())
		{
			/* add if we match the group and we have a valid name */
			const char *name = field->name();
			this_inputclass = field->type_class();
			this_player = field->player();

			if ((name)
			&& (field->enabled())
//check me		&& ((field->type() == IPT_OTHER && field->name()) || (machine.ioport().type_group(field->type(), field->player()) != IPG_INVALID))
			&& (this_player == player)
			&& (this_inputclass == inputclass))
			{
				/* store this InputPort/RECT combo in our list.  we do not
				 * necessarily want to add it yet because we the INI might
				 * want to reorder the tab order */
				if (portslot_count < max_portslots)
				{
					portslots[portslot_count].field = field;
					portslot_count++;
				}
			}
		}
	}

	/* finally add the portselects to the dialog */
	if (portslot_count > max_portslots)
		portslot_count = max_portslots;

	/* No keyboard? Just exit */
	if (!portslot_count)
		goto done;

	for (i = 0; i < portslot_count; i++)
	{
		if (portslots[i].field)
		{
			if (win_dialog_add_portselect(dlg, portslots[i].field))
				goto done;
		}
	}

	/* ...and now add OK/Cancel */
	if (win_dialog_add_standard_buttons(dlg))
		goto done;

	/* ...and finally display the dialog */
	win_dialog_runmodal(machine, wnd, dlg);

done:
	if (dlg)
		win_dialog_exit(dlg);
}



//============================================================
//  customise_joystick
//============================================================

static void customise_joystick(running_machine &machine, HWND wnd, int joystick_num)
{
	customise_input(machine, wnd, "Joysticks/Controllers", joystick_num, INPUT_CLASS_CONTROLLER);
}



//============================================================
//  customise_keyboard
//============================================================

static void customise_keyboard(running_machine &machine, HWND wnd)
{
	customise_input(machine, wnd, "Emulated Keyboard", 0, INPUT_CLASS_KEYBOARD);
}



//============================================================
//  check_for_miscinput
//  (to decide if "Miscellaneous Inputs" menu item should
//  show or not).
//  We do this here because the core check has been broken
//  for years (always returns false).
//============================================================

static bool check_for_miscinput(running_machine &machine)
{
	ioport_port *port;
	ioport_field *field;
	int this_inputclass = 0;

	for (port = machine.ioport().first_port(); port; port = port->next())
	{
		for (field = port->first_field(); field; field = field->next())
		{
			const char *name = field->name();
			this_inputclass = field->type_class();

			/* add if we match the group and we have a valid name */
			if ((name)
			&& (field->enabled())
			&& ((field->type() == IPT_OTHER && field->name()) || (machine.ioport().type_group(field->type(), field->player()) != IPG_INVALID))
			&& (this_inputclass != INPUT_CLASS_CONTROLLER)
			&& (this_inputclass != INPUT_CLASS_KEYBOARD))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}



//============================================================
//  customise_miscinput
//============================================================

static void customise_miscinput(running_machine &machine, HWND wnd)
{
	dialog_box *dlg;
	ioport_port *port;
	ioport_field *field;
	int this_inputclass = 0;
	int portslot_count = 0, i = 0;
	const int max_portslots = 256;
	const char *title = "Miscellaneous Inputs";

	struct
	{
		ioport_field *field;
	} portslots[max_portslots];

	/* create the dialog */
	dlg = win_dialog_init(title, NULL);
	if (!dlg)
		return;

	for (port = machine.ioport().first_port(); port; port = port->next())
	{
		for (field = port->first_field(); field; field = field->next())
		{
			const char *name = field->name();
			this_inputclass = field->type_class();

			/* add if we match the group and we have a valid name */
			if ((name)
			&& (field->enabled())
			&& ((field->type() == IPT_OTHER && field->name()) || (machine.ioport().type_group(field->type(), field->player()) != IPG_INVALID))
			&& (this_inputclass != INPUT_CLASS_CONTROLLER)
			&& (this_inputclass != INPUT_CLASS_KEYBOARD))
			{
					/* store this InputPort/RECT combo in our list.  we do not
					 * necessarily want to add it yet because we the INI might
					 * want to reorder the tab order */
				if (portslot_count < max_portslots)
				{
					portslots[portslot_count].field = field;
					portslot_count++;
				}
			}
		}
	}

	/* finally add the portselects to the dialog */
	if (portslot_count > max_portslots)
		portslot_count = max_portslots;

	for (i = 0; i < portslot_count; i++)
	{
		if (portslots[i].field)
		{
			if (win_dialog_add_portselect(dlg, portslots[i].field))
				goto done;
		}
	}

	/* ...and now add OK/Cancel */
	if (win_dialog_add_standard_buttons(dlg))
		goto done;

	/* ...and finally display the dialog */
	win_dialog_runmodal(machine, wnd, dlg);

done:
	if (dlg)
		win_dialog_exit(dlg);
}



//============================================================
//  storeval_inputport
//============================================================

static void storeval_inputport(void *param, int val)
{
	ioport_field *field = (ioport_field *) param;
	ioport_field::user_settings settings;

	field->get_user_settings(settings);
	settings.value = (ioport_value) val;
	field->set_user_settings(settings);
}



//============================================================
//  customise_switches
//============================================================

static void customise_switches(running_machine &machine, HWND wnd, const char* title_string, UINT32 ipt_name)
{
	dialog_box *dlg;
	ioport_port *port;
	ioport_field *field;
	ioport_setting *setting;
	const char *switch_name = NULL;
	ioport_field::user_settings settings;

	UINT32 type = 0;

	dlg = win_dialog_init(title_string, NULL);
	if (!dlg)
		return;

	for (port = machine.ioport().first_port(); port; port = port->next())
	{
		for (field = port->first_field(); field; field = field->next())
		{
			type = field->type();

			if (type == ipt_name)
			{
				switch_name = field->name();

				field->get_user_settings(settings);

				if (win_dialog_add_combobox(dlg, switch_name, settings.value, storeval_inputport, (void *) field))
					goto done;

				for (setting = field->first_setting(); setting; setting = setting->next())
				{
					if (win_dialog_add_combobox_item(dlg, setting->name(), setting->value()))
						goto done;
				}
			}
		}
	}

	if (win_dialog_add_standard_buttons(dlg))
		goto done;

	win_dialog_runmodal(machine, wnd, dlg);

done:
	if (dlg)
		win_dialog_exit(dlg);
}



//============================================================
//  customise_dipswitches
//============================================================

static void customise_dipswitches(running_machine &machine, HWND wnd)
{
	customise_switches(machine, wnd, "Dip Switches", IPT_DIPSWITCH);
}



//============================================================
//  customise_configuration
//============================================================

static void customise_configuration(running_machine &machine, HWND wnd)
{
	customise_switches(machine, wnd, "Driver Configuration", IPT_CONFIG);
}



//============================================================
//  customise_analogcontrols
//============================================================

enum
{
	ANALOG_ITEM_KEYSPEED,
	ANALOG_ITEM_CENTERSPEED,
	ANALOG_ITEM_REVERSE,
	ANALOG_ITEM_SENSITIVITY
};



static void store_analogitem(void *param, int val, int selected_item)
{
	ioport_field *field = (ioport_field *) param;
	ioport_field::user_settings settings;

	field->get_user_settings(settings);

	switch(selected_item)
	{
		case ANALOG_ITEM_KEYSPEED:		settings.delta = val;		break;
		case ANALOG_ITEM_CENTERSPEED:	settings.centerdelta = val;	break;
		case ANALOG_ITEM_REVERSE:		settings.reverse = val;		break;
		case ANALOG_ITEM_SENSITIVITY:	settings.sensitivity = val;	break;
	}
	field->set_user_settings(settings);
}



static void store_delta(void *param, int val)
{
	store_analogitem(param, val, ANALOG_ITEM_KEYSPEED);
}



static void store_centerdelta(void *param, int val)
{
	store_analogitem(param, val, ANALOG_ITEM_CENTERSPEED);
}



static void store_reverse(void *param, int val)
{
	store_analogitem(param, val, ANALOG_ITEM_REVERSE);
}



static void store_sensitivity(void *param, int val)
{
	store_analogitem(param, val, ANALOG_ITEM_SENSITIVITY);
}



static int port_type_is_analog(int type)
{
	return (type > IPT_ANALOG_FIRST && type < IPT_ANALOG_LAST);
}



static void customise_analogcontrols(running_machine &machine, HWND wnd)
{
	dialog_box *dlg;
	ioport_port *port;
	ioport_field *field;
	ioport_field::user_settings settings;
	const char *name;
	char buf[255];
	static const struct dialog_layout layout = { 120, 52 };

	dlg = win_dialog_init("Analog Controls", &layout);
	if (!dlg)
		return;

	for (port = machine.ioport().first_port(); port; port = port->next())
	{
		for (field = port->first_field(); field; field = field->next())
		{
			if (port_type_is_analog(field->type()))
			{
				field->get_user_settings(settings);
				name = field->name();

				_snprintf(buf, ARRAY_LENGTH(buf),
					"%s %s", name, "Digital Speed");
				if (win_dialog_add_adjuster(dlg, buf, settings.delta, 1, 255, FALSE, store_delta, (void *) field))
					goto done;

				_snprintf(buf, ARRAY_LENGTH(buf),
					"%s %s", name, "Autocenter Speed");
				if (win_dialog_add_adjuster(dlg, buf, settings.centerdelta, 1, 255, FALSE, store_centerdelta, (void *) field))
					goto done;

				_snprintf(buf, ARRAY_LENGTH(buf),
					"%s %s", name, "Reverse");
				if (win_dialog_add_combobox(dlg, buf, settings.reverse ? 1 : 0, store_reverse, (void *) field))
					goto done;
				if (win_dialog_add_combobox_item(dlg, "Off", 0))
					goto done;
				if (win_dialog_add_combobox_item(dlg, "On", 1))
					goto done;

				_snprintf(buf, ARRAY_LENGTH(buf),
					"%s %s", name, "Sensitivity");
				if (win_dialog_add_adjuster(dlg, buf, settings.sensitivity, 1, 255, TRUE, store_sensitivity, (void *) field))
					goto done;
			}
		}
	}

	if (win_dialog_add_standard_buttons(dlg))
		goto done;

	win_dialog_runmodal(machine, wnd, dlg);

done:
	if (dlg)
		win_dialog_exit(dlg);
}


//============================================================
//  win_dirname
//============================================================

static char *win_dirname(const char *filename)
{
	char *dirname;
	char *c;

	// NULL begets NULL
	if (!filename)
		return NULL;

	// allocate space for it
	dirname = (char*)malloc(strlen(filename) + 1);
	if (!dirname)
		return NULL;

	// copy in the name
	strcpy(dirname, filename);

	// search backward for a slash or a colon
	for (c = dirname + strlen(dirname) - 1; c >= dirname; c--)
		if (*c == '\\' || *c == '/' || *c == ':')
		{
			// found it: NULL terminate and return
			*(c + 1) = 0;
			return dirname;
		}

	// otherwise, return an empty string
	dirname[0] = 0;
	return dirname;
}


//============================================================
//  state_dialog
//============================================================

static void state_dialog(HWND wnd, win_file_dialog_type dlgtype,
	DWORD fileproc_flags, bool is_load,
	running_machine &machine)
{
	win_open_file_name ofn;
	char *dir = NULL;
	int result = 0;

	if (state_filename[0])
		dir = win_dirname(state_filename);

	memset(&ofn, 0, sizeof(ofn));
	ofn.type = dlgtype;
	ofn.owner = wnd;
	ofn.flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | fileproc_flags;
	ofn.filter = "State Files (*.sta)|*.sta|All Files (*.*)|*.*";
	ofn.initial_directory = dir;

	if (!core_filename_ends_with(ofn.filename, "sta"))
		snprintf(ofn.filename, ARRAY_LENGTH(ofn.filename), "%s.sta", state_filename);
	else
		snprintf(ofn.filename, ARRAY_LENGTH(ofn.filename), "%s", state_filename);

	result = win_get_file_name_dialog(&ofn);

	if (result)
	{
		// the core doesn't add the extension if it's an absolute path
		if (osd_is_absolute_path(ofn.filename) && !core_filename_ends_with(ofn.filename, "sta"))
			snprintf(state_filename, ARRAY_LENGTH(state_filename), "%s.sta", ofn.filename);
		else
			snprintf(state_filename, ARRAY_LENGTH(state_filename), "%s", ofn.filename);

		if (is_load)
			machine.schedule_load(state_filename);
		else
			machine.schedule_save(state_filename);
	}

	if (dir)
		free(dir);
}



static void state_load(HWND wnd, running_machine &machine)
{
	state_dialog(wnd, WIN_FILE_DIALOG_OPEN, OFN_FILEMUSTEXIST, TRUE, machine);
}

static void state_save_as(HWND wnd, running_machine &machine)
{
	state_dialog(wnd, WIN_FILE_DIALOG_SAVE, OFN_OVERWRITEPROMPT, FALSE, machine);
}

static void state_save(running_machine &machine)
{
	machine.schedule_save(state_filename);
}



//============================================================
//  format_combo_changed
//============================================================

static void format_combo_changed(dialog_box *dialog, HWND dlgwnd, NMHDR *notification, void *changed_param)
{
	HWND wnd;
	int format_combo_val = 0;
	device_image_interface *dev;
	const option_guide *guide;
	const char *optspec;
	struct file_dialog_params *params;
	int has_option = 0;
	TCHAR t_buf1[128];
	char *utf8_buf1;

	params = (struct file_dialog_params *) changed_param;

	// locate the format control
	format_combo_val = notification ? (((OFNOTIFY *) notification)->lpOFN->nFilterIndex - 1) : 0;
	if (format_combo_val < 0)
		format_combo_val = 0;
	*(params->create_format) = format_combo_val;

	// compute our parameters
	dev = params->dev;
	guide = dev->device_get_creation_option_guide();
	optspec =dev->device_get_indexed_creatable_format(format_combo_val)->optspec();

	// set the default extension
	CommDlg_OpenSave_SetDefExt(GetParent(dlgwnd),
		(const char*)dev->device_get_indexed_creatable_format(format_combo_val)->extensions());

	// enumerate through all of the child windows
	wnd = NULL;
	while((wnd = FindWindowEx(dlgwnd, wnd, NULL, NULL)))
	{
		// get label text, removing trailing NULL
		GetWindowText(wnd, t_buf1, ARRAY_LENGTH(t_buf1));
		utf8_buf1 = utf8_from_tstring(t_buf1);
		assert(utf8_buf1[strlen(utf8_buf1)-1] == ':');
		utf8_buf1[strlen(utf8_buf1)-1] = '\0';

		// find guide entry
		while(guide->option_type && strcmp(utf8_buf1, guide->display_name))
			guide++;

		wnd = GetNextWindow(wnd, GW_HWNDNEXT);
		if (wnd && guide)
		{
			// we now have the handle to the window, and the guide entry
			has_option = option_resolution_contains(optspec, guide->parameter);

			SendMessage(wnd, CB_GETLBTEXT, SendMessage(wnd, CB_GETCURSEL, 0, 0), (LPARAM) t_buf1);
			SendMessage(wnd, CB_RESETCONTENT, 0, 0);

			win_prepare_option_control(wnd,
				has_option ? guide : NULL,
				has_option ? optspec : NULL);
		}
		osd_free(utf8_buf1);
	}
}



//============================================================
//  storeval_option_resolution
//============================================================

struct storeval_optres_params
{
	struct file_dialog_params *fdparams;
	const option_guide *guide_entry;
};

static void storeval_option_resolution(void *storeval_param, int val)
{
	option_resolution *resolution;
	struct storeval_optres_params *params;
	device_image_interface *dev;
	char buf[16];

	params = (struct storeval_optres_params *) storeval_param;
	dev = params->fdparams->dev;

	// create the resolution, if necessary
	resolution = *(params->fdparams->create_args);
	if (!resolution)
	{
		const option_guide *optguide = dev->device_get_creation_option_guide();
		const image_device_format *format = dev->device_get_indexed_creatable_format(*(params->fdparams->create_format));

		resolution = option_resolution_create(optguide, format->optspec());
		if (!resolution)
			return;
		*(params->fdparams->create_args) = resolution;
	}

	snprintf(buf, ARRAY_LENGTH(buf), "%d", val);
	option_resolution_add_param(resolution, params->guide_entry->identifier, buf);
}



//============================================================
//  build_option_dialog
//============================================================

static dialog_box *build_option_dialog(device_image_interface *dev, char *filter, size_t filter_len, int *create_format, option_resolution **create_args)
{
	dialog_box *dialog;
	const option_guide *guide_entry;
	int found = 0, pos = 0;
	char buf[256];
	struct file_dialog_params *params;
	struct storeval_optres_params *storeval_params;
	static const struct dialog_layout filedialog_layout = { 44, 220 };
	const image_device_format *format;

	// make the filter
	pos = 0;
	for (format = dev->formatlist(); format; format = format->next())
		pos += add_filter_entry(filter + pos, filter_len - pos, format->description(), format->extensions());

	// create the dialog
	dialog = win_dialog_init(NULL, &filedialog_layout);
	if (!dialog)
		goto error;

	// allocate the params
	params = global_alloc(file_dialog_params);
	if (!params)
		goto error;
	params->dev = dev;
	params->create_format = create_format;
	params->create_args = create_args;

	// set the notify handler; so that we get notified when the format dialog changed
	if (win_dialog_add_notification(dialog, CDN_TYPECHANGE, format_combo_changed, params))
		goto error;

	// loop through the entries
	for (guide_entry = dev->device_get_creation_option_guide(); guide_entry->option_type != OPTIONTYPE_END; guide_entry++)
	{
		// make sure that this entry is present on at least one option specification
		found = FALSE;
		for (format = dev->formatlist(); format; format = format->next())
		{
			if (option_resolution_contains(format->optspec(), guide_entry->parameter))
			{
				found = TRUE;
				break;
			}
		}

		if (found)
		{
			storeval_params = global_alloc(storeval_optres_params);
			if (!storeval_params)
				goto error;
			storeval_params->fdparams = params;
			storeval_params->guide_entry = guide_entry;

			// this option is present on at least one of the specs
			switch(guide_entry->option_type)
			{
				case OPTIONTYPE_INT:
					snprintf(buf, ARRAY_LENGTH(buf), "%s:", guide_entry->display_name);
					if (win_dialog_add_combobox(dialog, buf, 0, storeval_option_resolution, storeval_params))
						goto error;
					break;

				default:
					break;
			}
		}
	}

	return dialog;

error:
	if (dialog)
		win_dialog_exit(dialog);
	return NULL;
}



//============================================================
//  copy_extension_list
//============================================================

static int copy_extension_list(char *dest, size_t dest_len, const char *extensions)
{
	const char *s;
	int pos = 0;

	// our extension lists are comma delimited; Win32 expects to see lists
	// delimited by semicolons
	s = extensions;
	while(*s)
	{
		// append a semicolon if not at the beginning
		if (s != extensions)
			pos += snprintf(&dest[pos], dest_len - pos, ";");

		// append ".*"
		pos += snprintf(&dest[pos], dest_len - pos, "*.");

		// append the file extension
		while(*s && (*s != ','))
		{
			pos += snprintf(&dest[pos], dest_len - pos, "%c", *s);
			s++;
		}

		// if we found a comma, advance
		while(*s == ',')
			s++;
	}
	return pos;
}



//============================================================
//  add_filter_entry
//============================================================

static int add_filter_entry(char *dest, size_t dest_len, const char *description, const char *extensions)
{
	int pos = 0;

	// add the description
	pos += snprintf(&dest[pos], dest_len - pos, "%s (", description);

	// add the extensions to the description
	pos += copy_extension_list(&dest[pos], dest_len - pos, extensions);

	// add the trailing rparen and '|' character
	pos += snprintf(&dest[pos], dest_len - pos, ")|");

	// now add the extension list itself
	pos += copy_extension_list(&dest[pos], dest_len - pos, extensions);

	// append a '|'
	pos += snprintf(&dest[pos], dest_len - pos, "|");

	return pos;
}



//============================================================
//  build_generic_filter
//============================================================

static void build_generic_filter(device_image_interface *dev, int is_save, char *filter, size_t filter_len)
{
	char *s;

	const char *file_extension;

	/* copy the string */
	file_extension = dev->file_extensions();
	const char* s2 = ",zip"; // Add ZIP extension as a default.
	char *result = (LPSTR) alloca(strlen(file_extension)+strlen(s2)+1);
	strcpy(result, file_extension);
	strcat(result, s2);

	// start writing the filter
	s = filter;

	// common image types
	s += add_filter_entry(filter, filter_len, "Common image types", result);
	//free(result);

	// all files
	s += sprintf(s, "All files (*.*)|*.*|");

	// compressed
	if (!is_save)
		s += sprintf(s, "Compressed Images (*.zip)|*.zip|"); //(*.zip;*.7z)|*.zip;*.7z|");

	*(s++) = '\0';
}



//============================================================
//  change_device
//============================================================

static void change_device(HWND wnd, device_image_interface *image, int is_save)
{
	dialog_box *dialog = NULL;
	char filter[2048];
	char filename[MAX_PATH];
	const char *initial_dir;
	BOOL result = 0;
	int create_format = 0;
	option_resolution *create_args = NULL;

	// sanity check
	assert(image);

	// get the file
	if (image->exists())
	{
		const char *imgname;
		imgname = image->basename();
		snprintf(filename, ARRAY_LENGTH(filename), "%s", imgname);
	}
	else
		filename[0] = '\0';

	// get the working directory, but if it is ".", then use the one specified in comments_path
	char *dst = NULL,*working = 0;
	osd_get_full_path(&dst,"."); // turn local directory into full path
	initial_dir = image->working_directory(); // get working directory from diimage.c
	// if . use comments_dir
	if (strcmp(dst, initial_dir) == 0)  // same?
		initial_dir = software_dir;

	// remove any trailing backslash
	working = core_strdup(initial_dir);
	int temp = strlen(working) - 1;
	if (temp > 2)
		if (working[temp] == '\\')
		{
			working[temp] = '\0';
			initial_dir = working;
		}

// NOTE: the working directory can come from the .cfg file. If it's wrong delete the cfg.
//printf("%s = %s = %s = %s\n",dst,working,initial_dir,software_dir);
	// add custom dialog elements, if appropriate
	if (is_save
		&& (image->device_get_creation_option_guide())
		&& (image->formatlist()))
	{
		dialog = build_option_dialog(image, filter, ARRAY_LENGTH(filter), &create_format, &create_args);
		if (!dialog)
			goto done;
	}
	else
		// build a normal filter
		build_generic_filter(image, is_save, filter, ARRAY_LENGTH(filter));

	// display the dialog
	result = win_file_dialog(image->device().machine(), wnd, is_save ? WIN_FILE_DIALOG_SAVE : WIN_FILE_DIALOG_OPEN,
		dialog, filter, initial_dir, filename, ARRAY_LENGTH(filename));
	if (result)
	{
		// mount the image
		if (is_save)
			(image_error_t)image->create(filename, image->device_get_indexed_creatable_format(create_format), create_args);
		else
			(image_error_t)image->load( filename);

		// no need to check the returnvalue and show a messagebox anymore.
		// a UI message will now be generated by the image code
	}

done:
	if (dialog)
		win_dialog_exit(dialog);
	if (create_args)
		option_resolution_close(create_args);
}



//============================================================
//  pause
//============================================================

static void pause(running_machine &machine)
{
	if (!winwindow_ui_is_paused(machine))
		machine.pause();
	else
		machine.resume();
}



//============================================================
//  get_menu_item_string
//============================================================

static BOOL get_menu_item_string(HMENU menu, UINT item, BOOL by_position, HMENU *sub_menu, LPTSTR buffer, size_t buffer_len)
{
	MENUITEMINFO mii;

	// clear out results
	memset(buffer, '\0', sizeof(*buffer) * buffer_len);
	if (sub_menu)
		*sub_menu = NULL;

	// prepare MENUITEMINFO structure
	memset(&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_TYPE | (sub_menu ? MIIM_SUBMENU : 0);
	mii.dwTypeData = buffer;
	mii.cch = buffer_len;

	// call GetMenuItemInfo()
	if (!GetMenuItemInfo(menu, item, by_position, &mii))
		return FALSE;

	// return results
	if (sub_menu)
		*sub_menu = mii.hSubMenu;
	if (mii.fType == MFT_SEPARATOR)
		_sntprintf(buffer, buffer_len, TEXT("-"));
	return TRUE;
}



//============================================================
//  find_sub_menu
//============================================================

static HMENU find_sub_menu(HMENU menu, const char *menutext, bool create_sub_menu)
{
	TCHAR buf[128];
	HMENU sub_menu;

	while(*menutext)
	{
		TCHAR *t_menutext = tstring_from_utf8(menutext);

		int i = -1;
		do
		{
			if (!get_menu_item_string(menu, ++i, TRUE, &sub_menu, buf, ARRAY_LENGTH(buf)))
			{
				osd_free(t_menutext);
				return NULL;
			}
		}
		while(_tcscmp(t_menutext, buf));

		osd_free(t_menutext);

		if (!sub_menu && create_sub_menu)
		{
			MENUITEMINFO mii;
			memset(&mii, 0, sizeof(mii));
			mii.cbSize = sizeof(mii);
			mii.fMask = MIIM_SUBMENU;
			mii.hSubMenu = CreateMenu();
			if (!SetMenuItemInfo(menu, i, TRUE, &mii))
			{
				i = GetLastError();
				return NULL;
			}
			sub_menu = mii.hSubMenu;
		}
		menu = sub_menu;
		if (!menu)
			return NULL;
		menutext += strlen(menutext) + 1;
	}
	return menu;
}



//============================================================
//  set_command_state
//============================================================

static void set_command_state(HMENU menu_bar, UINT command, UINT state)
{
	BOOL result = 0;

	MENUITEMINFO mii;

	memset(&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;
	mii.fState = state;
	result = SetMenuItemInfo(menu_bar, command, FALSE, &mii);
	result++;
}




//============================================================
//  remove_menu_items
//============================================================

static void remove_menu_items(HMENU menu)
{
	while(RemoveMenu(menu, 0, MF_BYPOSITION))
		;
}



//============================================================
//  setup_joystick_menu
//============================================================

static void setup_joystick_menu(running_machine &machine, HMENU menu_bar)
{
	int joystick_count = 0;
	HMENU joystick_menu;
	int i = 0;
	char buf[256];
	int child_count = 0;

	joystick_menu = find_sub_menu(menu_bar, "&Options\0&Joysticks\0", TRUE);
	if (!joystick_menu)
		return;

	// set up joystick menu
	joystick_count = machine.ioport().count_players();
	if (joystick_count > 0)
	{
		for (i = 0; i < joystick_count; i++)
		{
			snprintf(buf, ARRAY_LENGTH(buf), "Joystick %i", i + 1);
			win_append_menu_utf8(joystick_menu, MF_STRING, ID_JOYSTICK_0 + i, buf);
			child_count++;
		}
	}

	// last but not least, enable the joystick menu (or not)
	set_command_state(menu_bar, ID_OPTIONS_JOYSTICKS, child_count ? MFS_ENABLED : MFS_GRAYED);
}



//============================================================
//  is_windowed
//============================================================

static int is_windowed(void)
{
	return video_config.windowed;
}



//============================================================
//  frameskip_level_count
//============================================================

static int frameskip_level_count(running_machine &machine)
{
	static int count = -1;

	if (count < 0)
	{
		int frameskip_max = 10;
		count = frameskip_max + 1;
	}
	return count;
}



//============================================================
//  prepare_menus
//============================================================

static void prepare_menus(HWND wnd)
{
	int i = 0;
	char buf[MAX_PATH];
	TCHAR t_buf[MAX_PATH];
	const char *s;
	HMENU menu_bar;
	HMENU video_menu;
	HMENU device_menu;
	HMENU sub_menu;
	UINT_PTR new_item;
	UINT flags_for_exists = 0;
	UINT flags_for_writing = 0;
	bool has_config = 0, has_dipswitch = 0, has_keyboard = 0, has_misc = 0, has_analog = 0;
	ioport_port *port;
	ioport_field *field;
	int frameskip = 0;
	int orientation = 0;
	int speed = 0;
	LONG_PTR ptr = GetWindowLongPtr(wnd, GWLP_USERDATA);
	win_window_info *window = (win_window_info *)ptr;
	const char *view_name;
	int view_index = 0;
	ioport_field::user_settings settings;

	menu_bar = GetMenu(wnd);
	if (!menu_bar)
		return;

	if (!joystick_menu_setup)
	{
		setup_joystick_menu(window->machine(), menu_bar);
		joystick_menu_setup = 1;
	}

	frameskip = window->machine().video().frameskip();

	orientation = window->m_target->orientation();

	speed = window->machine().video().throttled() ? window->machine().video().speed_factor() : 0;

	has_config	= window->machine().ioport().type_class_present(INPUT_CLASS_CONFIG);
	has_dipswitch	= window->machine().ioport().type_class_present(INPUT_CLASS_DIPSWITCH);
	has_keyboard	= window->machine().ioport().type_class_present(INPUT_CLASS_KEYBOARD);
	has_misc	= check_for_miscinput(window->machine());

	has_analog = 0;
	for (port = window->machine().ioport().first_port(); port; port = port->next())
	{
		for (field = port->first_field(); field; field = field->next())
		{
			if (port_type_is_analog(field->type()))
			{
				has_analog = 1;
				break;
			}
		}
	}

	if (window->machine().system().flags & MACHINE_SUPPORTS_SAVE)
	{
		set_command_state(menu_bar, ID_FILE_LOADSTATE_NEWUI, MFS_ENABLED);
		set_command_state(menu_bar, ID_FILE_SAVESTATE_AS, MFS_ENABLED);
		set_command_state(menu_bar, ID_FILE_SAVESTATE, state_filename[0] != '\0' ? MFS_ENABLED : MFS_GRAYED);
	}
	else
	{
		set_command_state(menu_bar, ID_FILE_LOADSTATE_NEWUI, MFS_GRAYED);
		set_command_state(menu_bar, ID_FILE_SAVESTATE_AS, MFS_GRAYED);
		set_command_state(menu_bar, ID_FILE_SAVESTATE, MFS_GRAYED);
	}

	set_command_state(menu_bar, ID_EDIT_PASTE, window->machine().ioport().natkeyboard().can_post() ? MFS_ENABLED : MFS_GRAYED);

	set_command_state(menu_bar, ID_OPTIONS_PAUSE, winwindow_ui_is_paused(window->machine()) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_OPTIONS_CONFIGURATION, has_config ? MFS_ENABLED : MFS_GRAYED);
	set_command_state(menu_bar, ID_OPTIONS_DIPSWITCHES, has_dipswitch ? MFS_ENABLED : MFS_GRAYED);
	set_command_state(menu_bar, ID_OPTIONS_MISCINPUT, has_misc ? MFS_ENABLED : MFS_GRAYED);
	set_command_state(menu_bar, ID_OPTIONS_ANALOGCONTROLS, has_analog ? MFS_ENABLED : MFS_GRAYED);
	set_command_state(menu_bar, ID_FILE_FULLSCREEN, !is_windowed() ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_OPTIONS_TOGGLEFPS, window->machine().ui().show_fps() ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_FILE_UIACTIVE, window->machine().ioport().has_keyboard() ? (window->machine().ui_active() ? MFS_CHECKED : MFS_ENABLED): MFS_CHECKED | MFS_GRAYED);
#if HAS_PROFILER
	set_command_state(menu_bar, ID_FILE_PROFILER, window->machine().ui().show_profiler() ? MFS_CHECKED : MFS_ENABLED);
#endif

	set_command_state(menu_bar, ID_KEYBOARD_EMULATED, (has_keyboard) ? (!window->machine().ui().use_natural_keyboard() ? MFS_CHECKED : MFS_ENABLED): MFS_GRAYED);
	set_command_state(menu_bar, ID_KEYBOARD_NATURAL, (has_keyboard && window->machine().ioport().natkeyboard().can_post()) ? (window->machine().ui().use_natural_keyboard() ? MFS_CHECKED : MFS_ENABLED): MFS_GRAYED);
	set_command_state(menu_bar, ID_KEYBOARD_CUSTOMIZE, has_keyboard ? MFS_ENABLED : MFS_GRAYED);

	set_command_state(menu_bar, ID_VIDEO_ROTATE_0, (orientation == ROT0) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_VIDEO_ROTATE_90, (orientation == ROT90) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_VIDEO_ROTATE_180, (orientation == ROT180) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_VIDEO_ROTATE_270, (orientation == ROT270) ? MFS_CHECKED : MFS_ENABLED);

	set_command_state(menu_bar, ID_THROTTLE_50, (speed == 500) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_THROTTLE_100, (speed == 1000) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_THROTTLE_200, (speed == 2000) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_THROTTLE_500, (speed == 5000) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_THROTTLE_1000, (speed == 10000) ? MFS_CHECKED : MFS_ENABLED);
	set_command_state(menu_bar, ID_THROTTLE_UNTHROTTLED, (speed == 0) ? MFS_CHECKED : MFS_ENABLED);

	set_command_state(menu_bar, ID_FRAMESKIP_AUTO, (frameskip < 0) ? MFS_CHECKED : MFS_ENABLED);
	for (i = 0; i < frameskip_level_count(window->machine()); i++)
		set_command_state(menu_bar, ID_FRAMESKIP_0 + i, (frameskip == i) ? MFS_CHECKED : MFS_ENABLED);

	// set up screens in video menu
	video_menu = find_sub_menu(menu_bar, "&Options\0&Video\0", FALSE);
	do
	{
		get_menu_item_string(video_menu, 0, TRUE, NULL, t_buf, ARRAY_LENGTH(t_buf));
		if (_tcscmp(t_buf, TEXT("-")))
			RemoveMenu(video_menu, 0, MF_BYPOSITION);
	}
	while(_tcscmp(t_buf, TEXT("-")));
	i = 0;
	view_index = window->m_target->view();
	while((view_name = window->m_target->view_name(i)))
	{
		TCHAR *t_view_name = tstring_from_utf8(view_name);
		InsertMenu(video_menu, i, MF_BYPOSITION | (i == view_index ? MF_CHECKED : 0),
			ID_VIDEO_VIEW_0 + i, t_view_name);
		osd_free(t_view_name);
		i++;
	}

	// set up device menu; first remove all existing menu items
	device_menu = find_sub_menu(menu_bar, "&Media\0", FALSE);
	remove_menu_items(device_menu);

	int cnt = 0;
	// then set up the actual devices
	image_interface_iterator iter(window->machine().root_device());
	for (device_image_interface *img = iter.first(); img; img = iter.next())
	{
		new_item = ID_DEVICE_0 + (cnt * DEVOPTION_MAX);
		flags_for_exists = MF_STRING;

		if (!img->exists())
			flags_for_exists |= MF_GRAYED;

		flags_for_writing = flags_for_exists;
		if (img->is_readonly())
			flags_for_writing |= MF_GRAYED;

		sub_menu = CreateMenu();
		win_append_menu_utf8(sub_menu, MF_STRING, new_item + DEVOPTION_OPEN, "Mount File...");

		if (img->is_creatable())
			win_append_menu_utf8(sub_menu, MF_STRING, new_item + DEVOPTION_CREATE, "Create...");

		win_append_menu_utf8(sub_menu, flags_for_exists, new_item + DEVOPTION_CLOSE, "Unmount");

		if (img->device().type() == CASSETTE)
		{
			cassette_state state;
			state = (cassette_state)(img->exists() ? (dynamic_cast<cassette_image_device*>(&img->device())->get_state() & CASSETTE_MASK_UISTATE) : CASSETTE_STOPPED);
			win_append_menu_utf8(sub_menu, MF_SEPARATOR, 0, NULL);
			win_append_menu_utf8(sub_menu, flags_for_exists	| ((state == CASSETTE_STOPPED) ? MF_CHECKED : 0), new_item + DEVOPTION_CASSETTE_STOPPAUSE, "Pause/Stop");
			win_append_menu_utf8(sub_menu, flags_for_exists	| ((state == CASSETTE_PLAY) ? MF_CHECKED : 0), new_item + DEVOPTION_CASSETTE_PLAY, "Play");
			win_append_menu_utf8(sub_menu, flags_for_writing | ((state == CASSETTE_RECORD) ? MF_CHECKED : 0), new_item + DEVOPTION_CASSETTE_RECORD, "Record");
			win_append_menu_utf8(sub_menu, flags_for_exists, new_item + DEVOPTION_CASSETTE_REWIND, "Rewind");
			win_append_menu_utf8(sub_menu, flags_for_exists, new_item + DEVOPTION_CASSETTE_FASTFORWARD, "Fast Forward");
		}
		s = img->exists() ? img->filename() : "[empty slot]";

		snprintf(buf, ARRAY_LENGTH(buf), "%s: %s", img->device().name(), s);
		win_append_menu_utf8(device_menu, MF_POPUP, (UINT_PTR)sub_menu, buf);

		cnt++;
	}
}



//============================================================
//  set_speed
//============================================================

static void set_speed(running_machine &machine, int speed)
{
	std::string error_string;
	if (speed != 0)
	{
		machine.video().set_speed_factor(speed);
		machine.options().emu_options::set_value(OPTION_SPEED, speed / 1000, OPTION_PRIORITY_CMDLINE, error_string);
	}

	machine.video().set_throttled(speed != 0);
	machine.options().emu_options::set_value(OPTION_THROTTLE, (speed != 0), OPTION_PRIORITY_CMDLINE, error_string);
}



//============================================================
//  win_toggle_menubar
//============================================================

static void win_toggle_menubar(void)
{
	win_window_info *window;
	LONG width_diff = 0;
	LONG height_diff = 0;
	DWORD style = 0, exstyle = 0;
	HWND hwnd = 0;
	HMENU menu = 0;

	for (window = win_window_list; window; window = window->m_next)
	{
		RECT before_rect = { 100, 100, 200, 200 };
		RECT after_rect = { 100, 100, 200, 200 };

		hwnd = window->m_hwnd;

		// get current menu
		menu = GetMenu(hwnd);

		// get before rect
		style = GetWindowLong(hwnd, GWL_STYLE);
		exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		AdjustWindowRectEx(&before_rect, style, menu ? TRUE : FALSE, exstyle);

		// toggle the menu
		if (menu)
		{
			SetProp(hwnd, TEXT("menu"), (HANDLE) menu);
			menu = NULL;
		}
		else
			menu = (HMENU) GetProp(hwnd, TEXT("menu"));

		SetMenu(hwnd, menu);

		// get after rect, and width/height diff
		AdjustWindowRectEx(&after_rect, style, menu ? TRUE : FALSE, exstyle);
		width_diff = (after_rect.right - after_rect.left) - (before_rect.right - before_rect.left);
		height_diff = (after_rect.bottom - after_rect.top) - (before_rect.bottom - before_rect.top);

		if (is_windowed())
		{
			RECT window_rect;
			GetWindowRect(hwnd, &window_rect);
			SetWindowPos(hwnd, HWND_TOP, 0, 0,
				window_rect.right - window_rect.left + width_diff,
				window_rect.bottom - window_rect.top + height_diff,
				SWP_NOMOVE | SWP_NOZORDER);
		}

		RedrawWindow(hwnd, NULL, NULL, 0);
	}
}



//============================================================
//  device_command
//============================================================

static void device_command(HWND wnd, device_image_interface *img, int devoption)
{
	switch(devoption)
	{
		case DEVOPTION_OPEN:
			change_device(wnd, img, FALSE);
			break;

		case DEVOPTION_CREATE:
			change_device(wnd, img, TRUE);
			break;

		case DEVOPTION_CLOSE:
			img->unload();
			break;

		default:
			if (img->device().type() == CASSETTE)
			{
				cassette_image_device* cassette = dynamic_cast<cassette_image_device*>(&img->device());

				switch(devoption)
				{
					case DEVOPTION_CASSETTE_STOPPAUSE:
						cassette->change_state(CASSETTE_STOPPED, CASSETTE_MASK_UISTATE);
						break;

					case DEVOPTION_CASSETTE_PLAY:
						cassette->change_state(CASSETTE_PLAY, CASSETTE_MASK_UISTATE);
						break;

					case DEVOPTION_CASSETTE_RECORD:
						cassette->change_state(CASSETTE_RECORD, CASSETTE_MASK_UISTATE);
						break;

					case DEVOPTION_CASSETTE_REWIND:
						cassette->seek(-60.0, SEEK_CUR);
						break;

					case DEVOPTION_CASSETTE_FASTFORWARD:
						cassette->seek(+60.0, SEEK_CUR);
						break;
				}
			}
			break;
	}
}



//============================================================
//  help_display
//============================================================

static void help_display(HWND wnd, const char *chapter)
{
	typedef HWND (WINAPI *htmlhelpproc)(HWND hwndCaller, LPCTSTR pszFile, UINT uCommand, DWORD_PTR dwData);
	static htmlhelpproc htmlhelp;
	static DWORD htmlhelp_cookie;
	LPCSTR htmlhelp_funcname;

	if (htmlhelp == NULL)
	{
#ifdef UNICODE
		htmlhelp_funcname = "HtmlHelpW";
#else
		htmlhelp_funcname = "HtmlHelpA";
#endif
		htmlhelp = (htmlhelpproc) GetProcAddress(LoadLibrary(TEXT("hhctrl.ocx")), htmlhelp_funcname);
		if (!htmlhelp)
			return;
		htmlhelp(NULL, NULL, 28 /*HH_INITIALIZE*/, (DWORD_PTR) &htmlhelp_cookie);
	}

	// if full screen, turn it off
	if (!is_windowed())
		winwindow_toggle_full_screen();

	TCHAR *t_chapter = tstring_from_utf8(chapter);
//	htmlhelp(wnd, t_chapter, 0 /*HH_DISPLAY_TOPIC*/, 0);
//	TCHAR *szSite = new TCHAR[100];
//	_tcscpy(szSite, TEXT("http://messui.the-chronicles.org/onlinehelp/"));
//	_tcscat(szSite, t_chapter);
//	_tcscat(szSite, TEXT(".html"));
//	ShellExecute(wnd, TEXT("open"), TEXT("http://www.microsoft.com/directx"), TEXT(""), NULL, SW_SHOWNORMAL);
	ShellExecute(wnd, TEXT("open"), t_chapter, TEXT(""), NULL, SW_SHOWNORMAL);
	osd_free(t_chapter);
//	osd_free(szSite);
}



//============================================================
//  help_about_mess
//============================================================

static void help_about_mess(HWND wnd)
{
	//help_display(wnd, "mess.chm::/windows/main.htm"); //doesnt do anything
	//help_display(wnd, "mess.chm");
	help_display(wnd, "http://www.mess.org/");
}



//============================================================
//  help_about_thissystem
//============================================================

static void help_about_thissystem(running_machine &machine, HWND wnd)
{
	char buf[100];
//	snprintf(buf, ARRAY_LENGTH(buf), "mess.chm::/sysinfo/%s.htm", machine.system().name);
//	snprintf(buf, ARRAY_LENGTH(buf), "http://messui.the-chronicles.org/onlinehelp/%s.html", machine.system().name);
	snprintf(buf, ARRAY_LENGTH(buf), "http://www.progettoemma.net/mess/system.php?machine=%s", machine.system().name);
	help_display(wnd, buf);
}



//============================================================
//  decode_deviceoption
//============================================================

static device_image_interface *decode_deviceoption(running_machine &machine, int command, int *devoption)
{
	command -= ID_DEVICE_0;
	int absolute_index = command / DEVOPTION_MAX;

	if (devoption)
		*devoption = command % DEVOPTION_MAX;

	image_interface_iterator iter(machine.root_device());
	return iter.byindex(absolute_index);
}



//============================================================
//  set_window_orientation
//============================================================

static void set_window_orientation(win_window_info *window, int orientation)
{
	window->m_target->set_orientation(orientation);
	if (window->m_target->is_ui_target())
	{
		render_container::user_settings settings;
		window->machine().render().ui_container().get_user_settings(settings);
		settings.m_orientation = orientation;
		window->machine().render().ui_container().set_user_settings(settings);
	}
	window->update();
}



//============================================================
//  pause_for_command
//============================================================

static int pause_for_command(UINT command)
{
	// we really should be more conservative and only pause for commands
	// that do dialog stuff
	return (command != ID_OPTIONS_PAUSE);
}



//============================================================
//  invoke_command
//============================================================

static int invoke_command(HWND wnd, UINT command)
{
	std::string error_string;
	int handled = 1;
	int dev_command = 0;
	device_image_interface *img;
	ioport_field *field;
	ioport_setting *setting;
	LONG_PTR ptr = GetWindowLongPtr(wnd, GWLP_USERDATA);
	win_window_info *window = (win_window_info *)ptr;
	ioport_field::user_settings settings;

	// pause while invoking certain commands
	if (pause_for_command(command))
		winwindow_ui_pause_from_window_thread(window->machine(), TRUE);

	switch(command)
	{
		case ID_FILE_LOADSTATE_NEWUI:
			state_load(wnd, window->machine());
			break;

		case ID_FILE_SAVESTATE:
			state_save(window->machine());
			break;

		case ID_FILE_SAVESTATE_AS:
			state_save_as(wnd, window->machine());
			break;

		case ID_FILE_SAVESCREENSHOT:
			window->machine().video().save_active_screen_snapshots();
			break;

		case ID_FILE_UIACTIVE:
			window->machine().set_ui_active(!window->machine().ui_active());
			break;

		case ID_FILE_EXIT_NEWUI:
			window->machine().schedule_exit();
			break;

		case ID_EDIT_PASTE:
			PostMessage(wnd, WM_PASTE, 0, 0);
			break;

		case ID_KEYBOARD_NATURAL:
			window->machine().ui().set_use_natural_keyboard(TRUE);
			break;

		case ID_KEYBOARD_EMULATED:
			window->machine().ui().set_use_natural_keyboard(FALSE);
			break;

		case ID_KEYBOARD_CUSTOMIZE:
			customise_keyboard(window->machine(), wnd);
			break;

		case ID_VIDEO_ROTATE_0:
			set_window_orientation(window, ROT0);
			break;

		case ID_VIDEO_ROTATE_90:
			set_window_orientation(window, ROT90);
			break;

		case ID_VIDEO_ROTATE_180:
			set_window_orientation(window, ROT180);
			break;

		case ID_VIDEO_ROTATE_270:
			set_window_orientation(window, ROT270);
			break;

		case ID_OPTIONS_PAUSE:
			pause(window->machine());
			break;

		case ID_OPTIONS_HARDRESET:
			window->machine().schedule_hard_reset();
			break;

		case ID_OPTIONS_SOFTRESET:
			window->machine().schedule_soft_reset();
			break;

#if HAS_PROFILER
		case ID_FILE_PROFILER:
			window->machine().ui().set_show_profiler(window->machine().ui().show_profiler());
			break;
#endif // HAS_PROFILER

		case ID_FILE_DEBUGGER:
			debug_cpu_get_visible_cpu(window->machine())->debug()->halt_on_next_instruction("User-initiated break\n");
			break;

		case ID_OPTIONS_CONFIGURATION:
			customise_configuration(window->machine(), wnd);
			break;

		case ID_OPTIONS_DIPSWITCHES:
			customise_dipswitches(window->machine(), wnd);
			break;

		case ID_OPTIONS_MISCINPUT:
			customise_miscinput(window->machine(), wnd);
			break;

		case ID_OPTIONS_ANALOGCONTROLS:
			customise_analogcontrols(window->machine(), wnd);
			break;

		case ID_FILE_OLDUI:
			window->machine().ui().show_menu();
			break;

		case ID_FILE_FULLSCREEN:
			winwindow_toggle_full_screen();
			break;

		case ID_OPTIONS_TOGGLEFPS:
			window->machine().ui().set_show_fps(!window->machine().ui().show_fps());
			break;

		case ID_OPTIONS_USEMOUSE:
			{
				// FIXME
//              extern int win_use_mouse;
//              win_use_mouse = !win_use_mouse;
			}
			break;

		case ID_FILE_TOGGLEMENUBAR:
			win_toggle_menubar();
			break;

		case ID_FRAMESKIP_AUTO:
			window->machine().video().set_frameskip(-1);
			window->machine().options().emu_options::set_value(OPTION_AUTOFRAMESKIP, 1, OPTION_PRIORITY_CMDLINE, error_string);
			break;

		case ID_HELP_ABOUT_NEWUI:
			help_about_mess(wnd);
			break;

		case ID_HELP_ABOUTSYSTEM:
			help_about_thissystem(window->machine(), wnd);
			break;

		case ID_THROTTLE_50:
			set_speed(window->machine(), 500);
			break;

		case ID_THROTTLE_100:
			set_speed(window->machine(), 1000);
			break;

		case ID_THROTTLE_200:
			set_speed(window->machine(), 2000);
			break;

		case ID_THROTTLE_500:
			set_speed(window->machine(), 5000);
			break;

		case ID_THROTTLE_1000:
			set_speed(window->machine(), 10000);
			break;

		case ID_THROTTLE_UNTHROTTLED:
			set_speed(window->machine(), 0);
			break;

		default:
			if ((command >= ID_FRAMESKIP_0) && (command < ID_FRAMESKIP_0 + frameskip_level_count(window->machine())))
			{
				// change frameskip
				window->machine().video().set_frameskip(command - ID_FRAMESKIP_0);
				window->machine().options().emu_options::set_value(OPTION_AUTOFRAMESKIP, 0, OPTION_PRIORITY_CMDLINE, error_string);
				window->machine().options().emu_options::set_value(OPTION_FRAMESKIP, (int)command - ID_FRAMESKIP_0, OPTION_PRIORITY_CMDLINE, error_string);
			}
			else
			if ((command >= ID_DEVICE_0) && (command < ID_DEVICE_0 + (IO_COUNT*DEVOPTION_MAX)))
			{
				// change device
				img = decode_deviceoption(window->machine(), command, &dev_command);
				device_command(wnd, img, dev_command);
			}
			else
			if ((command >= ID_JOYSTICK_0) && (command < ID_JOYSTICK_0 + MAX_JOYSTICKS))
				// customise joystick
				customise_joystick(window->machine(), wnd, command - ID_JOYSTICK_0);
			else
			if ((command >= ID_VIDEO_VIEW_0) && (command < ID_VIDEO_VIEW_0 + 1000))
			{
				// render views
				window->m_target->set_view(command - ID_VIDEO_VIEW_0);
				window->update(); // actually change window size
			}
			else
			if (input_item_from_serial_number(window->machine(), command - ID_INPUT_0, NULL, &field, &setting))
				// should never happen
				handled = 0;
			else
				// bogus command
				handled = 0;

			break;
	}

	// resume emulation
	if (pause_for_command(command))
		winwindow_ui_pause_from_window_thread(window->machine(), FALSE);

	return handled;
}



//============================================================
//  set_menu_text
//============================================================

static void set_menu_text(HMENU menu_bar, int command, const char *text)
{
	TCHAR *t_text;
	MENUITEMINFO mii;

	// convert to TCHAR
	t_text = tstring_from_utf8(text);

	// invoke SetMenuItemInfo()
	memset(&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_TYPE;
	mii.dwTypeData = t_text;
	SetMenuItemInfo(menu_bar, command, FALSE, &mii);

	// cleanup
	osd_free(t_text);
}



//============================================================
//  win_setup_menus
//============================================================

static int win_setup_menus(running_machine &machine, HMODULE module, HMENU menu_bar)
{
	HMENU frameskip_menu;
	char buf[256];
	int i = 0;

	// verify that our magic numbers work
	assert((ID_DEVICE_0 + IO_COUNT * DEVOPTION_MAX) < ID_JOYSTICK_0);

	// initialize critical values
	joystick_menu_setup = 0;

	// remove the profiler menu item if it doesn't exist
#if HAS_PROFILER
	machine.ui().show_profiler();
#else
	DeleteMenu(menu_bar, ID_FILE_PROFILER, MF_BYCOMMAND);
#endif

	if ((machine.debug_flags & DEBUG_FLAG_ENABLED) == 0)
		DeleteMenu(menu_bar, ID_FILE_DEBUGGER, MF_BYCOMMAND);

	// set up frameskip menu
	frameskip_menu = find_sub_menu(menu_bar, "&Options\0&Frameskip\0", FALSE);

	if (!frameskip_menu)
		return 1;

	for(i = 0; i < frameskip_level_count(machine); i++)
	{
		snprintf(buf, ARRAY_LENGTH(buf), "%i", i);
		win_append_menu_utf8(frameskip_menu, MF_STRING, ID_FRAMESKIP_0 + i, buf);
	}

	// set the help menu to refer to this machine
	snprintf(buf, ARRAY_LENGTH(buf), "About %s (%s)...", machine.system().description, machine.system().name);
	set_menu_text(menu_bar, ID_HELP_ABOUTSYSTEM, buf);

	// initialize state_filename for each driver, so we don't carry names in-between them
	{
		char *src;
		char *dst;

		snprintf(state_filename, ARRAY_LENGTH(state_filename),
			"%s State", machine.system().description);

		src = state_filename;
		dst = state_filename;
		do
		{
			if ((*src == '\0') || isalnum(*src) || isspace(*src) || strchr("(),.", *src))
				*(dst++) = *src;
		}
		while(*(src++));
	}

	return 0;
}



//============================================================
//  win_resource_module
//============================================================

static HMODULE win_resource_module(void)
{
	static HMODULE module;
	if (module == NULL)
	{
		MEMORY_BASIC_INFORMATION info;
		if ((VirtualQuery((const void*)win_resource_module, &info, sizeof(info))) == sizeof(info))
			module = (HMODULE)info.AllocationBase;
	}
	return module;
}

///// Interface to the core /////

//============================================================
//  win_create_menu
//============================================================

int win_create_menu(running_machine &machine, HMENU *menus)
{
	// Get the path for loose software from <gamename>.ini
	// if this is invalid, then windows chooses whatever directory it used last.
	const char* t = machine.options().emu_options::comment_directory();
	// This pulls out the first path from a multipath field
	const char * t1 = strtok((char*)t, ";");
	if (t1)
		software_dir = t1; // the first path of many
	else
		software_dir = t; // the only path

	HMENU menu_bar = NULL;
	HMODULE module;

	module = win_resource_module();
	menu_bar = LoadMenu(module, MAKEINTRESOURCE(IDR_RUNTIME_MENU));
	if (!menu_bar)
		goto error;

	if (win_setup_menus(machine, module, menu_bar))
		goto error;

	*menus = menu_bar;
	return 0;

error:
	if (menu_bar)
		DestroyMenu(menu_bar);
	return 1;
}



//============================================================
//  winwindow_video_window_proc_ui
//============================================================

LRESULT CALLBACK winwindow_video_window_proc_ui(HWND wnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
		case WM_INITMENU:
			prepare_menus(wnd);
			break;

		case WM_PASTE:
			{
				LONG_PTR ptr = GetWindowLongPtr(wnd, GWLP_USERDATA);
				win_window_info *window = (win_window_info *)ptr;
				//ui_manager ui(window->machine());
				window->machine().ui().paste();
			}
			break;

		case WM_COMMAND:
			if (invoke_command(wnd, wparam))
				break;
			/* fall through */

		default:
			return win_window_info::video_window_proc(wnd, message, wparam, lparam);
	}
	return 0;
}

