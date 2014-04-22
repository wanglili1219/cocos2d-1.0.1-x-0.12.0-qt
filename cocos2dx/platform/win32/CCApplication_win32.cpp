#include "CCApplication.h"

#include "CCDirector.h"

/**
@brief	This function change the PVRFrame show/hide setting in register.
@param  bEnable If true show the PVRFrame window, otherwise hide.
*/
static void PVRFrameEnableControlWindow(bool bEnable);

NS_CC_BEGIN;

// sharedApplication pointer
CCApplication * CCApplication::sm_pSharedApplication = 0;

CCApplication::CCApplication()
: m_hInstance(NULL)
, m_hAccelTable(NULL)
{
    m_hInstance	= GetModuleHandle(NULL);
    m_nAnimationInterval.QuadPart = 0;
    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

CCApplication::~CCApplication()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = NULL;
}

/*
int CCApplication::run()
{
    PVRFrameEnableControlWindow(false);

    // Main message loop:
    MSG msg;
    LARGE_INTEGER nFreq;
    LARGE_INTEGER nLast;
    LARGE_INTEGER nNow;

    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nLast);

    // Initialize instance and cocos2d.
    if (! initInstance() || ! applicationDidFinishLaunching())
    {
        return 0;
    }

    CCEGLView& mainWnd = CCEGLView::sharedOpenGLView();
    mainWnd.centerWindow();
    ShowWindow(mainWnd.getHWnd(), SW_SHOW);

    while (1)
    {
        if (! PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Get current time tick.
            QueryPerformanceCounter(&nNow);

            // If it's the time to draw next frame, draw it, else sleep a while.
            if (nNow.QuadPart - nLast.QuadPart > m_nAnimationInterval.QuadPart)
            {
                nLast.QuadPart = nNow.QuadPart;
                CCDirector::sharedDirector()->mainLoop();
            }
            else
            {
                Sleep(0);
            }
            continue;
        }

        if (WM_QUIT == msg.message)
        {
            // Quit message loop.
            break;
        }

        // Deal with windows message.
        if (! m_hAccelTable || ! TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

*/


int CCApplication::run(HWND hWnd,LPCTSTR szTitle,UINT wWidth,UINT wHeight)
{
    PVRFrameEnableControlWindow(false);

    // Main message loop:
    MSG msg;
    LARGE_INTEGER nFreq;
    LARGE_INTEGER nNow;

    QueryPerformanceFrequency(&nFreq);
   //��ԭ��ʱ����nLast��Ϊ��Ա����m_nLast����Ϊ����Ҫ��ÿ֡����Ⱦ�����ⲿ���õ�public�����������Ҫ��һ����֡��ʱ�䡣
    QueryPerformanceCounter(&m_nLast);

    // �ؼ���1�����뽫����hWndҲ��Ϊ��������initInstance������
    if (! initInstance(hWnd,szTitle,wWidth,wHeight) || ! applicationDidFinishLaunching())
    {
        return 0;
    }

    CCEGLView& mainWnd = CCEGLView::sharedOpenGLView();
	//�ؼ���2���������Ҫʹ��ָ����CWnd����������OpenGL����ô�������ʹ��Cocos2d-x��װ�õ���Ϣѭ������Ϊ��Щ�������ύ��MFC����������Ϊ�˲��ƻ�ԭ����Cocos2d-x�������ڷ�ʽ���������������жϴ������Ҫʹ��ԭ����Cocos2d-x�������ڵķ�ʽ��ֻ��Ҫ��һ��������NULL�ͺ��ˡ�
    if(NULL == hWnd)
	{
		mainWnd.centerWindow();
		ShowWindow(mainWnd.getHWnd(), SW_SHOW);

		while (1)
		{
			if (! PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// Get current time tick.
				QueryPerformanceCounter(&nNow);

				// If it's the time to draw next frame, draw it, else sleep a while.
				if (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
				{
					m_nLast.QuadPart = nNow.QuadPart;
					CCDirector::sharedDirector()->mainLoop();
				}
				else
				{
					Sleep(0);
				}
				continue;
			}

			if (WM_QUIT == msg.message)
			{
				// Quit message loop.
				break;
			}

			// Deal with windows message.
			if (! m_hAccelTable || ! TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return (int) msg.wParam;
	}
	return 0;
}


void CCApplication::setAnimationInterval(double interval)
{
    LARGE_INTEGER nFreq;
    QueryPerformanceFrequency(&nFreq);
    m_nAnimationInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
}

CCApplication::Orientation CCApplication::setOrientation(Orientation orientation)
{
    // swap width and height
    CCEGLView * pView = CCDirector::sharedDirector()->getOpenGLView();
    if (pView)
    {
        return (Orientation)pView->setDeviceOrientation(orientation);
    }
    return (Orientation)CCDirector::sharedDirector()->getDeviceOrientation();
}

void CCApplication::statusBarFrame(CCRect * rect)
{
    if (rect)
    {
        // Windows doesn't have status bar.
        *rect = CCRectMake(0, 0, 0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
CCApplication& CCApplication::sharedApplication()
{
    CC_ASSERT(sm_pSharedApplication);
    return *sm_pSharedApplication;
}

ccLanguageType CCApplication::getCurrentLanguage()
{
    ccLanguageType ret = kLanguageEnglish;

	LCID localeID = GetUserDefaultLCID();
	unsigned short primaryLanguageID = localeID & 0xFF;
	
	switch (primaryLanguageID)
	{
		case LANG_CHINESE:
			ret = kLanguageChinese;
			break;
		case LANG_FRENCH:
			ret = kLanguageFrench;
			break;
		case LANG_ITALIAN:
			ret = kLanguageItalian;
			break;
		case LANG_GERMAN:
			ret = kLanguageGerman;
			break;
		case LANG_SPANISH:
			ret = kLanguageSpanish;
			break;
		case LANG_RUSSIAN:
			ret = kLanguageRussian;
			break;
	}

    return ret;
}

NS_CC_END;

//////////////////////////////////////////////////////////////////////////
// Local function
//////////////////////////////////////////////////////////////////////////
static void PVRFrameEnableControlWindow(bool bEnable)
{
    HKEY hKey = 0;

    // Open PVRFrame control key, if not exist create it.
    if(ERROR_SUCCESS != RegCreateKeyExW(HKEY_CURRENT_USER,
        L"Software\\Imagination Technologies\\PVRVFRame\\STARTUP\\",
        0,
        0,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        0,
        &hKey,
        NULL))
    {
        return;
    }

    const wchar_t * wszValue = L"hide_gui";
    const wchar_t * wszNewData = (bEnable) ? L"NO" : L"YES";
    wchar_t wszOldData[256] = {0};
    DWORD   dwSize = sizeof(wszOldData);
    LSTATUS status = RegQueryValueExW(hKey, wszValue, 0, NULL, (LPBYTE)wszOldData, &dwSize);
    if (ERROR_FILE_NOT_FOUND == status              // the key not exist
        || (ERROR_SUCCESS == status                 // or the hide_gui value is exist
        && 0 != wcscmp(wszNewData, wszOldData)))    // but new data and old data not equal
    {
        dwSize = sizeof(wchar_t) * (wcslen(wszNewData) + 1);
        RegSetValueEx(hKey, wszValue, 0, REG_SZ, (const BYTE *)wszNewData, dwSize);
    }

    RegCloseKey(hKey);
}
