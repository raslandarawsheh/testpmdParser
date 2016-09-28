#include "parser.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int
main(int argc, char ** argv){
	if (argc != 3)
		return 1;
	char buf[3][argv[1]];
	struct parse_output p ={
		.size =4,
		.desc = buf[0],
		.mask = buf[1],
		.limit = buf[2],
	};
	printf("len = %d", parse_int(argv[2], &p))
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
int parse_int(const char *str, struct parse_output *out){
	if (!str)
		return -1;
	int i, len = 0;
	int hex = 0;
	for (len =0; str[len]; len++){
		c = str[len];
		if (isspace(c) || c == '#')
			break;

		if (c == 'x'){
			if (!hex){
				hex = 1;
				continue;
			}
			if (len != 1)
				return -1;
		}

		if (char_to_value(c) == 0xff)
			return -1;
	}
	if (!out)
		return len;
	printf("%s\n", str);
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

