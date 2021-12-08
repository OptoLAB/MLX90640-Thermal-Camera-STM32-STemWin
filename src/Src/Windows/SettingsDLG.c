/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
#include "SettingsDLG.h"
#include "img.h"

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Set temperature range", ID_FRAMEWIN_0, 25, 75, 360, 200, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "SliderMinTemp", ID_SLIDER_0, 5, 100, 265, 20, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "SliderMaxTemp", ID_SLIDER_1, 5, 60, 265, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextMinTemp", ID_TEXT_0, 5, 85, 60, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextMaxTemp", ID_TEXT_1, 5, 45, 56, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextPallete", ID_TEXT_2, 182, 13, 70, 20, 0, 0x64, 0 },
  { SPINBOX_CreateIndirect, "SpinboxMinTemp", ID_SPINBOX_0, 282, 100, 60, 23, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "SpinboxMaxTemp", ID_SPINBOX_1, 282, 60, 60, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BtnCancel", ID_BUTTON_0, 115, 145, 60, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BtnOk", ID_BUTTON_1, 185, 145, 60, 25, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "CheckAutoscale", ID_CHECKBOX_0, 5, 10, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "CheckInterpolate", ID_CHECKBOX_1, 90, 10, 80, 20, 0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, "DropdownPallete", ID_DROPDOWN_0, 252, 10, 90, 145, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)

/*
*/
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
 // char value[5];
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Texts'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Tmin");

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Tmax");

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Color Pallette");
    //
    // Initialization of 'Spinboxes'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
    SPINBOX_SetFont(hItem, GUI_FONT_13B_1);
    SPINBOX_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x00CA0000));
    SPINBOX_SetRange(hItem, -40, 300);
    SPINBOX_SetValue(hItem, 37);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
    SPINBOX_SetFont(hItem, GUI_FONT_13B_1);
    SPINBOX_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x000000D9));
    SPINBOX_SetRange(hItem, -40, 300);
    SPINBOX_SetValue(hItem, 20);
    //
    // Initialization of 'Sliders'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
	SLIDER_SetRange(hItem, -40, 300);
	SLIDER_SetValue(hItem, getMinTemp());

    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1);
	SLIDER_SetRange(hItem, -40, 300);
	SLIDER_SetValue(hItem, getMaxTemp());
    //
    // Initialization of 'Buttons'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "Cancel");
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText(hItem, "Ok");
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);
    //
    // Initialization of 'Check boxes'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "Autoscale");
    CHECKBOX_SetState(hItem, getAutoscale());

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
    CHECKBOX_SetText(hItem, "Interpolate");
    CHECKBOX_SetState(hItem, getInterpolate());
    //
    // Initialization of 'Dropdown'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
    DROPDOWN_AddString(hItem, "Plasma");
    DROPDOWN_AddString(hItem, "Rainbow");
    DROPDOWN_AddString(hItem, "Seismic");
    DROPDOWN_AddString(hItem, "Hot");
    DROPDOWN_AddString(hItem, "Grayscale");
    DROPDOWN_AddString(hItem, "Viridis");
    DROPDOWN_AddString(hItem, "HotSpot");
    DROPDOWN_AddString(hItem, "ColdSpot");
    DROPDOWN_AddString(hItem, "Hot&Cold");
    DROPDOWN_SetSel(hItem, getColorMode());
    DROPDOWN_SetFont(hItem, GUI_FONT_16_1);

    // USER START (Optionally insert additional code for further widget initialization)

    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_SLIDER_0: // Notifications sent by 'MinTemp'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
		  int val=SLIDER_GetValue(hItem);
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
		  SPINBOX_SetValue(hItem,val);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_SLIDER_1: // Notifications sent by 'MaxTemp'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1);
		  int val=SLIDER_GetValue(hItem);
		  hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
		  SPINBOX_SetValue(hItem,val);

        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'BtnOk'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'BtnOk'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateSettings
*/

WM_HWIN CreateSettings(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
