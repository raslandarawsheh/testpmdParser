#ifndef PARSER_H_
#define PARSER_H_

struct parse_output {
     unsigned int size;
     void *desc;
     void *mask;
     void *limit;
 };
int parse_int(const char *str, struct parse_output *out);
int parse_ipv4(const char *str, struct parse_output *out);
int parse_ipv6(const char *str, struct parse_output *out);
int parse_mac(const char *str, struct parse_output *out);

#endif
