/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ocean/platform/win/System.h"

#include "ocean/base/String.h"

#include "ocean/io/File.h"

namespace Ocean
{

namespace Platform
{

namespace Win
{

IO::Directory System::currentDirectory()
{
	const DWORD size = GetCurrentDirectoryW(0, nullptr); // including the null-terminating character

	if (size <= 1)
	{
		return IO::Directory();
	}

	std::vector<wchar_t> buffer(size);

	const DWORD written = GetCurrentDirectoryW(DWORD(buffer.size()), buffer.data()); // not including the null-terminating character

	if (written >= DWORD(buffer.size()))
	{
		return IO::Directory();
	}

	ocean_assert(buffer.back() == wchar_t(0));

	return IO::Directory(String::toAString(buffer.data()));
}

IO::Directory System::processDirectory()
{
	static_assert(MAX_PATH > 0, "Value of macro MAX_PATH is zero");
	wchar_t buffer[MAX_PATH + 1];

	if (GetModuleFileNameW(nullptr, buffer, MAX_PATH) == 0)
	{
		return IO::Directory();
	}

	const IO::File applicationFile(String::toAString(buffer));
	return IO::Directory(applicationFile);
}

ScopedHMODULE System::currentLibraryModule()
{
	ScopedHMODULE scopedModule;

	if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, LPCTSTR(System::currentLibraryModule), &scopedModule.resetObject()) == TRUE)
	{
		// now we check whether the calling process is the application or a library
		if (GetModuleHandle(nullptr) == scopedModule.object())
		{
			return ScopedHMODULE();
		}

		return scopedModule;
	}

	return ScopedHMODULE();
}

std::wstring System::moduleName(const HMODULE handle)
{
	if (handle == nullptr)
	{
		return std::wstring();
	}

	wchar_t name[4096 + 2];
	const DWORD result = GetModuleFileName(handle, name, 4096);
	ocean_assert(result != 4096);

	if (result == 0)
	{
		return std::wstring();
	}

	// check whether the name is a zero-terminated string (issue on Windows XP)
	if (name[result - 1] != L'\0')
	{
		name[result] = L'\0';
	}

	return std::wstring(name);
}

std::string System::environmentVariable(const std::string& variable, const bool removeQuotes)
{
	const DWORD size = GetEnvironmentVariableA(variable.c_str(), nullptr, 0);

	if (size == 0)
	{
		return std::string();
	}

	std::string value(size_t(size), ' ');

	const DWORD writtenSize = GetEnvironmentVariableA(variable.c_str(), &value[0], DWORD(value.size()));

	if (writtenSize == 0 || writtenSize >= size)
	{
		return std::string();
	}

	value.resize(size_t(writtenSize));

	if (removeQuotes)
	{
		while (value.length() != 0)
		{
			if (value[0] == '\"')
			{
				value = value.substr(1);
			}
			else if (value[value.length() - 1] == '\"')
			{
				value = value.substr(0, value.length() - 1);
			}
			else
			{
				break;
			}
		}
	}

	return value;
}

}

}

}
