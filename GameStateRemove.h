#ifndef __GAME_STATE_REMO_H__
#define __GAME_STATE_REMO_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateRemove : public GameState
	{
		MainGame* m_main_game;

		std::vector<int> reload_number_square_indexs;

		void pop_low_number_values();
		void transform_locked_squares_to_unlocked();
		void increment_possible_number_values();
		bool transitioned_to_reload_state;
	public:
		GameStateRemove(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
		void enter_reload_state();

	};
}
#endif 