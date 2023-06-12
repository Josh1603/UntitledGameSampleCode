#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include "axmol.h"

using namespace ax;

namespace fafo {

	class GameState
	{
	public:
		virtual void on_enter() = 0;
		virtual void on_exit() = 0;
		virtual std::unique_ptr<GameState> preprocess_state() = 0;
		virtual void update(float dt) = 0;
	};
}
#endif 