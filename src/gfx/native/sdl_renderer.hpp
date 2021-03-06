// sdl_renderer.hpp
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
#include <set>
#include <map>
#include "opengl_renderer.hpp"

namespace neogfx
{
    class i_native_surface;

    class offscreen_window
    {
    public:
        virtual ~offscreen_window() = default;
    public:
        virtual void* handle() const = 0;
    };

    class sdl_renderer : public opengl_renderer
    {
    public:
        struct unsupported_renderer : std::runtime_error { unsupported_renderer() : std::runtime_error("neogfx::sdl_renderer::unsupported_renderer") {} };
        struct failed_to_create_offscreen_window : std::runtime_error { failed_to_create_offscreen_window(const std::string& aReason) : std::runtime_error("neogfx::sdl_renderer::failed_to_create_offscreen_window: " + aReason) {} };
        struct failed_to_create_opengl_context : std::runtime_error { failed_to_create_opengl_context(const std::string& aReason) : std::runtime_error("neogfx::sdl_renderer::failed_to_create_opengl_context: " + aReason) {} };
        struct failed_to_activate_opengl_context : std::runtime_error { failed_to_activate_opengl_context(const std::string& aReason) : std::runtime_error("neogfx::sdl_renderer::failed_to_activate_opengl_context: " + aReason) {} };
        struct no_target_active : std::logic_error { no_target_active() : std::logic_error("neogfx::sdl_renderer::no_target_active") {} };
    public:
        sdl_renderer(neogfx::renderer aRenderer, bool aDoubleBufferedWindows);
        ~sdl_renderer();
    public:
        void initialize() override;
        void cleanup() override;
    public:
        bool double_buffering() const override;
        const i_render_target* active_target() const override;
        void activate_context(const i_render_target& aTarget) override;
        void deactivate_context() override;
        handle create_context(const i_render_target& aTarget) override;
        void destroy_context(handle aContext) override;
        std::unique_ptr<i_native_window> create_window(i_surface_manager& aSurfaceManager, i_surface_window& aWindow, const video_mode& aVideoMode, const std::string& aWindowTitle, window_style aStyle) override;
        std::unique_ptr<i_native_window> create_window(i_surface_manager& aSurfaceManager, i_surface_window& aWindow, const size& aDimensions, const std::string& aWindowTitle, window_style aStyle) override;
        std::unique_ptr<i_native_window> create_window(i_surface_manager& aSurfaceManager, i_surface_window& aWindow, const point& aPosition, const size& aDimensions, const std::string& aWindowTitle, window_style aStyle) override;
        std::unique_ptr<i_native_window> create_window(i_surface_manager& aSurfaceManager, i_surface_window& aWindow, i_native_surface& aParent, const video_mode& aVideoMode, const std::string& aWindowTitle, window_style aStyle) override;
        std::unique_ptr<i_native_window> create_window(i_surface_manager& aSurfaceManager, i_surface_window& aWindow, i_native_surface& aParent, const size& aDimensions, const std::string& aWindowTitle, window_style aStyle) override;
        std::unique_ptr<i_native_window> create_window(i_surface_manager& aSurfaceManager, i_surface_window& aWindow, i_native_surface& aParent, const point& aPosition, const size& aDimensions, const std::string& aWindowTitle, window_style aStyle) override;
        bool creating_window() const override;
    public:
        void render_now() override;
        bool use_rendering_priority() const override;
    public:
        virtual bool process_events();
    private:
        handle create_context(void* aNativeSurfaceHandle);
        std::shared_ptr<offscreen_window> allocate_offscreen_window(const i_render_target* aRenderTarget);
        void deallocate_offscreen_window(const i_render_target* aRenderTarget);
        void activate_current_target();
        static int filter_event(void* aSelf, SDL_Event* aEvent);
        bool queue_events();
    private:
        bool iInitialized;
        bool iDoubleBuffering;
        std::vector<std::shared_ptr<offscreen_window>> iOffscreenWindowPool;
        std::unordered_map<const i_render_target*, std::shared_ptr<offscreen_window>> iOffscreenWindows;
        std::weak_ptr<offscreen_window> iDefaultOffscreenWindow;
        handle iContext;
        uint32_t iCreatingWindow;
        std::vector<const i_render_target*> iTargetStack;
    };
}