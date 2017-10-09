

#ifndef MCP_H_
#define MCP_H_

char* mcp_read(char address, int bytes);
void mcp_write(char address, char* data, int length);
void mcp_request_to_send(int TXB2, int TXB1, int TXB0);
char mcp_read_status();
void mcp_bit_modify(char address, char mask, char data);
void mcp_reset();



#endif /* MCP_H_ */