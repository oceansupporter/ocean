/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ocean/platform/win/ToolTip.h"
#include "ocean/platform/win/Utilities.h"

#include <CommCtrl.h>

namespace Ocean
{

namespace Platform
{

namespace Win
{

bool ToolTip::addToolTip(const HWND handle, const std::wstring& text)
{
	ocean_assert(handle != nullptr);
	if (handle == nullptr)
	{
		return false;
	}

	const HWND hwndTip = CreateWindowExW(0, TOOLTIPS_CLASSW, nullptr, WS_POPUP | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
										GetParent(handle), nullptr, GetModuleHandleW(nullptr), nullptr);

	ocean_assert(hwndTip != nullptr);
	if (hwndTip == nullptr)
	{
		return false;
	}

	TOOLINFOW toolInfo = {0};
	toolInfo.cbSize = TTTOOLINFOW_V1_SIZE;
	toolInfo.hwnd = GetParent(handle);
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = UINT_PTR(handle);
	toolInfo.lpszText = const_cast<wchar_t*>(text.c_str());

	if (SendMessageW(hwndTip, TTM_ACTIVATE, TRUE, 0))
	{
		ocean_assert(false && "This should never happen!");
		return false;
	}

	if (!SendMessageW(hwndTip, TTM_ADDTOOLW, 0, LPARAM(&toolInfo)))
	{
		ocean_assert(false && "This should never happen!");
		return false;
	}

	// check whether we have a multiline tool tip
	if (text.find(L'\n') != std::string::npos)
	{
		HFONT fontHandle = HFONT(SendMessageW(hwndTip, WM_GETFONT, 0, 0));

		if (!fontHandle)
		{
			fontHandle = HFONT(GetStockObject(DEFAULT_GUI_FONT));
			if (!fontHandle)
			{
				ocean_assert(false && "This should never happen!");
				return false;
			}
		}

		const ScopedDC hdc(nullptr);

		if (!hdc.isValid())
		{
			ocean_assert(false && "This should never happen!");
			return false;
		}

		// the font belongs to the tool tip window or is a stock object, so it must not be deleted here
		if (!SelectObject(*hdc, fontHandle))
		{
			return false;
		}

		int max = 0;

		std::string::size_type start = 0;
		std::string::size_type end = std::string::npos;

		while (true)
		{
			end = text.find(L'\n', start);
			std::wstring line = text.substr(start, end == std::string::npos ? end : end - start);

			SIZE sz;
			if (!GetTextExtentPoint32W(*hdc, line.c_str(), int(line.length()), &sz))
			{
				ocean_assert(false && "This should never happen!");
			}

			if (sz.cx > max)
			{
				max = sz.cx;
			}

			if (end == std::string::npos)
			{
				break;
			}

			start = end + 1;
		}

		if (max > SendMessageW(hwndTip, TTM_GETMAXTIPWIDTH, 0, 0))
		{
			SendMessageW(hwndTip, TTM_SETMAXTIPWIDTH, 0, max);
		}
	}

	return true;
}

}

}

}
