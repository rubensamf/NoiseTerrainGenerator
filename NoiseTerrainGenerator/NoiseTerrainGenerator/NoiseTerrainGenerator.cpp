/*	Author: Ruben Fitch
 *	CMPM265 HW #2: Noise And Terrain Generation
 *	Description: 
 *	In this assignment you will use a noise function to create a tile-based terrain map
 *	suitable for a 2D game.
 */
// NoiseTerrainGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <noise/noise.h>
#include "noiseutils.h"
#include <iostream>
#include <SFML/Graphics.hpp>
//#include <array>

#define _INVALID		-1;
#define _WATER			171;
#define _BARREL			171;
//#define _BARREL		68;
#define _SAND			100;
#define _GRASS			150;
#define _SHRUB			150;
//#define _SHRUB		281;
//#define _ROCK			167;
#define _ROCK			34;
#define _DIRT			34;

using namespace std;
using namespace noise;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

	bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
	{
		// load the tileset texture
		if (!m_tileset.loadFromFile(tileset))
			return false;

		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < width; ++i)
			for (unsigned int j = 0; j < height; ++j)
			{
				// get the current tile number
				int tileNumber = tiles[i + j * width];

				// find its position in the tileset texture
				int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
				quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
				quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			}

		return true;
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};

int * initializeTiles(int t_width, int t_height)
{
	const int t_size = t_width * t_height;
	auto tiles = new int[t_size];
	for (int i = 0; i < t_size; i++)
	{ tiles[i] = _INVALID; }
	return tiles;
}

void copyTiles(int *tiles, int* m_copy, int t_width, int t_height)
{
	//auto m_copy = new int[t_width * t_height];
	const int limit = t_width * t_height;
	for (int i = 0; i < limit; i++)	
	{	
		const int t = tiles[i];
		m_copy[i] = t;
	}
	//return m_copy;
}

//This function and the tileset were made with help from the class Piazza page Note @20 "Tileset Transitions" by Adam Summerville
//int tileTransitions(int ul, int u, int ur, int l, int c, int r, int dl, int d, int dr, int level_width, int level_height)
int tileTransitions(int ul, int u, int ur, int l, int c, int r, int dl, int d, int dr)
{
/*
The tiles are 16x16 and if a tile's index goes from top to bottom,
left to right (e.g. tile 0 is the top left corner, 22 is the top right corner, 
						23 is the the left most of the second column, etc.) 
then the indices are:
Look at the 8 neighbors or a tile .
UL	U	UR
L	c	R
DL	D	DR
Assume that those are true if the tile is grass and false if it isn't
*/
	const int invalid_range = _INVALID;
	const int water			= _WATER;
	const int barrel		= _BARREL;
	const int sand			= _SAND;
	const int grass			= _GRASS;
	const int shrub			= _SHRUB;
	const int rock			= _ROCK;
	const int dirt			= _DIRT;
	//const int last_tile		= (level_width * level_height) - 1;

	//Correct for invalid input
	/*
	if (c < 0)
		c = water;
	if (c > last_tile)
		c = grass;
	*/
	//Boolean check for transitions
	bool UL, U, UR, L, R, DL, D, DR;
	UL = (ul == grass);
	U = (u == grass);
	UR = (ur == grass);
	L = (l == grass);
	R = (r == grass);
	DL = (dl == grass);
	D = (d == grass);
	DR = (dr == grass);

	//If c is not grass, just return c
	if (c != grass)
		return c;

	/*
	UL = (ul == c);
	U = (u == c);
	UR = (ur == c);
	L = (l == c);
	R = (r == c);
	DL = (dl == c);
	D = (d == c);
	DR = (dr == c);
	
	//Check that c is a valid index
	assert(c >= 0);
	assert(c <= last_tile);

	//int ul, u, ur, l, r, dl, d, dr;
	//ul = u = ur = l = r = dl = d = dr = _INVALID_RANGE;
	//sf::Vector2i tile_indices(level_width, level_height);
	if (c == grass)
	{
		UL = U = UR = L = R = DL = D = DR = true;
	}
	//int c = _INVALID_RANGE;
	//c = _WATER;
	//c = _ICE;
	//c = _SAND;
	//c = _GRASS;
	//c = _SHRUB;
	//c = _FOREST;
	//c = _DIRT;
*/
	
	int index = 0;

	if (c == grass){
		if (U && D && L && R)
		{	
			//index = 174;		
			index = c;
		}
		else if (U && D && L && R && !DR)
		{
			index = 0;
			//Check if DR tile is water
			//DR = (dr == water) ? true : false;
			if(dr == water)
				index += 144;
		}
		else if (U && D && L && R && !DL)
		{
			index = 5;
			//Check if DL tile is water
			if (dl == water)
				index += 144;
		}
		else if (U && D && L && R && !UL)
		{
			index = 125;
			if (ul == water)
				index += 144;
		}		
		else if (U && D && L && !R) 
		{
			index = 24;
			if (r == water)
				index += 144;
		}
		else if (U && D && !L && R)
		{
			index = 29;
			if (l == water)
				index += 144;
		}
		else if (!U && D && L && R) 
		{
			index = 121;
			if (u == water)
				index += 144;
		}
		else if (U && !D && L && R) {
			index = 1;
			if (d == water)
				index += 144;
		}
		else if (!U && D && L && !R) {
			index = 26;
			if (r == water)
				index += 144;
		}
		else if (!U && D && !L && R) {
			index = 25;
			if (u == water)
				index += 144;
		}
		else if (U && !D && !L && R)
		{
			index = 49;
			if (l == water)
				index += 144;
		}
		else if (U && !D && L && !R) 
		{
			index = 50;
			if (r == water)
				index += 144;
		}
	}
	//Check if index is valid
	/*
	if (index < 0)
		return 0;
	if (index > last_tile)
		return last_tile;
	
	assert(index >= 0);
	assert(index <= last_tile);
	*/
	return index;
}

