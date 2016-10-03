#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
extern int errno;
main(int argc, char ** argv){
	if (argc != 3)
		return 1;
	char buf[3][atoi(argv[1])];

	struct parse_output p ={
		.size = 4,
		.desc = buf[0],
		.mask = buf[1],
		.limit = buf[2],
	};
	//printf("parse_output = %d \n%s\n", atoi(argv[1]), argv[2]);
	printf("len = %d\n", parse_int(argv[2], &p));
	printf("p.desc = %u\n", *(int32_t *)p.desc);
	printf("p.mask = %u\n", *(int32_t *)p.mask);
	printf("p.limit = %u\n", *(int32_t *)p.limit);
	return 0;
}

int
char_to_value(char c){
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <='f')
		return c - 'a' + 10;
	if (c > 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0xff; 
}
int
parse_int (const char *str, struct parse_output *out) {
	if (!str)
		return -1;
	int idx = 0, len = 0;
	char c, rng;
	int hex = 0;

 	for (len =0; str[len]; len++){
		c = str[len];
		if (isspace(c) || c == '#')
			break;

		if ((c == '/' || c == '-' || c == 'x') && len > 0)
			continue;

		if (char_to_value(c) == 0xff)
			return -1;
	}

	if (!len)
		return -1;

	char *end = NULL;
	int i;
	errno = 0;
	intmax_t desc = strtoimax(str, &end, 0);

	char parsed;
	if (errno || !out || !out->desc)
		return len;

	if (!end || !(*end) || isspace((char)*end) || *end == '#')
		parsed = 'n';
	else if (*end == '/')
		parsed = 'm';
	else if (*end == '-')
		parsed = 'r';
	else
		return -1;

	switch (out->size) {
		case 1:
			if ((int8_t)desc > INT8_MAX || (int8_t)desc < INT8_MIN)
				return -1;
			*(int8_t *)out->desc = (int8_t)desc;
			break;
		case 2:
			if ((int16_t)desc > INT16_MAX || (int16_t)desc < INT16_MIN)
                        	return -1;
			*(int16_t *)out->desc = (int16_t)desc;
			break;
		case 4:
			if ((int32_t)desc > INT32_MAX || (int32_t)desc < INT32_MIN)
				return -1;
			*(int32_t *)out->desc = (int32_t)desc;		
                        break;
		case 8:
			if ((int64_t)desc > INT64_MAX || (int64_t)desc < INT64_MIN)
				return -1;
                        *(int64_t *)out->desc = (int64_t)desc;
                        break;
		default: /* in case the size is invailed size */
			return -1;
	}
	errno = 0;
	end ++;
	char * newstart = end;
	end = NULL;
	char tmp1[out->size];
        for (i=0; i <out->size; i++)
                tmp1[i] = 0xff;

	if (parsed == 'n'){
		if (out->mask)
			out->mask = tmp1;
		if (out->limit)
			memcpy(out->limit, out->desc, out->size);
		return len;
	}
	intmax_t limit = strtoimax(newstart, &end, 0);
	if (errno || !end)
		return len;
	char tmp[out->size];

	switch (out->size) {
		case 1:
			if ((int8_t)limit > INT8_MAX || (int8_t)limit < INT8_MIN)
				return -1;
			*(int8_t *)tmp = (int8_t)limit;
			break;
		case 2:
			if ((int16_t)limit > INT16_MAX || (int16_t)limit < INT16_MIN)
				return -1;
			*(int16_t *)tmp = (int16_t)limit;
			break;
		case 4:
			if ((int32_t)limit > INT32_MAX || (int32_t)limit < INT32_MIN)
				return -1;
			*(int32_t *)tmp = (int32_t)limit;
			break;
		case 8:
			if ((int64_t)limit > INT64_MAX || (int64_t)limit < INT64_MIN)
				return -1;
			*(int64_t *)tmp = (int64_t)limit;
			break;
		default:
			break;
	}
	switch (parsed){
		case 'm':
			if (out->mask)
				memcpy(out->mask, tmp, out->size);
			if (out->limit)
				memcpy(out->limit, out->desc, out->size);
			break;
		case 'r':
			if (out->mask)
				memcpy(out->mask, tmp1, out->size);
			if (out->limit)
				memcpy(out->limit, tmp, out->size);
			break;
		default:
			if (out->mask)
				memcpy(out->mask, tmp1, out->size);
			if (out->limit)
				memcpy(out->limit, out->desc, out->size);
			break;
	}
 
	return len;
}
int
parse_ipv4(const char *str, struct parse_output *out) {
	return 0;
}
int
parse_ipv6(const char *str, struct parse_output *out) {
	return 0;
}
int
parse_mac(const char *str, struct parse_output *out) {
	return 0;
}



