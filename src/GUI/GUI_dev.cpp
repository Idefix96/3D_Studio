// Always include the necessary header files.
// Including SFGUI/Widgets.hpp includes everything
// you can possibly need automatically.
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>


class Button
{
protected:
	sf::RenderWindow*   m_ParentWindow;
	sf::Sprite			m_Sprite;
	sf::Texture			m_EmptyTexture;
	sf::Vector2f		m_Size;
	sf::Vector2f		m_Position;
	sf::Text			m_Text;
	sf::Font			m_Font;

public:

	virtual void Action() = 0;
	virtual void Draw() = 0;
	bool isMouseOver()
	{
		if ((sf::Mouse::getPosition(*m_ParentWindow).x >= m_Sprite.getGlobalBounds().left) && (sf::Mouse::getPosition(*m_ParentWindow).x <= m_Sprite.getGlobalBounds().left + m_Sprite.getGlobalBounds().width))
			if ((sf::Mouse::getPosition(*m_ParentWindow).y >= m_Sprite.getGlobalBounds().top) && (sf::Mouse::getPosition(*m_ParentWindow).y <= m_Sprite.getGlobalBounds().top + m_Sprite.getGlobalBounds().height))
			{
				return true;
			}

		return false;
	}

	bool isClicked()
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			if (isMouseOver())				
			{
				
					return true;
			}

		return false;
	}

	void setSize(sf::Vector2f size)
	{
		m_Size = size;
		Resize();
	}

	void Resize()
	{
		m_Sprite.setScale(m_Size.x / m_Sprite.getGlobalBounds().width, m_Size.y / m_Sprite.getGlobalBounds().height);

	}

	void setWindow(sf::RenderWindow* window)
	{
		m_ParentWindow = window;
	}

	void setPosition(sf::Vector2f position)
	{
		m_Sprite.setPosition(position);
	}

	void setText(std::string text)
	{
		m_Text.setString(text);
		
		m_Text.setPosition(m_Sprite.getPosition());
		
	}
};

class QuitButton: public Button
{
public:
	QuitButton(sf::RenderWindow* window)
	{
		m_Size = sf::Vector2f(200.f, 50.f);
		m_Sprite.setColor(sf::Color(100, 100, 100));
		m_EmptyTexture.create(m_Size.x, m_Size.y);
		sf::Uint8* pixels = new sf::Uint8[m_Size.x * m_Size.y * 4]; // * 4 because pixels have 4 components (RGBA)
		m_EmptyTexture.update(pixels);
		m_Sprite.setTexture(m_EmptyTexture);
		m_ParentWindow = window;
		m_Font.loadFromFile("arial.ttf");
		m_Text.setFont(m_Font);
		
	}

	void Action()
	{
		m_ParentWindow->close();
	}

	void Draw()
	{
		if(isClicked())
			Action();
		m_ParentWindow->draw(m_Sprite);
	
		m_ParentWindow->draw(m_Text);
		
	}
};

class UseableObject
{
private:
	sf::Sprite			m_Sprite;
	sf::Sprite			m_SmallSprite;
	sf::Texture			m_Texture;
	sf::Texture			m_EmptyTexture;
	sf::RenderWindow*	m_ParentWindow;
	sf::Vector2f		m_Size;
	sf::Vector2f		m_Position;
	bool				m_Active;
	bool				m_Empty;
public:
	UseableObject()
	{
		m_Size = sf::Vector2f(100.f, 100.f);
		m_Sprite.setColor(sf::Color(100, 100, 100));
		m_EmptyTexture.create(100, 100);
		sf::Uint8* pixels = new sf::Uint8[100 * 100 * 4]; // * 4 because pixels have 4 components (RGBA)

		m_EmptyTexture.update(pixels);
		m_Sprite.setTexture(m_EmptyTexture);

		m_SmallSprite = m_Sprite;
		m_SmallSprite.setScale(0.5, 0.5);
	}

	UseableObject(sf::RenderWindow* window)
	{
		m_Size = sf::Vector2f(100.f, 100.f);
		m_Sprite.setColor(sf::Color(100,100,100));
		m_EmptyTexture.create(100, 100);
		sf::Uint8* pixels = new sf::Uint8[100 * 100 * 4]; // * 4 because pixels have 4 components (RGBA)
		
		m_EmptyTexture.update(pixels);
		m_Sprite.setTexture(m_EmptyTexture);
	
		m_ParentWindow = window;
		m_SmallSprite = m_Sprite;
		m_SmallSprite.setScale(0.25, 0.25);
	}

