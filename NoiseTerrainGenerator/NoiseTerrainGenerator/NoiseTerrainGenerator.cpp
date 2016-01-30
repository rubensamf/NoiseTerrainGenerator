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
//#define _BARREL			171;
#define _BARREL			68;
#define _SAND			100;
//#define _SAND			34;
#define _GRASS			150;
//#define _SHRUB			150;
#define _SHRUB			281;
#define _ROCK			239;
//#define _ROCK			34;
#define _DIRT			34;
#define _STUMP			205;
#define _SAND_STUFF		75;
#define _SAND_ROCK		99;
#define _DIRT_STUFF		106;
#define _GRASS_LEAVES	54;
#define _GRASS_ROCK		126;
#define _ICE			41;
#define _TREASURE		140;
#define _OTHER			23;

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
	/*
	UL = (ul == grass);
	U = (u == grass);
	UR = (ur == grass);
	L = (l == grass);
	R = (r == grass);
	DL = (dl == grass);
	D = (d == grass);
	DR = (dr == grass);
	
	//If c is not grass, just return c
*/	
	/*if (c != grass || c != sand || c != dirt || c != water)
		return c;*/

	UL = (ul == c);
	U = (u == c);
	UR = (ur == c);
	L = (l == c);
	R = (r == c);
	DL = (dl == c);
	D = (d == c);
	DR = (dr == c);
	/*
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
	
	int index = _INVALID;

	//if (c == grass){
		if (U && D && L && R)
		{	
			//index = 174;		
			index = c;
		}
		if (U && D && L && R && !DR)
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 0;
			}
			else if (c == sand) {
				index = 25;
			}
			else if (c == dirt) {
				index = 32;
			}
			else if (c == water) {
				index = 169;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && dr == sand) {
				index += 0;
			}
			else if (c == grass && dr == dirt) {
				index += 13;
			}
			else if (c == grass && dr == water) {
				index += 144;
			}
			else if (c == sand && dr == grass) {
				index += 0;
			}
			else if (c == sand && dr == dirt) {
				index += 7;
			}
			else if (c == sand && dr == water) {
				index += 151;
			}
			else if (c == water && dr == grass) {
				index += 0;
			}
			else if (c == water && dr == sand) {
				index += 7;
			}
			else if (c == water && dr == dirt) {
				index += 7;
			}
			else if (c == dirt && dr == grass) {
				index += 6;
			}
			else if (c == dirt & dr == sand) {
				index += 0;
			}
			else if (c == dirt && dr == water) {
				index += 144;
			}
		}
		else if (U && D && L && R && !DL)
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 5;
			}
			else if (c == sand) {
				index = 26;
			}
			else if (c == dirt) {
				index = 33;
			}
			else if (c == water) {
				index = 170;
			}
			else {//Should not occur
				assert(false);
			}
			//Then offset index by the appropriate amount
			if (c == grass && dl == sand) {
				index += 0;
			}
			else if (c == grass && dl == dirt) {
				index += 13;
			}
			else if (c == grass && dl == water) {
				index += 144;
			}
			else if (c == sand && dl == grass) {
				index += 0;
			}
			else if (c == sand && dl == dirt) {
				index += 7;
			}
			else if (c == sand && dl == water) {
				index += 151;
			}
			else if (c == water && dl == grass) {
				index += 0;
			}
			else if (c == water && dl == sand) {
				index += 7;
			}
			else if (c == water && dl == dirt) {
				index += 7;
			}
			else if (c == dirt && dl == grass) {
				index += 6;
			}
			else if (c == dirt & dl == sand) {
				index += 0;
			}
			else if (c == dirt && dl == water) {
				index += 144;
			}
		}
		else if (U && D && L && R && !UL)
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 125;
			}
			else if (c == sand) {
				index = 50;
			}
			else if (c == dirt) {
				index = 57;
			}
			else if (c == water) {
				index = 194;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && ul == sand) {
				index += 0;
			}
			else if (c == grass && ul == dirt) {
				index += 13;
			}
			else if (c == grass && ul == water) {
				index += 144;
			}
			else if (c == sand && ul == grass) {
				index += 0;
			}
			else if (c == sand && ul == dirt) {
				index += 82;
			}
			else if (c == sand && ul == water) {
				index += 226;
			}
			else if (c == water && ul == grass) {
				index += 0;
			}
			else if (c == water && ul == sand) {
				index += 7;
			}
			else if (c == water && ul == dirt) {
				index += 7;
			}
			else if (c == dirt && ul == grass) {
				index += 6;
			}
			else if (c == dirt & ul == sand) {
				index += 0;
			}
			else if (c == dirt && ul == water) {
				index += 219;
			}
			/*index = 125;
			if (ul == water)
				index += 144;*/
		}		
		else if (U && D && L && R && !UR)
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 120;
			}
			else if (c == sand) {
				index = 49;
			}
			else if (c == dirt) {
				index = 56;
			}
			else if (c == water) {
				index = 193;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && ur == sand) {
				index += 0;
			}
			else if (c == grass && ur == dirt) {
				index += 13;
			}
			else if (c == grass && ur == water) {
				index += 144;
			}
			else if (c == sand && ur == grass) {
				index += 0;
			}
			else if (c == sand && ur == dirt) {
				index += 78;
			}
			else if (c == sand && ur == water) {
				index += 222;
			}
			else if (c == water && ur == grass) {
				index += 0;
			}
			else if (c == water && ur == sand) {
				index += 7;
			}
			else if (c == water && ur == dirt) {
				index += 7;
			}
			else if (c == dirt && ur == grass) {
				index += 6;
			}
			else if (c == dirt & ur == sand) {
				index += 0;
			}
			else if (c == dirt && ur == water) {
				index += 215;
			}
		}
		else if (U && D && L && !R) 
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 24;
			}
			else if (c == sand) {
				index = 29;
			}
			else if (c == dirt) {
				index = 36;
			}
			else if (c == water) {
				index = 173;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && r == sand) {
				index += 0;
			}
			else if (c == grass && r == dirt) {
				index += 13;
			}
			else if (c == grass && r == water) {
				index += 144;
			}
			else if (c == sand && r == grass) {
				index += 0;
			}
			else if (c == sand && r == dirt) {
				index += 2;
			}
			else if (c == sand && r == water) {
				index += 146;
			}
			else if (c == water && r == grass) {
				index += 0;
			}
			else if (c == water && r == sand) {
				index += 7;
			}
			else if (c == water && r == dirt) {
				index += 7;
			}
			else if (c == dirt && r == grass) {
				index += 6;
			}
			else if (c == dirt & r == sand) {
				index += 0;
			}
			else if (c == dirt && r == water) {
				index += 139;
			}
			/*index = 24;
			if (r == water)
				index += 144;*/
		}
		else if (U && D && !L && R)
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 29;
			}
			else if (c == sand) {
				index = 24;
			}
			else if (c == dirt) {
				index = 31;
			}
			else if (c == water) {
				index = 168;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && l == sand) {
				index += 0;
			}
			else if (c == grass && l == dirt) {
				index += 13;
			}
			else if (c == grass && l == water) {
				index += 144;
			}
			else if (c == sand && l == grass) {
				index += 0;
			}
			else if (c == sand && l == dirt) {
				index += 12;
			}
			else if (c == sand && l == water) {
				index += 156;
			}
			else if (c == water && l == grass) {
				index += 0;
			}
			else if (c == water && l == sand) {
				index += 7;
			}
			else if (c == water && l == dirt) {
				index += 7;
			}
			else if (c == dirt && l == grass) {
				index += 6;
			}
			else if (c == dirt & l == sand) {
				index += 0;
			}
			else if (c == dirt && l == water) {
				index += 149;
			}
			/*index = 29;
			if (l == water)
				index += 144;*/
		}
		else if (!U && D && L && R) 
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 121;
			}
			else if (c == sand) {
				index = 1;
			}
			else if (c == dirt) {
				index = 8;
			}
			else if (c == water) {
				index = 145;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && u == sand) {
				index += 0;
			}
			else if (c == grass && u == dirt) {
				index += 13;
			}
			else if (c == grass && u == water) {
				index += 144;
			}
			else if (c == sand && u == grass) {
				index += 0;
			}
			else if (c == sand && u == dirt) {
				index += 127;
			}
			else if (c == sand && u == water) {
				index += 271;
			}
			else if (c == water && u == grass) {
				index += 0;
			}
			else if (c == water && u == sand) {
				index += 7;
			}
			else if (c == water && u == dirt) {
				index += 7;
			}
			else if (c == dirt && u == grass) {
				index += 6;
			}
			else if (c == dirt & u == sand) {
				index += 0;
			}
			else if (c == dirt && u == water) {
				index += 264;
			}

			/*index = 121;
			if (u == water)
				index += 144;*/
		}
		else if (U && !D && L && R) {
			//Set initial index value based on c
			if (c == grass) {
				index = 1;
			}
			else if (c == sand) {
				index = 8;
			}
			else if (c == dirt) {
				index = 128;
			}
			else if (c == water) {
				index = 265;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && d == sand) {
				index += 0;
			}
			else if (c == grass && d == dirt) {
				index += 13;
			}
			else if (c == grass && d == water) {
				index += 144;
			}
			else if (c == sand && d == grass) {
				index += 121;
			}
			else if (c == sand && d == dirt) {
				index += 0;
			}
			else if (c == sand && d == water) {
				index += 144;
			}
			else if (c == water && d == grass) {
				index += 0;
			}
			else if (c == water && d == sand) {
				index += 7;
			}
			else if (c == water && d == dirt) {
				index += 7;
			}
			else if (c == dirt && d == grass) {
				index += 6;
			}
			else if (c == dirt & d == sand) {
				index += 0;
			}
			else if (c == dirt && d == water) {
				index += 24;
			}

			/*index = 1;
			if (d == water)
				index += 144;*/
		}
		else if (!U && D && L && !R) {
			//Set initial index value based on c
			if (c == grass) {
				index = 26;
			}
			else if (c == sand) {
				index = 5;
			}
			else if (c == dirt) {
				index = 12;
			}
			else if (c == water) {
				index = 149;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && (r == sand || u == sand)) {
				index += 0;
			}
			else if (c == grass && (r == dirt || u == dirt)) {
				index += 13;
			}
			else if (c == grass && (r == water || u == water)) {
				index += 144;
			}
			else if (c == sand && (r == grass || u == grass)) {
				index += 0;
			}
			else if (c == sand && (r == dirt || u == dirt)) {
				index += 28;
			}
			else if (c == sand && (r == water || u == water)) {
				index += 172;
			}
			else if (c == water && (r == grass || u == grass)) {
				index += 0;
			}
			else if (c == water && (r == sand || u == sand)) {
				index += 7;
			}
			else if (c == water && (r == dirt || u == dirt)) {
				index += 7;
			}
			else if (c == dirt && (r == grass || u == grass)) {
				index += 6;
			}
			else if (c == dirt & (r == sand || u == sand)) {
				index += 0;
			}
			else if (c == dirt && (r == water || u == water)) {
				index += 165;
			}
			/*index = 26;
			if (r == water)
				index += 144;*/
		}
		else if (!U && D && !L && R) {
			//Set initial index value based on c
			if (c == grass) {
				index = 25;
			}
			else if (c == sand) {
				index = 0;
			}
			else if (c == dirt) {
				index = 7;
			}
			else if (c == water) {
				index = 144;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && (l == sand || u == sand)) {
				index += 0;
			}
			else if (c == grass && (l == dirt || u == dirt)) {
				index += 13;
			}
			else if (c == grass && (l == water || u == water)) {
				index += 144;
			}
			else if (c == sand && ( l == grass || u == grass)) {
				index += 0;
			}
			else if (c == sand && ( l == dirt || u == dirt)) {
				index += 32;
			}
			else if (c == sand && ( l == water || u == water)) {
				index += 169;
			}
			else if (c == water && ( l == grass || u == grass)) {
				index += 0;
			}
			else if (c == water && ( l == sand || u == sand)) {
				index += 7;
			}
			else if (c == water && ( l == dirt || u == dirt)) {
				index += 7;
			}
			else if (c == dirt && ( l == grass || u == grass)) {
				index += 6;
			}
			else if (c == dirt & ( l == sand || u == sand)) {
				index += 0;
			}
			else if (c == dirt && ( l == water || u == water)) {
				index += 169;
			}
			/*index = 25;
			if (u == water)
				index += 144;*/
		}
		else if (U && !D && !L && R)
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 49;
			}
			else if (c == sand) {
				index = 56;
			}
			else if (c == dirt) {
				index = 127;
			}
			else if (c == water) {
				index = 264;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && (l == sand || d == sand)) {
				index += 0;
			}
			else if (c == grass && (l == dirt || d == dirt)) {
				index += 13;
			}
			else if (c == grass && (l == water || d == water)) {
				index += 144;
			}
			else if (c == sand && (l == grass || d == grass)) {
				index += 64;
			}
			else if (c == sand && (l == dirt || d == dirt)) {
				index += 0;
			}
			else if (c == sand && (l == water || d == water)) {
				index += 144;
			}
			else if (c == water && (l == grass || d == grass)) {
				index += 0;
			}
			else if (c == water && (l == sand || d == sand)) {
				index += 7;
			}
			else if (c == water && (l == dirt || d == dirt)) {
				index += 7;
			}
			else if (c == dirt && (l == grass || d == grass)) {
				index += 6;
			}
			else if (c == dirt & (l == sand || d == sand)) {
				index += 0;
			}
			else if (c == dirt && (l == water || d == water)) {
				index += 73;
			}
			/*index = 49;
			if (l == water)
				index += 144;*/
		}
		else if (U && !D && L && !R) 
		{
			//Set initial index value based on c
			if (c == grass) {
				index = 50;
			}
			else if (c == sand) {
				index = 57;
			}
			else if (c == dirt) {
				index = 132;
			}
			else if (c == water) {
				index = 269;
			}
			else {//Should not occur
				assert(false);
			}

			//Then offset index by the appropriate amount
			if (c == grass && (r == sand || d == sand)) {
				index += 0;
			}
			else if (c == grass && (r == dirt || d == dirt)) {
				index += 13;
			}
			else if (c == grass && (r == water || d == water)) {
				index += 144;
			}
			else if (c == sand && (r == grass || d == grass)) {
				index += 68;
			}
			else if (c == sand && (r == dirt || d == dirt)) {
				index += 0;
			}
			else if (c == sand && (r == water || d == water)) {
				index += 144;
			}
			else if (c == water && (r == grass || d == grass)) {
				index += 0;
			}
			else if (c == water && (r == sand || d == sand)) {
				index += 7;
			}
			else if (c == water && (r == dirt || d == dirt)) {
				index += 7;
			}
			else if (c == dirt && (r == grass || d == grass)) {
				index += 6;
			}
			else if (c == dirt & (r == sand || d == sand)) {
				index += 0;
			}
			else if (c == dirt && (r == water || d == water)) {
				index += 69;
			}
			/*index = 50;
			if (r == water)
				index += 144;*/
		}
		else {
			index = c;
		}
	//}
	//Check if index is valid
	///*
	if (index < 0)
		return c;
	/*if (index > last_tile)
		return last_tile;
	
	assert(index >= 0);
	assert(index <= last_tile);
	*/
	return index;
}

