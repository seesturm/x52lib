/*
 * x52output utility
 * Copyright (C) 2007 Eduard Hasenleithner <eduard@hasenleithner.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x52pro.h>

/* just for debugging */
int x52_custom(struct x52 *x52, int index, int value);

int main(int argc, char**argv)
{
	struct x52 *hdl = x52_init();
	if (hdl==NULL)
		return 1;
	x52_debug(hdl, 1);

	char *prog = argv[0];

	if (argc < 2) {
		printf("usage: %s {text|bri|led|time|offset|date|custom}\n", prog);
		return 0;
	}
	char *cmd = argv[1];

	if (!strcmp(cmd, "text")) {
		if (argc < 4) {
			printf("usage: %s text <line> <text>\n", prog);
			return 0;
		}
		x52_settext(hdl, atoi(argv[2]), argv[3], strlen(argv[3]));
	} else if (!strcmp(cmd, "bri")) {
		if (argc < 4) {
			printf("usage: %s bri <mfd> <value>\n", prog);
			return 0;
		}
		x52_setbri(hdl, atoi(argv[2]), atoi(argv[3]));
	} else if (!strcmp(cmd, "led")) {
		if (argc < 4) {
			printf("usage: %s led <id> <on>\n", prog);
			return 0;
		}
		x52_setled(hdl, atoi(argv[2]), atoi(argv[3]));
	} else if (!strcmp(cmd, "time")) {
		if (argc < 5) {
			printf("usage: %s time <h24> <hour> <min>\n", prog);
			return 0;
		}
		x52_settime(hdl, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	} else if (!strcmp(cmd, "offset")) {
		if (argc < 6) {
			printf("usage: %s time <idx> <h24> <inv> <minutes>\n", prog);
			return 0;
		}
		x52_setoffs(hdl, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]),
			atoi(argv[5]));
	} else if (!strcmp(cmd, "date")) {
		if (argc < 5) {
			printf("usage: %s date <year> <month> <day>\n", prog);
			return 0;
		}
		x52_setdate(hdl, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	} else if (!strcmp(cmd, "second")) {
		if (argc < 3) {
			printf("usage: %s date <year> <month> <day>\n", prog);
			return 0;
		}
		x52_setsecond(hdl, atoi(argv[2]));
	} else if (!strcmp(cmd, "model")) {
		enum x52_type type = x52_gettype(hdl);
		switch (type) {
		case DEV_X52:
			printf("Model: X52 Flight Control System\n");
			break;
		case DEV_X52PRO:
			printf("Model: X52 Pro Flight Control System\n");
			break;
		case DEV_YOKE:
			printf("Model: Pro Flight Yoke\n");
			break;
		}
	} else if (!strcmp(cmd, "custom")) {
		if (argc < 4) {
			printf("usage: %s custom <idx> <value>\n", prog);
			return 0;
		}
		int index = strtol(argv[2], NULL, 16);
		int value = strtol(argv[3], NULL, 16);
		x52_custom(hdl, index, value);
	} else {
		printf("unknown command %s\n", cmd);
	}

	x52_close(hdl);
	return 0;
}
