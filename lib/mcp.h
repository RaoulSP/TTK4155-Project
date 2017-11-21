#ifndef MCP_H_
#define MCP_H_

#include "MCP2515.h"

char mcp_read(char address);
void mcp_write(char address, char data);
void mcp_request_to_send(int TXB2, int TXB1, int TXB0);
char mcp_read_status();
void mcp_bit_modify(char address, char mask, char data);
void mcp_reset();
void mcp_init();
int mcp_test();

#endif /* MCP_H_ */