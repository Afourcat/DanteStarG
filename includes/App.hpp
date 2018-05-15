/*
** EPITECH PROJECT, 2017
** Maze.hpp
** File description:
** Maze class
*/

#ifndef MAP_HPP_
#define MAP_HPP_

#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include "Maze.hpp"

class App {
public:

	// Constructor
	App(int argc, char *argv[]) :
		win(sf::VideoMode(1000, 1000, 32), "Dante"),
		speed(2),
		perfect(false),
		x(10),
		y(10)
	{
		if (argc < 3 || argc > 5)
			exit(1);
		x = std::atoi(argv[1]);
		y = std::atoi(argv[2]);
		if (!x || !y)
			exit(1);
		getOpt(argc, argv);
		win.setFramerateLimit(30);
		if (this->update)
			maze = Maze(x, y, win, speed, perfect);
		else
			maze = Maze(x, y, perfect);
	};

	// Destructor
	virtual ~App() 
	{
	};
	
	int run()
	{
		sf::Event event;

		while (this->win.isOpen()) { 
			while (this->win.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					this->win.close();
				}
			}
			this->win.clear();
			this->win.draw(this->maze);
			this->win.display();
		}
		return 0;
	};

private:

	void getOpt(int argc, char *argv[])
	{
		int c;

		while ((c = getopt(argc, argv, "pus:")) != -1) {
			switch (c) {
			case 's':
				this->speed = std::atof(optarg);
				break;
			case 'p':
				this->perfect = true;
				break;
			case 'u':
				this->update = true;
				break;
			case '?':
				std::cout << "Error while parsing" << std::endl;
				exit(1);
			}
		}
	};

// Attribute
	sf::RenderWindow win;
	float speed;
	bool perfect;
	bool update;
	int x;
	int y;
	Maze maze;
};

#endif /* MAP_HPP_ */
