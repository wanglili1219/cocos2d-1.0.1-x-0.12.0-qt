#pragma once

#include "CCEGLView.h"

USING_NS_CC;

class CCEGLView_qt : public CCEGLView
{
public:
	CCEGLView_qt(void);
	~CCEGLView_qt(void);

	static CCEGLView_qt* sharedOpenGLViewWithHwnd(HWND mHWND);
	
	bool CreateWithHwnd(HWND mHwnd);

//	static ParticleView* s_pMainWindow;

};

