#include "GameStateUnlock.h"
#include "GameStateSelect.h"
#include "GameStateRemove.h"
#include "AudioEngine.h"
#include "SettingsValues.h"


using namespace ax;

namespace fafo {
	GameStateUnlock::GameStateUnlock(MainGame* main_game) : m_main_game{ main_game } {
		is_a_match = false;
	};

	void GameStateUnlock::on_enter() {		
		check_matches();
	};


	void GameStateUnlock::check_matches() {
		auto& high_number_square_indexs = m_main_game->get_high_number_square_indexs();
		for (int i = 0; i < high_number_square_indexs.size(); i++) {
			//Use a square indexs to check collect to prevent multiple checks of the same pairs of number squares, and same square checking
			set_grid_positions_to_check_against(i);
			//If a match is found break the loop
			if (check_match(m_main_game->get_number_square_grid_position(high_number_square_indexs[i]))) {
				is_a_match = true;
				break;
			};
		};
		matches_checked = true;
	}

	bool GameStateUnlock::check_match(int grid_position) {
		if (check_up_match(grid_position)) { if (check_up_match(grid_position - m_main_game->get_game_grid_width()) || check_down_match(grid_position)) { return true; } }
		if (check_right_match(grid_position)) { if (check_left_match(grid_position) || check_right_match(grid_position + 1)) { return true; } }
		if (check_down_match(grid_position)) { if (check_down_match(grid_position + m_main_game->get_game_grid_width()) || check_up_match(grid_position)) { return true; } }
		if (check_left_match(grid_position)) { if (check_right_match(grid_position) || check_left_match(grid_position - 1)) { return true; } }
		return false;
	}
	bool GameStateUnlock::check_up_match(int grid_position) {
		auto up_position = grid_position - m_main_game->get_game_grid_width();
		if (up_position >= 0) {
			return find_match(up_position);
		}
		return false;
	};
	bool GameStateUnlock::check_right_match(int grid_position) {
		auto right_position = grid_position + 1;
		if (((right_position / m_main_game->get_game_grid_width()) == (grid_position / m_main_game->get_game_grid_width()))) {
			return find_match(right_position);
		}
		return false;
	};
	bool GameStateUnlock::check_down_match(int grid_position) {
		auto down_position = grid_position + m_main_game->get_game_grid_width();
		if (down_position < m_main_game->get_game_grid_width() * m_main_game->get_game_grid_height()) {
			return find_match(down_position);
		}
		return false;
	};
	bool GameStateUnlock::check_left_match(int grid_position) {
		auto left_position = grid_position - 1;
		if (((left_position / m_main_game->get_game_grid_width()) == (grid_position / m_main_game->get_game_grid_width()))) {
			return find_match(left_position);
		}
		return false;
	};

	bool GameStateUnlock::find_match(int grid_position) {
		for (auto& grid_position_to_check_against : grid_positions_to_check_against) {
			if (grid_position == grid_position_to_check_against) {
				return true;
			}
		}
		return false;
	}

	void GameStateUnlock::set_grid_positions_to_check_against(int first_index) {
		grid_positions_to_check_against.clear();
		auto& high_number_square_indexs = m_main_game->get_high_number_square_indexs();

		//Skip the first index to prevent self checking
		for (int i = first_index + 1; i < high_number_square_indexs.size(); i++)
		{
			grid_positions_to_check_against.push_back(m_main_game->get_number_square_grid_position(high_number_square_indexs[i]));
		}
	}

	void GameStateUnlock::enter_select_state() {
		m_main_game->update_state(std::make_unique<GameStateSelect>(m_main_game));
	}

	void GameStateUnlock::enter_remove_state() {
		m_main_game->update_state(std::make_unique<GameStateRemove>(m_main_game));
	}

	void GameStateUnlock::on_exit() {};
	std::unique_ptr<GameState> GameStateUnlock::preprocess_state() {
		return nullptr;
	};
	void GameStateUnlock::update(float dt) {
		if (matches_checked) {
			if (is_a_match) {
				AudioEngine::play2d(m_main_game->get_unlock_effect_filename(), false, SettingsValues::audio_volume / 100.0F);
				enter_remove_state();
			}
			else {
				enter_select_state();
			}
			matches_checked = false;
		}
	};

}