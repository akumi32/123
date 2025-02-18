#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Entity.h"
class GameState :
	public State
{

private:
	Entity player;
	sf::Texture frameTexture;
	sf::RectangleShape background;
	//func

	void initKeybinds();


public: GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	  virtual  ~GameState();


	  //func
	  void endState();
	  void updateInput(const float& dt);
	  void update(const float& dt);
	  void render(sf::RenderTarget* target=nullptr);


};

#endif