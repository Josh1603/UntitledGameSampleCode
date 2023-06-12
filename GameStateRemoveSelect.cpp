#include "GameStateRemoveSelect.h"
#include "GameStateReload.h"
#include "GameStateSelect.h"
#include "AudioEngine.h"
#include "SettingsValues.h"

using namespace ax;

namespace fafo {
	GameStateRemoveSelect::GameStateRemoveSelect(MainGame* main_game) : m_main_game{ main_game } { number_square_selected = false; };

	void GameStateRemoveSelect::on_enter() {
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
					remove_select_square(number_square);
				}
				if (tet == ui::Widget::TouchEventType::CANCELED) {}
				});
		}
	};
	void GameStateRemoveSelect::on_exit() {
		m_main_game->set_responsive(false);
	};

	void GameStateRemoveSelect::update(float dt) {
		if (number_square_selected && !m_main_game->is_destroy_animation_running()) {
			enter_reload_state();
		}
	};

	void GameStateRemoveSelect::remove_select_square(NumberSquare& number_square) {
		if (!number_square.is_unlocked()) {
			m_main_game->show_remove_boost_extended_explanation_text();
		}
		else if (m_main_game->get_selected_square_indexs().size() == 0) {
			auto ruby_count_value = m_main_game->get_ruby_count_value();
			auto remove_boost_cost = m_main_game->get_remove_boost_cost();
			if (ruby_count_value >= remove_boost_cost) {

				m_main_game->set_ruby_count_value(ruby_count_value - remove_boost_cost);
				m_main_game->hide_boost_explanation_and_description_layouts();
				m_main_game->add_selected_square_index(number_square.get_index());
				number_square_selected = true;
				AudioEngine::play2d(m_main_game->get_random_chord_filename(), false, SettingsValues::audio_volume / 100.0F);
				number_square.destroy_animation({ 0.0F,0.0F }, true, false);
				number_square.set_destroyed(true);
			}
			else {
				m_main_game->hide_boost_explanation_and_description_layouts();
				m_main_game->show_insufficient_rubies_explanation_text();
				enter_select_state();
			}
		}
	}

	void GameStateRemoveSelect::enter_reload_state() {
		auto number_squares_indexs = m_main_game->get_selected_square_indexs();
		m_main_game->update_state(std::make_unique<GameStateReload>(m_main_game, std::vector<int>{ number_squares_indexs[0] }, false));
	}

	std::unique_ptr<GameState> GameStateRemoveSelect::preprocess_state() {
		return nullptr;
	};

	void GameStateRemoveSelect::enter_select_state() {
		m_main_game->update_state(std::make_unique<GameStateSelect>(m_main_game));
	}
}