int *getTileTransitions(int *level, int l_width, int l_height)
{
	const int zero = 0;
	const int invalid = _INVALID;
	const int water = _WATER;
	const int barrel = _BARREL;
	const int sand = _SAND;
	const int grass = _GRASS;
	const int shrub = _SHRUB;
	const int rock = _ROCK;
	const int dirt = _DIRT;
	const int last_tile = (l_width * l_height) - 1;
	const int t_width = l_width / 4;
	const int t_height = l_height / 4;

	const int limit = l_width * l_height;
	const int t_last = t_width * t_height - 1;

	//auto m_tiles = initializeTiles(l_width, l_height);
	auto m_tiles = initializeTiles(t_width, t_height);
	
	//Set up variables
	int ul_i, u_i, ur_i, l_i, c_i, r_i, dl_i, d_i, dr_i; //Tile indices
	ul_i = u_i = ur_i = l_i = r_i = dl_i = d_i = dr_i = invalid;
	int UL_V, U_V, UR_V, L_V, C_V, R_V, DL_V, D_V, DR_V; //Tile values
	UL_V = U_V = UR_V = L_V = C_V = R_V = DL_V = D_V = DR_V = invalid;
	//bool UL, U, UR, L, R, DL, D, DR;					 //Tile flags
	//UL = U = UR = L = R = DL = D = DR = false;

	int t_index = 0;
	//Set Tile Transitions
	//for (int j = 0; j < l_height; j ++)
	for (int j = 1; j < l_height - 1; j += 4)
	{
		if (t_index > t_last)
			break;
		//for (int i = 0; i < l_width; i++)
		for (int i = 1; i < l_width - 1; i += 4)
		{
			if (t_index > t_last)
				break;
			const int l_index = i * l_width + j;

			assert(l_index >= 0 && l_index < limit);
			ul_i = l_index - 1 - l_width;	u_i = l_index - l_width;	ur_i = l_index + 1 - l_width;
			l_i  = l_index - 1;				c_i = l_index;				r_i  = l_index + 1;
			dl_i = l_index - 1 + l_width;	d_i = l_index + l_width;	dr_i = l_index + 1 + l_width;

			assert(c_i >= 0 && c_i < limit);
			C_V = level[c_i];

			UL_V = (ul_i < 0 || ul_i >= limit) ? C_V : level[ul_i];
			U_V  = (u_i  < 0 || u_i  >= limit) ? C_V : level[u_i];
			UR_V = (ur_i < 0 || ur_i >= limit) ? C_V : level[ur_i];

			L_V  = (l_i  < 0 || l_i  >= limit) ? C_V : level[l_i];
			R_V  = (r_i  < 0 || r_i  >= limit) ? C_V : level[r_i];

			DL_V = (dl_i < 0 || dl_i >= limit) ? C_V : level[dl_i];
			D_V  = (d_i  < 0 || d_i  >= limit) ? C_V : level[d_i];
			DR_V = (dr_i < 0 || dr_i >= limit) ? C_V : level[dr_i];

			/*
			UL = (UL_V == grass);
			U = (U_V == grass);
			UR = (UR_V == grass);
			L = (L_V == grass);
			R = (R_V == grass);
			DL = (DL_V == grass);
			D = (D_V == grass);
			DR = (DR_V == grass);
			*/
			//Compute tile transitions
			assert(C_V != invalid);
			//m_tiles[index] = tileTransitions(UL_V, U_V, UR_V, L_V, C_V, R_V, DL_V, D_V, DR_V, l_width, l_height);
			int tile = tileTransitions(UL_V, U_V, UR_V, L_V, C_V, R_V, DL_V, D_V, DR_V);
			m_tiles[t_index] = tile;
			t_index++;
		}
	}
	//copyTiles(m_tiles, level, l_width, l_height);
	//delete m_tiles;
	return m_tiles;
}

