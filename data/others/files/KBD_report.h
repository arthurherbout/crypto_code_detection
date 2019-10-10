#ifndef KBD_REPORT_H
#define KBD_REPORT_H

#include "hal_types.h"

/* Macros/Definitions */

//HID report buffer size: 1 modifier, 1 unused, 6 data
#define REPORT_BUFFER_SIZE              8

//Modifier offset in report buffer 
#define MODIFIER_OFFSET                 0

//Data offset in report buffer
#define DATA_OFFSET                     2

//Mark all data slots high
#define ALL_SLOTS                       0xFC

//Taken from hidkbdmservice.h and hidKbdMouse.c
#define HID_KEYBOARD_IN_RPT_LEN         8
#define HID_RPT_ID_KEY_IN               2  // Keyboard input report ID

/* 8-bit register definition */
typedef union
{
  uint8 byte;
  struct
  {
    uint8  b0  :1;
    uint8  b1  :1;
    uint8  b2  :1;
    uint8  b3  :1;
    uint8  b4  :1;
    uint8  b5  :1;
    uint8  b6  :1;
    uint8  b7  :1;
  }bit;
} KBD_8bitreg_t;

/* HID keyboard buffer */
typedef struct {
  bool          update;                         //update report
  KBD_8bitreg_t emptySlots;                     //empty slots
  uint8         size;                           //queue size
  uint8         Buff[REPORT_BUFFER_SIZE];       //circular buffer elements
} ReportBuffer_t;


/* Public functions */
extern void KBD_Report_Init(void);
extern bool KBD_Report_AddKey(uint8 keycode);
extern bool KBD_Report_RemoveKey(uint8 keycode);
extern void KBD_Report_Update(void);

#endif /* KBD_REPORT_H */