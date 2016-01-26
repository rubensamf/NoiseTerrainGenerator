/*	Author: Ruben Fitch
 *	CMPM265 HW #2: Noise And Terrain Generation
 *	Description: In this assignment you will use a noise function to create a tile-based terrain map suitable for a 2D game.
 */
// NoiseTerrainGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <noise/noise.h>
#include "noiseutils.h"
#include <iostream>
#include <SFML/Graphics.hpp>
//#include <array>

#define _INVALID_RANGE  0;
#define _WATER			1;
#define _ICE			2;
#define _SAND			3;
#define _GRASS			4;
#define _SHRUB			5;
#define _FOREST			6;
#define _DIRT			7;

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

void tileTransitions()
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
	sf::Vector2i tile_indices(24, 12);
	bool UL, U, UR, L, R, DL, D, DR;
	UL = U = UR = L = R = DL = D = DR = false;
	int ul, u, ur, l, r, dl, d, dr;
	ul = u = ur = l = r = dl = d = dr = _INVALID_RANGE;
	const int invalid_range = _INVALID_RANGE;
	const int water			= _WATER;
	const int ice			= _ICE;
	const int sand			= _SAND;
	const int grass			= _GRASS;
	const int shrub			= _SHRUB;
	const int forest		= _FOREST;
	const int dirt			= _DIRT;

	int c = _INVALID_RANGE;
	//c = _WATER;
	//c = _ICE;
	//c = _SAND;
	c = _GRASS;
	//c = _SHRUB;
	//c = _FOREST;
	//c = _DIRT;

	int index = -1;

	if (c == water)
		index = 196;
	if (c == dirt)
		index = 100;
	if (c == grass)
	{
		if (U && D && L && R)
			index = 174;
		if (U && D && L && R && !DR)
		{
			index = 0;
			//Check if DR tile is water
			DR = (dr == water) ? true : false;
			if(DR)
				index += 144;
		}
		if (U && D && L && R && !DL)
		{
			index = 5;
			//Check if DL tile is water
			if (dl == water)
				index += 144;
		}
		if (U && D && L && R && !UL)
		{
			index = 125;
			if (ul == water)
				index += 144;
		}
		
		if (U && D && L && !R) 
		{
			index = 24;
			if (r == water)
				index += 144;
		}

		/////////////////////
		if (U && D && !L && R)
		{
			index = 29;
			if (l == water)
				index += 144;
		}
		if (!U && D && L && R) 
		{
			index = 121;
			if (u == water)
				index += 144;
		}
		if (U && !D && L && R) {
			index = 1;
			if (d == water)
				index += 144;
		}
		if (!U && D && L && !R) {
			index = 26;
			if (r == water)
				index += 144;
		}
		if (!U && D && !L && R) {
			index = 25;
			if (u == water)
				index += 144;
		}
		if (U && !D && !L && R)
		{
			index = 49;
			if (l == water)
				index += 144;
		}
		if (U && !D && L && !R) 
		{
			index = 50;
			if (r == water)
				index += 144;
		}
	}

/*
If c is water
	index = 196
If c is dirt
	index = 100
If c is grass
	if U && D && L && R
		index = 174
	if U && D && L && R && !DR
                index = 0
                if DR is water
                	index += 144;
	if U && D && L && R && !DL
                index = 5
                if DL is water
                	index += 144;
    if U && D && L && R && !UR
                index = 120
                if UR is water
                	index += 144;
	if U && D && L && R && !UL
                index = 125
                if UL is water
                	index += 144;    
        
	if U && D && L && !R
                index = 24
                if R is water
                	index += 144;
	if U && D && !L && R
                index = 29
                if L is water
                	index += 144;
        if !U && D && L && R
                index = 121
                if U is water
                	index += 144;
	if U && !D && L && R
                index = 1
                if D is water
                	index += 144;  
        
        if !U && D && L && !R
                index = 26
                if R is water
                	index += 144;        
	if !U && D && !L && R
                index = 25
                if U is water
                	index += 144;
	if U && !D && !L && R
                index = 49
                if L is water
                	index += 144;
	if U && !D && L && !R
                index = 50
                if R is water
                	index += 144;  
*/
}
int *generate_samples()
{
	//1. Create an array of noise values
	//Creating a terrain height map
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
	const int m_sample_width = heightMap.GetWidth() / 8;;
	const int m_sample_height = heightMap.GetHeight() / 8;
	auto m_samples = new int[m_sample_width * m_sample_height];

	//2. Covert this noise array into (typically integer) values that represent the different terrain types.
	for (int i = 0, mw = 8; i < m_sample_width, mw < heightMap.GetWidth(); i++, mw += 8)
	{
		for (int j = 0, mh = 8; j < m_sample_height, mh < heightMap.GetHeight(); j++, mh += 8)
		{
			//TESTING: PRINT OUT VALUES
			//cout << heightMap.GetValue(i,j) << "     ";

			//Computer the index as if it was a 2D array
			int index = i * m_sample_width + j;

			//Sample the noise at index (mw, mh)
			float noise_sample = heightMap.GetValue(mw, mh);

			//Convert the noise values into usable terrain values
			int terrain_value = -100;
			if (noise_sample < -0.4f)
				terrain_value = _WATER;	//Water
			if (noise_sample < -0.1f && noise_sample >= -0.4f)
				terrain_value = _SAND;
			if (noise_sample < 0.5f && noise_sample >= -0.1f)
				terrain_value = _GRASS;
			if (noise_sample < 0.9f && noise_sample >= 0.5f)
				terrain_value = _SHRUB;
			if (noise_sample < 1.1f && noise_sample >= 0.9f)
				terrain_value = _FOREST;
			if (noise_sample <= 1.2f && noise_sample >= 1.1f) 
			{
				terrain_value = _ICE;
			}
			else
				terrain_value = _INVALID_RANGE;

			//Store the terrain value
			m_samples[index] = terrain_value;

			//Testing tilemap
			//cout << m_samples[index] << "\t";
		}
		//cout << "\n";
	}
	return m_samples;
}

int main(int argc, char** argv)
{
	//Generate noise and sample
	//const int *m_samples = generate_samples();
	
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 256), "Tilemap");

	// define the level with an array of tile indices
	//const int *level = m_samples;
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
	const int level_size = 24 * 12;
	int level[level_size];
	for (int i = 0; i < level_size; i++)
		level[i] = i;
	
	// create the tilemap from the level definition
	TileMap map;
	if (!map.load("C:\\Users\\Ruben\\Pictures\\sheet.png", sf::Vector2u(16, 16), level, 24, 12))
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
	return 0;
}

