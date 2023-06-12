#ifndef __GAME_STATE_SWAP_SEL_H__
#define __GAME_STATE_SWAP_SEL_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateSwapSelect : public GameState
	{
		MainGame* m_main_game;
		void swap_select_square(NumberSquare number_square);
		void enter_swap_state();
		void enter_select_state();
		bool number_squares_selected;

	public:
		GameStateSwapSelect(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 