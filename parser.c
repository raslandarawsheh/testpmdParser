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

	struct parse_output p = {
		.size = sizeof(buf[0]),
		.desc = buf[0],
		.mask = buf[1],
		.limit = buf[2],
	};
	printf("len = %d\n", parse_int(argv[2], &p));
	printf("p.desc = %u\n", *(int32_t *)p.desc);
	printf("p.mask = %u\n", *(int32_t *)p.mask);
	printf("p.limit = %u\n", *(int32_t *)p.limit);
	return 0;
}

int
char_to_value(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <='f')
		return c - 'a' + 10;
	if (c > 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0xff; 
}
int
parse_int (const char *str, struct parse_output *out)
{
	int i;
	char *end = NULL;
	char operation = 0;
	intmax_t spec;
	intmax_t  tmp[2];
	uintmax_t mask = UINTMAX_MAX;
	char *start = (char *)&str[0];

	for (i = 0; i < 2; i++) {
		errno = 0;
		spec = strtoimax(start, &end, 0);

		if (errno || (end == start))
			return -1;

		switch (out->size) {
		case sizeof(int8_t):
			if (spec > INT8_MAX ||
				spec < INT8_MIN)
				return -1;
			tmp[i] = (int8_t)spec;
			break;
		case sizeof(int16_t):
			if (spec > INT16_MAX ||
				spec < INT16_MIN)
				return -1;
			tmp[i] = (int16_t)spec;
			break;
		case sizeof(int32_t):
			if (spec > INT32_MAX ||
				spec < INT32_MIN)
				return -1;
			tmp[i] = (int32_t)spec;
			break;
		case sizeof(int64_t):
			if (spec > INT64_MAX ||
				spec < INT64_MIN)
				return -1;
			tmp[i] = (int64_t)spec;
			break;
		default:
			return -1;
		}
		if (isspace((char)*end) || !(*end))
			break;
		if ((*end != '/' && *end != ',') ||
			operation)
			return -1;
		start = end + 1;
		operation = *end;
	}

	if (out->desc)
		memcpy(out->desc, &tmp[0], out->size);

	switch (operation) {
	case '/':
		if (out->mask)
			memcpy(out->mask, &tmp[1], out->size);
		if (out->limit)
			memcpy(out->limit, &tmp[0], out->size);
		break;
	case ',':
		if (out->mask)
			memcpy(out->mask, &mask, out->size);
		if (out->limit)
			memcpy(out->limit, &tmp[1], out->size);
		break;
	default:
		if (out->limit)
			memcpy(out->limit, &tmp[0], out->size);
		break;
	}

	return end - str;
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




