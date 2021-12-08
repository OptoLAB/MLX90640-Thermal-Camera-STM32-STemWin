/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#include "DIALOG.h"
#include <stdint.h>

#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_BUTTON_2   (GUI_ID_USER + 0x01)
#define ID_EDIT_0     (GUI_ID_USER + 0x07)
#define ID_EDIT_1     (GUI_ID_USER + 0x08)
#define ID_EDIT_2     (GUI_ID_USER + 0x03)
#define ID_EDIT_3     (GUI_ID_USER + 0x04)
#define ID_EDIT_4     (GUI_ID_USER + 0x05)
#define ID_TEXT_3     (GUI_ID_USER + 0x06)
#define ID_IMAGE_0	  (GUI_ID_USER + 0x09)
#define ID_IMAGE_1	  (GUI_ID_USER + 0x0A)
#define ID_IMAGE_2	  (GUI_ID_USER + 0x0B)
#define ID_IMAGE_3	  (GUI_ID_USER + 0x0C)
#define ID_BUTTON_3	  (GUI_ID_USER + 0x0D)

// USER START (Optionally insert additional static data)
void updateLabels(WM_HWIN hWin, float min, float max);
void invalidateLegend(WM_HWIN hWin);
void invalidateImage(WM_HWIN hWin);
WM_HWIN CreateMainWindow(void);
// USER END

