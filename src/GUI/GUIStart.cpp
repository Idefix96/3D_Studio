#include "GUIStart.h"

GUIStart::GUIStart()
{
	//create elements
	m_Box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_StartButton = sfg::Button::Create("Spiel starten");
	m_OptionsButton = sfg::Button::Create("Optionen");
	m_QuitButton = sfg::Button::Create("Beenden");
	m_StartButton->SetClass("button");
	m_OptionsButton->SetClass("button");
	m_QuitButton->SetClass("button");

	m_Box->Pack(m_StartButton);
	m_Box->Pack(m_OptionsButton);
	m_Box->Pack(m_QuitButton);

	m_Box->SetSpacing(15.f);
}

GUIStart::~GUIStart()
{

}

void GUIStart::AddElements(std::shared_ptr<sfg::Window> window)
{
	window->SetClass("start");
	window->SetStyle(0);
	window->Add(m_Box);

	window->SetAllocation(sf::FloatRect(0.25*DEFAULT_WIDTH, 0.25*DEFAULT_HEIGHT, 0.5*DEFAULT_WIDTH, 0.5*DEFAULT_HEIGHT));
	window->SetPosition(sf::Vector2f(DEFAULT_WIDTH / 2 - window.get()->GetAllocation().width / 2.0, DEFAULT_HEIGHT / 2 - window.get()->GetAllocation().height / 2.0));

	//define the classes
	sfg::Context::Get().GetEngine().SetProperty(".start", "BackgroundColor", sf::Color::Transparent);
	sfg::Context::Get().GetEngine().SetProperty(".button", "FontSize", 50);
	sfg::Context::Get().GetEngine().SetProperty(".button", "BackgroundColor", sf::Color::Black);

	m_StartButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {
		this->Remove();
	});

	m_QuitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {
		this->Quit();
	});
}

void GUIStart::Hide()
{	
	m_Box->Show(false);
}

