#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream> 
#include "map.h"
#include "view.h"
#include <vector>

using namespace sf;
using namespace std;

///////////////////////////////////////////////////////////////////////////
int hit(int PlayerV, int PlayerG, int MobV, int MobG, int range)
{
	if ((PlayerV - MobV < range && PlayerV - MobV > -range) && (PlayerG - MobG < range && PlayerG - MobG > -range))
		return 1;
	else
		return 0;
}

int trap(int PlayerV, int PlayerG)
{
	PlayerV /= 32;
	PlayerG /= 32;
	if (TileMap[PlayerV][PlayerG] == 't')
	{
		TileMap[PlayerV][PlayerG] = 'b';
		return 1;
	}
	else
		return 0;
}

int doorScript(int PlayerV, int PlayerG)
{
	PlayerV /= 32;
	PlayerG /= 32;
	if (TileMap[PlayerV][PlayerG] == 's')
	{
		TileMap[PlayerV][PlayerG] == ' ';
		return 1;
	}
	return 0;
}



int dif(int a, int b)
{
	int c = a - b;
	if (c < 0)
		c = c * -1;

	return c;
}

int MeleMob(int PlayerV, int PlayerG, int MobV, int MobG)
{

	int DifV = dif(PlayerV, MobV);
	int DifG = dif(PlayerG, MobG);


	if (DifV < 20 && DifG < 16/*Ренджа до которой моб сокращает дистанцию*/)
	{
		return 1; // EBOSH

	}////////////////////////////////сокращаем дистанцию
	else
	{
		if (DifV > DifG)
		{
			if (PlayerV > MobV)
				return 22;//бег вниз
			else
				return 11;//бег вверх
		}
		else
		{
			if (PlayerG > MobG)
				return 33;//бег вправо
			else
				return 44;//бег влево
		}
	}

}
int OrbitMob(int PlayerV, int PlayerG, int MobV, int MobG)
{
	const int orbit = 60; //внутренняя граница орбиты
	const int exorbit = 80;//внешняя граница орбиты (при зазоре менее 20 его жует)

	int DifV = dif(PlayerV, MobV);
	int DifG = dif(PlayerG, MobG);

	if (DifV < exorbit && DifG < exorbit/*Ренджа до которой моб сокращает дистанцию*/)
	{
		if ((MobG - PlayerG) <= orbit && (MobV - PlayerV) <= -orbit)
		{
			return 33;//<<<<<<<<<<
		}
		else if ((MobG - PlayerG) >=orbit && (MobV - PlayerV) <= orbit)
		{
			return 22;// ♠♠♠♠♠♠♠♠
		}
		else if ((MobG - PlayerG) >= -orbit && (MobV - PlayerV) >= orbit)
		{
			return 44;//>>>>>>>>>>
		}
		else if ((MobG - PlayerG) <= -orbit && (MobV - PlayerV) >= -orbit)
		{
			return 11;//♥♥♥♥♥♥♥
		}
	}

	////////////////////////////////сокращаем дистанцию
	else
	{
		if (DifV > DifG)
		{
			if (PlayerV > MobV)
				return 22;//бег вниз
			else
				return 11;//бег вверх
		}
		else
		{
			if (PlayerG > MobG)
				return 33;//бег вправо
			else
				return 44;//бег влево
		}
	}
}
int MageMob(int PlayerV, int PlayerG, int MobV, int MobG)
{

	int DifV = dif(PlayerV, MobV);
	int DifG = dif(PlayerG, MobG);

	if (DifV < 160 && DifG < 160/*Ренджа до которой моб сокращает дистанцию*/)
	{
		if (DifV < 9 || DifG < 9)
		{
			if (DifV > DifG)
			{
				if (PlayerV > MobV)
					return 2;//выстрел вниз
				else
					return 1;//выстрел вверх
			}
			else
			{
				if (PlayerG > MobG)
					return 3;//выстрел вправо
				else
					return 4;//выстрел влево
			}

		}
		else {

			if (DifV > DifG)
			{
				if (PlayerG > MobG)
					return 33;//бег вправо
				else
					return 44;
			}
			else


			{
				if (PlayerV > MobV)
					return 22;//бег вверх
				else
					return 11;//бег вниз
			}
		}

	}////////////////////////////////сокращаем дистанцию
	else
	{
		if (DifV > DifG)
		{
			if (PlayerV > MobV)
				return 22;//бег вниз
			else
				return 11;//бег вверх
		}
		else
		{
			if (PlayerG > MobG)
				return 33;//бег вправо
			else
				return 44;//бег влево
		}
	}
}