	void setSize(sf::Vector2f size)
	{
		m_Size = size;
		Resize();
	}

	void Resize()
	{
		m_Sprite.setScale(m_Size.x/m_Sprite.getGlobalBounds().width, m_Size.y / m_Sprite.getGlobalBounds().height);
		m_SmallSprite = m_Sprite;
		m_SmallSprite.setScale(0.25, 0.25);
	}

	void setWindow(sf::RenderWindow* window)
	{
		m_ParentWindow = window;
	}

	void Draw()
	{
		if (isClicked())
			m_Active = true;
		m_ParentWindow->draw(m_Sprite);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			m_Active = false;
		}

		if (m_Active)
		{		
			m_SmallSprite.setPosition(sf::Vector2f(sf::Mouse::getPosition(*m_ParentWindow).x, sf::Mouse::getPosition(*m_ParentWindow).y));
			m_ParentWindow->draw(m_SmallSprite);
		}
	}

	bool isClicked()
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			if ((sf::Mouse::getPosition(*m_ParentWindow).x >= m_Sprite.getGlobalBounds().left) && (sf::Mouse::getPosition(*m_ParentWindow).x <= m_Sprite.getGlobalBounds().left + m_Sprite.getGlobalBounds().width))
				if ((sf::Mouse::getPosition(*m_ParentWindow).y >= m_Sprite.getGlobalBounds().top) && (sf::Mouse::getPosition(*m_ParentWindow).y <= m_Sprite.getGlobalBounds().top + m_Sprite.getGlobalBounds().height))
				{
					return true;
				}	
		return false;
	}

	void setPosition(sf::Vector2f position)
	{
		m_Sprite.setPosition(position);
	}
};

class TableOfUsables
{
#define TABLE_COLUMNS	10
#define TABLE_ROWS 2
#define DEFAULT_WIDTH	800
#define DEFAULT_HEIGHT	600

private:
	sf::Vector2f m_Position;
	sf::RenderWindow* m_ParentWindow;
	float m_Width;
	float m_Height;
	float m_Distance;
	float m_ItemHeight;
	float m_ItemWidth;
	sf::Sprite m_Background;
	sf::Texture			m_EmptyTexture;

	UseableObject m_UIFields[TABLE_COLUMNS][TABLE_ROWS];
	
public:
	TableOfUsables(sf::RenderWindow* window)
	{
		m_ParentWindow = window;
		m_Width = window->getSize().x*2.f / 3.f;
		m_Height = window->getSize().y / 4.f;
		m_Position = sf::Vector2f(5.f, window->getSize().y*3.f / 4.f);

		m_Background.setColor(sf::Color(50, 50, 50));
		m_EmptyTexture.create(m_Width + 5.f, m_Height + 5.f);
		sf::Uint8* pixels = new sf::Uint8[(m_Width +5.f) * (m_Height+5.f) * 4]; // * 4 because pixels have 4 components (RGBA)
		m_EmptyTexture.update(pixels);
		m_Background.setTexture(m_EmptyTexture);
		m_Background.setPosition(m_Position - sf::Vector2f(5.f, 5.f));
		m_Distance = 5.f;  //Distance between fields
		
		m_ItemWidth = m_Width / TABLE_COLUMNS - m_Distance;
		m_ItemHeight = m_Height / TABLE_ROWS  - m_Distance;

		for(int i = 0; i < TABLE_COLUMNS; i++)
			for (int j = 0; j < TABLE_ROWS; j++)
			{
				m_UIFields[i][j].setSize(sf::Vector2f(m_ItemWidth, m_ItemHeight));
				m_UIFields[i][j].setPosition(sf::Vector2f((i+1)*m_Position.x + i*m_ItemWidth, m_Position.y + j*(m_ItemHeight + 5.f)));
				m_UIFields[i][j].setWindow(m_ParentWindow);
			}
	}

	void Draw()
	{
		m_ParentWindow->draw(m_Background);
		for (int i = 0; i < TABLE_COLUMNS; i++)
			for (int j = 0; j < TABLE_ROWS; j++)
			{
				m_UIFields[i][j].Draw();
			}
	}

	float getWidth()
	{
		return m_Width;
	}
	float getHeight()
	{
		return m_Height;
	}

};

