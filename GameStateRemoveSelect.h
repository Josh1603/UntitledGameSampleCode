#ifndef __GAME_STATE_REM_SEL_H__
#define __GAME_STATE_REM_SEL_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateRemoveSelect : public GameState
	{
		MainGame* m_main_game;
		bool number_square_selected;
		void remove_select_square(NumberSquare& number_square);
		void enter_reload_state();
		void enter_select_state();
		//void reinitialise_selected_number_square();
	public:
		GameStateRemoveSelect(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 