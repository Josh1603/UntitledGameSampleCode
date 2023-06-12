#include "GameStateReload.h"
#include "GameStateSelect.h"
#include "GameStateTutorialSelect.h"
#include "GameStateLockedTileTutorialSelect.h"
#include "GameStateUnlock.h"
#include "SquareConstraints.h"
#include "AudioEngine.h"
#include "SettingsValues.h"

using namespace ax;

namespace fafo {

	//Game state reload reloads an arbitrary number of NumberSquares, handling both their data representation and animation
	GameStateReload::GameStateReload(MainGame* main_game, std::vector<int> removed_number_square_indexs, bool new_square_unlocked) :
		m_main_game{ main_game },
		m_removed_number_square_indexs{ removed_number_square_indexs },
		m_new_square_unlocked { new_square_unlocked }
	{
		reload_time = 0.333F;
		run_time = 0.0F;
		new_state_set = false;
		m_game_grid_width = m_main_game->get_game_grid_width();
		y_diff_1 = SquareConstraints::square_length + m_main_game->get_number_sprite_spacing();
		move_animation_complete = false;
		time_elapsed = 0.0F;
		prev_time_elapsed = 0.0F;
	};

	void GameStateReload::on_enter() {
		m_main_game->add_game_grid_to_clipping_node();
		populate_maps();
		recycle_number_squares();
		move_number_squares();
		map_number_square_position_to_indexs();
	};

	// Identify number squares to move, and how far to move by id index
	// Identify new number square count for each column
	// Identify position movement count for each affected number square by position index
	void GameStateReload::populate_maps() {

		for (int i = m_removed_number_square_indexs.size() - 1; i >= 0; i--) {

			auto grid_position = m_main_game->get_number_squares()[m_removed_number_square_indexs[i]].get_grid_position();

			auto number_square_column = grid_position % m_game_grid_width;
			//Increase number square count for removed number square's column
			if (number_square_column_and_new_number_square_count_map[number_square_column]) {
				number_square_column_and_new_number_square_count_map[number_square_column]++;
			}
			else {
				number_square_column_and_new_number_square_count_map[number_square_column] = 1;
			}

			//First index above empty index if there is one
			grid_position -= m_game_grid_width;

			while (grid_position >= 0) {

				auto number_square_index = m_main_game->get_number_square_grid_position_to_index_map()[grid_position];
				//Move count for number square index
				if (!m_main_game->get_number_squares()[number_square_index].is_destroyed()) {
					if (number_square_index_and_position_move_count_map[number_square_index]) {
						number_square_index_and_position_move_count_map[number_square_index]++;
					}
					else {
						number_square_index_and_position_move_count_map[number_square_index] = 1;
					}
				}
				grid_position -= m_game_grid_width;
			}
		}
	}
	void GameStateReload::recycle_number_squares() {
		auto map_copy = number_square_column_and_new_number_square_count_map;

		for (auto removed_number_square_index : m_removed_number_square_indexs) {
			auto grid_position = m_main_game->get_number_squares()[removed_number_square_index].get_grid_position();
			auto row = grid_position / m_game_grid_width;
			auto column = grid_position % m_game_grid_width;
			//Where the recycled number square will sit prior to reloading
			auto reload_row = map_copy[column];
			//Y distance from current grid position to reload grid position
			auto y_diff = (row + reload_row) * y_diff_1;
			//Assossiate index of the recycled number square with how far it needs to move
			auto grid_position_diff = number_square_column_and_new_number_square_count_map[column];
			number_square_index_and_position_move_count_map[removed_number_square_index] = grid_position_diff;

			//Reset the grid position to the new 'off grid' location
			auto& number_square = m_main_game->get_number_squares()[removed_number_square_index]; 
			number_square.set_grid_position(number_square.get_grid_position() - ((row + reload_row) * m_game_grid_width));

			//Decrement reload row for the next number square in that column if there is one
			map_copy[column]--;

			reinitialise_number_square(removed_number_square_index);
			reposition_number_square(removed_number_square_index, y_diff);
		}
	}
		void GameStateReload::reinitialise_number_square(int index) {
		auto& number_square = m_main_game->get_number_squares()[index];
		number_square.reinit();
		number_square.reap_hidden_gems();
		number_square.revalue();
		number_square.set_destroyed(false);
	}
		void GameStateReload::reposition_number_square(int index, float y_diff) {
		auto& number_square = m_main_game->get_number_squares()[index];
		auto x_pos = number_square.get_square_sprite()->getPositionX();
		auto y_pos = number_square.get_square_sprite()->getPositionY() + y_diff;
		number_square.set_asset_positions({ x_pos, y_pos });
}
	void GameStateReload::move_number_squares() {

		auto& number_squares = m_main_game->get_number_squares();
		int i = 0;

		for (auto index_count : number_square_index_and_position_move_count_map) {

			float index = index_count.first;
			float count = index_count.second;

			auto& number_square = number_squares[index];

			if (prev_time_elapsed == 0.0F) {
				final_positions.push_back(number_square.get_square_sprite()->getPositionY() - (y_diff_1 * count));
			}

			if (time_elapsed < reload_time) {
				auto step_y_diff = ((time_elapsed / reload_time) * -y_diff_1 * count) - ((prev_time_elapsed / reload_time) * -y_diff_1 * count);
				number_square.set_asset_positions({ number_square.get_square_sprite()->getPositionX(), number_square.get_square_sprite()->getPositionY() + step_y_diff });
			}
			else {
				number_square.set_asset_positions({ number_square.get_square_sprite()->getPositionX(), final_positions[i] });
				move_animation_complete = true;
			}
			i++;
		}
	}
	void GameStateReload::map_number_square_position_to_indexs() {

		//Set new position index of each number square
		for (auto& index_count : number_square_index_and_position_move_count_map) {
			auto index = index_count.first;
			auto count = index_count.second;

			auto& number_square = m_main_game->get_number_squares()[index];
			number_square.set_grid_position(number_square.get_grid_position() + (count * m_game_grid_width));
		}

		//Remap the index to position map
		for (auto& number_square : m_main_game->get_number_squares()) {
			m_main_game->get_number_square_grid_position_to_index_map()[number_square.get_grid_position()] = number_square.get_index();
		}
	}

