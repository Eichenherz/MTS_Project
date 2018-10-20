#pragma once

#include <random>
#include <iterator>
#include "SimpleMath.h"


namespace TILE_DRAW_PARAMS
{
	constexpr UINT	tile_width = 80;
	constexpr UINT	tile_height = 100;
	constexpr UINT	tile_slop = 15;
	constexpr Vec2	tile_offset { tile_width / 2 - tile_slop, tile_height / 2 };
	constexpr UINT	tile_x_step = tile_width + tile_slop;
}

// NOTE: for simplicity I won't OOP the tiles
struct Tile
{
	// For collision detection
	struct AABB
	{
		DirectX::SimpleMath::Vector2 c;
		DirectX::SimpleMath::Vector2 r;
	};

	// For changing the color of the corresp number
	enum class STATE
	{
		UNMOVED,
		PENDING,
		COLLIED,
		COUNT
	} state = STATE::UNMOVED;

	std::wstring	number;
	AABB			aabb;

};

// Collision detection 
inline bool AABB_vs_AABB( const Tile::AABB& a, const Tile::AABB& b )
{
	// Optimized by removing conditional branches
	const bool x = std::abs( a.c.x - b.c.x ) <= ( a.r.x + b.r.x );
	const bool y = std::abs( a.c.y - b.c.y ) <= ( a.r.y + b.r.y );

	return x && y;
}
inline bool AABB_vs_Point( const Tile::AABB& a, const Vec2& pt )
{
	const bool x = std::abs( a.c.x - pt.x ) <= a.r.x;
	const bool y = std::abs( a.c.y - pt.y ) <= a.r.y;

	return x && y;
}


// Random shuffeling
// If you change the # of tiles go change it in Tile.h too
using Tile_Iter = std::array<Tile, 9>::iterator;
template<class URBG>
void shuffle_tiles( Tile_Iter first, Tile_Iter last, URBG&& g )
{
	using diff_t = typename std::iterator_traits<Tile_Iter>::difference_type;
	using distr_t = std::uniform_int_distribution<diff_t>;
	using param_t =  typename distr_t::param_type;

	distr_t D;
	diff_t n = last - first;

	for ( diff_t i = n - 1; i > 0; --i ) {
		using std::swap;
		swap( first [i].number, first [D( g, param_t( 0, i ) )].number );
	}
}