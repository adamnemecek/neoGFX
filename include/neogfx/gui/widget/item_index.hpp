// item_index.hpp
/*
  neogfx C++ GUI Library
  Copyright (c) 2015 Leigh Johnston.  All Rights Reserved.
  
  This program is free software: you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <neogfx/neogfx.hpp>
#include <tuple>
#include <optional>

namespace neogfx
{
	class item_index
	{
	public:
		typedef uint32_t value_type;
		typedef value_type row_type;
		typedef value_type column_type;
	public:
		item_index() :
			iRow{ 0 }, iColumn{ 0 }
		{
		}
		item_index(row_type aRow) :
			iRow{ aRow }, iColumn{ 0 }
		{
		}
		item_index(row_type aRow, column_type aColumn) :
			iRow{ aRow }, iColumn{ aColumn }
		{
		}
	public:
		item_index& operator+=(const item_index& aRhs)
		{
			iRow += aRhs.iRow;
			iColumn += aRhs.iColumn;
			return *this;
		}
		item_index& operator-=(const item_index& aRhs)
		{
			iRow -= aRhs.iRow;
			iColumn -= aRhs.iColumn;
			return *this;
		}
	public:
		row_type row() const
		{
			return iRow;
		}
		void set_row(row_type aRow)
		{
			iRow = aRow;
		}
		column_type column() const
		{
			return iColumn;
		}
		void set_column(column_type aColumn)
		{
			iColumn = aColumn;
		}
	private:
		row_type iRow;
		column_type iColumn;
	};

	inline bool operator==(const item_index& aLhs, const item_index& aRhs)
	{
		return aLhs.row() == aRhs.row() && aLhs.column() == aRhs.column();
	}

	inline bool operator!=(const item_index& aLhs, const item_index& aRhs)
	{
		return !(aLhs == aRhs);
	}

	inline bool operator<(const item_index& aLhs, const item_index& aRhs)
	{
		return std::forward_as_tuple(aLhs.row(), aLhs.column()) < std::forward_as_tuple(aRhs.row(), aRhs.column());
	}

	inline bool operator>(const item_index& aLhs, const item_index& aRhs)
	{
		return std::forward_as_tuple(aLhs.row(), aLhs.column()) > std::forward_as_tuple(aRhs.row(), aRhs.column());
	}

	typedef std::optional<item_index> optional_item_index;
}