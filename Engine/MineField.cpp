#include "MineField.h"
#include "RectI.h"
#include <random>
#include <assert.h>

void MineField::Tile::SetPosition(Vei2 topLeftPos)
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
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 2:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 3:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 4:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 5:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 6:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 7:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 8:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
				break;
			case 9:
				SpriteCodex::DrawTile0(topLeftPosition, gfx);
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

MineField::MineField(const int minesNumbers, Vei2 topLeftPos)
{
	topLeftPosition = topLeftPos;
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {
			Vei2 currentScreenPos = topLeftPosition + Vei2(x * SpriteCodex::tileSize, y * SpriteCodex::tileSize);
			tiles[x*fieldWidth + y].SetPosition(currentScreenPos);
			// tiles[x * fieldWidth + y].Reveal(); //test to check if everything is working under the tiles
		}
	}
	InsertMines(minesNumbers);


}

void MineField::Draw(Graphics& gfx)
{
	SpriteCodex::DrawBackground(topLeftPosition, fieldWidth * SpriteCodex::tileSize, fieldHeight * SpriteCodex::tileSize, gfx);
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {
			tiles[x*fieldWidth + y].Draw(gfx);
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
		int currentTileGridPos = xDist(rng) * fieldWidth + yDist(rng);
		if (tiles[currentTileGridPos].AddBomb()) {
			minesNumber--;
		}
	}

}
