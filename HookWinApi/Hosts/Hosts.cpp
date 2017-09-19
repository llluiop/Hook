
// Hosts.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
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

// CHostsApp 构造


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
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Hosts.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CHostsApp 对象

CHostsApp theApp;


// CHostsApp 初始化

BOOL CHostsApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_HostsTYPE,
		RUNTIME_CLASS(CHostsDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CHostsView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
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

	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CHostsApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CHostsApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CHostsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CHostsApp 消息处理程序