/*int *getHorizontalTransitions(int *level, int l_width, int l_height)
{
	const int limit = l_width * l_height;
	assert(limit > 0);
	const int water = _WATER;
	const int sand = _SAND;
	const int sand_stuff = _SAND_STUFF;
	const int sand_rock = _SAND_ROCK;
	const int dirt = _DIRT;
	const int dirt_stuff = _DIRT_STUFF;
	const int grass = _GRASS;
	const int grass_leaves = _GRASS_LEAVES;
	const int grass_rock = _GRASS_ROCK;
	const int treasure = _TREASURE;
	const int other = _OTHER;

	bool W, S, D, G, T, X, transition;
	W = S = D = G = T = X = transition = false;
	
	int index = _INVALID;
	int tile = _INVALID;
	int type = _INVALID;
	
	int l_tile = level[0];
	int l_type = _INVALID;
	
	int c_tile = level[0];
	int c_type = _INVALID;
	
	int r_tile = level[0];
	int r_type = _INVALID;

	//bool WL, WC, WR, SL, SC, SR, DL, DC, DR, GL, GC, GR, TL, TC, TR, XL, XC, XR;
	//WL =  WC =  WR =  SL =  SC =  SR =  DL =  DC =  DR =  GL =  GC =  GR =  TL =  TC =  TR =  XL =  XC =  XR = false;
	for (int j = 0; j < l_height; j++)
	{
		for (int i = 0; i < l_width; i++)
		{
			W = S = D = G = T = X = transition = false;
			index = i + l_width * j;
			assert(index >= 0);
			assert(index < limit);
			tile = level[index];
			if (tile == water)														{ type = water; }
			else if (tile == sand || tile == sand_rock || tile == sand_stuff)		{ type = sand; }
			else if (tile == dirt || tile == dirt_stuff)							{ type = dirt; }
			else if (tile == grass || tile == grass_leaves || tile == grass_rock)	{ type = grass; }
			else if (tile == treasure)												{ type = treasure; }
			else																	{ type = other; }
			l_tile = c_tile;
			c_tile = r_tile;
			r_tile = tile;

			level[index] = transition;
		}
	}
}*/
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
			UL_V = level[ul_i];
			U_V = level[u_i];
			UR_V = level[ur_i];
			L_V = level[l_i];
			R_V = level[r_i];
			DL_V = level[dl_i];
			D_V = level[d_i];
			DR_V = level[dr_i];
			/*
			UL_V = (ul_i < 0 || ul_i >= limit) ? C_V : level[ul_i];
			U_V  = (u_i  < 0 || u_i  >= limit) ? C_V : level[u_i];
			UR_V = (ur_i < 0 || ur_i >= limit) ? C_V : level[ur_i];

			L_V  = (l_i  < 0 || l_i  >= limit) ? C_V : level[l_i];
			R_V  = (r_i  < 0 || r_i  >= limit) ? C_V : level[r_i];

			DL_V = (dl_i < 0 || dl_i >= limit) ? C_V : level[dl_i];
			D_V  = (d_i  < 0 || d_i  >= limit) ? C_V : level[d_i];
			DR_V = (dr_i < 0 || dr_i >= limit) ? C_V : level[dr_i];
			*/
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

