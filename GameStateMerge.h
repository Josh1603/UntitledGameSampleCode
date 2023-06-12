#ifndef __GAME_STATE_MER_H__
#define __GAME_STATE_MER_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateMerge : public GameState
	{
		MainGame* m_main_game;
		bool pivot_number_square_reinitialised;
		bool pivot_animation_complete;
		std::pair<std::string, std::string> square_value;

		void reinitialise_pivot_number_square();
		void merge_animate_pivot_number_square();
		void enter_reload_state();

	public:
		GameStateMerge(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 