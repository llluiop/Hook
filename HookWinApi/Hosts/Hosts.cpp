
// Hosts.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Hosts.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "HostsDoc.h"
#include "HostsView.h"
#include "mhook-lib/mhook.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <Shobjidl.h>

// CHostsApp

BEGIN_MESSAGE_MAP(CHostsApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CHostsApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

typedef BOOL(WINAPI* _GetSaveFileNameA)(_Inout_ LPOPENFILENAME lpofn);
typedef BOOL(WINAPI* _GetSaveFileNameW)(_Inout_ LPOPENFILENAME lpofn);

_GetSaveFileNameA getsavefilenameA = nullptr;
_GetSaveFileNameA getsavefilenameW = nullptr;

UINT_PTR CALLBACK OFNHookProc(
	_In_ HWND   hdlg,
	_In_ UINT   uiMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	return 0;
}

UINT_PTR CALLBACK OFNHookProcOldStyle(
	_In_ HWND   hdlg,
	_In_ UINT   uiMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	return 0;
}

BOOL WINAPI MyGetSaveFileNameA(_Inout_ LPOPENFILENAME lpofn)
{
	if ((lpofn->Flags & OFN_EXPLORER) && (lpofn->Flags & OFN_ENABLEHOOK))
	{
		lpofn->lpfnHook = OFNHookProc;
	}
	else
	{

	}
	return getsavefilenameA(lpofn);
}

BOOL WINAPI MyGetSaveFileNameW(_Inout_ LPOPENFILENAME lpofn)
{
	if ((lpofn->Flags & OFN_EXPLORER) && (lpofn->Flags & OFN_ENABLEHOOK))
	{
		lpofn->lpfnHook = OFNHookProc;
	}
	else
	{

	}
	return getsavefilenameW(lpofn);
}

IFileDialog* pIFileDialog;
IFileDialogEvents* pIFileDialogEvents;

typedef HRESULT (STDMETHODCALLTYPE* _Show)(
	IFileDialog* pIFileDialog,
	_In_opt_  HWND hwndOwner);

_Show show = nullptr;

HRESULT STDMETHODCALLTYPE MyShow(
	IFileDialog* pIFileDialog,
	_In_opt_  HWND hwndOwner)
{
	DWORD cook;
	pIFileDialog->Advise(pIFileDialogEvents, &cook);
	return show(pIFileDialog, hwndOwner);
}


typedef HRESULT (STDMETHODCALLTYPE * _OnFileOk)(
	__RPC__in IFileDialogEvents * This,
	/* [in] */ __RPC__in_opt IFileDialog *pfd);

_OnFileOk ok = nullptr;

HRESULT STDMETHODCALLTYPE MyOnFileOK(
	__RPC__in IFileDialogEvents * This,
	/* [in] */ __RPC__in_opt IFileDialog *pfd)
{
	return ok(This, pfd);
}

// CHostsApp ����


class Event : public IFileDialogEvents
{
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		return S_OK;
	};

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		return 1;
	};

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		return 0;
	};

	virtual HRESULT STDMETHODCALLTYPE OnFileOk(
		/* [in] */ __RPC__in_opt IFileDialog *pfd)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnFolderChanging(
		/* [in] */ __RPC__in_opt IFileDialog *pfd,
		/* [in] */ __RPC__in_opt IShellItem *psiFolder)
	{
		return S_OK;
	};

	virtual HRESULT STDMETHODCALLTYPE OnFolderChange(
		/* [in] */ __RPC__in_opt IFileDialog *pfd) {
		return S_OK;
	};

	virtual HRESULT STDMETHODCALLTYPE OnSelectionChange(
		/* [in] */ __RPC__in_opt IFileDialog *pfd) {
		return S_OK;
	};

	virtual HRESULT STDMETHODCALLTYPE OnShareViolation(
		/* [in] */ __RPC__in_opt IFileDialog *pfd,
		/* [in] */ __RPC__in_opt IShellItem *psi,
		/* [out] */ __RPC__out FDE_SHAREVIOLATION_RESPONSE *pResponse) {
		return S_OK;
	};

	virtual HRESULT STDMETHODCALLTYPE OnTypeChange(
		/* [in] */ __RPC__in_opt IFileDialog *pfd) {
		return S_OK;
	};

	virtual HRESULT STDMETHODCALLTYPE OnOverwrite(
		/* [in] */ __RPC__in_opt IFileDialog *pfd,
		/* [in] */ __RPC__in_opt IShellItem *psi,
		/* [out] */ __RPC__out FDE_OVERWRITE_RESPONSE *pResponse) {
		return S_OK;
	};
};

CHostsApp::CHostsApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Hosts.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CHostsApp ����

CHostsApp theApp;


// CHostsApp ��ʼ��

BOOL CHostsApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_HostsTYPE,
		RUNTIME_CLASS(CHostsDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CHostsView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (nullptr == GetModuleHandle(L"comdlg32.dll"))
	{
		LoadLibrary(L"comdlg32.dll");
	}
	
	_GetSaveFileNameA getsavefilenameA = (_GetSaveFileNameA)
		GetProcAddress(GetModuleHandle(L"comdlg32.dll"), "GetSaveFileNameA");

	_GetSaveFileNameW getsavefilenameW = (_GetSaveFileNameW)
		GetProcAddress(GetModuleHandle(L"comdlg32.dll"), "GetSaveFileNameW");

	Mhook_SetHook((PVOID*)&getsavefilenameW, MyGetSaveFileNameW);
	//Mhook_SetHook((PVOID*)&getsavefilenameA, MyGetSaveFileNameA);




	HRESULT hr;

	//USE_INTERFACE_PART_STD(FileDialogEvents);
	//USE_INTERFACE_PART_STD(FileDialogControlEvents);


	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pIFileDialog));


	pIFileDialogEvents  = new Event;

	//auto file = static_cast<IFileSaveDialog*>(pIFileDialog);
	show = (_Show)*((int*)*(int*)pIFileDialog + 3);
	Mhook_SetHook((PVOID*)&show, MyShow);

	ok = (_OnFileOk)*((int*)*(int*)pIFileDialogEvents + 3);
	Mhook_SetHook((PVOID*)&show, MyOnFileOK);

	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CHostsApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CHostsApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CHostsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CHostsApp ��Ϣ�������



