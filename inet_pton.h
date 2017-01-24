#ifndef INET_PTON_H
#define INET_PTON_H

#include <SDKDDKVer.h>
#if(_WIN32_WINNT < _WIN32_WINNT_WIN7)
#define _WIN32_WINNT _WIN32_WINNT_WINBLUE
#endif
#include <winsock.h>
#include <winsock2.h>
#include <windows.h>
#include <errno.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

//int inet_pton(int af, const char *src, void *dst)
//{
//  struct sockaddr_storage ss;
//  int size = sizeof(ss);
//  wchar_t src_copy[INET6_ADDRSTRLEN+1];

//  mbstowcs(src_copy,src,INET6_ADDRSTRLEN+1);

//  ZeroMemory(&ss, sizeof(ss));
//  /* stupid non-const API */

//  if (WSAStringToAddress( src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
//    switch(af) {
//      case AF_INET:
//    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
//    return 1;
//      case AF_INET6:
//    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
//    return 1;
//    }
//  }
//  return 0;
//}

#endif // INET_PTON_H
