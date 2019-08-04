#include "platform.h"

#ifdef ANGE_PLATFORM_LINUX
#include <X11/X.h>
#include <X11/Xlib.h>
#elif ANGE_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace Ange {

	Ange::Point<int> MousePosition()
	{
	#ifdef ANGE_PLATFORM_WINDOWS
		POINT p;
		GetCursorPos(&p);
		return Point<int>{p.x,p.y}; 
	#elif ANGE_PLATFORM_LINUX
		int wx, wy, rx, ry;
		unsigned int m;
		Window win;
		Display* disp = XOpenDisplay(NULL);
		XQueryPointer(disp, XRootWindow(disp,0),&win, &win, &rx, &ry, &wx, &wy, &m);
		return Point<int>{rx,ry}; 
	#endif
	}

};
