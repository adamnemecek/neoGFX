// i_sprite.hpp
/*
  neogfx C++ GUI Library
  Copyright(C) 2016 Leigh Johnston
  
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
#include <chrono>
#include <boost/optional.hpp>
#include <neogfx/core/geometry.hpp>
#include <neogfx/core/path.hpp>
#include "i_shape.hpp"
#include "i_physical_object.hpp"

namespace neogfx
{
	class i_sprite : public virtual i_shape, public virtual i_physical_object
	{
		// types
	public:
		typedef i_physical_object::time_interval time_interval;
		typedef i_physical_object::optional_time_interval optional_time_interval;
		typedef i_physical_object::step_time_interval step_time_interval;
		typedef i_physical_object::optional_step_time_interval optional_step_time_interval;
		// object
	public:
		virtual ~i_sprite() {}
		// physics
	public:
		virtual bool update(const optional_time_interval& aNow, const vec3& aForce) = 0;
		virtual const optional_time_interval& update_time() const = 0;
		virtual void set_update_time(const optional_time_interval& aLastUpdateTime) = 0;
		// geometry
	public:
		virtual vec3 origin() const = 0;
		virtual vec3 position() const = 0;
		virtual void set_origin(const vec3& aOrigin) = 0;
		virtual void set_position(const vec3& aPosition) = 0;
		// physics
	public:
		virtual const optional_path& path() const = 0;
		virtual void set_path(const optional_path& aPath) = 0;
	};
}