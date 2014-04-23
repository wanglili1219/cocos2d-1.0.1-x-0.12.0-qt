#include "CCEGLView_qt.h"
#include "EGL/egl.h"
#include "gles/gl.h"
extern  bool CheckTouchSupport();


CCEGLView_qt::CCEGLView_qt(void)
{

}
	
CCEGLView_qt::~CCEGLView_qt(void)
{
	
}

bool CCEGLView_qt::CreateWithHwnd(HWND mHwnd)
{
	
	bool bRet = false;
	do
	{
		m_hWnd = mHwnd;
		resize(480, 320);
		// init egl
		m_pEGL = CCEGL::create(this);
	

		if (! m_pEGL)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = NULL;
			break;
		}

		s_pMainWindow = this;
		bRet = true;
	} while (0);
	
	/*
#if(_MSC_VER >= 1600)
	m_bSupportTouch = CheckTouchSupport();
	if(m_bSupportTouch)
	{
		m_bSupportTouch = (s_pfRegisterTouchWindowFunction(m_hWnd, 0) != 0);
	}
#endif /* #if(_MSC_VER >= 1600) */
	
	return bRet;
}

CCEGLView_qt* CCEGLView_qt::sharedOpenGLViewWithHwnd(HWND mHWND)
{
	static CCEGLView_qt* s_pEglView = NULL;
	if (s_pEglView == NULL)
	{
		s_pEglView = new CCEGLView_qt();
		if(!s_pEglView->CreateWithHwnd(mHWND))
		{
			delete s_pEglView;
			s_pEglView = NULL;
		}
	}
	return s_pEglView;
}