int *generate_samples()
{
	//1. Create an array of noise values
	//Creating a terrain height map
	const int invalid = _INVALID;
	const int water = _WATER;
	const int barrel = _BARREL;
	const int sand = _SAND;
	const int grass = _GRASS;
	const int shrub = _SHRUB;
	const int rock = _ROCK;
	const int dirt = _DIRT;

	module::Perlin myModule;

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(256, 256);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	//Rendering the terrain height map
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	//Writing the image to an output file
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("noise_map.bmp");
	writer.WriteDestFile();

	//Create an array of noise values from this generated image
	//const int m_sample_width = heightMap.GetWidth() / 8;
	//const int m_sample_height = heightMap.GetHeight() / 8;
	const int m_sample_width = heightMap.GetWidth();
	const int m_sample_height = heightMap.GetHeight();
	auto m_samples = new int[m_sample_width * m_sample_height];

	cout << "m_samples width: " << m_sample_width << "\n";
	cout << "m_samples height: " << m_sample_height << "\n";

	//2. Covert this noise array into (typically integer) values that represent the different terrain types.
	//for (int j = 0, mh = 8; j < m_sample_height, mh < heightMap.GetHeight(); j++, mh += 8)
	for (int j = 0, mh = 0; j < m_sample_height, mh < heightMap.GetHeight(); j++, mh++)
	{
		//for (int i = 0, mw = 8; i < m_sample_width, mw < heightMap.GetWidth(); i++, mw += 8)
		for (int i = 0, mw = 0; i < m_sample_width, mw < heightMap.GetWidth(); i++, mw++)
		{
			//TESTING: PRINT OUT VALUES
			//cout << heightMap.GetValue(i,j) << "     ";

			//Computer the index as if it was a 2D array
			int index = i * m_sample_width + j;

			//Sample the noise at index (mw, mh)
			float noise_sample = heightMap.GetValue(mw, mh);

			//TESTING: PRINT OUT VALUES
			//cout << noise_sample << "     ";

			//Convert the noise values into usable terrain values
			int terrain_value = invalid;
			if (noise_sample < -0.4f)								{	terrain_value = water;		}
			else if (noise_sample < -0.1f && noise_sample >= -0.4f) {	terrain_value = dirt;		}
			else if (noise_sample < 0.5f && noise_sample >= -0.1f)	{	terrain_value = grass;		}
			else if (noise_sample < 0.9f && noise_sample >= 0.5f)	{	terrain_value = shrub;		}
			else if (noise_sample < 1.1f && noise_sample >= 0.9f)	{	terrain_value = rock;		}
			else if (noise_sample <= 1.2f && noise_sample >= 1.1f)	{	terrain_value = barrel;		}
			else													{	terrain_value = sand;		}
			//Check for valid terrain_value and index 
			assert(terrain_value != invalid);
			assert(index >= 0);
			assert(index < m_sample_width * m_sample_height);
			
			//Store the terrain value
			m_samples[index] = terrain_value;

			//Testing tilemap
			//cout << m_samples[index] << " ";
		}
		//cout << "\n";
	}
	int *tiles = getTileTransitions(m_samples, m_sample_width, m_sample_height);
	delete m_samples;
	//return m_samples;
	return tiles;
}

int main(int argc, char** argv)
{
	//Generate noise and sample
	const int *m_samples = generate_samples();
	
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 256), "Tilemap");

	// define the level with an array of tile indices
	const int *level = m_samples;
	const int level_width = 256/4;
	const int level_height = 256/4;
	//const int level_width = 256;
	//const int level_height = 256;
	//const int level_width = 24;
	//const int level_height = 12;
	/*const int level[] =
	{
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
	};*/
	/*
	const int level_size = 24 * 12;
	int level[level_size];
	for (int i = 0; i < level_size; i++)
		level[i] = i;
	*/
	// create the tilemap from the level definition
	TileMap map;
	if (!map.load("C:\\Users\\Ruben\\Pictures\\sheet.png", sf::Vector2u(16, 16), level, level_width, level_height))
		return -1;

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// draw the map
		window.clear();
		window.draw(map);
		window.display();
	}
	delete m_samples;
	return 0;
}

