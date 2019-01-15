#pragma once
#include <afxsock.h>
#include "FTPDlg.h"

class CFTPDlg;
class CommandSocket :
	public CAsyncSocket
{
public:
	CommandSocket();
	~CommandSocket();
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	CFTPDlg *m_dlg;
	void setDlg(CFTPDlg *dlg);
};

