#ifndef __GAME_STATE_UNLCK_H__
#define __GAME_STATE_UNLCK_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateUnlock : public GameState
	{
		MainGame* m_main_game;
		void check_matches();

		bool check_match(int grid_position);
		bool check_up_match(int grid_position);
		bool check_right_match(int grid_position);
		bool check_down_match(int grid_position);
		bool check_left_match(int grid_position);

		bool find_match(int grid_position);

		void set_grid_positions_to_check_against(int first_index);

		std::vector<int> grid_positions_to_check_against;

		bool is_a_match;
		bool matches_checked;

		void enter_select_state();
		void enter_remove_state();

	public:
		GameStateUnlock(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 