#ifndef __GAME_STATE_UNLCK_SEL_H__
#define __GAME_STATE_UNLCK_SEL_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateUnlockSelect : public GameState
	{
		MainGame* m_main_game;
		void unlock_select_square(NumberSquare number_square);
		void enter_remove_state();
		void enter_select_state();
		bool number_square_selected;
		float animation_delay;

	public:
		GameStateUnlockSelect(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 