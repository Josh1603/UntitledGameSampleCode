#include "GameStateSwap.h"
#include "GameStateUnlock.h"

using namespace ax;

namespace fafo {
	GameStateSwap::GameStateSwap(MainGame* main_game) : m_main_game{ main_game } {
		number_squares_reinitialised = false;
	};

	void GameStateSwap::on_enter() {
		auto& number_squares = m_main_game->get_number_squares();
		auto& selected_number_squares_indexs = m_main_game->get_selected_square_indexs();
		auto& first_selected_number_square = number_squares[selected_number_squares_indexs[0]];
		auto& second_selected_number_square = number_squares[selected_number_squares_indexs[1]];

		first_selected_number_square.quick_deselection_animation();
		second_selected_number_square.quick_deselection_animation();


		auto first_selected_number_square_position = m_main_game->get_world_space_from_game_grid_group(first_selected_number_square.get_square_sprite()->getPosition());
		auto second_selected_number_square_position = m_main_game->get_world_space_from_game_grid_group(second_selected_number_square.get_square_sprite()->getPosition());

		m_main_game->set_destroy_animation_running(true);

		first_selected_number_square.destroy_animation(second_selected_number_square_position - first_selected_number_square_position, false, false);
		second_selected_number_square.destroy_animation(first_selected_number_square_position - second_selected_number_square_position, false, false);

		first_selected_number_square.set_destroyed(true);
		second_selected_number_square.set_destroyed(true);
	};
	void GameStateSwap::on_exit() {
		for (auto& number_square : m_main_game->get_number_squares())
			number_square.reinit_node_grid();
	};

	void GameStateSwap::update(float dt) {
		if (!m_main_game->is_destroy_animation_running() && !number_squares_reinitialised) {
			reinitialise_number_squares();
			number_squares_reinitialised = true;
			enter_unlock_state();
		}
	};

	void GameStateSwap::reinitialise_number_squares() {
		auto& number_square_indexs = m_main_game->get_selected_square_indexs();
		auto& number_squares = m_main_game->get_number_squares();
		auto& number_square_0 = number_squares[number_square_indexs[0]];
		auto& number_square_1 = number_squares[number_square_indexs[1]];
		int number_square_0_grid_position = number_square_0.get_grid_position();

		//Swap positions
		auto number_square_0_position = number_square_0.get_square_sprite()->getPosition();
		auto number_square_1_position = number_square_1.get_square_sprite()->getPosition();

		number_square_0.set_asset_positions(number_square_1_position);
		number_square_1.set_asset_positions(number_square_0_position);

		//Swap mapping
		m_main_game->get_number_square_grid_position_to_index_map()[number_square_1.get_grid_position()] = number_square_0.get_index();
		m_main_game->get_number_square_grid_position_to_index_map()[number_square_0.get_grid_position()] = number_square_1.get_index();

		//Swap stored values and reinitialise
		number_square_0.set_grid_position(number_square_1.get_grid_position());
		number_square_0.reinit();
		number_square_0.set_destroyed(false);

		number_square_1.set_grid_position(number_square_0_grid_position);
		number_square_1.reinit();
		number_square_1.set_destroyed(false);

		number_squares_reinitialised = true;
	}

	void GameStateSwap::enter_unlock_state() {
		m_main_game->update_state(std::make_unique<GameStateUnlock>(m_main_game));
	}

	std::unique_ptr<GameState> GameStateSwap::preprocess_state() {
		return nullptr;
	};

}