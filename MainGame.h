#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__

#include "axmol.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "GameState.h"

//Omitted: various includes
using namespace ax;

namespace fafo {

	class MainGame : public ax::Scene
	{
		//Omitted: various variables, enums and functions
	public:

		//Class functions
		virtual bool init() override;
		void update_state(std::unique_ptr<GameState> state_update);
		void transition_to_main_game_state();
		void transition_to_unlocked_tutorial();
		void transition_to_tutorial();

	private:
		
		//Class variables
		bool tutorial;
		std::unique_ptr<GameState> game_state;
		std::unique_ptr<GameState> m_state_update;

		//Class assets
		ui::Button* remove_boost_button;
		ui::Button* swap_boost_button;
		ui::Button* hidden_boost_description_dismiss_button;
		
		//Class init functions
		void draw_boost_buttons();
		void draw_boost_description_layout();

		//Class functions
		void set_state();
		void update(float dt);

	};
}

#endif  // __MAINGAME_SCENE_H__
