#ifndef __SOCKET_H
#define __SOCKET_H

void writeSocket(int fd,FCGI_Header *h,unsigned int len);
void writeLen(int len, char **p);
int addNameValuePair(FCGI_Header *h,char *name,char *value);
void sendGetValue(int fd);
void sendBeginRequest(int fd,unsigned short requestId,unsigned short role,unsigned char flags);
void sendAbortRequest(int fd,unsigned short requestId);
void sendWebData(int fd,unsigned char type,unsigned short requestId,char *data,unsigned int len);
int createSocket(int port);

#endif
