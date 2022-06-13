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


// ��ӡ
void macArr2Str(const u_char * const mac_arr);

// ����ָ����ָ������ݵ��������,ֻ�´��һ�㣬����ݹ鿽������ָ����ָ����
template<class T>
shared_ptr<T> shallowCopy(T* src)
{
	T* dump = new T;
	*dump = *src;
	return shared_ptr<T>(dump);
}


void print_packet_hex(const u_char* pkt, int size_pkt, CString *buf);
