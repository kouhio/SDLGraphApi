#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#ifdef __cplusplus
	extern "C" {
#endif

SDLKey readKeyPress(void);
SDLKey readKeyPressRelease(void);
SDLKey readKeyPressed(void);
SDLKey readKeyReleased(void);
int setKeyRepeatTime(int delay, int interval);

#ifdef __cplusplus
	}
#endif

#endif // __KEYBOARD_H__

