#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "wintoastlib.h"

TCHAR szClassName[] = TEXT("Window");

class CustomHandler : public WinToastLib::IWinToastHandler {
public:
	void toastActivated() const
	{
		//L"The user clicked in this toast"
		//ユーザがトーストをクリックした際に呼ばれる。
	}
	void toastActivated(int actionIndex) const
	{
		//ユーザがアクションをクリックした際に呼ばれる。
	}
	void toastDismissed(WinToastDismissalReason state) const
	{
		switch (state) {
		case UserCanceled:
			break;
		case TimedOut:
			break;
		case ApplicationHidden:
			break;
		default:
			break;
		}
	}
	void toastFailed() const
	{
	}
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton;
	switch (msg)
	{
	case WM_CREATE:
		hButton = CreateWindowW(L"BUTTON", L"変換", WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SIZE:
		MoveWindow(hButton, 10, 10, 256, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			if (WinToastLib::WinToast::isCompatible())
			{
				WinToastLib::WinToast::instance()->setAppName(L"アプリ名");
				WinToastLib::WinToast::instance()->setAppUserModelId(szClassName);
				WinToastLib::WinToast::WinToastError error1;
				const bool succedded = WinToastLib::WinToast::instance()->initialize(&error1);
				if (succedded)
				{
					WinToastLib::WinToastTemplate templ = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::Text02);
					templ.setTextField(L"タイトル", WinToastLib::WinToastTemplate::FirstLine);
					templ.setTextField(L"本文", WinToastLib::WinToastTemplate::SecondLine);
					std::vector<std::wstring> actions;
					actions.push_back(L"了解しました。");
					for (auto const& action : actions)
					{
						templ.addAction(action);
					}
					WinToastLib::WinToast::instance()->showToast(templ, new CustomHandler);
				}
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		L"Window",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
