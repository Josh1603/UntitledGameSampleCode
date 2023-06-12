#ifndef __GAME_STATE_TRN_TO_MAIN_G_H__
#define __GAME_STATE_TRN_TO_MAIN_G_H__

#include "axmol.h"
#include "GameState.h"
#include "MainGame.h"

using namespace ax;

namespace fafo {

	class GameStateTransitionToMainGame : public GameState
	{
		MainGame* m_main_game;
		void enter_select_state();
		bool entered_select_state;
		float delay;
		float time_expended;
		void transition();
	public:
		GameStateTransitionToMainGame(MainGame* main_game);
		void on_enter();
		void on_exit();
		std::unique_ptr<GameState> preprocess_state();
		void update(float dt);
	};
}
#endif 