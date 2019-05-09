#include "MineField.h"
#include "RectI.h"
#include <random>
#include <assert.h>
#include <algorithm>

void MineField::Tile::SetPosition(const Vei2& topLeftPos)
{
	topLeftPosition = topLeftPos;
}

bool MineField::Tile::AddBomb() //return true if successful put bomb, false if already has a bomb
{
	if (!hasBomb) {
		hasBomb = true;
		return true;
	}
	return false;
}

void MineField::Tile::SetNeighbourBombs(const int bombCount)
{
	neighbourBombs = bombCount;
}

void MineField::Tile::Draw(Graphics& gfx)
{
	switch (state) {
	case TileState::Hidden:
		SpriteCodex::DrawTileButton(topLeftPosition, gfx);
		break;
	case TileState::Flagged:
		SpriteCodex::DrawTileFlag(topLeftPosition, gfx);
		SpriteCodex::DrawTileButton(topLeftPosition, gfx);
		break;
	case TileState::Revealed:
		if (hasBomb) {
			SpriteCodex::DrawTileBomb(topLeftPosition, gfx);
			break;
		}
		else {
			switch (neighbourBombs) {
			case 0:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 1:
				SpriteCodex::DrawTile1(topLeftPosition, gfx);
				break;
			case 2:
				SpriteCodex::DrawTile2(topLeftPosition, gfx);
				break;
			case 3:
				SpriteCodex::DrawTile3(topLeftPosition, gfx);
				break;
			case 4:
				SpriteCodex::DrawTile4(topLeftPosition, gfx);
				break;
			case 5:
				SpriteCodex::DrawTile5(topLeftPosition, gfx);
				break;
			case 6:
				SpriteCodex::DrawTile6(topLeftPosition, gfx);
				break;
			case 7:
				SpriteCodex::DrawTile7(topLeftPosition, gfx);
				break;
			case 8:
				SpriteCodex::DrawTile8(topLeftPosition, gfx);
				break;
			default:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
			}
			break;
		}
	case TileState::Exploded:
		SpriteCodex::DrawTileBombRed(topLeftPosition, gfx);
		SpriteCodex::DrawTileCross(topLeftPosition, gfx);
		break;
	default:
		SpriteCodex::DrawTileButton(topLeftPosition, gfx);
	}
}

void MineField::Tile::Reveal()
{
	state = TileState::Revealed;
}

void MineField::Tile::Flag()
{
	if (state == TileState::Hidden && state != TileState::Flagged) {
		state = TileState::Flagged;
	}
	else if (state == TileState::Flagged){
		state = TileState::Hidden;
	}
}

bool MineField::Tile::HasBomb()
{
	return hasBomb;
}

bool MineField::Tile::isFlagged()
{
	if (state == TileState::Flagged) {
		return true;
	}
	return false;
}

bool MineField::Tile::haveNoNeighbours()
{
	if (neighbourBombs == 0) {
		return true;
	}
	return false;
}

bool MineField::Tile::isHidden()
{
	if (state == TileState::Hidden) {
		return true;
	}
	return false;
}

void MineField::Tile::Explode()
{
	state = TileState::Exploded;
}

MineField::MineField(const int minesNumbers, Graphics& gfx)
{
	topLeftPosition = GetTopLeftPos(gfx);
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {
			Vei2 currentScreenPos = topLeftPosition + Vei2(x * SpriteCodex::tileSize, y * SpriteCodex::tileSize);
			tiles[Map2D(x, y)].SetPosition(currentScreenPos);
			// tiles[Map2D(x, y)].Reveal(); //test to check if everything is working under the tiles
		}
	}
	InsertMines(minesNumbers);
	CountNeighbourhood();
}

Vei2 MineField::GetTopLeftPos(const Graphics& gfx) const
{
	Vei2 GridSizePx(fieldWidth * SpriteCodex::tileSize, fieldHeight * SpriteCodex::tileSize);
	assert(GridSizePx.x < gfx.ScreenWidth);
	assert(GridSizePx.y < gfx.ScreenHeight);
	const int topLeftX = (gfx.ScreenWidth - GridSizePx.x) / 2;
	const int topLeftY = (gfx.ScreenHeight - GridSizePx.y) / 2;
	return Vei2(topLeftX,topLeftY);
}

