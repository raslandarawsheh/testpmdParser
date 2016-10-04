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
	if (!str || !out)
		return -1;
	int i, len = 0;
	char *end = NULL;
	intmax_t spec;
	char tmp[out->size];
	char desc[out->size];
	char mask[out->size];
	char limit[out->size];
	char old = 'n';
	for (i = 0; i < out->size; i++)
		mask[i] = 0xff;

	char *start = (char *)&str[0];

	while (1) {
		errno = 0;
		spec = strtoimax(start, &end, 0);

		if (errno)
			return -1;

		if (*end == '/' || *end == '-' ||
			isspace((char)*end) || !(*end) ) {
			switch (out->size) {
			case sizeof(int8_t):
					if (spec > INT8_MAX ||
						spec < INT8_MIN)
						return -1;
					*(int8_t *)tmp = (int8_t)spec;
					break;
			case sizeof(int16_t):
					if (spec > INT16_MAX ||
						spec < INT16_MIN)
						return -1;
					*(int16_t *)tmp = (int16_t)spec;
					break;
			case sizeof(int32_t):
					if (spec > INT32_MAX ||
						spec < INT32_MIN)
						return -1;
					*(int32_t *)tmp = (int32_t)spec;
					printf("tmp = %d\n", *(int32_t *)tmp);
					break;
			case sizeof(int64_t):
					if (spec > INT64_MAX ||
						spec < INT64_MIN)
						return -1;
					*(int64_t *)tmp = (int64_t)spec;
					break;
			default:
				return -1;
			}

			if (*end == '/') {
				memcpy(desc, tmp, out->size);
				memcpy(limit, tmp, out->size);
			} else if (*end == '-') {
					memcpy(desc, tmp, out->size);
			} else if (old == '/') {
				memcpy(mask, tmp, out->size);
				break;
			} else if (old == '-') {
				memcpy(limit, tmp, out->size);
				break;
			} else if (!(*end) || isspace((char)*end))
				break;
			old = (*end ? *end : 'n');
			start = end + 1;
		} else
			return -1;
	}

	if (old == 'n') {
		memcpy(desc, tmp, out->size);
		memcpy(limit, tmp, out->size);
	}

	if (out->desc)
		memcpy(out->desc, desc, out->size);

	if (out->mask)
		memcpy(out->mask, mask, out->size);

	if (out->limit)
		memcpy(out->limit, limit, out->size);

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




