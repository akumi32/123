#include "Game.h"



void Game::initVariables()
{
	this->window = NULL;
	this->fullscreen = false;
	this->dt = 0.f;

}

void Game::initWindow()
{
	std::ifstream ifs("Config/window.ini");
	this->videoModes = sf::VideoMode::getFullscreenModes();
	std::string title = "None";
	sf::VideoMode window_bounds= sf::VideoMode::getDesktopMode();
	
	bool fullscreen = false;


	unsigned framerate_limit = 120;
		bool vertical_sync_enabled = false;
		unsigned antialiasing_level = 0;
	if(ifs.is_open())
	{
		std::getline(ifs, title);
		ifs >> window_bounds.width >> window_bounds.height;
		ifs >> fullscreen;
		ifs >> framerate_limit;
		ifs >> vertical_sync_enabled;
		ifs >> antialiasing_level;


	}
	ifs.close();

	this->fullscreen = fullscreen;
	this->windowSettings.antialiasingLevel = antialiasing_level;
	if(this->fullscreen)
		this->window = new sf::RenderWindow(window_bounds,title,sf::Style::Fullscreen,windowSettings);
	else
		this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Close, windowSettings);
	this->window->setFramerateLimit(framerate_limit);
	this->window->setVerticalSyncEnabled(vertical_sync_enabled);
}


void Game::initKeys()
{
	std::ifstream ifs("Config/supported_keys.ini");
	if(ifs.is_open())
	{
		std::string key = "";
		int key_value = 0;
		while (ifs >> key >> key_value)
		{
			this->supportedKeys[key] = key_value;

		}
	}
	ifs.close();
	
	for (auto i:this->supportedKeys)
	{
		std::cout << i.first << " " << i.second << std::endl;

	}

}

void Game::initStates()
{
	this->states.push(new MainMenuState(this->window,&this->supportedKeys,&this->states));

}


Game::Game()
{
	this->initWindow();
	
	this->initKeys();
	this->initStates();
}
Game::~Game()
{
	delete this->window;
	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}

}

//func

void Game::endApplication()
{
	std::cout << "Ending app" << std::endl;
}

void Game::updateDt()
{//update dt variable wiht time it takes to upate n render one frame
	this->dt = this->dtClock.restart().asSeconds();

}

void Game::updateSFMLEvents()
{
	while (this->window->pollEvent(this->sfEvent))
	{

		switch (this->sfEvent.type)
		{

		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->sfEvent.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;

		}
	}

}

void Game::update()
{
	this->updateSFMLEvents();
	if (!this->states.empty())
	{
		this->states.top()->update(this->dt);
		
		if(this->states.top()->getQuit())
		{
			this->states.top()->endState();

			delete this->states.top();
			this->states.pop();
		}
	}
	//application end
	else
	{
		this->endApplication();
		this->window->close();
	}
}


void Game::render()
{

	this->window->clear();
	if (!this->states.empty())
		this->states.top()->render();

	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->updateDt();

		this->update();
		this->render();

	}
}


