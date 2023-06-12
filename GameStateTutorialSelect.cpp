#include "GameStateTutorialSelect.h"
#include "GameStateMerge.h"
#include "AudioEngine.h"
#include "SettingsValues.h"


using namespace ax;

namespace fafo {
	GameStateTutorialSelect::GameStateTutorialSelect(MainGame* main_game) : m_main_game{ main_game } {};

	void GameStateTutorialSelect::on_enter() {

		m_main_game->set_responsive(true);
		m_main_game->clear_selected_square_indexs();

		for (auto& number_square : m_main_game->get_number_squares()) {

			number_square.get_sensor_button()->setTouchEnabled(true);
			number_square.get_sensor_button()->addTouchEventListener([this, number_square](Ref* ref, ui::Widget::TouchEventType tet) mutable {
				if (tet == ui::Widget::TouchEventType::BEGAN) {}
				if (tet == ui::Widget::TouchEventType::MOVED) {}
				if (tet == ui::Widget::TouchEventType::ENDED) {
					if (number_square.is_unlocked())
						select_square(number_square);
				}
				if (tet == ui::Widget::TouchEventType::CANCELED) {}
				});
		}
	};

	void GameStateTutorialSelect::select_square(NumberSquare number_square) {
		bool already_selected = false;

		//Check if already selected
		for (int i = 0; i < m_main_game->get_selected_square_indexs().size(); i++)
		{
			if (m_main_game->get_selected_square_indexs()[i] == number_square.get_index()) { 
				already_selected = true;
				break;
			}
		}

		if (!already_selected) {
			SquareConstraints::last_selected_color_index = number_square.get_color();

			//Check if the same type as previous selection
			if (m_main_game->get_selected_square_indexs().size()) {
				auto previous_selection = m_main_game->get_number_squares()[m_main_game->get_selected_square_indexs()[0]];
				if (previous_selection.get_color() != number_square.get_color() || previous_selection.get_number() != number_square.get_number()) {
					for (auto index : m_main_game->get_selected_square_indexs()) {
						m_main_game->get_number_squares()[index].deselection_animation();
					}
					m_main_game->clear_selected_square_indexs();
				}
			}

			//Add the new index
			m_main_game->add_selected_square_index(number_square.get_index());

			if (m_main_game->get_selected_square_indexs().size() < 3) {
				AudioEngine::play2d(m_main_game->get_random_note_filename(), false, SettingsValues::audio_volume / 100.0F);
				m_main_game->get_number_squares()[number_square.get_index()].selection_animation();
			}

			else {
				AudioEngine::play2d(m_main_game->get_random_chord_filename(), false, SettingsValues::audio_volume / 100.0F);
				for (auto& number_square : m_main_game->get_number_squares()) {
					number_square.get_sensor_button()->setTouchEnabled(false);
				}

				m_main_game->transition_away_tutorial_explanation();
				enter_merge_state();
			}
		}
		else {
			for (int i = 0; i < m_main_game->get_selected_square_indexs().size(); i++)
			{
				if (m_main_game->get_selected_square_indexs()[i] == number_square.get_index()) {
					continue;
				}
				else {
					m_main_game->get_number_squares()[m_main_game->get_selected_square_indexs()[i]].deselection_animation();
				}
			}
			m_main_game->clear_selected_square_indexs();
			m_main_game->add_selected_square_index(number_square.get_index());
		}
	}

	void GameStateTutorialSelect::enter_merge_state() {
		m_main_game->update_state(std::make_unique<GameStateMerge>(m_main_game));
	}

	void GameStateTutorialSelect::on_exit() {
		m_main_game->set_responsive(false);
		m_main_game->disable_boost_buttons();
		m_main_game->set_tutorial_complete(true);
	};

	std::unique_ptr<GameState> GameStateTutorialSelect::preprocess_state() {
		return nullptr;
	};
	void GameStateTutorialSelect::update(float dt) {};

}