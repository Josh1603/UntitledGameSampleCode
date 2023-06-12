#include "GameStateUnlockSelect.h"
#include "GameStateRemove.h"
#include "GameStateSelect.h"
#include "AudioEngine.h"
#include "SettingsValues.h"


using namespace ax;

namespace fafo {
	GameStateUnlockSelect::GameStateUnlockSelect(MainGame* main_game) : m_main_game{ main_game } {
		number_square_selected = false;
		animation_delay = 0.0F;
	};

	void GameStateUnlockSelect::on_enter() {
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
					unlock_select_square(number_square);
				}
				if (tet == ui::Widget::TouchEventType::CANCELED) {}
				});
		}
	};

	void GameStateUnlockSelect::unlock_select_square(NumberSquare number_square) {
		if (number_square.get_number().first == m_main_game->get_high_number().first && number_square.get_number().second == m_main_game->get_high_number().second) {
			auto ruby_count_value = m_main_game->get_ruby_count_value();
			auto remove_boost_cost = m_main_game->get_unlock_boost_cost();
			if (ruby_count_value >= remove_boost_cost) {
				m_main_game->set_ruby_count_value(ruby_count_value - remove_boost_cost);
				number_square_selected = true;
				AudioEngine::play2d(m_main_game->get_unlock_effect_filename(), false, SettingsValues::audio_volume / 100.0F);
				m_main_game->hide_boost_explanation_and_description_layouts();
			}
			else {
				m_main_game->hide_boost_explanation_and_description_layouts();
				m_main_game->show_insufficient_rubies_explanation_text();
				enter_select_state();
			}


		}
		else {
			m_main_game->show_unlock_boost_extended_explanation_text();
		}
	}

	void GameStateUnlockSelect::on_exit() {
		m_main_game->set_responsive(false);
	};
	std::unique_ptr<GameState> GameStateUnlockSelect::preprocess_state() {
		return nullptr;
	};
	void GameStateUnlockSelect::update(float dt) {
		if (number_square_selected) 
		{
			animation_delay += dt;
			if (animation_delay > 0.333F) {
				enter_remove_state();
			}
		}
	};

	void GameStateUnlockSelect::enter_remove_state() {
		m_main_game->update_state(std::make_unique<GameStateRemove>(m_main_game));
	}

	void GameStateUnlockSelect::enter_select_state() {
		m_main_game->update_state(std::make_unique<GameStateSelect>(m_main_game));
	}
}