int *generate_samples(int seed, int width, int height)
{
	//1. Create an array of noise values
	//Creating a terrain height map
	const int invalid = _INVALID;
	const int water = _WATER;
	const int barrel = _BARREL;
	//const int barrel = _WATER;
	const int sand = _SAND;
	//const int sand = _WATER;
	const int grass = _GRASS;
	const int shrub = _SHRUB;
	//const int shrub = _GRASS;
	const int rock = _ROCK;
	//const int rock = _DIRT;
	const int dirt = _DIRT;
	const int stump = _STUMP;

	module::Perlin myModule;
	myModule.SetSeed(seed);

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(256, 256);
	heightMapBuilder.SetBounds(6.0, 10.0, 1.0, 5.0);
	//heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	//Rendering the terrain height map
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
	renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
	renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
	renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
	renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
	renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
	renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
	renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
	renderer.EnableLight();
	renderer.SetLightContrast(3.0);
	renderer.SetLightBrightness(2.0);
	renderer.SetLightAzimuth(60.0);
	renderer.SetLightElev(30.0);
	renderer.Render();

	//Writing the image to an output file
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("noise_map.bmp");
	writer.WriteDestFile();

	//Create an array of noise values from this generated image
	const int m_sample_width = heightMap.GetWidth() / 8;
	const int m_sample_height = heightMap.GetHeight() / 8;
	//const int m_sample_width = heightMap.GetWidth();
	//const int m_sample_height = heightMap.GetHeight();
	auto m_samples = new int[m_sample_width * m_sample_height];

	//cout << "m_samples width: " << m_sample_width << "\n";
	//cout << "m_samples height: " << m_sample_height << "\n";

	//2. Covert this noise array into (typically integer) values that represent the different terrain types.
	for (int j = 0, mh = 8; j < m_sample_height, mh < heightMap.GetHeight(); j++, mh += 8)
	//for (int j = 0, mh = 0; j < m_sample_height, mh < heightMap.GetHeight(); j++, mh++)
	{
		for (int i = 0, mw = 8; i < m_sample_width, mw < heightMap.GetWidth(); i++, mw += 8)
		//for (int i = 0, mw = 0; i < m_sample_width, mw < heightMap.GetWidth(); i++, mw++)
		{
			//TESTING: PRINT OUT VALUES
			//cout << heightMap.GetValue(i,j) << "     ";

			//Computer the index as if it was a 2D array
			int index = i + m_sample_width * j;

			//Sample the noise at index (mw, mh)
			float noise_sample = heightMap.GetValue(mw, mh);

			//TESTING: PRINT OUT VALUES
			//cout << noise_sample << "     ";

			//Convert the noise values into usable terrain values
			int terrain_value = invalid;
			if (noise_sample < -0.4f)								{	terrain_value = water;		}
			else if (noise_sample < -0.3f && noise_sample >= -0.4f) {	terrain_value = _SAND_STUFF;		}
			else if (noise_sample < -0.1f && noise_sample >= -0.3f) { terrain_value = sand; }
			else if (noise_sample < 0.0f && noise_sample >= -0.1f)	{	terrain_value = _SAND_ROCK;		}
			else if (noise_sample < 0.1f && noise_sample >= 0.0f) { terrain_value = dirt; }
			else if (noise_sample < 0.2f && noise_sample >= 0.1f)  { terrain_value = _DIRT_STUFF; }
			else if (noise_sample < 0.3f && noise_sample >= 0.2f)	{	terrain_value = _GRASS_LEAVES;		}
			else if (noise_sample < 1.1f && noise_sample >= 0.3f)	{	terrain_value = grass;		}
			else if (noise_sample <= 1.2f && noise_sample >= 1.1f)	{	terrain_value = _GRASS_ROCK;		}
			else													{	terrain_value = _TREASURE;		}
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
	//int *tiles = getTileTransitions(m_samples, m_sample_width, m_sample_height);
	//int *tiles = getHorizontalTransitions(m_samples, m_sample_width, m_sample_height);
	//delete m_samples;
	return m_samples;
	//return tiles;
}

int main(int argc, char** argv)
{
	int seed = rand();
	//int seed = 777;
	//Generate noise and sample
	const int width = 256;
	const int height = 256;
	const int render_width = 512;
	const int render_height = 256;
	const int level_width = width / 4;
	const int level_height = height / 4;
	const int vector_width = 16;
	const int vector_height = 16;
	const int *m_samples = generate_samples(seed, width, height);

	// create the window
	sf::RenderWindow window(sf::VideoMode(render_width, render_height), "Tilemap");

	// define the level with an array of tile indices
	const int *level = m_samples;

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
	if (!map.load("C:\\Users\\Ruben\\Pictures\\sheet.png", sf::Vector2u(vector_width, vector_height), level, level_width, level_height))
		return -1;

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed) {
				delete m_samples;
				seed = rand();
				m_samples = generate_samples(seed, width, height);
				level = m_samples;
				if (!map.load("C:\\Users\\Ruben\\Pictures\\sheet.png", sf::Vector2u(vector_width, vector_height), level, level_width, level_height))
					return -1;
			}
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

