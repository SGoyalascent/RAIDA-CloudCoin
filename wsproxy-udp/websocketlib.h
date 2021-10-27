#ifndef WEBSOCKETLIB_H
#define WEBSOCKETLIB_H




void traffic(char * token);
int encode_hixie(u_char const *src, size_t srclength,
                 char *target, size_t targsize);
int encode_hybi(u_char const *src, size_t srclength,
                char *target, size_t targsize, unsigned int opcode);
int decode_hixie(char *src, size_t srclength,
                 u_char *target, size_t targsize,
                 unsigned int *opcode, unsigned int *left);
int decode_hybi(unsigned char *src, size_t srclength,
                u_char *target, size_t targsize,
                unsigned int *opcode, unsigned int *left);
int resolve_host(struct in_addr *sin_addr, const char *hostname);
void start_server();


#endif