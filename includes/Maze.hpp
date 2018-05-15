/*
** EPITECH PROJECT, 2017
** Maze.hpp
** File description:
** Maze class
*/

#ifndef MAZE_HPP_
#define MAZE_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <random>
#include <time.h>

#define LEFT	0x01
#define UP	0x02
#define RIGHT	0x04
#define DOWN	0x08
#define MAP_UP    map[y - 1][x]
#define MAP_DOWN  map[y + 1][x]
#define MAP_LEFT  map[y][x - 1]
#define MAP_RIGHT map[y][x + 1]

class Maze : public sf::Drawable {
public:

	// Constructor
	Maze() {};

	Maze(int x, int y, bool perfect = false): 
		size_x(x), 
		size_y(y), 
		offset_x(1000 / x), 
		offset_y(1000 / y),
		speed(0.0),
		perfect(perfect),
		path(sf::RectangleShape(sf::Vector2f(offset_x, offset_y))),
		wall(sf::RectangleShape(sf::Vector2f(offset_x, offset_y)))
	{
		srandom(time(NULL));
		this->path.setFillColor(sf::Color::White);
		this->wall.setFillColor(sf::Color::Black);
		this->createMap();
	};

	Maze(int x, int y, sf::RenderWindow& win, float speed, bool perfect = false): 
		size_x(x), 
		size_y(y), 
		offset_x(1000 / x), 
		offset_y(1000 / y),
		speed(speed),
		perfect(perfect),
		path(sf::RectangleShape(sf::Vector2f(offset_x, offset_y))),
		wall(sf::RectangleShape(sf::Vector2f(offset_x, offset_y)))
	{
		srandom(time(NULL));
		this->path.setFillColor(sf::Color::White);
		this->wall.setFillColor(sf::Color::Black);
		this->createMap(win);
	};
	
	// Destructor
	virtual ~Maze() noexcept
	{
		for (int y = 0; y < this->size_y; ++y)
			map.pop_back();
	};
	
	virtual void draw(sf::RenderTarget& target) final
	{
		for (int y = 0; y < this->size_y; ++y) {
			for (int x = 0; x < this->size_x; ++x) {
				drawShape(target, x, y);
			}
		}
	};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) final
	{
		for (int y = 0; y < this->size_y; ++y) {
			for (int x = 0; x < this->size_x; ++x) {
				drawShape(target, x, y, states);
			}
		}
	};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final
	{
		for (int y = 0; y < this->size_y; ++y) {
			for (int x = 0; x < this->size_x; ++x) {
				if (this->map[y][x] == 'X') {
					wall.setPosition(sf::Vector2f(x * offset_x, y * offset_y));
					target.draw(wall, states);
				} else {
					path.setPosition(sf::Vector2f(x * offset_x, y * offset_y));
					target.draw(path, states);
				}
			}
		}
	};

private:

