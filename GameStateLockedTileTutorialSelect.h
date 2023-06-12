#ifndef __GAME_STATE_LCK_T_TUT_SEL_H__
#define __GAME_STATE_LCK_T_TUT_SEL_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateLockedTileTutorialSelect : public GameState
	{
		MainGame* m_main_game;

		void select_square(NumberSquare number_square);
		void enter_merge_state();

	public:
		GameStateLockedTileTutorialSelect(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 