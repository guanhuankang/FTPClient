#include "stdafx.h"
#include "DataSocket.h"


DataSocket::DataSocket()
{
	m_result_string = _T("");
}

DataSocket::~DataSocket()
{
}

void DataSocket::OnConnect(int nErrorCode)
{
	if (!nErrorCode) {
		m_live = true;
	}
	else {
		m_live = false;
	}
	CAsyncSocket::OnConnect(nErrorCode);
	m_dlg->runNext();
}

void DataSocket::OnClose(int nErrorCode)
{
	m_live = false;
	if (m_type == T_LIST) {
		m_dlg->dealDataSocket();
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void DataSocket::OnReceive(int nErrorCode)
{
	if (nErrorCode) {
		/*	0 The function executed successfully.
			WSAENETDOWN The Windows Sockets implementation detected that the network subsystem failed.
		*/
		return;
	}
	CString msg;
	char str[MAX_LENGTH];
	int len = this->Receive(str,MAX_LENGTH);
	switch (m_type)
	{
	case T_LIST: {
		m_unicode2utf8.utf82Unicode(str, len, msg);
		m_result_string += msg;
		break;
	}
	case T_DOWNLOAD: {
		m_file->Write(str,len);
		//m_file->Flush();
		m_size += len;
		m_dlg->setTip(T_DOWNLOAD, m_size, m_src);
		break;
	}
	case T_UPLOAD: {
		m_dlg->setTip(T_UPLOAD, m_size, m_src);
		;;
	}
	default:
		break;
	}
}

void DataSocket::setParam(int type, CString src, CString dst, CFTPDlg * dlg)
{
	m_type = type;
	m_src = src;
	m_dst = dst;
	m_dlg = dlg;

	switch (type) {
	case T_LIST: {
		m_result_string = _T("");
		break;
	}
	case T_DOWNLOAD: {
		m_file = new CFile;
		m_file->Open(m_src, CFile::modeCreate | CFile::modeReadWrite);
		m_size = 0;
		break;
	}
	}
}

int DataSocket::getType()
{
	return m_type;
}

CString DataSocket::getString()
{
	return m_result_string;
}

CString DataSocket::finDownload()
{
	m_file->Flush();
	m_file->Close();
	delete m_file;
	m_file = NULL;
	return m_src;
}
void DataSocket::sendFile()
{
	m_file = new CFile;
	m_file->Open(m_src,CFile::typeBinary|CFile::modeReadWrite);
	char str[MAX_FILES];
	UINT len = m_file->Read(str,MAX_FILES);
	while (len>0) {
		this->Send(str, len);
		len = m_file->Read(str, MAX_FILES);
	}
	m_file->Close();
	delete m_file;
	m_file = NULL;
	this->Close();
}
