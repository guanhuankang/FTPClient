#pragma once
#include <afxsock.h>
#include "FTP.h"
#include "FTPDlg.h"
#include "FTPDef.h"
#include "Unicode2Utf8.h"
class CFTPDlg;

class DataSocket :
	public CAsyncSocket
{
public:
	DataSocket();
	~DataSocket();

	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

public:
//TODO
	void setParam(int type, CString src, CString dst, CFTPDlg *dlg = NULL);
	int getType();
	CString getString();
	CString finDownload();
	void sendFile();
//TODO
	Unicode2Utf8 m_unicode2utf8;
	CString m_src, m_dst;
	CString m_result_string;
	CFTPDlg *m_dlg;
	CFile *m_file;
	bool m_live;
	int m_type;//List or Download or Upload A/I
	int m_size;
};

