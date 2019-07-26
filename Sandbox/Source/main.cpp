#include "Ange.h"
using namespace Ange;

int main()
{

	auto window = new Window(
		nullptr,
		"ANGE Hello world!",
		{ {300,200}, {500,300}, WindowFlags::ChildAutoOperate | WindowFlags::AutoInvokeRender | WindowFlags::FifoDrawable }
	);
	window->Init();
	window->SetMinMaxDimensions(900, 300, -1, -1);
	window->SetClearColor(Color{0xA0A4B8});

	auto font = new Font("arial.ttf");
	font->LoadFontSize(12);

	Theme wf = DefTheme;
	wf.ContentText.UsedFont = font;
	wf.SimpleButtonBG.TextTh.UsedFont = font;
	wf.SimpleInput.Text.UsedFont = font;
	wf.SimpleInput.DefaultText.UsedFont = font;

	auto tex = new Texture("circlebottom.png");

	//Button test
	auto button = new SimpleButton<Background>(
		window,
		{ {250,150}, {150, 80}, Anchor::HorizontalCenter | Anchor::VerticalCenter },
		wf.SimpleButtonBG,
		std::wstring(L"Add some offsets!"), tex
	);
	button->SetResizeProportions(50, 50, 0, 0);


	//Input test
	auto input = new SimpleInput(
		window,
		{ {250, 34}, {250, 24}, Anchor::HorizontalCenter | Anchor::VerticalCenter },
		wf.SimpleInput,
		L"Default"
	);
	input->SetText(L"hmm?");
	input->SetResizeProportions(50, 50, 50, 0);
	input->SetCallback([&window, &input](Event* ev)->bool {
		std::string temp = utf8_encode((std::wstring)input->GetTextRef());
		window->SetWindowTitle(temp);
		return true;
	});
	
	SimpleInput copy(*input);
	copy.SetText(L"Dupa");
	copy.SetPosition({ 150, 50 });

	swap(copy, *input);

	auto scrArea = new AreaWidget(
		window,
		{ {500, 150}, {350, 130}, Anchor::Left | Anchor::VerticalCenter }
	);
	scrArea->SetResizeProportions(75, 50, 25, 50);

	auto scroller = new VScroller(
		window,
		{ {860, 150}, {20, 130}, Anchor::Left | Anchor::VerticalCenter | ScrollerFlags::SmartPlacement | ScrollerFlags::AutoDisable},
		wf.VScroller
	);

	scroller->ConnectArea(scrArea);
	scroller->SetResizeProportions(100, 50, 0, 50);

	for (int i = 0; i < 40; i++) {
		scroller->PushBack(new Background(window, { {0,0}, {113, 113}}, BackgroundTheme{ {255 - i * 15, i * 10, i * 20, 255 } }));
	}
	
	button->SetCallback([&window, &scroller, &scrArea](Event* ev)->bool {
		if (ev->GetEventType() == EventType::MouseClick) {
			
			scroller->SetInsertOffsets({ 5,10 });
			scroller->SetFlags(Anchor::Left | Anchor::Bottom | ScrollerFlags::SmartPlacement | ScrollerFlags::AutoDisable);
			scrArea->SetFlags(Anchor::Left | Anchor::Bottom);
		}
		return true;
	});

	while (window->Operate())
	{
		//We can also use polling instead of setting callback function for button.
		//if (button->GetState() == WidgetMouseState::Press) window->Close();



		//Notice: No need to invoke "btn->Render()" when parent window have WindowFlags::AutoInvokeRender flag set.
		window->ClearScene();
	}

	delete scroller;
	//delete button;
	delete font;
	delete window;


	return 0;
}
