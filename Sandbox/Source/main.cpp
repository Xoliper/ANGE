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
	window->SetClearColor(Color{ 255,255,0,255 });
	int a;
	auto font = new Font("arial.ttf");
	font->LoadFontSize(19);

	auto button = new SimpleButton(
		window,
		{ {250,150}, {150, 80}, Anchor::HorizontalCenter | Anchor::VerticalCenter },
		{ Color{0, 0, 0, 255}, Color{255, 255, 255, 255}, {1,1} },
		{ font, 19, L"Close" }
	);
	button->SetResizeProportions(50, 50, 0, 0);
	button->SetFontColor(Color{ 255,255,255,255 });
	button->SetColor(WidgetMouseState::Hover, Color{ 255,0,0,255 });

	while (window->Operate())
	{
		if (button->GetState() == WidgetMouseState::Press) window->Close();
		//We can also set callback function for button.
		//Notice: No need to invoke "btn->Render()" when parent window have WindowFlags::AutoInvokeRender flag set.
		window->ClearScene();
	}

	delete button;
	delete font;
	delete window;


	return 0;
}