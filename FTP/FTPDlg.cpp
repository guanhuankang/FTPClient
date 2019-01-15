
// FTPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FTP.h"
#include "FTPDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFTPDlg dialog



CFTPDlg::CFTPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FTP_DIALOG, pParent)
	, m_tip(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFTPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDX_Text(pDX, IDC_EDIT2, m_username);
	DDX_Text(pDX, IDC_EDIT3, m_password);
	DDX_Control(pDX, IDC_TIPS, m_dispCmd);
	DDX_Control(pDX, IDC_LIST, m_dispDir);
	DDX_Text(pDX, IDC_EDIT1, m_tip);
	DDX_Control(pDX, IDC_LOGIN, m_login);
	DDX_Control(pDX, IDC_LOGOUT, m_logout);
	DDX_Control(pDX, IDC_BCLEAR, m_clear);
	DDX_Control(pDX, IDC_BUPDATE, m_update);
	DDX_Control(pDX, IDC_UPLOAD, m_upload);
}

BEGIN_MESSAGE_MAP(CFTPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOGIN, &CFTPDlg::OnBnClickedLogin)
	ON_LBN_DBLCLK(IDC_LIST, &CFTPDlg::OnLbnDblclkList)
	ON_BN_CLICKED(IDC_LOGOUT, &CFTPDlg::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_BCLEAR, &CFTPDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_BUPDATE, &CFTPDlg::OnBnClickedUpdate)
	ON_LBN_SELCHANGE(IDC_LIST, &CFTPDlg::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_UPLOAD, &CFTPDlg::OnBnClickedUpload)
END_MESSAGE_MAP()


// CFTPDlg message handlers

BOOL CFTPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	Initial();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFTPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFTPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFTPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFTPDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if (!AfxSocketInit()) {
		DispCmd(_T("WindowsthisSocket initial fail"));
		return;
	}
	m_cmdSocket = new CommandSocket;
	m_cmdSocket->setDlg(this);
	m_cmdSocket->Create();
	m_cmdSocket->Connect(m_address, 21);
}



void CFTPDlg::OnClose()
{
	SocketReset();
	m_curDir = _T("");
	m_front = m_rear = 0;
	ClsDir();
	ClsCmd();
}


void CFTPDlg::OnReceive(int nErrorCode)
{
	if (nErrorCode) {
		return;
	}
	char recv[MAX_LENGTH];
	CString msg;
	int status,len,n;
	len = m_cmdSocket->Receive(recv, MAX_LENGTH);
	m_unicode2utf8.utf82Unicode(recv, len, msg);
	status = _ttoi(msg.Left(3));
	n = msg.GetLength();

	switch (status)
	{
	case FTP_PASSOK: {
		m_login.EnableWindow(false);
		m_upload.EnableWindow(true);
		m_update.EnableWindow(true);
		m_logout.EnableWindow(true);
		break;
	}
	case FTP_CWD: {
		break;
	}
	case FTP_PWD: {
		m_curDir = msg.Mid( msg.Find(_T("\""), 0) + 1, msg.Find(_T("\""),5) - 1 - msg.Find(_T("\"")));
		break;
	}
	case FTP_PASV: {
		CString tmp = msg.Mid(msg.Find(_T("(")) + 1, msg.Find(_T(")")) - 1 - msg.Find(_T("(")));
		int last = tmp.ReverseFind(TCHAR(','));
		m_dataPort = _ttoi(tmp.Mid(last+1));
		tmp = tmp.Left(last);
		last = tmp.ReverseFind(TCHAR(','));
		m_dataPort += 256 * (_ttoi(tmp.Mid(last+1)));
		m_dataAddr = tmp.Left(last);
		m_dataAddr.Replace(_T(","), _T("."));

		openDataSocket();
		showPrompt(msg);
		return;
		break;
	}
	case FTP_ODC: {
		m_tip = _T("Tranfering!Please wait!");
		UpdateData(false);
		if (is_upload) {
			m_dataSocket[m_upIndex]->sendFile();
		}
		showPrompt(msg);
		return;
	}
	case FTP_TCOMPLETE: {
		m_tip = _T("Tranfer Complete!");
		UpdateData(false);
		dealDataSocket();
		showPrompt(msg);
		return;
	}
	case FTP_WPASS: {
		MessageBox(_T("Wrong Password"));
		SocketReset();
		flushDataSocket();
		showPrompt(msg);
		return;
	}
	case FTP_TFAIL: 
	case FTP_OCF: {
		flushDataSocket();
		break;
	}
	default:
		break;
	}

	//next instruction
	runNext();
	showPrompt(msg);
}

