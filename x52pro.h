/**
@file
	x52pro driver library
@copyright
	Copyright (C) 2007 Eduard Hasenleithner <eduard@hasenleithner.at>
	Licensed under the LGPL. Please see "COPYING".
	*/

#pragma once

/**	Opaque handle to device */
struct x52;

/** device model indicator */
enum x52_type {
	DEV_X52, /**< Original X52 */
	DEV_X52PRO, /**< PRO version of X52 */
	DEV_YOKE, /**< Flight YOKE */
};

#define X52PRO_LED_FIRE     1 /**< @brief RED/GREEN LED of Fire Button */
#define X52PRO_LED_ARED     2 /**< @brief RED LED of A Button */
#define X52PRO_LED_AGREEN   3 /**< @brief GREEN LED of A Button */
#define X52PRO_LED_BRED     4 /**< @brief RED LED of B Button */
#define X52PRO_LED_BGREEN   5 /**< @brief GREEN LED of B Button */
#define X52PRO_LED_DRED     6 /**< @brief RED LED of D Button */
#define X52PRO_LED_DGREEN   7 /**< @brief GREEN LED of D Button */
#define X52PRO_LED_ERED     8 /**< @brief RED LED of E Button */
#define X52PRO_LED_EGREEN   9 /**< @brief GREEN LED of E Button */
#define X52PRO_LED_T1RED   10 /**< @brief RED LED of T1 Switch */
#define X52PRO_LED_T1GREEN 11 /**< @brief GREEN LED of T1 Switch */
#define X52PRO_LED_T2RED   12 /**< @brief RED LED of T2 Switch */
#define X52PRO_LED_T2GREEN 13 /**< @brief GREEN LED of T2 Switch */
#define X52PRO_LED_T3RED   14 /**< @brief RED LED of T3 Switch */
#define X52PRO_LED_T3GREEN 15 /**< @brief GREEN LED of T3 Switch */
#define X52PRO_LED_CORED   16 /**< @brief RED LED of Coolie Hat */
#define X52PRO_LED_COGREEN 17 /**< @brief GREEN LED of Coolie Hat */
#define X52PRO_LED_IRED    18 /**< @brief RED LED of i Button */
#define X52PRO_LED_IGREEN  19 /**< @brief GREEN LED of i Button */

/**	This function searches for a supported device and
	connects to the first one found.
@brief Open device and provide handle
@return handle to device if opened successful
@retval NULL device could not be opened
	*/
struct x52* x52_init(void);

/**When debug is activated the library provides error messages
	on stderr.
@brief Debug activation function
@param hdl device handle returned by x52_init
@param debug activation/deactivation flag. Use nonzero for activation.
	*/
void x52_debug(struct x52* hdl, int debug);

/**
@brief Get device type
@param hdl device handle returned by x52_init
@return type (DEV_X52, DEV_X52PRO, DEV_YOKE)
	*/
enum x52_type x52_gettype(struct x52* hdl);

/**
@brief Close device and free resources
@param hdl device handle returned by x52_init
 	*/
void x52_close(struct x52* hdl);

/** This function is supported by X52 and X52PRO
@brief Set the text of the MFD
@retval 0 success
@param hdl device handle returned by x52_init
@param line line of text to change
@param text character string of new text
@param length number of characters in string
	*/
int x52_settext(struct x52 *hdl, int line, char *text, int length);

/** This function is supported by X52 and X52PRO
@brief Set the brightness of the MFD or the LEDs
@retval 0 success
@param hdl device handle returned by x52_init
@param mfd
 0: change the LED brightness
 1: change the MFD brightness
@param brightness brightness level (0-127)
	*/
int x52_setbri(struct x52 *hdl, int mfd, int brightness);

/** This function is supported by X52 and X52PRO
@brief Set or clear one of the LEDs
@retval 0 success
@param hdl device handle returned by x52_init
@param led led index (1-19), see X52PRO_LED_XXX defines
@param on 1: activate led, 0: deactivate led
	*/
int x52_setled(struct x52 *hdl, int led, int on);

/** This function is supported by X52 and X52PRO and YOKE
@brief Set the MFD display time
@retval 0 success
@param hdl device handle returned by x52_init
@param h24 boolean, 24h output (1=on, 0=off)
@param hour value (0-23)
@param minute value (0-60)
    */
int x52_settime(struct x52 *hdl, int h24, int hour, int minute);

/** This function is supported by X52 and X52PRO
@brief Set time 2/3 offset
@retval 0 success
@param hdl device handle returned by x52_init
@param idx offset index (==0 is offs2; >0 is offs3)
@param inv invert offset (1=invert, 0=normal)
@param offset time offset in minutes (0..1023)
    */
int x52_setoffs(struct x52 *hdl, int idx, int h24, int inv, int offset);

/** This function is only supported by YOKE
@brief Set second of saitek yoke
@retval 0 success
@param hdl device handle returned by x52_init
@param second second (0-59, actually higher values are also accepted)
    */
int x52_setsecond(struct x52 *x52, int second);

/** This function is supported by X52 and X52PRO
@brief Set the MFD display date
@retval 0 success
@param hdl device handle returned by x52_init
@param year value (0-99)
@param month value (1-12)
@param day value (1-31)
    */
int x52_setdate(struct x52 *hdl, int year, int month, int day);

