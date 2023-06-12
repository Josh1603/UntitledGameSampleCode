#include "GameStateRemove.h"
#include "GameStateReload.h"

using namespace ax;

namespace fafo {
	GameStateRemove::GameStateRemove(MainGame* main_game) : m_main_game{ main_game } {
		transitioned_to_reload_state = false;
	};

	void GameStateRemove::on_enter() {
		pop_low_number_values();
		transform_locked_squares_to_unlocked();
		increment_possible_number_values();
		m_main_game->set_high_number_present(false);
		m_main_game->clear_high_number_square_indexs();
	};

	void GameStateRemove::on_exit() {
	};
	std::unique_ptr<GameState> GameStateRemove::preprocess_state() {
		return nullptr;
	};

	void GameStateRemove::update(float dt) {
		if (!m_main_game->is_destroy_animation_running() && !transitioned_to_reload_state) {
			enter_reload_state();
			transitioned_to_reload_state = true;
		}
	};

	void GameStateRemove::pop_low_number_values() {
		m_main_game->set_destroy_animation_running(true);

		for (auto& number_square : m_main_game->get_number_squares()) {
			if (number_square.get_number().first == m_main_game->get_low_number().first && number_square.get_number().second == m_main_game->get_low_number().second) {
				number_square.destroy_animation({ 0.0F, 0.0F }, true, false);
				number_square.set_destroyed(true);
				reload_number_square_indexs.push_back(number_square.get_index());
			}
		
		}
	}

	void GameStateRemove::transform_locked_squares_to_unlocked() {
		for (auto& locked_square_index : m_main_game->get_high_number_square_indexs()) {
			m_main_game->get_number_squares()[locked_square_index].transform_to_unlocked_square();
		}
	}

	void GameStateRemove::increment_possible_number_values() {
		m_main_game->increment_possible_number_values();
	}

	void GameStateRemove::enter_reload_state() {
		auto number_squares_indexs = m_main_game->get_selected_square_indexs();
		m_main_game->update_state(std::make_unique<GameStateReload>(m_main_game, reload_number_square_indexs, true));
	};
}