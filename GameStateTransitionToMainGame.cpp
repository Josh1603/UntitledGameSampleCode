#include "GameStateTransitionToMainGame.h"
#include "GameStateSelect.h"
#include "AudioEngine.h"
#include <fstream>
#include "SettingsValues.h"


using namespace ax;

namespace fafo {
	GameStateTransitionToMainGame::GameStateTransitionToMainGame(MainGame* main_game) : m_main_game{ main_game } {
		entered_select_state = false;
		delay = 2.0F;
		time_expended = 0.0F;
	};

	void GameStateTransitionToMainGame::on_enter() {
		m_main_game->show_you_got_it_explanation(delay);
	};

	void GameStateTransitionToMainGame::enter_select_state() {
		m_main_game->update_state(std::make_unique<GameStateSelect>(m_main_game));
	}

	void GameStateTransitionToMainGame::on_exit() {
		//Prevents auto-tutorial on future game runs
		auto file_util = FileUtils::getInstance();
		auto file_path = file_util->getWritablePath() + "TutorialCheck.txt";
		if (!file_util->isFileExist(file_path)) {
			std::ofstream ofstr{ file_path };
			ofstr << 0;
		}
	};

	std::unique_ptr<GameState> GameStateTransitionToMainGame::preprocess_state() {
		return nullptr;
	};

	void GameStateTransitionToMainGame::transition() {
		AudioEngine::play2d(m_main_game->get_whoosh_transition_effect_filename(), false, SettingsValues::audio_volume / 100.0F);
		m_main_game->enable_ruby_button();
		m_main_game->set_tutorial_complete(false);
		m_main_game->set_unlock_tutorial_mode(false);
		m_main_game->set_tutorial_mode(false);
		m_main_game->transition_away_description_and_explanation();
		m_main_game->clear_selected_square_indexs();
		m_main_game->clear_high_number_square_indexs();

		for (auto& number_square : m_main_game->get_number_squares()) {
			number_square.shatter_animation();
			if (!number_square.is_unlocked()) {
				number_square.lock_shatter_animation();
			}
			number_square.get_sensor_button()->setTouchEnabled(false);
		}

		m_main_game->get_number_squares().clear();
		m_main_game->set_game_grid_dimensions_to_main_game();
		m_main_game->set_hidden_gem_probabilities_and_values_to_main_game();
		m_main_game->set_main_game_possible_number_values_and_probabilities();
		m_main_game->redraw_game_grid();
		for (auto& number_square : m_main_game->get_number_squares()) {
			number_square.drop_animation(m_main_game->get_game_grid_screen_height());
		}
		m_main_game->reset_game_grid_clipping_node_size();
	}

	void GameStateTransitionToMainGame::update(float dt) {
		time_expended += dt;
		if (!entered_select_state && time_expended > delay) {
			transition();
			enter_select_state();
			entered_select_state = true;
		}
	};

}