void CFTPDlg::SocketReset()
{
	if (m_cmdSocket != NULL) {
		delete m_cmdSocket;
		m_cmdSocket = NULL;
	}
	for (int i = 0; i < MAX_DATASOCKET; i++) {
		if (m_dataSocket[i] != nullptr) {
			delete m_dataSocket[i];
			m_dataSocket[i] = nullptr;
		}
	}
}

void CFTPDlg::Initial()
{
	SocketReset();
	m_curDir = _T("");
	m_front = m_rear = 0;
	m_address = _T("127.0.0.1");
	m_username = _T("kitty");
	m_password = _T("123456ab");
	m_count = 0;
	m_logout.EnableWindow(false);
	m_update.EnableWindow(false);
	m_upload.EnableWindow(false);
	ClsCmd();
	ClsDir();
	UpdateData(false);
}

void CFTPDlg::DispDir(CString dir)
{
	UpdateData(true); 
	ClsDir();
	CString line;
	int line_num = 0;
	int first = dir.Find(_T("\n"),0);
	while (first>=0) {
		line = dir.Left(first);
		m_dispDir.InsertString(line_num++,line);
		dir = dir.Mid(first+1);
		first = dir.Find(_T("\n"));
	}
	m_tip.Format(_T("List finish! Current dir is %s"),m_curDir);
	UpdateData(false);
}

void CFTPDlg::ClsDir()
{
	while (m_dispDir.GetCount()) {
		m_dispDir.DeleteString(0);
	}
	UpdateData(false);
}

void CFTPDlg::DispCmd(CString cmds){
	m_dispCmd.AddString(cmds);
	UpdateData(false);
}

void CFTPDlg::ClsCmd()
{
	while (m_dispCmd.GetCount()) {
		m_dispCmd.DeleteString(0);
	}
	UpdateData(false);
}

bool CFTPDlg::pushCmd(CString cmd)
{
	if ((m_front + 1) % MAX_CMD == m_rear) {
		//queue fill
		return false;
	}
	m_cmd[m_front] = cmd;
	m_front = (m_front + 1) % MAX_CMD;
	return true;
}

CString CFTPDlg::popCmd(){
	if (m_front == m_rear)return _T("");
	CString rs = m_cmd[m_rear];
	m_rear = (m_rear + 1) % MAX_CMD;
	return rs;
}

void CFTPDlg::showPrompt(CString input)
{
	int status = _ttoi(input.Left(3));
	CString prompt;
	switch (status)
	{
	case FTP_WEL:
		prompt = _T("Welcome to FTP sever");
	default:
		prompt = input;
	}
	m_dispCmd.SetTopIndex(m_dispCmd.GetCount() - 1);
	m_dispCmd.AddString(prompt);
	UpdateData(false);
	return;
}

DataSocket * CFTPDlg::getDataSocket(CString addr, int port)
{
	for (int i = 0; i < MAX_DATASOCKET; i++) {
		if (m_dataSocket[i] == nullptr) {
			CString msg; msg.Format(_T("%s %d"),addr,port);
			m_dataSocket[i] = new DataSocket();
			m_upIndex = i;
			return m_dataSocket[i];
		}
	}
	return nullptr;
}

void CFTPDlg::dealDataSocket()
{
	Sleep(300);
	//MessageBox(_T("Hello"));
	for (int i = 0; i < MAX_DATASOCKET;i++) {
		if (m_dataSocket[i]!=NULL && m_dataSocket[i]->m_live == false) {
			//deal with it
			DataSocket *tar = m_dataSocket[i];
			switch (tar->getType()) {
				case T_LIST: {
					CString rs = tar->getString();
					buildFileStruct(rs);
					DispDir(rs);
					break;
				}
				case T_DOWNLOAD: {
					m_dataSocket[i]->finDownload();
					CString msg;
					msg.Format(_T("%s\r\nDownload finish! [%s]"),msg,m_dataSocket[i]->m_src);
					DispCmd(msg);
					break;
				}
				case T_UPLOAD: {
					tar->Close();
					break;
				}
				default:
					break;
			}
			//free 
			delete tar;
			m_dataSocket[i] = NULL;
		}
	}
	runNext();
	return;
}

