/* x52pro driver library
 * Copyright (C) 2007 Eduard Hasenleithner <eduard@hasenleithner.at>
 * Licensed under the LGPL. Please see "COPYING".
 */
struct x52;

enum x52_type {DEV_X52, DEV_X52PRO, DEV_YOKE};

#define X52PRO_LED_FIRE     1 /* RED/GREEN LED of Fire Button */
#define X52PRO_LED_ARED     2 /* RED LED of A Button */
#define X52PRO_LED_AGREEN   3 /* GREEN LED of A Button */
#define X52PRO_LED_BRED     4 /* RED LED of B Button */
#define X52PRO_LED_BGREEN   5 /* GREEN LED of B Button */
#define X52PRO_LED_DRED     6 /* RED LED of D Button */ 
#define X52PRO_LED_DGREEN   7 /* GREEN LED of D Button */
#define X52PRO_LED_ERED     8 /* RED LED of E Button */
#define X52PRO_LED_EGREEN   9 /* GREEN LED of E Button */
#define X52PRO_LED_T1RED   10 /* RED LED of T1 Switch */
#define X52PRO_LED_T1GREEN 11 /* GREEN LED of T1 Switch */
#define X52PRO_LED_T2RED   12 /* RED LED of T2 Switch */
#define X52PRO_LED_T2GREEN 13 /* GREEN LED of T2 Switch */
#define X52PRO_LED_T3RED   14 /* RED LED of T3 Switch */
#define X52PRO_LED_T3GREEN 15 /* GREEN LED of T3 Switch */
#define X52PRO_LED_CORED   16 /* RED LED of Coolie Hat */
#define X52PRO_LED_COGREEN 17 /* GREEN LED of Coolie Hat */
#define X52PRO_LED_IRED    18 /* RED LED of i Button */
#define X52PRO_LED_IGREEN  19 /* GREEN LED of i Button */

/* Init function
 * RETURN: handle to x52 if device opened successful */
struct x52* x52_init(void);

/* Debug activation function */
void x52_debug(struct x52* hdl, int debug);

/* Get device type
 * RETURN: type (DEV_X52, DEV_X52PRO, DEV_YOKE) */
enum x52_type x52_gettype(struct x52* hdl);

/* Close function
 * hdl: handle to xte device */
void x52_close(struct x52* hdl);

/* Set the text of the MFD
 * Supported by: X52, X52PRO
 * RETURN: 0 if successful
 * hdl: handle to x52 device
 * line: line of text to change
 * text: character string of new text
 * length: number of characters in string */
int x52_settext(struct x52 *hdl, int line, char *text, int length);

/* Set the brightness of the MFD or the LEDs
 * Supported by: X52, X52PRO
 * RETURN: 0 if successful
 * hdl: handle to x52 device
 * mfd: boolean
 *     0: change the LED brightness
 *     1: change the MFD brightness
 * brightness: value (0-127) */
int x52_setbri(struct x52 *hdl, int mfd, int brightness);

/* Set or clear one of the LEDs
 * Supported by: X52PRO only
 * RETURN: 0 if successful
 * hdl: handle to x52 device
 * led: led index (1-19), see X52PRO_LED_XXX defines
 * on: boolean */
int x52_setled(struct x52 *hdl, int led, int on);

/* Set the MFD display time
 * Supported by: X52, X52PRO, YOKE
 * RETURN: 0 if successful
 * hdl: handle to x52 device
 * h24: boolean, 24h output (1=on, 0=off)
 * hour: value (0-23)
 * minute: value (0-60) */
int x52_settime(struct x52 *hdl, int h24, int hour, int minute);

/* Set time 2/3 offset
 * Supported by: X52?, X52PRO
 * RETURN: 0 if successful
 * hdl: handle to x52 device
 * idx: offset index (==0 is offs2; >0 is offs3)
 * inv: invert offset (1=invert, 0=normal)
 * offset: time offset in minutes (0..1023) */
int x52_setoffs(struct x52 *hdl, int idx, int h24, int inv, int offset);

/* Set second of saitek yoke
 * Supported by: YOKE only
 * RETURN: 0 if successful
 * hdl: handle to x52 device (only yoke!)
 * second: second (0-59, actually higher values are also accepted) */
int x52_setsecond(struct x52 *x52, int second);

/* Set the MFD display date
 * Supported by: X52, X52PRO
 * RETURN: 0 if successful
 * year: value (0-99)
 * month: value (1-12)
 * day: value (1-31) */
int x52_setdate(struct x52 *hdl, int year, int month, int day);