// Methods

	void updateFrame(sf::RenderWindow& win)
	{
		win.clear();
		win.draw(*this);
		win.display();
	}

	__attribute__((hot))
	void reccursive(int x, int y, sf::RenderWindow& win)
	{
		int ret = 0;

		map[y][x] = '*';
		ret = thereIsFreeCell(x, y);
		this->updateFrame(win);
		while (ret != -1) {
			switch (ret) {
			case UP:
				reccursive(x, y - 1, win);
				break;
			case DOWN:
				reccursive(x, y + 1, win);
				break;
			case LEFT:
				reccursive(x - 1, y, win);
				break;
			case RIGHT:
				reccursive(x + 1, y, win);
			}
			ret = thereIsFreeCell(x, y);
		}	
	};

	__attribute__((hot))
	void reccursive(int x, int y)
	{
		int ret = 0;

		map[y][x] = '*';
		ret = thereIsFreeCell(x, y);
		while (ret != -1) {
			switch (ret) {
			case UP:
				reccursive(x, y - 1);
				break;
			case DOWN:
				reccursive(x, y + 1);
				break;
			case LEFT:
				reccursive(x - 1, y);
				break;
			case RIGHT:
				reccursive(x + 1, y);
			}
			ret = thereIsFreeCell(x, y);
		}
	};	

	bool isFree(int x, int y) {
		int null_up = (y - 1 < 0) ? 1 : 0;
		int null_down = (y + 1 >= this->size_y) ? 1 : 0;
		int null_right = (x + 1 >= this->size_x) ? 1 : 0;
		int null_left = (x - 1 < 0) ? 1 : 0;
		int nb_star = 0;

		if (!null_up && MAP_UP == '*')
			nb_star++;
		if (!null_down && MAP_DOWN == '*')
			nb_star++;
		if (!null_left && MAP_LEFT == '*')
			nb_star++;
		if (!null_right && MAP_RIGHT == '*')
			nb_star++;
		return ((nb_star > 1) ? false : true);
	};

	int thereIsFreeCell(int x, int y)
	{
		int null_up = (y - 1 < 0) ? 1 : 0;
		int null_down = (y + 1 >= this->size_y) ? 1 : 0;
		int null_left = (x - 1 < 0) ? 1 : 0;
		int null_right = (x + 1 >= this->size_x) ? 1 : 0;
		int ret = 0;
		int i = 0;

		if (!null_up && MAP_UP != '*' && isFree(x, y - 1)) {
			ret |= UP;
		} if (!null_down && MAP_DOWN != '*' && isFree(x, y + 1)) {
			ret |= DOWN;
		} if (!null_left && MAP_LEFT != '*' && isFree(x - 1, y)) {
			ret |= LEFT;
		} if (!null_right && MAP_RIGHT != '*' && isFree(x + 1, y)) {
			ret |= RIGHT;
		} if (ret == 0)
			return (-1);
		while (!(ret & (i = this->VAL[random() % 4])));
		return (i);
	};

	void drawShape(sf::RenderTarget& target, int x, int y, sf::RenderStates states = sf::RenderStates())
	{
		if (this->map[y][x] == 'X') {
			wall.setPosition(sf::Vector2f(x * offset_x, y * offset_y));
			target.draw(wall, states);
		} else {
			path.setPosition(sf::Vector2f(x * offset_x, y * offset_y));
			target.draw(path, states);
		}
	}

	void createMap()
	{
		for (int y = 0; y < this->size_y; ++y)
			map.push_back(std::string(this->size_x, 'X'));
		this->reccursive(0, 0);
		this->end_lab(this->perfect);
	};

	void createMap(sf::RenderWindow& win)
	{
		for (int y = 0; y < this->size_y; ++y)
			map.push_back(std::string(this->size_x, 'X'));
		this->reccursive(0, 0, win);
		this->end_lab(this->perfect);
	};

	void end_lab(bool perfect)
	{
		int pt = rand() % size_y;
		int x = 0;
		int y = 0;

		if (map[size_y - 1][size_x - 1] == 'X') {
			if (map[size_y - 1][size_x - 2] == 'X' &&
					map[size_y - 2][size_x - 1] == 'X')
				map[size_y - 2][size_x - 1] = '*';
			else
				map[size_y - 2][size_x - 2] = 'X';
			map[size_y - 1][size_x - 1] = '*';
		}
		if (perfect)
			return ;
		for (int i = 0; i < pt; ++i) {
			x = rand() % size_x;
			y = rand() % size_y;
			if (map[y][x] == 'X')
				map[y][x] = '*';
		}
	}

// Attribute

	int size_x;
	int size_y;
	int offset_x;
	int offset_y;
	int frame = 0;
	float speed;
	bool perfect;
	int VAL[4] = {UP, DOWN, LEFT, RIGHT};
	std::vector<std::string> map;
	mutable sf::RectangleShape path;
	mutable sf::RectangleShape wall;
};

#endif /* MAZE_HPP_ */
