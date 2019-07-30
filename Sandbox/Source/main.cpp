#include "Ange.h"
#include <random> //<- For random colors
using namespace Ange;

int main()
{
	//-----------------------------------------------------------------------------------
	//Init
	//-----------------------------------------------------------------------------------

	//Create main window
	Window mainWindow(
		nullptr,
		"ANGE Preview",
		{ {300,200}, {500,300}, WindowFlags::ChildAutoOperate | WindowFlags::AutoInvokeRender | WindowFlags::FifoDrawable }
	);
	mainWindow.Init();
	mainWindow.SetMinMaxDimensions(800, 500, -1, -1);
	mainWindow.SetClearColor(Color{255,255,255,255});

	//Load main font
	Font font("segoeui.ttf");
	font.LoadFontSize(12);
	font.LoadFontSize(24);
	font.LoadFontSize(19);
	font.LoadFontSize(15);

	//Create sub-theme and attatch font
	Theme theme = DefTheme;
	theme.AssignFontToAll(&font);

	//Load some texture
	Texture angeTex("ange.png");


	//-----------------------------------------------------------------------------------
	//Create scene
	//-----------------------------------------------------------------------------------

	auto dim = mainWindow.GetDimension();

	//Window widget
	Window header(&mainWindow, "Header", { {0,  (int)dim.tHeight - 48 }, {dim.tWidth, 48} });
	header.Init();
	header.SetResizeProportions(0, 100, 100, 0);
	
	//Backgorund widget
	Background bgHeader(
		&header,
		{ {0,0}, {0,0},  Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill },
		theme
	);

	//Text widget
	Ange::Text txtHeader(
		&header,
		{ { 20,3 }, { 400, (size_t)font.GetLineHeight(19) }, Anchor::Left | Anchor::Bottom },
		theme.Header1,
		L"Widgets preview"
	);

	//Window widget
	Window content(&mainWindow, "Content", { {0, 0}, {dim.tWidth, dim.tHeight-48} });
	content.Init();
	content.SetResizeProportions(0, 0, 100, 100);

	//Backgorund widget
	Background bgContent(
		&content,
		{ { 0,0 }, { 0,0 },  Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill },
		{ {255, 255, 255, 255}, {230,233,235,255}, {1,1} }
	);
	bgContent.SetResizeProportions(0, 0, 100, 100);

	Image img(
		&content,
		{ {60,310},{0,0}, Anchor::Left | Anchor::Bottom | ImageFlags::DetectSize },
		theme.Image,
		&angeTex
	);
	img.SetResizeProportions(0,100,0,0);

	Ange::Text imgText(
		&content,
		{ { 30,350 }, { 400, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Image"
	);
	imgText.ChangeRotation(90.0f);
	imgText.SetResizeProportions(0, 100, 0, 0);


	SimpleButton<Background> btnProgressbar(
		&content,
		{ {50,212},{130,80}, Anchor::Left | Anchor::Bottom },
		theme,
		L"Add 5%"
	);
	btnProgressbar.SetResizeProportions(0, 100, 0, 0);


	SimpleButton<Background> btnVScroller(
		&content,
		{ {200,212},{130,80}, Anchor::Left | Anchor::Bottom },
		theme,
		L"Add to VScr"
	);
	btnVScroller.SetResizeProportions(0, 100, 0, 0);


	Ange::Text btnText(
		&content,
		{ { 30,220 }, { 400, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Buttons"
	);
	btnText.ChangeRotation(90.0f);
	btnText.SetResizeProportions(0, 100, 0, 0);



	SimpleInput input(
		&content,
		{ {50,130},{280,30}, Anchor::Left | Anchor::Bottom },
		theme,
		L"Default text"
	);
	input.SetResizeProportions(0, 100, 0, 0);

	Ange::Text inpText(
		&content,
		{ { 30,120 }, { 400, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Input"
	);
	inpText.ChangeRotation(90.0f);
	inpText.SetResizeProportions(0, 100, 0, 0);


	auto pbt = theme.ContentText;
	pbt.Tint = { 0,0,0,255 };
	float val = 0.2f;
	ProgressBar pb(
		&content,
		{ {50, 60}, {280, 30}, Anchor::Left | Anchor::Bottom |ProgressBarFlags::PrecentageInfo|ProgressBarFlags::AutoUpdate },
		BackgroundTheme(),
		Color(100, 100, 255, 255),
		pbt,
		L"Working... ", 1.0f
	);
	pb.SetResizeProportions(0, 100, 0, 0);


	Ange::Text pbText(
		&content,
		{ { 30,20 }, { 80, (size_t)font.GetLineHeight(15)*2 }, Anchor::Left | Anchor::Bottom | TextFlags::Multiline },
		theme.Header3,
		L"Progress Bar"
	);
	pbText.ChangeRotation(90.0f);
	//pbText.SetResizeProportions(0, 100, 0, 0);


	auto contentDim = content.GetDimension();
	AreaWidget aw(
		&content,
		{ {450, 10}, {contentDim.tWidth-450-17-4, contentDim.tHeight-20}, Anchor::Left|Anchor::Bottom }
	);
	aw.SetResizeProportions(0, 0, 100, 100);
	VScroller scroller(
		&content,
		{ {(int)contentDim.tWidth - 15, 10}, {17, contentDim.tHeight-20}, Anchor::Left|Anchor::Bottom | ScrollerFlags::SmartPlacement|ScrollerFlags::AutoDisable  },
		theme.VScroller
	);
	scroller.SetInsertOffsets({10,10});
	scroller.ConnectArea(&aw);
	scroller.SetResizeProportions(100, 0, 0, 100);

	Ange::Text vsText(
		&content,
		{ { 430, 162 }, { 300, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Vertical Scroller"
	);
	vsText.ChangeRotation(90.0f);
	vsText.SetResizeProportions(0, 50, 0, 0);

	//-----------------------------------------------------------------------------------
	//Connect widgets
	//-----------------------------------------------------------------------------------


	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, 255); // define the range

	btnVScroller.SetCallback([&content, &scroller, &distr, &eng](Event* ev)->bool{
		if (ev->GetEventType() == EventType::MouseClick) {
			
			//Check if this is release click event
			MouseClickEvent* mce = (MouseClickEvent*)ev;
			if (mce->GetAction() == 0) return false;

			scroller.PushBack(
				new Background(
					&content,
					{ {0,0}, {100, 100} },
					BackgroundTheme({ distr(eng), distr(eng), distr(eng), 255 }, { 0,0,0,255 }, { 1,1 })
				)
			);
			return true;
		}
		return false;
	});

	btnProgressbar.SetCallback([&val](Event*ev)->bool {
		if (ev->GetEventType() == EventType::MouseClick){
			MouseClickEvent* mce = (MouseClickEvent*)ev;
			if (mce->GetAction() == 0) return false;
			val += 0.05f;
			return true;
		}
		return false;
	});

	pb.SetToObserve(&val);

	//-----------------------------------------------------------------------------------
	//Program Loop
	//-----------------------------------------------------------------------------------

	while (mainWindow.Operate())
	{
		mainWindow.ClearScene();
	}

	return 0;
}
