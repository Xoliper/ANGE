#include "Ange.h"
#include "Ange/Core/Modules/Graphics/ComplexWidgets.h"
#include "Ange/Core/Modules/Graphics/Font.h"

using namespace Ange;

int main()
{

	auto window = new Window(
		nullptr,
		"ANGE Hello world!",
		{ {300,200}, {500,300}, WindowFlags::ChildAutoOperate | WindowFlags::AutoInvokeRender | WindowFlags::FifoDrawable }
	);
	window->Init();
	window->SetMinMaxDimensions(500, 300, -1, -1);
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
		std::wstring(L"Close"), tex
	);
	button->SetResizeProportions(50, 50, 0, 0);
	button->SetCallback([&window](Event* ev)->bool {
		if (ev->GetEventType() == EventType::MouseClick) {
			//window->Close();
		}
		return true;
	});
	
	//Input test
	auto input = new SimpleInput(
		window,
		{ {250, 34}, {250, 24}, Anchor::HorizontalCenter | Anchor::VerticalCenter },
		wf.SimpleInput,
		L"Default"
	);
	input->SetResizeProportions(50, 50, 50, 0);
	input->SetCallback([&window, &input](Event* ev)->bool {
		std::string temp = utf8_encode((std::wstring)input->GetTextRef());
		window->SetWindowTitle(temp);
		return true;
	});

	while (window->Operate())
	{
		//We can also use polling instead of setting callback function for button.
		//if (button->GetState() == WidgetMouseState::Press) window->Close();

		//Notice: No need to invoke "btn->Render()" when parent window have WindowFlags::AutoInvokeRender flag set.
		window->ClearScene();
	}

	delete button;
	delete font;
	delete window;


	return 0;
}
