#pragma once
/*
* Copyright (c) lang@NWPU
* Description:
* Helper funcions
*/

#include "stdafx.h"
#include "Winuser.h" // MESSAGE
#include "string"
#include "iostream"
#include "netprotocol.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

std::string IpToStr(const sockaddr *addr);

std::string CString2string(CString cstring);


// 打印
void macArr2Str(const u_char * const mac_arr);

// 拷贝指针所指向的内容到程序堆区,只会拷贝一层，不会递归拷贝所有指针所指内容
template<class T>
shared_ptr<T> shallowCopy(T* src)
{
	T* dump = new T;
	*dump = *src;
	return shared_ptr<T>(dump);
}


void print_packet_hex(const u_char* pkt, int size_pkt, CString *buf);
