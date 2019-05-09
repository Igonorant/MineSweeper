#pragma once
#include "SpriteCodex.h"
#include "Vei2.h"
#include "Graphics.h"

class MineField {

	//Enum for game states
	enum class GameState{Running,WinGameOver,LoseGameOver};

	//Tile class to build MineField grid
private:
	class Tile {

		enum class TileState{Hidden,Revealed,Flagged,Exploded};

	public:
		Tile() = default;
		void SetPosition(const Vei2& topLeftPos);
		bool AddBomb(); //return true if successful put bomb, false if already has a bomb
		void SetNeighbourBombs(const int bombCount);
		void Draw(Graphics& gfx);
		void Reveal();
		void Flag();
		bool HasBomb();
		bool isFlagged();
		bool haveNoNeighbours();
		bool isHidden();
		void Explode();

	private:
		static constexpr int tileSize = SpriteCodex::tileSize;
		Vei2 topLeftPosition = { 0,0 };
		TileState state = TileState::Hidden;
		bool hasBomb = false;
		int neighbourBombs = 0;
	};

public:
	MineField() = default;
	MineField(const int minesNumbers, Graphics& gfx);
	Vei2 GetTopLeftPos(const Graphics& gfx) const;
	void Draw(Graphics& gfx);
	void InsertMines(const int minesNumber);
	int Screen2Grid(const Vei2& screenPos);
	Vei2 Grid2Screen(const int gridPos);
	void TileClick(const Vei2& clickPosition, bool isRight);
	int Map2D(const int x,const int y) const;
	Vei2 Unmap2D(const int gridPos) const;
	void CountNeighbourhood() ;

private:
	static constexpr int fieldWidth = 10;
	static constexpr int fieldHeight = 5;
	static constexpr int fieldSize = fieldWidth * fieldHeight;
	Tile tiles[fieldSize];
	Vei2 topLeftPosition;
	GameState gameState = GameState::Running;
};