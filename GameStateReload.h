#ifndef __GAME_STATE_RELOAD_H__
#define __GAME_STATE_RELOAD_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateReload : public GameState
	{
		MainGame* m_main_game;
		std::vector<int> m_removed_number_square_indexs;
		bool m_new_square_unlocked;

		std::unordered_map<int,int> number_square_index_and_position_move_count_map;
		std::unordered_map<int, int> number_square_column_and_new_number_square_count_map;

		int m_game_grid_width;
		float y_diff_1;
		float reload_time;
		float run_time;
		bool new_state_set;
		bool move_animation_complete;
		float time_elapsed;
		float prev_time_elapsed;
		std::vector<float> final_positions;

		void populate_maps();
		void recycle_number_squares();
		void reinitialise_number_square(int index);
		void reposition_number_square(int index, float y_diff);
		void move_number_squares();
		void map_number_square_position_to_indexs();
		void enter_select_state();
		void enter_unlock_state();
	public:
		GameStateReload(MainGame* main_game, std::vector<int> removed_number_square_indexs, bool new_square_unlocked);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 