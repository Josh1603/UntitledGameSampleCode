#include "GameStateTransitionToLockedTileTutorial.h"
#include "GameStateLockedTileTutorialSelect.h"
#include "AudioEngine.h"
#include "SettingsValues.h"


using namespace ax;

namespace fafo {
	GameStateTransitionToLockedTileTutorial::GameStateTransitionToLockedTileTutorial(MainGame* main_game) : m_main_game{ main_game } {

		entered_select_state = false;
		delay = 1.75F;
		time_expended = 0.0F;
	};

	void GameStateTransitionToLockedTileTutorial::on_enter() {
		m_main_game->show_thats_it_explanation(delay);
	};

	void GameStateTransitionToLockedTileTutorial::enter_locked_tile_tutorial_select_state() {
		m_main_game->update_state(std::make_unique<GameStateLockedTileTutorialSelect>(m_main_game));
	}

	void GameStateTransitionToLockedTileTutorial::on_exit() {
	};

	std::unique_ptr<GameState> GameStateTransitionToLockedTileTutorial::preprocess_state() {
		return nullptr;
	};

	void GameStateTransitionToLockedTileTutorial::transition() {
		AudioEngine::play2d(m_main_game->get_whoosh_transition_effect_filename(), false, SettingsValues::audio_volume / 100.0F);
		m_main_game->set_tutorial_complete(false);
		m_main_game->set_unlock_tutorial_mode(true);
		m_main_game->transition_to_locked_tile_explanation();
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
		m_main_game->set_game_grid_dimensions_to_unlock_tutorial();
		m_main_game->redraw_game_grid();
		m_main_game->reset_game_grid_clipping_node_size();
	}
	void GameStateTransitionToLockedTileTutorial::update(float dt) {
		time_expended += dt;
		if (!entered_select_state && time_expended > delay) {
			transition();
			enter_locked_tile_tutorial_select_state();
			entered_select_state = true;
		}
	};

}