	void GameStateReload::update(float dt) {

		if (!move_animation_complete) {
			time_elapsed += dt;
			move_number_squares();
			prev_time_elapsed += dt;
		}

		if (!new_state_set && move_animation_complete) {


			if (m_main_game->is_tutorial_mode()) {
				if (m_main_game->is_unlock_tutorial_mode()) {
					if (m_main_game->get_high_number_square_indexs().size() == 0) {
						m_main_game->transition_to_main_game_state();
						new_state_set = true;
					}
					else {
						enter_unlock_state();
					}
				}
				else {
					AudioEngine::play2d(m_main_game->get_new_lock_effect_filename(), false, SettingsValues::audio_volume / 100.0F);
					m_main_game->transition_to_unlocked_tutorial();
					new_state_set = true;
				}
			}
			else {

				if (m_main_game->get_high_number_square_indexs().size() > 1) {
					enter_unlock_state();
				}
				else {
					enter_select_state();
				}
				new_state_set = true;
			}
		}
		run_time += dt;
	};

	void GameStateReload::enter_select_state() {
		if (m_main_game->is_tutorial_mode()) {
			if (m_main_game->is_unlock_tutorial_mode()) {
				m_main_game->update_state(std::make_unique<GameStateLockedTileTutorialSelect>(m_main_game));
			}
			else {
				m_main_game->update_state(std::make_unique<GameStateTutorialSelect>(m_main_game));
			}
		}
		else {
			m_main_game->update_state(std::make_unique<GameStateSelect>(m_main_game));
		}
	}

	void GameStateReload::enter_unlock_state() {
		m_main_game->update_state(std::make_unique<GameStateUnlock>(m_main_game));
	}

	std::unique_ptr<GameState> GameStateReload::preprocess_state() {
		return nullptr;
	};
	void GameStateReload::on_exit() {
		if (m_new_square_unlocked && !m_main_game->is_tutorial_mode()) {
			m_main_game->show_high_number_unlock_notification();
		}
		m_main_game->remove_game_grid_from_clipping_node();
		for (auto& number_square : m_main_game->get_number_squares())
			number_square.reinit_node_grid();
	};

}