int moveCH(int obV, int obG, int door, int m)
{

	int jir = 32;

	obV += jir;
	obG += jir;

	obV /= 32;
	obG /= 32;


	if (m == 1 && (TileMap[obV - 1][obG] == '0' || (door == 1 && TileMap[obV - 1][obG] == '7')))
		return 0;
	if (m == 2 && (TileMap[obV + 1][obG] == '0' || (door == 1 && TileMap[obV + 1][obG] == '7')))
		return 0;
	if (m == 3 && (TileMap[obV][obG + 1] == '0' || (door == 1 && TileMap[obV][obG + 1] == '7')))
		return 0;
	if (m == 4 && (TileMap[obV][obG - 1] == '0' || (door == 1 && TileMap[obV][obG - 1] == '7')))
		return 0;

	return 1;
}




//  PlayerV = X           PlayerG = Y



////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player {

public:
	float x = 0, y = 0;
	float w, h, dx, dy, speed;
	int dir;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;


	//int dir, playerScore, health;
	bool life;
	//String File;
	//Image image;
   // Texture texture;
   // Sprite sprite;
	Player(String F, float X, float Y, float W, float H) {
		dx = 0; dy = 0; speed = 0; dir = 0;
		//	dir = 0; speed = 0; playerScore = 0; health = 100;
		//	life = true;
		File = F;
		w = W; h = H;
		image.loadFromFile("img/" + File);
		image.createMaskFromColor(Color(41, 33, 59));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}
	void update(float time)
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0; break;
		case 1: dx = -speed; dy = 0; break;
		case 2: dx = 0; dy = speed; break;
		case 3: dx = 0; dy = -speed; break;
		}

		x += dx * time;
		y += dy * time;

		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
	}








	void interactionWithMap()//ф-ция взаимодействия с картой
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy > 0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx > 0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}
			}
	}



	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}

};
/*
	case 72:
		1
	case 80:
		  2;
	case 77:
		3;
	case 75:
	 4
*/


