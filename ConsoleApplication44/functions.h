#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <math.h>
#include <chrono>
using namespace sf;
using namespace std;


class fieldClass
{
public:
	// количество соседей дл€ рождени€, жизни
	vector<int> toBorn;
	vector<int> toLive;

	int cellSize, cellAmX, cellAmY, generation, population;
	vector<vector<pair<RectangleShape, bool>>> fieldCells;
	vector<pair<int, int>> liveCells;


	void draw(RenderWindow& window)
	{
		for (int i = 0; i < cellAmX; ++i)
		{
			for (int j = 0; j < cellAmY; ++j)
			{
				window.draw(fieldCells[i][j].first);	
			}
		}
	}


	void onClick(Vector2f mousePos, RenderWindow& window, int side) // -1 left, 1 right
	{
		int cellX = mousePos.x / cellSize;
		int cellY = mousePos.y / cellSize;

		if (cellX >= 0 && cellX < cellAmX && cellY >= 0 && cellY < cellAmY)
		{
			if (side == -1 && fieldCells[cellX][cellY].second == false)
			{
				fieldCells[cellX][cellY].second = true;
				fieldCells[cellX][cellY].first.setFillColor(Color::Black);
				liveCells.push_back({ cellX, cellY });
				cout << "Added new cell, position: " << cellX << " " << cellY << endl;
			}
			else if (side == 1 && fieldCells[cellX][cellY].second == true)
			{
				fieldCells[cellX][cellY].second = false;
				fieldCells[cellX][cellY].first.setFillColor(Color(205, 200, 190));

				auto it = find(liveCells.begin(), liveCells.end(), make_pair(cellX, cellY));
				if (it != liveCells.end())
					liveCells.erase(it);
				cout << "Erased a cell, position: " << cellX << " " << cellY << endl;
			}		
		}
	}

	// надо будет как то оптимизировать(((
	void stepInit(RenderWindow& window)
	{
		++generation;
		population = 0;
		vector<vector<bool>> nextGeneration(cellAmX, vector<bool>(cellAmY, false));

		for (int i = 0; i < cellAmX; ++i)
		{
			for (int j = 0; j < cellAmY; ++j)
			{
				int cnt = 0; // количество живых соседей
				// ѕровер€ем всех 8 соседей с проверкой границ
				for (int di = -1; di <= 1; ++di)
				{
					for (int dj = -1; dj <= 1; ++dj)
					{
						if (di == 0 && dj == 0) continue; // пропускаем саму клетку

						int ni = i + di;
						int nj = j + dj;

						if (ni >= 0 && ni < cellAmX && nj >= 0 && nj < cellAmY)
						{
							if (fieldCells[ni][nj].second) cnt++;
						}
					}
				}

				bool isAlive = fieldCells[i][j].second;
				if (isAlive)
				{
					//  летка выживает только если число соседей в toLive
					auto it = find(toLive.begin(), toLive.end(), cnt);
					nextGeneration[i][j] = (it != toLive.end()); // лень писать развЄрнуто
				}
				else
				{
					// ћертва€ клетка рождаетс€ если число соседей в toBorn
					auto it = find(toBorn.begin(), toBorn.end(), cnt);
					nextGeneration[i][j] = (it != toBorn.end());
				}
			}
		}

		// ѕрименение нового поколени€
		for (int i = 0; i < cellAmX; ++i)
		{
			for (int j = 0; j < cellAmY; ++j)
			{
				fieldCells[i][j].second = nextGeneration[i][j];
				if (nextGeneration[i][j] == true)
				{
					fieldCells[i][j].first.setFillColor(Color::Black);
					++population;
				}
				else
				{
					fieldCells[i][j].first.setFillColor(Color(205, 200, 190));
				}
			}
		}

		cout << "Generation: " << generation << endl << "Population: " << population << " " << endl << endl;
	}




	fieldClass(int winSizeX, int winSizeY, int cellSize, vector<int> toLive, vector<int> toBorn)
	{
		this->toLive = toLive;
		this->toBorn = toBorn;
		this->cellSize = cellSize;
		cellAmX = winSizeX / cellSize;
		cellAmY = winSizeY / cellSize; 
		fieldCells.resize(cellAmX);
		generation = 0;
		population = 0;
		
		for (int i = 0; i < cellAmX; ++i)
		{
			fieldCells[i].resize(cellAmY);

			for (int j = 0; j < cellAmY; ++j)
			{
				RectangleShape cell(Vector2f(cellSize, cellSize));
				cell.setPosition(i * cellSize, j * cellSize);
				cell.setFillColor(Color(205, 200, 190));
				cell.setOutlineColor(Color(147, 159, 143));
				cell.setOutlineThickness(1);
				
				fieldCells[i][j].first = cell;
				fieldCells[i][j].second = false;
			}
		}
	}
};