void CFTPDlg::runNext()
{
	CString cmd = popCmd();
	while (cmd.Left(5) == _T("MYCMD"))cmd = popCmd();
	if (cmd == _T(""))return;
	send(cmd);
}

void CFTPDlg::send(CString cmd)
{
	char str[MAX_LENGTH];
	int n = m_unicode2utf8.unicode2Utf8(str,cmd);
	str[n++] = 13;//0d
	str[n++] = 10;//0a
	m_cmdSocket->Send(str, n);
}

bool CFTPDlg::openDataSocket()
{
	DataSocket *ds = getDataSocket(m_dataAddr,m_dataPort);
	CString cmd = popCmd(); 
	if (ds == NULL) {
		while (cmd.Left(5) != _T("MYCMD"))cmd = popCmd();
		return false;
	}
	m_rear = (m_rear - 1 + MAX_CMD) % MAX_CMD;
	ds->Create();
	if (cmd.Left(4) == _T("LIST")) {
		is_upload = false;
		ds->setParam(T_LIST, _T(""), _T(""), this);
	}
	else if(cmd.Left(4) == _T("RETR")){
		is_upload = false;
		ds->setParam(T_DOWNLOAD,m_src,m_dst,this);
	}
	else if(cmd.Left(4) == _T("STOR")){
		is_upload = true;
		ds->setParam(T_UPLOAD,m_src,m_dst,this);
	}
	else {
		return false;
	}
	ds->Connect(m_dataAddr,m_dataPort);
	return true;
}

void CFTPDlg::getItem(int n,CString line) {
	int i=0,tot = line.GetLength(),cur=0,first = 0;
	CString tmp;
	while (tot >= 1 && (line[tot - 1] == TCHAR(13) || line[tot - 1] == TCHAR(10)))tot--;
	line = line.Left(tot);
	while (i<tot) {
		while (i < tot && line[i] == TCHAR(' '))i++;
		if(cur<8)
			tmp = _T("");
		else if(cur==8){
			tmp = _T("");
			first = i;
		}
		else {
			cur = 8;
		}
		while (i<tot&&line[i]!=TCHAR(' ')) {
			tmp = tmp + line[i++];
		}
		m_fileStruct[n][cur++] = tmp;
	}
	m_fileStruct[n][8] = line.Mid(first);
	return;
}
void CFTPDlg::setTip(int type, int size,CString filename)
{
	if (type == T_DOWNLOAD) {
		m_tip.Format(_T("[%s] download ->>> %d bytes"), filename, size);
	}
	else if (type == T_UPLOAD) {
		m_tip.Format(_T("[%s] upload <<<- %d bytes"), filename, size);
	}
	UpdateData(false);
}
void CFTPDlg::flushDataSocket()
{
	for (int i = 0; i < MAX_DATASOCKET; i++) {
		if (m_dataSocket[i] != nullptr) {
			delete m_dataSocket[i];
			m_dataSocket[i] = nullptr;
		}
	}
	flushCMD();
}
void CFTPDlg::flushCMD()
{
	CString cmd = popCmd();
	while (cmd.Left(5)!=_T("MYCMD")) {
		cmd = popCmd();
	}
}
void CFTPDlg::buildFileStruct(CString dirs)
{
	CString line;
	m_totLines = 0;
	int newline = dirs.Find(_T("\n"));
	while (newline>=0) {
		line = dirs.Left(newline);
		//deal-----------------
		getItem(m_totLines++,line);
		//MessageBox(m_fileStruct[m_totLines-1][8]);
		//---------------
		dirs = dirs.Mid(newline+1);
		newline = dirs.Find(_T("\n"));
	}
}


void CFTPDlg::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	send(_T("QUIT"));
	m_update.EnableWindow(false);
	m_upload.EnableWindow(false);
	m_logout.EnableWindow(false);
	m_login.EnableWindow(true);
	Initial();
}

void CFTPDlg::OnBnClickedClear()
{
	// TODO: Add your control notification handler code here
	ClsCmd();
}


