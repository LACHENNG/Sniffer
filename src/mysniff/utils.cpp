#include "stdafx.h"
#include "utils.h"


std::string IpToStr(const sockaddr *addr) {
	std::string s;
	s = inet_ntoa((*(sockaddr_in*)addr).sin_addr);
	return s;
}

std::string CString2string(CString cstring) {
	std::string res;;
	res = CT2A(cstring.GetString());
	return res;
}

void macArr2Str(const u_char * const mac_arr)
{
	printf("%02x", mac_arr[0]);
	for (int i = 1; i < ETH_MLEN; i++) {
		printf(":%02x", mac_arr[i]);
	}
	printf("\n");
}

void print_packet_hex(const u_char* pkt, int size_pkt, CString *buf)
{
	int i = 0, j = 0, rowcount;
	u_char ch;

	char tempbuf[256];
	memset(tempbuf, 0, 256);

	for (i = 0; i < size_pkt; i += 16)
	{
		buf->AppendFormat(_T("%04x:  "), (u_int)i);
		rowcount = (size_pkt - i) > 16 ? 16 : (size_pkt - i);

		for (j = 0; j < rowcount; j++)
			buf->AppendFormat(_T("%02x  "), (u_int)pkt[i + j]);

		//²»×ã16£¬ÓÃ¿Õ¸ñ²¹×ã
		if (rowcount < 16)
			for (j = rowcount; j < 16; j++)
				buf->AppendFormat(_T("  "));


		for (j = 0; j < rowcount; j++)
		{
			ch = pkt[i + j];
			ch = isprint(ch) ? ch : '.';
			buf->AppendFormat(_T("%1c"), ch);
		}

		buf->Append(_T("\r\n"));
		if (rowcount < 16)
			return;
	}
}
