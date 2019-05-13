// CTHackFramework 														      
//	A framework for general game hacking 									      
// Copyright © 2019 Celestial Tech All Rights Reserved

#pragma once

#include <string>
#include <vector>

namespace Util
{
	namespace StringManipulation
	{
		// string转wstring
		std::wstring StringToWstring(const std::string& _szString)
		{
			std::wstring szWideString(_szString.begin(), _szString.end());
			return szWideString;
		}

		// wstring转string
		std::string WstringToString(const std::wstring& _szWstring)
		{
			std::string szString(_szWstring.begin(), _szWstring.end());
			return szString;
		}

		// 字符串分割
		/// 参数：_str：目标字符串
		///           _delimiter：分隔符
		/// 返回值：分割后的子字符串向量
		std::vector<std::string> SplitString(const std::string& _str, const std::string& _delimiter)
		{
			std::vector<std::string> Return;
			auto start = 0U;
			auto end = _str.find(_delimiter);
			while (end != std::string::npos)
			{
				Return.push_back(_str.substr(start, end - start));
				start = end + _delimiter.length();
				end = _str.find(_delimiter, start);
			}

			Return.push_back(_str.substr(start, end));
			return Return;
		}
	}
}