void CFTPDlg::OnBnClickedUpdate()
{
	// TODO: Add your control notification handler code here
	CString cmd;
	cmd.Format(_T("PWD")); pushCmd(cmd);
	cmd.Format(_T("PASV")); pushCmd(cmd);
	cmd.Format(_T("LIST")); pushCmd(cmd);
	runNext();
}



void CFTPDlg::OnLbnSelchangeList()
{
	// TODO: Add your control notification handler code here
}


void CFTPDlg::OnBnClickedUpload()
{
	// TODO: Add your control notification handler code here
	TCHAR szFilter[] = _T("all files(*.*)|*.*||");
	CFileDialog fileDlg(true, _T(""), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString localpath;
	if (IDOK == fileDlg.DoModal()) {
		localpath = fileDlg.GetPathName();
		m_src = localpath;
		m_dst = m_curDir + fileDlg.GetFileName();
		/*----------UPLOAD----------*/
		CString cmd;
		cmd.Format(_T("MYCMD_UPLOAD")); pushCmd(cmd);
		cmd.Format(_T("TYPE I")); pushCmd(cmd);
		cmd.Format(_T("PASV")); pushCmd(cmd);
		cmd.Format(_T("STOR %s"), fileDlg.GetFileName()); pushCmd(cmd);
		cmd.Format(_T("TYPE I")); pushCmd(cmd);
		cmd.Format(_T("PASV")); pushCmd(cmd);
		cmd.Format(_T("LIST")); pushCmd(cmd);
		cmd.Format(_T("MYCMD_END_UPLOAD")); pushCmd(cmd);
		/*----------UPLOAD----------*/
		runNext();
	}
}
void CFTPDlg::OnConnect()
{
	//login begin
	UpdateData(true);
	CString cmd;
	cmd.Format(_T("MYCMD_LOGIN")); pushCmd(cmd);
	cmd.Format(_T("USER %s"), m_username); pushCmd(cmd);
	cmd.Format(_T("PASS %s"), m_password); pushCmd(cmd);
	cmd.Format(_T("OPTS UTF8 ON")); pushCmd(cmd);
	cmd.Format(_T("PWD")); pushCmd(cmd);
	cmd.Format(_T("TYPE A")); pushCmd(cmd);
	cmd.Format(_T("PASV")); pushCmd(cmd);
	cmd.Format(_T("LIST")); pushCmd(cmd);
	cmd.Format(_T("MYCMD_END_LOGIN")); pushCmd(cmd);
}

void CFTPDlg::OnLbnDblclkList()
{
	// TODO: Add your control notification handler code here
	int selnum = m_dispDir.GetCurSel();
	if (selnum >= m_totLines || selnum < 0)return;
	CString remoteFilename = m_curDir + m_fileStruct[selnum][8];

	//CWD
	if (m_fileStruct[selnum][0][0] == TCHAR('d')) {
		CString cmd;
		cmd.Format(_T("CWD %s"), m_fileStruct[selnum][8]); pushCmd(cmd);
		cmd.Format(_T("PWD")); pushCmd(cmd);
		cmd.Format(_T("TYPE A")); pushCmd(cmd);
		cmd.Format(_T("PASV")); pushCmd(cmd);
		cmd.Format(_T("LIST")); pushCmd(cmd);
		cmd.Format(_T("PWD")); pushCmd(cmd);
		runNext();
		return;
	}

	/*-----------DOWNLOAD--------------*/
	//Open a fileDlg
	TCHAR szFilter[] = _T("all files(*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T(""), m_fileStruct[selnum][8], OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString localFileName;
	if (IDOK == fileDlg.DoModal()) {
		localFileName = fileDlg.GetPathName();
		m_src = localFileName;
		m_dst = remoteFilename;

		m_tip.Format(_T("Start to Download! Please wait!"));
		UpdateData(false);

		CString cmd;
		//sequeue
		cmd.Format(_T("MYCMD_DOWNLOAD")); pushCmd(cmd);
		cmd.Format(_T("TYPE I")); pushCmd(cmd);
		cmd.Format(_T("PASV")); pushCmd(cmd);
		cmd.Format(_T("RETR %s"), m_fileStruct[selnum][8]); pushCmd(cmd);
		cmd.Format(_T("MYCMD_END_DOWNLOAD")); pushCmd(cmd);
		runNext();
	}
	/*-----------END--------------*/
}
