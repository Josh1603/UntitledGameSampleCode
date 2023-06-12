#include "GameStateSwapSelect.h"
#include "GameStateSwap.h"
#include "GameStateSelect.h"
#include "AudioEngine.h"
#include "SettingsValues.h"



using namespace ax;

namespace fafo {
	GameStateSwapSelect::GameStateSwapSelect(MainGame* main_game) : m_main_game{ main_game } {
		number_squares_selected = false;
	};

	void GameStateSwapSelect::on_enter() {
		auto& number_squares = m_main_game->get_number_squares();
		auto& selected_number_squares_indexs = m_main_game->get_selected_square_indexs();
		for (auto selected_number_square_index : selected_number_squares_indexs) {
			number_squares[selected_number_square_index].quick_deselection_animation();
		}

		m_main_game->set_responsive(true);
		m_main_game->clear_selected_square_indexs();

		for (auto& number_square : m_main_game->get_number_squares()) {

			number_square.get_sensor_button()->setTouchEnabled(true);
			number_square.get_sensor_button()->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType tet) {
				if (tet == ui::Widget::TouchEventType::BEGAN) {}
				if (tet == ui::Widget::TouchEventType::MOVED) {}
				if (tet == ui::Widget::TouchEventType::ENDED) {
					swap_select_square(number_square);
				}
				if (tet == ui::Widget::TouchEventType::CANCELED) {}
				});
		}

	};

	void GameStateSwapSelect::swap_select_square(NumberSquare number_square) {

		auto ruby_count_value = m_main_game->get_ruby_count_value();
		auto remove_boost_cost = m_main_game->get_swap_boost_cost();
		if (ruby_count_value >= remove_boost_cost) {

			if (m_main_game->get_selected_square_indexs().size() == 0) {
				m_main_game->add_selected_square_index(number_square.get_index());
				number_square.selection_animation();
				AudioEngine::play2d(m_main_game->get_random_note_filename(), false, SettingsValues::audio_volume / 100.0F);
			}
			else if (m_main_game->get_selected_square_indexs().size() == 1) {
				if (m_main_game->get_selected_square_indexs()[0] != number_square.get_index()) {
					m_main_game->add_selected_square_index(number_square.get_index());
					number_square.selection_animation();
					m_main_game->set_ruby_count_value(ruby_count_value - remove_boost_cost);
					number_squares_selected = true;
					AudioEngine::play2d(m_main_game->get_random_chord_filename(), false, SettingsValues::audio_volume / 100.0F);
				}
				else {
					number_square.quick_deselection_animation();
					m_main_game->clear_selected_square_indexs();
				}
			}
		}
		else {
			m_main_game->hide_boost_explanation_and_description_layouts();
			m_main_game->show_insufficient_rubies_explanation_text();
			enter_select_state();
		}
	}

	void GameStateSwapSelect::enter_swap_state() {
		m_main_game->update_state(std::make_unique<GameStateSwap>(m_main_game));
	}

	void GameStateSwapSelect::update(float dt) {
		if (number_squares_selected)
		{
			enter_swap_state();
			m_main_game->hide_boost_explanation_and_description_layouts();
		}
	};

	void GameStateSwapSelect::on_exit() {
		m_main_game->set_responsive(false);
	};
	std::unique_ptr<GameState> GameStateSwapSelect::preprocess_state() {
		return nullptr;
	};

	void GameStateSwapSelect::enter_select_state() {
		m_main_game->update_state(std::make_unique<GameStateSelect>(m_main_game));
	}
}