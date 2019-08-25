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
		{ {300,50}, {500,300}, WindowFlags::ChildAutoOperate | WindowFlags::AutoInvokeRender | WindowFlags::FifoDrawable }
	);
	mainWindow.Init();
	mainWindow.SetMinMaxDimensions(800, 650, -1, -1);
	mainWindow.SetClearColor(Color{ 255,255,255,255 });

	//Load main font
	Font font("segoeui.ttf");

	//Create sub-theme and attatch font
	Theme theme = DefTheme;
	theme.AssignFontToAll(&font);

	//Load some texture
	Texture angeTex("ange.png");
	Texture minusTex("minus.png");



	//-----------------------------------------------------------------------------------
	//Create scene
	//-----------------------------------------------------------------------------------

	auto dim = mainWindow.GetDimension();

	//Top window for ContextMenu
	Window topWnd(&mainWindow, "TOP", { {0,0}, dim, WindowFlags::AutoInvokeRender | WindowFlags::FifoDrawable });
	topWnd.SetResizeProportions(0, 0, 100, 100);
	topWnd.Init();

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
	Window content(&mainWindow, "Content", { {0, 0}, {dim.tWidth, dim.tHeight - 48} , WindowFlags::ChildAutoOperate | WindowFlags::AutoInvokeRender | WindowFlags::FifoDrawable });
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
		{ {60,460},{0,0}, Anchor::Left | Anchor::Bottom | ImageFlags::DetectSize },
		theme.Image,
		&angeTex
	);
	img.SetResizeProportions(0, 100, 0, 0);

	Ange::Text imgText(
		&content,
		{ { 30,500 }, { 400, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Image"
	);
	imgText.ChangeRotation(90.0f);
	imgText.SetResizeProportions(0, 100, 0, 0);


	SimpleButton<Background> btnProgressbar(
		&content,
		{ {50,362},{130,80}, Anchor::Left | Anchor::Bottom },
		theme,
		L"Add 5%"
	);
	btnProgressbar.SetResizeProportions(0, 100, 0, 0);


	SimpleButton<Background> btnVScroller(
		&content,
		{ {200,362},{130,80}, Anchor::Left | Anchor::Bottom },
		theme,
		L"Add to VScr"
	);
	btnVScroller.SetResizeProportions(0, 100, 0, 0);


	Ange::Text btnText(
		&content,
		{ { 30,370 }, { 400, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Buttons"
	);
	btnText.ChangeRotation(90.0f);
	btnText.SetResizeProportions(0, 100, 0, 0);



	SimpleInput input(
		&content,
		{ {50,280},{280,30}, Anchor::Left | Anchor::Bottom },
		theme,
		L"Default text"
	);
	input.SetResizeProportions(0, 100, 0, 0);

	Ange::Text inpText(
		&content,
		{ { 30,270 }, { 400, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Input"
	);
	inpText.ChangeRotation(90.0f);
	inpText.SetResizeProportions(0, 100, 0, 0);



	float val = 0.2f;
	ProgressBar pb(
		&content,
		{ {50, 210}, {280, 30}, Anchor::Left | Anchor::Bottom | ProgressBarFlags::PrecentageInfo | ProgressBarFlags::AutoUpdate },
		theme.ProgressBar,
		L"Working... ", 1.0f
	);
	pb.SetResizeProportions(0, 100, 0, 0);


	Ange::Text pbText(
		&content,
		{ { 30,170 }, { 80, (size_t)font.GetLineHeight(15) * 2 }, Anchor::Left | Anchor::Bottom | TextFlags::Multiline },
		theme.Header3,
		L"Progress Bar"
	);
	pbText.ChangeRotation(90.0f);
	pbText.SetResizeProportions(0, 100, 0, 0);


	auto contentDim = content.GetDimension();
	AreaWidget aw(
		&content,
		{ {450, 10}, {contentDim.tWidth - 450 - 17 - 4, contentDim.tHeight - 20}, Anchor::Left | Anchor::Bottom }
	);
	aw.SetResizeProportions(0, 0, 100, 100);
	VScroller scroller(
		&content,
		{ {(int)contentDim.tWidth - 15, 10}, {17, contentDim.tHeight - 20}, Anchor::Left | Anchor::Bottom | ScrollerFlags::SmartPlacement | ScrollerFlags::AutoDisable },
		theme.VScroller
	);
	scroller.SetInsertOffsets({ 10,10 });
	scroller.ConnectArea(&aw);
	scroller.SetResizeProportions(100, 0, 0, 100);

	Ange::Text vsText(
		&content,
		{ { 430, 262 }, { 300, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Vertical Scroller"
	);
	vsText.ChangeRotation(90.0f);
	vsText.SetResizeProportions(0, 50, 0, 0);


	//Context menu example:

	ContextMenu cm(
		&topWnd,
		{ 160,0 },
		theme.ContextMenu
	);
	ContextMenuItem* cmi1 = cm.AddItem(L"Subtract 10%", &minusTex);
	cm.AddItem(L"This do nothing...");
	cm.AddDivider({ 0,0,0,255 });
	ContextMenuItem* cmi2 = cm.AddItem(L"Exit App");
	cm.DisableWidget();

	SimpleButton<Background> contextTest(
		&topWnd,
		{ {0,0}, dim, Anchor::Left | Anchor::Bottom },
		{
			{{0,0,0,0}, {0,0,0,0}},
			{{0,0,0,0}, {0,0,0,0}},
			{{0,0,0,0}, {0,0,0,0}},
			{0,0},
			{0, {0,0,0,0}}
		}
		);

	//Checkbox example
	Ange::Text cbText(
		&content,
		{ { 10,110 }, { 100, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Checkbox"
	);
	cbText.SetResizeProportions(0, 100, 0, 0);
	Checkbox cb(&content, { {120, 114}, {18, 18}, Anchor::Left | Anchor::Bottom }, theme.Checkbox);
	cb.SetResizeProportions(0, 100, 0, 0);

	Ange::Text cbtText(
		&content,
		{ { 10,84 }, { 420, (size_t)font.GetLineHeight(12) }, Anchor::Left | Anchor::Bottom },
		theme.ContentText,
		L"Tip: Please click on checkbox."
	);
	cbtText.SetResizeProportions(0, 100, 0, 0);
	cbtText.SetColor({ 255,0,0,255 });

	//Ratio example
	Ange::Text rText(
		&content,
		{ { 10,50 }, { 100, (size_t)font.GetLineHeight(15) }, Anchor::Left | Anchor::Bottom },
		theme.Header3,
		L"Ratio"
	);
	rText.SetResizeProportions(0, 100, 0, 0);

	Ratio ratio(&content);
	ratio.AddOption(0, { {119, 54}, {18, 18}, Anchor::Left | Anchor::Bottom }, theme.Checkbox);
	ratio.AddOption(3, { {149, 54}, {18, 18}, Anchor::Left | Anchor::Bottom }, theme.Checkbox);
	ratio.AddOption(4, { {179, 54}, {18, 18}, Anchor::Left | Anchor::Bottom }, theme.Checkbox);
	ratio.AddOption(50, { {209, 54}, {18, 18}, Anchor::Left | Anchor::Bottom }, theme.Checkbox);
	ratio.SetSelection(0);
	ratio.SetResizeProportions(0, 100, 0, 0);


	Ange::Text rtText(
		&content,
		{ { 10,24 }, { 300, (size_t)font.GetLineHeight(12) }, Anchor::Left | Anchor::Bottom },
		theme.ContentText,
		L""
	);
	rtText.SetResizeProportions(0, 100, 0, 0);


	//-----------------------------------------------------------------------------------
	//Connect widgets
	//-----------------------------------------------------------------------------------


	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, 255); // define the range

	btnVScroller.SetCallback([&content, &scroller, &distr, &eng](Event* ev)->bool {
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
		if (ev->GetEventType() == EventType::MouseClick) {
			MouseClickEvent* mce = (MouseClickEvent*)ev;
			if (mce->GetAction() == 0) return false;
			val += 0.05f;
			return true;
		}
		return false;
	});

	pb.SetToObserve(&val);

	contextTest.SetCallback([&cm](Event* ev)->bool {
		if (ev->GetEventType() == EventType::MouseClick) {
			MouseClickEvent* mce = (MouseClickEvent*)ev;
			if (mce->GetAction() == 0 && mce->GetButton() == 1) {
				cm.SetPosition(mce->GetPosition());
				cm.EnableWidget();
				return true;
			}
		}
		return false;
	});

	cmi1->SetCallback([&val](Event* ev) {
		if (ev->GetEventType() == EventType::MouseClick) {
			val -= 0.1f;
		}
		return true;
	});

	cmi2->SetCallback([&mainWindow](Event* ev) {
		if (ev->GetEventType() == EventType::MouseClick) {
			mainWindow.Close();
		}
		return true;
	});

	cb.SetCallback([&cbtText](Event* ev) {
		CheckboxChange* cc = (CheckboxChange*)ev;
		//Checkbox* widget = (Checkbox*)cc->GetWidget();
		if (cc->GetState() == true)
		{
			cbtText.SetColor({ 0,0,255,255 });
			cbtText.SetText(L"Hurray! You just changed the checkbox status and this text.");
		}
		else {
			cbtText.SetColor({ 255,0,0,255 });
			cbtText.SetText(L"Tip: Please click on checkbox.");
		}

		return true;
	});

	ratio.SetCallback([&rtText](Event* ev) {
		RatioChange* r = (RatioChange*)ev;
		Ratio* rt = (Ratio*)r->GetWidget();
		int i = r->GetSelectedFieldId();

		if (rt->GetSelection() == std::numeric_limits<int>::min()) {
			rtText.SetText(L"Currently selected: None");
		}
		else {
			wchar_t buf[64];
			swprintf(buf, 64, L"Currently selected: %i", i);
			rtText.SetText(buf);
		}
		return true;
	});

	//-----------------------------------------------------------------------------------
	//Program Loop
	//-----------------------------------------------------------------------------------

	while (mainWindow.Operate())
	{
		mainWindow.ClearScene();
	}

	return 0;
}
