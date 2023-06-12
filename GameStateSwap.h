#ifndef __GAME_STATE_SWAP_H__
#define __GAME_STATE_SWAP_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateSwap : public GameState
	{
		MainGame* m_main_game;
		bool number_squares_reinitialised;
		void reinitialise_number_squares();
		void enter_unlock_state();
	public:
		GameStateSwap(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 