/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ocean/platform/win/PopupMenu.h"
#include "ocean/platform/win/Mouse.h"

namespace Ocean
{

namespace Platform
{

namespace Win
{

PopupMenu::PopupMenu()
{
	handle_ = CreatePopupMenu();
}

PopupMenu::~PopupMenu()
{
	release();
}

size_t PopupMenu::size() const
{
	ocean_assert(handle_ != nullptr);

	const int number = GetMenuItemCount(handle_);

	ocean_assert(number >= 0);
	if (number < 0)
	{
		return 0;
	}

	return size_t(number);
}

bool PopupMenu::addEntry(const std::wstring& text, const unsigned int id, const bool checked)
{
	ocean_assert(handle_ != nullptr);

	if (checked)
	{
		return AppendMenuW(handle_, MF_STRING | MF_CHECKED, id, text.c_str()) == TRUE;
	}
	else
	{
		return AppendMenuW(handle_, MF_STRING, id, text.c_str()) == TRUE;
	}
}

bool PopupMenu::addSeparator()
{
	ocean_assert(handle_ != nullptr);

	return AppendMenuW(handle_, MF_SEPARATOR, 0, nullptr) == TRUE;
}

PopupMenu& PopupMenu::addMenu(const std::wstring& text)
{
	ocean_assert(handle_ != nullptr);

	UniquePopupMenu menu = std::make_unique<PopupMenu>();

	const bool appendResult = AppendMenuW(handle_, MF_POPUP, UINT_PTR(menu->handle_), text.c_str()) == TRUE;
	ocean_assert(appendResult);

	menu->ownsHandle_ = !appendResult;

	subMenus_.emplace_back(std::move(menu));

	return *subMenus_.back();
}

unsigned int PopupMenu::show(const int x, const int y, const HWND parent)
{
	ocean_assert(handle_ != nullptr);
	ocean_assert(parent != nullptr);

	return (unsigned int)TrackPopupMenu(handle_, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, x, y, 0, parent, nullptr);
}

unsigned int PopupMenu::show(const HWND parent)
{
	ocean_assert(parent != nullptr);

	const VectorI2 cursorPosition(Mouse::screenPosition());
	return show(cursorPosition.x(), cursorPosition.y(), parent);
}

void PopupMenu::release()
{
	// attached sub-menus are recursively destroyed by their owning ancestor menu, while unattached sub-menus retain ownership of their handles
	subMenus_.clear();

	if (handle_ != nullptr && ownsHandle_)
	{
		const BOOL result = DestroyMenu(handle_); // recursively destroys all attached sub-menus
		ocean_assert_and_suppress_unused(result == TRUE, result);
	}

	handle_ = nullptr;
	ownsHandle_ = false;
}

}

}

}
