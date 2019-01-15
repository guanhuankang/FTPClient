#include "stdafx.h"
#include "CommandSocket.h"


CommandSocket::CommandSocket()
{
}


CommandSocket::~CommandSocket()
{
}

void CommandSocket::OnConnect(int nErrorCode)
{
	m_dlg->OnConnect();
	CAsyncSocket::OnConnect(nErrorCode);
}

void CommandSocket::OnClose(int nErrorCode)
{
	m_dlg->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

void CommandSocket::OnReceive(int nErrorCode)
{
	m_dlg->OnReceive(nErrorCode);
	CAsyncSocket::OnReceive(nErrorCode);
}

void CommandSocket::setDlg(CFTPDlg * dlg)
{
	m_dlg = dlg;
}
