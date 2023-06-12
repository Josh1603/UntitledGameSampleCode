#include "GameStateMerge.h"
#include "GameStateReload.h"
#include "SquareConstraints.h"


using namespace ax;

namespace fafo {
	GameStateMerge::GameStateMerge(MainGame* main_game) : m_main_game{ main_game } {
		pivot_number_square_reinitialised = false;
		pivot_animation_complete = false;
	};

	//Run a directed "destroy" animation for each selected number square and increment the score counter
	void GameStateMerge::on_enter() {
		auto& number_squares = m_main_game->get_number_squares();
		auto& selected_number_squares_indexs = m_main_game->get_selected_square_indexs();
		auto& first_selected_number_square = number_squares[selected_number_squares_indexs[0]];
		auto& pivot_selected_number_square = number_squares[selected_number_squares_indexs[1]];
		auto& last_selected_number_square = number_squares[selected_number_squares_indexs[2]];

		square_value = first_selected_number_square.get_number();

		first_selected_number_square.quick_deselection_animation();
		pivot_selected_number_square.quick_deselection_animation();


		auto first_selected_number_square_position = m_main_game->get_world_space_from_game_grid_group(first_selected_number_square.get_square_sprite()->getPosition());
		auto last_selected_number_square_position = m_main_game->get_world_space_from_game_grid_group(last_selected_number_square.get_square_sprite()->getPosition());
		auto pivot_selected_number_square_position = m_main_game->get_world_space_from_game_grid_group(pivot_selected_number_square.get_square_sprite()->getPosition());
		auto pivot_selected_number_square_target_position = m_main_game->get_world_space_from_score_group({ m_main_game->get_score_count_text_position().x - (m_main_game->get_score_count_text_width() / 2.0F), m_main_game->get_score_count_text_position().y });

		m_main_game->set_destroy_animation_running(true);

		first_selected_number_square.destroy_animation(pivot_selected_number_square_position - first_selected_number_square_position, true, true);

		if (m_main_game->is_tutorial_mode()) {
			pivot_selected_number_square.destroy_animation({0.0F,0.0F}, false, true);

		}
		else {
			pivot_selected_number_square.destroy_and_shrink_animation(
				{ pivot_selected_number_square.get_square_sprite()->getPosition().x + SquareConstraints::square_length / 2.0F,
				pivot_selected_number_square.get_square_sprite()->getPosition().y - SquareConstraints::square_length / 2.0F
				}, false, pivot_animation_complete);

		}
		last_selected_number_square.destroy_animation(pivot_selected_number_square_position - last_selected_number_square_position, true, true);

		first_selected_number_square.set_destroyed(true);
		pivot_selected_number_square.set_destroyed(true);
		last_selected_number_square.set_destroyed(true);

	};

	//Reinitialise the pivot number square, animate the new pivot number square value, transition to reload state
	void GameStateMerge::update(float dt) {
		if (pivot_animation_complete) {
			m_main_game->increment_score_counter(square_value);
			pivot_animation_complete = false;
		}


		if (!m_main_game->is_destroy_animation_running() && !pivot_number_square_reinitialised) {
			reinitialise_pivot_number_square();
			merge_animate_pivot_number_square();
			pivot_number_square_reinitialised = true;


			enter_reload_state();

		}
	};
	//Reinitialises the pivot number square by switching out the node grid. Asset sprites are retained and reused
	void GameStateMerge::reinitialise_pivot_number_square() {

		auto& pivot_number_square = m_main_game->get_number_squares()[m_main_game->get_selected_square_indexs()[1]];
		pivot_number_square.reinit();
		pivot_number_square.set_destroyed(false);

	};
	void GameStateMerge::merge_animate_pivot_number_square() {
		auto& pivot_number_square = m_main_game->get_number_squares()[m_main_game->get_selected_square_indexs()[1]];
		auto new_number_square_value = m_main_game->increment_possible_number_value(pivot_number_square.get_number());
		pivot_number_square.set_number(new_number_square_value);

		if (new_number_square_value.first == m_main_game->get_high_number().first && new_number_square_value.second == m_main_game->get_high_number().second) {
			m_main_game->add_high_number_square_index(pivot_number_square.get_index());
			pivot_number_square.transform_to_locked_square();

			if (!m_main_game->is_high_number_present() && !m_main_game->is_tutorial_mode()) {
				m_main_game->show_new_locked_square_notification();
			}
				
			m_main_game->set_high_number_present(true);
		}
		pivot_number_square.merge_animation();
	}

	void GameStateMerge::enter_reload_state() {
		auto number_squares_indexs = m_main_game->get_selected_square_indexs();
		m_main_game->update_state(std::make_unique<GameStateReload>(m_main_game, std::vector<int>{ number_squares_indexs[0], number_squares_indexs[2] }, false));
	}

	void GameStateMerge::on_exit() {
		auto& number_squares = m_main_game->get_number_squares();
		auto& selected_number_squares_indexs = m_main_game->get_selected_square_indexs();
		auto& first_selected_number_square = number_squares[selected_number_squares_indexs[0]];
	};
	std::unique_ptr<GameState> GameStateMerge::preprocess_state() {
		return nullptr;
	};
}