/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ocean/platform/win/Taskbar.h"

#include <winsock2.h>
#include <windows.h>

namespace Ocean
{

namespace Platform
{

namespace Win
{

bool Taskbar::show()
{
	const HWND handle = FindWindowA("Shell_TrayWnd", "");

	if (handle == nullptr)
	{
		return false;
	}

	ShowWindow(handle, SW_SHOW);

	return true;
}

bool Taskbar::hide()
{
	const HWND handle = FindWindowA("Shell_TrayWnd", "");

	if (handle == nullptr)
	{
		return false;
	}

	return ShowWindow(handle, SW_HIDE) == TRUE;
}

}

}

}
