#include "MainGame.h"
#include "GameStateSelect.h"
#include "GameStateTutorialSelect.h"
#include "GameStateRemoveSelect.h"
#include "GameStateSwapSelect.h"
#include "GameStateUnlockSelect.h"
#include "GameStateTransitionToLockedTileTutorial.h"
#include "GameStateTransitionToMainGame.h"
#include "GameStateTransitionToTutorial.h"
//Omitted: various includes

namespace fafo {

	//Omitted: initialise variables and interop

	bool MainGame::init()
	{
		if (!Scene::init())
		{
			return false;
		}

		//Omitted: initialise variables

		if (tutorial)
			game_state = std::make_unique<GameStateTutorialSelect>(this);
		else
			game_state = std::make_unique<GameStateSelect>(this);

		game_state->on_enter();

		scheduleUpdate();


		return true;
	}
	void MainGame::update_state(std::unique_ptr<GameState> state_update) {
		m_state_update = std::move(state_update);
	}
	void MainGame::transition_to_main_game_state() { update_state(std::make_unique<GameStateTransitionToMainGame>(this)); }
	void MainGame::transition_to_unlocked_tutorial() { update_state(std::make_unique<GameStateTransitionToLockedTileTutorial>(this)); }
	void MainGame::transition_to_tutorial() { update_state(std::make_unique<GameStateTransitionToTutorial>(this)); }

	//Omitted: initialise APIs
	//Omitted: parse and initialise data 

	void MainGame::draw_boost_buttons() {

		//Omitted: boost button initialisation

		remove_boost_button->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType tet) {

			if (tet == ui::Widget::TouchEventType::ENDED) {
				update_state(std::make_unique<GameStateRemoveSelect>(this));
			}
			});

		swap_boost_button->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType tet) {
			if (tet == ui::Widget::TouchEventType::ENDED) {
				update_state(std::make_unique<GameStateSwapSelect>(this));
			}
			});

		unlock_boost_button->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType tet) {
			if (tet == ui::Widget::TouchEventType::ENDED) {
				if (high_number_present) {
					update_state(std::make_unique<GameStateUnlockSelect>(this));
				}
				else {
					//Omitted: show notification
				}
			}
			});

	
	};
	void MainGame::draw_boost_description_layout() {
		//Omitted: boost description layout initialisation
		hidden_boost_description_dismiss_button->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType tet) {
			if (tet == ui::Widget::TouchEventType::ENDED) {
				update_state(std::make_unique<GameStateSelect>(this));
			}
			});
	}

	//Omitted: Various animation and notification functions

	void MainGame::set_state() {
		game_state->on_exit();
		game_state = std::move(m_state_update);
		game_state->on_enter();
		m_state_update = nullptr;
	}
	//Main game loop
	void MainGame::update(float dt) {

		//Omitted: Animation updates

		//Handle Game State changes
		if (!m_state_update) {
			//Preprocess state, Handle input etc.
			m_state_update = (game_state->preprocess_state());
		}

		//If game state has changed set the new state
		if (m_state_update) {
			set_state();
		}

		//Upate the game state
		game_state->update(dt);

		//Omitted: Animation and API updates
	}

	//Omitted: Remaining code (API calls, layout menus etc.)
}
