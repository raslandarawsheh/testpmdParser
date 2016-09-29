#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	printf("len = %d\n, p->desc = %d\n", parse_int(argv[2], &p), p.desc);
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
parse_int (const char *str, struct parse_output *out){
	if (!str)
		return -1;
	int idx = 0, len = 0;
	char c;
	int hex = 0;
	for (len =0; str[len]; len++){
		c = str[len];
		if (isspace(c) || c == '#')
			break;

		if (c == 'x'){
			if (!hex){
				hex = 1;
				if (len != 1 &&
					str[len - 1] != '0')
					return -1;
				continue;
			}
		}
		if (c == '/' || c == '-') {
			printf("ranges\n");
			idx = len;
			if (hex) {
			len += 2;
			if (!str[len - 1] || !str[len] ||
				(str[len - 1] != '0' && str[len] != 'x'))
				return -1;
			}
			continue;
		}
		if (char_to_value(c) == 0xff)
			return -1;
	}
	if (!out)
		return len;
	printf("%s\n", str);	
	int base = (hex ? 16 : 10);
	int value1;
	int value = strtol(str, NULL, base);
	if (idx)
		value1 = strtol(&str[idx + 1], NULL, base);
		printf("value1 = %d\n", value1);
	printf("value = %d\nout->size =%d\n", value, out->size);
	if ((out->size != 1 && out->size != 2 &&
		out->size != 4 && out->size != 8) ||
		(hex && len <= 2))
		return -1;
	if (out->desc)
		memcpy(out->desc, &value, out->size);
	return len;
}
int
parse_ipv4(const char *str, struct parse_output *out){
	return 0;
}
int
parse_ipv6(const char *str, struct parse_output *out){
	return 0;
}
int
parse_mac(const char *str, struct parse_output *out){
	return 0;
}



