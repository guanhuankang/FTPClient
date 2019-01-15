
// FTPDlg.h : header file
//

#pragma once
#include "CommandSocket.h"
#include "DataSocket.h"
#include "FTPDef.h"
#include "Unicode2Utf8.h"
class CommandSocket;
class DataSocket;

// CFTPDlg dialog
class CFTPDlg : public CDialogEx
{
// Construction
public:
	CFTPDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
//TODO
	void OnClose();
	void OnConnect();
	void OnReceive(int nErrorCode);
	void SocketReset();
	void Initial();
	void DispDir(CString dir);
	void ClsDir();
	void DispCmd(CString cmds);
	void ClsCmd();
	bool pushCmd(CString cmd);
	CString popCmd();
	void showPrompt(CString input);
	DataSocket* getDataSocket(CString addr,int port);
	void dealDataSocket();
	void runNext();
	void send(CString cmd);
	bool openDataSocket();
	void buildFileStruct(CString);
	void getItem(int n, CString line);
	void setTip(int type,int size,CString filename);
	void flushDataSocket();
	void flushCMD();
//TODO
	Unicode2Utf8 m_unicode2utf8;
	CommandSocket *m_cmdSocket;
	DataSocket *m_dataSocket[MAX_DATASOCKET];

	CString m_fileStruct[MAX_FILES][10];
	int m_totLines;
	CString m_src, m_dst;
	CString m_dataAddr;
	int m_dataPort;
	CString m_curDir;
	CString m_cmd[MAX_CMD];
	int m_front, m_rear;
	int m_count;
	bool is_upload;
	int m_upIndex;
	CString m_address;
	CString m_username;
	CString m_password;
	CListBox m_dispCmd;
	CListBox m_dispDir;
	CString m_tip;
	afx_msg void OnBnClickedLogin();
	afx_msg void OnLbnDblclkList();
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedUpload();
	CButton m_login;
	CButton m_logout;
	CButton m_clear;
	CButton m_update;
	CButton m_upload;
};