int main()
{
	int rng = 75;
	int switcher1 = 1;
	int switcher2 = -1;
	int switcher3 = 1;
	int switcher4 = 1;
	int door = 0;
	int X3 = 50, Y3 = 50;
	int bld = 65; //на сколько далеко моб блядует
	const double mspeed = 0.3;
	RenderWindow window(sf::VideoMode(1920, 1080), "Game");
	Image mage_img;

	Texture mage_texture;



	view.reset(sf::FloatRect(0, 0, 1920, 1080));

	Image map_image;
	map_image.loadFromFile("img/mapp.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);
	////


	////

	Player p("hero.png", 80, 210, 90.0, 80.0);

	mage_img.loadFromFile("mage/6_enemies_1_attack_000.png");
	mage_texture.loadFromImage(mage_img);

	//Sprite hero_sprite;
	//hero_sprite.setTexture(hero_texture);
	Sprite mage_sprite;
	mage_sprite.setTexture(mage_texture);

	//hero_sprite.setPosition(X, Y);
	//hero_sprite.setTextureRect(IntRect(0, 210, 90, 80));
	mage_sprite.setPosition(X3, Y3);
	mage_sprite.setTextureRect(IntRect(50, 50, 75, 75));

	vector<FloatRect> blocks;

	float CurrentFrame = 0;
	Clock clock;

	while (window.isOpen())
	{
		blocks.clear();
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float coordinatePlayerX = 5, coordinatePlayerY = 5;
		coordinatePlayerX = p.getplayercoordinateX();
		coordinatePlayerY = p.getplayercoordinateY();

		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			p.dir = 1; p.speed = 0.9;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			//	p.sprite.setTextureRect(IntRect(463 - 79 * int(CurrentFrame), 210, 90, 80));
			p.sprite.setTextureRect(IntRect(463 - 79 * int(CurrentFrame), 210, 90, 80));
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			p.dir = 0; p.speed = 0.9;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			//	p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
			p.sprite.setTextureRect(IntRect(79 * int(CurrentFrame), 210, 90, 80));
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			p.dir = 3; p.speed = 0.9;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			//	p.sprite.setTextureRect(IntRect(463 - 79 * int(CurrentFrame), 210, 90, 80));
			p.sprite.setTextureRect(IntRect(463 - 79 * int(CurrentFrame), 210, 90, 80));
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			p.dir = 2; p.speed = 0.9;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(79 * int(CurrentFrame), 210, 90, 80));
			//	p.sprite.setTextureRect(IntRect(463 -79  int(CurrentFrame), 210, 90, 80));
		}
		getplayercoordinateforview(coordinatePlayerX, coordinatePlayerY);
		p.update(time);



		window.setView(view);
		window.clear();

		if (doorScript(coordinatePlayerY, coordinatePlayerX) == 1)
			door = 1;



		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(98, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == 'm')) {


					//mage_sprite.getPosition(X3,Y3);
					//	mage_sprite.getPosition();
					/*bool a = false;
					for (auto x : blocks)
					{

						if (!mage_sprite.getGlobalBounds().intersects(x))
						{
							a = true;
					}

					}*/
					//if (a == false)
					//


					switch (OrbitMob(coordinatePlayerY, coordinatePlayerX, mage_sprite.getPosition().y, mage_sprite.getPosition().x))
					{
					case 1:
					{

						mage_sprite.move(-0, 0.001 * time);
						break;
					}
					case 2:
					{
						mage_sprite.move(0, 0.001 * time);
						break;

					}
					case 3:
					{
						mage_sprite.move(0.001 * time, 0);
						break;

					}
					case 4:
					{
						mage_sprite.move(-0.001 * time, 0);
						break;

					}
					case 11:
					{
						if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 1)
							mage_sprite.move(0, -mspeed * time);
						else
						{
							if (coordinatePlayerX - mage_sprite.getPosition().x > bld)
								switcher1 = -1;
							if (coordinatePlayerX - mage_sprite.getPosition().x < -bld)
								switcher1 = 1;


							if (switcher1 < 0 || (dif(coordinatePlayerY, mage_sprite.getPosition().y) < rng && dif(coordinatePlayerX, mage_sprite.getPosition().x) < rng))
							{
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 1)
									mage_sprite.move(mspeed*time, 0);
							}
							else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 1)
								mage_sprite.move(-mspeed * time, 0);
							else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 1)
								mage_sprite.move(0, mspeed*time);

						}
						break;
					}
					case 22:
					{
						if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 1)
							mage_sprite.move(0, mspeed*time);
						else
						{
							if (coordinatePlayerX - mage_sprite.getPosition().x > bld)
								switcher2 = -1;
							if (coordinatePlayerX - mage_sprite.getPosition().x < -bld)
								switcher2 = 1;


							if (switcher2 > 0 || (dif(coordinatePlayerY, mage_sprite.getPosition().y) < rng && dif(coordinatePlayerX, mage_sprite.getPosition().x) < rng))
							{
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 1)
									mage_sprite.move(-mspeed * time, 0);
							}
							else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 1)
								mage_sprite.move(mspeed*time, 0);
							else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 1)
								mage_sprite.move(0, -mspeed * time);
						}
						break;
					}
					case 33:
					{
						if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 1)
							mage_sprite.move(mspeed*time, 0);
						else
						{
							if (coordinatePlayerY - mage_sprite.getPosition().y > bld)
								switcher3 = -1;
							if (coordinatePlayerY - mage_sprite.getPosition().y < -bld)
								switcher3 = 1;


							if (switcher3 < 0 || (dif(coordinatePlayerY, mage_sprite.getPosition().y) < rng && dif(coordinatePlayerX, mage_sprite.getPosition().x) < rng))
							{
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 1)
									mage_sprite.move(0, mspeed*time);
							}
							else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 1)
								mage_sprite.move(0, -mspeed * time);
							else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 1)
								mage_sprite.move(-mspeed * time, 0);
						}
						break;
					}
					case 44:
					{
						if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 1)
							mage_sprite.move(-mspeed * time, 0);
						else
						{
							if (coordinatePlayerY - mage_sprite.getPosition().y > bld)
								switcher4 = -1;
							if (coordinatePlayerY - mage_sprite.getPosition().y < -bld)
								switcher4 = 1;


							if (switcher4 > 0 || (dif(coordinatePlayerY, mage_sprite.getPosition().y) < rng && dif(coordinatePlayerX, mage_sprite.getPosition().x) < rng))
							{
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 1)
									mage_sprite.move(0, -mspeed * time);
							}
							else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 1)
								mage_sprite.move(0, mspeed*time);
							else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 1)
								mage_sprite.move(mspeed*time, 0);

						}
						break;
					}

					}
					//}

				}
				s_map.setPosition(j * 32, i * 32);
				//	if ((TileMap[i][j] == '0'))	blocks.push_back(s_map.getGlobalBounds());


				window.draw(s_map);
			}

		window.draw(mage_sprite);
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}