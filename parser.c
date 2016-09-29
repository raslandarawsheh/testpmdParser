#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int
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
	printf("p->desc = %d\n", *(uint32_t *)p.desc);
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

		if (c == 'x') {
			if (!hex || idx) {
				hex = 1;
				if (len < 1 ||
					str[len - 1] != '0' ||
					!str[len + 1])
					return -1;
				continue;
			}
		}
		if (c == '/' || c == '-') {
			rng = c;
			idx = len;
			continue;
		}
		if (char_to_value(c) == 0xff)
			return -1;
	}
	if (!out)
		return len;
	printf("%s\n", str);
	int limit = -1;
	int desc = strtol(str, NULL, 0);
	if (idx)
		limit = strtol(&str[idx + 1], NULL, 0);
		printf("limit = %d\n", limit);
	printf("value = %d\nout->size =%d\n", desc, out->size);
	if ((out->size != 1 && out->size != 2 &&
		out->size != 4 && out->size != 8) ||
		(hex && len <= 2))
		return -1;
	if (out->desc)
		memcpy(out->desc, &desc, out->size);
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



