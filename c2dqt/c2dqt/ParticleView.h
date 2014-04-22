#pragma once

#include "CCEGLView.h"

USING_NS_CC;

class ParticleView : public CCEGLView
{
public:
	ParticleView(void);
	~ParticleView(void);

	static ParticleView* sharedOpenGLViewWithHwnd(HWND mHWND);
	
	bool CreateWithHwnd(HWND mHwnd);

//	static ParticleView* s_pMainWindow;

};