void MineField::Draw(Graphics& gfx)
{
	SpriteCodex::DrawBackground(topLeftPosition, fieldWidth * SpriteCodex::tileSize, fieldHeight * SpriteCodex::tileSize, gfx);
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {
			tiles[Map2D(x, y)].Draw(gfx);
		}
	}
}

void MineField::InsertMines(int minesNumber)
{
	assert(minesNumber > 0 && minesNumber < fieldWidth * fieldHeight);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, fieldWidth - 1);
	std::uniform_int_distribution<int> yDist(0, fieldHeight - 1);

	while (minesNumber > 0) {
		int currentTileGridPos = xDist(rng) * fieldHeight + yDist(rng);
		if (tiles[currentTileGridPos].AddBomb()) {
			minesNumber--;
		}
	}

}

int MineField::Screen2Grid(const Vei2& screenPos)
{
	int x = (screenPos.x - topLeftPosition.x) / SpriteCodex::tileSize;
	int y = (screenPos.y - topLeftPosition.y) / SpriteCodex::tileSize;
	
	x = std::max(x, 0);
	x = std::min(x, fieldWidth - 1);
	y = std::max(y, 0);
	y = std::min(y, fieldHeight - 1);
	return (Map2D(x, y));
}

Vei2 MineField::Grid2Screen(const int gridPos)
{
	const int x = (gridPos / fieldHeight) * SpriteCodex::tileSize + topLeftPosition.x;
	const int y = (gridPos % fieldHeight) * SpriteCodex::tileSize + topLeftPosition.y;
	return Vei2(x,y);
}

void MineField::TileClick(const Vei2& clickPosition, bool isRight)
{
	const int gridPos = Screen2Grid(clickPosition);
	if (isRight) {
		tiles[gridPos].Flag();
	}
	else {
		if (tiles[gridPos].HasBomb()) {
			tiles[gridPos].Explode();
			gameState = GameState::LoseGameOver;
		}
		if (!tiles[gridPos].isFlagged() && tiles[gridPos].isHidden()) {
			tiles[gridPos].Reveal();
			// recursive implementation to reveal all empty tiles
			if (!tiles[gridPos].HasBomb() && tiles[gridPos].haveNoNeighbours()) {
				// ensure that neighbourhood do not exceed the grid boundaries
				Vei2 unmappedPos = Unmap2D(gridPos);
				const int minX = std::max(0, unmappedPos.x - 1);
				const int minY = std::max(0, unmappedPos.y - 1);
				const int maxX = std::min(fieldWidth - 1, unmappedPos.x + 1);
				const int maxY = std::min(fieldHeight - 1, unmappedPos.y + 1);

				// loop through neighbourhood and revel if has no bomb
				for (int neighX = minX; neighX <= maxX; neighX++) {
					for (int neighY = minY; neighY <= maxY; neighY++) {
						Vei2 neighPos = Grid2Screen(Map2D(neighX, neighY));
						TileClick(neighPos, false);
					}
				}
			}

		}
	}
}

int MineField::Map2D(const int x, const int y) const
{
	return x * fieldHeight + y;
}

Vei2 MineField::Unmap2D(const int gridPos) const
{
	const int x = gridPos / fieldHeight;
	const int y = gridPos % fieldHeight;
	return Vei2(x, y);
}

void MineField::CountNeighbourhood()
{
	// loop through all tiles
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {

			const int currentPos = Map2D(x, y);
			// check if it has no bomb
			if (!tiles[currentPos].HasBomb()) {
				// ensure that neighbourhood do not exceed the grid boundaries
				const int minX = std::max(0, x - 1);
				const int minY = std::max(0, y - 1);
				const int maxX = std::min(fieldWidth - 1, x + 1);
				const int maxY = std::min(fieldHeight - 1, y + 1);

				// loop through neighbourhood and count mines
				int bombCount = 0;
				for (int neighX = minX; neighX <= maxX; neighX++) {
					for (int neighY = minY; neighY <= maxY; neighY++) {
						const int neighPos = Map2D(neighX, neighY);
						// ensure to not count itself
						if (currentPos != neighPos && tiles[neighPos].HasBomb()) {
							bombCount++;
						}
					}
				}
				// set tile neighbourhood count
				tiles[currentPos].SetNeighbourBombs(bombCount);
			}
		}
	}

}
