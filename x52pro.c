/* x52pro driver library
 * Copyright (C) 2007 Eduard Hasenleithner <eduard@hasenleithner.at>
 * Licensed under the LGPL. Please see "COPYING".
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libusb.h>
#include "x52pro.h"

struct x52 {
	libusb_device_handle *hdl;
	enum x52_type type;
	unsigned feat_mfd:1;
	unsigned feat_led:1;
	unsigned feat_sec:1;
	unsigned debug:1;
};

#define X52PRO_REQUEST 0x91

#define X52PRO_CLEAR1 0xd9
#define X52PRO_CLEAR2 0xda
#define X52PRO_CLEAR3 0xdc
#define X52PRO_WRITE1 0xd1
#define X52PRO_WRITE2 0xd2
#define X52PRO_WRITE3 0xd4
#define X52PRO_SETLED 0xb8
#define X52PRO_MFDBRI 0xb1
#define X52PRO_LEDBRI 0xb2

#define X52PRO_TIME   0xc0
#define X52PRO_OFFS2  0xc1
#define X52PRO_OFFS3  0xc2
#define X52PRO_DATE   0xc4
#define X52PRO_YEAR   0xc8

#define YOKE_SECOND   0xca

int write_idx[3] = {
	X52PRO_WRITE1,
	X52PRO_WRITE2,
	X52PRO_WRITE3,
};

int clear_idx[3] = {
	X52PRO_CLEAR1,
	X52PRO_CLEAR2,
	X52PRO_CLEAR3,
};

#define x52printf if (x52->debug) fprintf

int x52_settext(struct x52 *x52, int line, char *text, int length)
{
	if ((!x52) || (!text)) {
		// Reject NULL pointers
		return -10;
	}

	int r;
	if (!x52->feat_mfd) {
		x52printf(stderr, "settext not supported\n");
		return -3;
	}
	if (line > 3) return -1;
	r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST, 0x00, clear_idx[line], NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_settext failed at clear command (%s)\n",
			libusb_strerror(r));
		return -2;
	}

	while (length >= 1) {
		int chars;
		if (length == 1) chars = (' ' << 8) + *text;
		else chars = *(unsigned short*) text;
		r = libusb_control_transfer(x52->hdl,
			LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
			X52PRO_REQUEST, chars, write_idx[line], NULL, 0, 1000);
		if (r != LIBUSB_SUCCESS) {
			x52printf(stderr, "x52_settext failed at write %d (%s)\n",
				length, libusb_strerror(r));
			return -2;
		}
		length -= 2;
		text += 2;
	}
	return 0;
}

int x52_setbri(struct x52 *x52, int mfd, int brightness)
{
	if (!x52) {
		// Reject NULL pointers
		return -10;
	}

	int r;
	if (!x52->feat_mfd) {
		x52printf(stderr, "setbri not supported\n");
		return -3;
	}
	r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST, brightness, mfd ? X52PRO_MFDBRI : X52PRO_LEDBRI,
		NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_setbri failed (%s)\n", libusb_strerror(r));
		return -2;
	}
	return 0;
}

int x52_setled(struct x52 *x52, int led, int on)
{
	if (!x52) {
		// Reject NULL pointers
		return -10;
	}

	int r;
	if (!x52->feat_led) {
		x52printf(stderr, "setled not supported\n");
		return -3;
	}

	on = !!on; // Ensure that only least significant bit is set

	r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST, on | (led<<8), X52PRO_SETLED,
		NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_setled failed (%s)\n", libusb_strerror(r));
		return -2;
	}
	return 0;
}

int x52_settime(struct x52 *x52, int h24, int hour, int minute)
{
	if (!x52) {
		// Reject NULL pointers
		return -10;
	}

	int r;
	r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST, minute | (hour<<8) | (h24?0x8000:0), X52PRO_TIME,
		NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_settime failed (%s)\n", libusb_strerror(r));
		return -2;
	}
	return 0;
}

int x52_setoffs(struct x52 *x52, int idx, int h24, int inv, int offset)
{
	if (!x52) {
		// Reject NULL pointers
		return -10;
	}

	int r;
	r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST,
		offset | (inv?1024:0) | (h24?0x8000:0), idx?X52PRO_OFFS3:X52PRO_OFFS2,
		NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_settime failed (%s)\n", libusb_strerror(r));
		return -2;
	}

	return 0;
}

int x52_setsecond(struct x52 *x52, int second)
{
	if (!x52) {
		// Reject NULL pointers
		return -10;
	}

	int r;
	if (!x52->feat_sec) {
		x52printf(stderr, "setsecond not supported\n");
		return -3;
	}
	r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST, second<<8, YOKE_SECOND,
		NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_setsecond failed (%s)\n", libusb_strerror(r));
		return -2;
	}
	return 0;
}

int x52_setdate(struct x52 *x52, int year, int month, int day)
{
	if (!x52) {
		// Reject NULL pointers
		return -10;
	}

	int r;
	if (!x52->feat_mfd) {
		x52printf(stderr, "setdate not supported\n");
		return -3;
	}
	r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST, day | (month<<8), X52PRO_DATE,
		NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_setdate failed (%s)\n", libusb_strerror(r));
		return -2;
	}
	r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST, year, X52PRO_YEAR,
		NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_setdate failed for year (%s)\n", libusb_strerror(r));
		return -2;
	}
	return 0;
}

int x52_custom(struct x52 *x52, int index, int value)
{
	if (!x52) {
		// Reject NULL pointers
		return -10;
	}

	int r = libusb_control_transfer(x52->hdl,
		LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_RECIPIENT_DEVICE,
		X52PRO_REQUEST, value, index, NULL, 0, 1000);
	if (r != LIBUSB_SUCCESS) {
		x52printf(stderr, "x52_settext failed at clear command (%s)\n",
			libusb_strerror(r));
		return -2;
	}
	return 0;
}

#define VENDOR_SAITEK 0x6a3
#define PRODUCT_X52_0 0x255
#define PRODUCT_X52_1 0x75c
#define PRODUCT_X52PRO 0x762
#define PRODUCT_YOKE 0xbac

struct x52* x52_init(void)
{
	int r;
	struct x52 x52, *x52p;
	libusb_device **device_list;
	libusb_device *joydev = NULL;

	libusb_init_context(NULL, NULL, 0);
	ssize_t num_devices = libusb_get_device_list(NULL, &device_list);

	memset(&x52, 0, sizeof(x52));

	for (int i=0; i<num_devices; i++) {
		struct libusb_device_descriptor desc;
		libusb_device *dev = device_list[i];
		r = libusb_get_device_descriptor(dev, &desc);
		if (desc.idVendor != VENDOR_SAITEK) continue;
		switch (desc.idProduct) {
		case PRODUCT_X52_0:
		case PRODUCT_X52_1:
			x52.feat_mfd = 1;
			x52.type = DEV_X52;
			joydev = dev;
			break;
		case PRODUCT_X52PRO:
			x52.feat_mfd = 1;
			x52.feat_led = 1;
			x52.type = DEV_X52PRO;
			joydev = dev;
			break;
		case PRODUCT_YOKE:
			x52.feat_sec = 1;
			x52.type = DEV_YOKE;
			joydev = dev;
			break;
		}
		if (joydev) break;
	}
	if (!joydev) {
		fprintf(stderr, "joystick not found\n");
		libusb_free_device_list(device_list, 1);
		return NULL;
	}
	r = libusb_open(joydev, &x52.hdl);
	if (r != LIBUSB_SUCCESS) {
		fprintf(stderr, "joystick open failed\n");
		libusb_free_device_list(device_list, 1);
		return NULL;
	}
	libusb_free_device_list(device_list, 1);
	x52p = malloc(sizeof(*x52p));
	*x52p = x52;
	return x52p;
}

enum x52_type x52_gettype(struct x52* hdl)
{
	if (!hdl) {
		// Reject NULL pointers
		return DEV_INVALID;
	}

	return hdl->type;
}

void x52_close(struct x52* x52) 
{
	if (!x52) {
		// Reject NULL pointers
		return;
	}

	libusb_close(x52->hdl);
	free(x52);
}

void x52_debug(struct x52* x52, int debug)
{
	if (!x52) {
		// Reject NULL pointers
		return;
	}

	x52->debug = !!debug;
}
