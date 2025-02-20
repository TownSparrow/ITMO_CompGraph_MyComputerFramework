#include "InputDevice.h"
#include "Game.h"
#include "DisplayWin32.h"
#include <iostream>
#include <SimpleMath.h>

InputDevice::InputDevice(Game* game)
  : game(game) {
    keys = new std::unordered_set<Keys>();
    RAWINPUTDEVICE rawInputDevice[2];

		rawInputDevice[0].usUsagePage = 0x01;
		rawInputDevice[0].usUsage = 0x02;
		rawInputDevice[0].dwFlags = 0;
		rawInputDevice[0].hwndTarget = game->window->hWnd;

		rawInputDevice[1].usUsagePage = 0x01;
		rawInputDevice[1].usUsage = 0x06;
		rawInputDevice[1].dwFlags = 0;
		rawInputDevice[1].hwndTarget = game->window->hWnd;

		if (RegisterRawInputDevices(
			rawInputDevice,
			2, 
			sizeof(rawInputDevice[0])) == FALSE
			) {
			auto errorCode = GetLastError();
			std::cout << "ERROR: " << errorCode << std::endl;
		}
}
InputDevice::~InputDevice() {
	delete keys;
}

void InputDevice::OnKeyDown(KeyboardInputEventArgs args) {
	bool isBreak = args.Flags & 0x01;
	auto key = static_cast<Keys>(args.VKey);

	if (args.MakeCode == 42) key = Keys::LeftShift;
	if (args.MakeCode == 54) key = Keys::RightShift;

	if (isBreak) {
		if (keys->count(key))
			RemovePressedKey(key);
	}
	else {
		if (!keys->count(key))
			AddPressedKey(key);
	}
}

void InputDevice::OnMouseMove(RawMouseEventArgs args) {
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::LeftButtonDown))
		AddPressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::LeftButtonUp))
		RemovePressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::RightButtonDown))
		AddPressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::RightButtonUp))
		RemovePressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::MiddleButtonDown))
		AddPressedKey(Keys::MiddleButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::MiddleButtonUp))
		RemovePressedKey(Keys::MiddleButton);

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(
		game->window->hWnd,
		&point
	);

	MousePosition = DirectX::SimpleMath::Vector2::Vector2(point.x, point.y);
	MouseOffset = DirectX::SimpleMath::Vector2::Vector2(args.X, args.Y);
	MouseWheelDelta = args.WheelDelta;

	const MouseMoveEventArgs moveArgs = { MousePosition, MouseOffset, MouseWheelDelta };
	MouseMove.Broadcast(moveArgs);
}

void InputDevice::AddPressedKey(Keys key) {
	keys->insert(key);
}

void InputDevice::RemovePressedKey(Keys key) {
	keys->erase(key);
}

bool InputDevice::IsKeyDown(Keys key) {
	return keys->count(key);
}