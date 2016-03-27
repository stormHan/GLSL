
#ifndef HAN_CALLBACKS_H
#define HAN_CALLBACKS_H

#include "Keys_han.h"

class ICallbacks
{
public:

	virtual void KeyboardCB(HAN_KEY Key) {};

	virtual void PassiveMouseCB(int x, int y) {};

	virtual void RenderSceneCB() {};

	virtual void MouseCB(HAN_MOUSE Button, HAN_KEY_STATE State, int x, int y) {};
};


#endif