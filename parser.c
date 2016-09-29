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
	printf("p.desc = %u\n", *(uint32_t *)p.desc);
	printf("p.mask = %u\n", *(uint32_t *)p.mask);
	printf("p.limit = %u\n", *(uint32_t *)p.limit);
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
			
		if (c == '/' || c == '-' || c == 'x') {
			continue;
		}
		if (char_to_value(c) == 0xff)
			return -1;
	}
	char *end;
	int i;
	errno = 0;
	//	intmax_t limit = 0;
	intmax_t desc = strtoimax(str, end, 0);
	printf("desc = %d\n", desc);
	char parsed;
	if (isspace(*end) || *end == '#' || !(*end))
		parsed = 'n';
	else if (*end == '/')
		parsed = 'm';
	else if (*end == '-')
		parsed = 'r';
	else
		return -1;
	if (errno || !out || !out->desc)
		return -1;
	
	switch (out->size) {
		case 1:
			*(uint8_t *)out->desc = (uint8_t)desc;
			break;
		case 2:
                        *(uint16_t *)out->desc = (uint16_t)desc;
                        break;
		case 4:
                        *(uint32_t *)out->desc = (uint32_t)desc;
                        break;
		case 8:
                        *(uint64_t *)out->desc = (uint64_t)desc;
                        break;
		default: /* in case the size is invailed size */
			return -1;
	}
	errno = 0;
	intmax_t limit = strtoimax(str, end, 0);
	if (errno)
		return -1;
	char tmp[out->size];
	char tmp1[out->size];
	for (i=0; i <out->size; i++)
		tmp1[i] = 0xff;
	switch (out->size) {
		case 1:
			*(uint8_t *)tmp = (uint8_t)limit;
			break;
		case 2:
			*(uint16_t *)tmp = (uint16_t)limit;
			break;
		case 4: 
			*(uint32_t *)tmp = (uint32_t)limit;
			break;
		case 8:
			*(uint64_t *)tmp = (uint64_t)limit;
			break;
		default:
			break;
	}
	switch (parsed){
		case 'm':
			if (out->mask)
				out->mask = tmp;
			if (out->limit)
				memcpy(out->desc, out->limit, out->size);
			break;
		case 'r':
			if (out->mask)
				out->mask = tmp1;
			if (out->limit)
				out->limit = tmp;
			break;
		default:
			if (out->mask)
				out->mask = tmp1;
			if (out->limit)
				memcpy(out->desc, out->limit, out->size);
			break;
	}
	
	printf("%s\n", str);
	/*uint64_t limit = -1, tmp = -1;
	uint64_t desc = strtol(str, NULL, 0);

	printf("desc = %d\nout->size =%d\n", desc, out->size);
	if ((out->size != 1 && out->size != 2 &&
		out->size != 4 && out->size != 8) ||
		(hex && len <= 2) || (sizeof(desc) < out->size))
		return -1;
	if (out->desc)
		memcpy(out->desc, &desc, out->size);
	if (idx) {
		limit = strtol(&str[idx + 1], NULL, 0);
		if (out->mask)
			if (str[idx] == '-')
				memcpy(out->mask, &tmp, out->size);
				if (out->limit)
					memcpy(out->limit, &limit, out->size);
			else{
				memcpy(out->mask, &limit, out->size);
				memcpy(out->limit, &desc, out->size);
			}
	}
	else{
		if (out->mask)
			memcpy(out->mask, &tmp, out->size);
		if (out->limit)
			memcpy(out->limit, &desc, out->size);
	}
	if (out->limit)
		memcpy(out->limit, &limit, out->size);
	*/
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



