// text_widget.cpp
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

#include <neogfx/neogfx.hpp>
#include <neogfx/app/i_app.hpp>
#include <neogfx/gfx/graphics_context.hpp>
#include <neogfx/gui/layout/i_layout.hpp>
#include <neogfx/gui/widget/text_widget.hpp>

namespace neogfx
{
    text_widget::text_widget(const std::string& aText, text_widget_type aType, text_widget_flags aFlags) :
        widget{}, iText{ aText }, iType{ aType }, iFlags{ aFlags }, iAlignment { neogfx::alignment::Centre | neogfx::alignment::VCentre }
    {
        init();
    }

    text_widget::text_widget(i_widget& aParent, const std::string& aText, text_widget_type aType, text_widget_flags aFlags) :
        widget{ aParent }, iText{ aText }, iType{ aType }, iFlags{ aFlags }, iAlignment{ neogfx::alignment::Centre | neogfx::alignment::VCentre }
    {
        init();
    }

    text_widget::text_widget(i_layout& aLayout, const std::string& aText, text_widget_type aType, text_widget_flags aFlags) :
        widget{ aLayout }, iText{ aText }, iType{ aType }, iFlags{ aFlags }, iAlignment{ neogfx::alignment::Centre | neogfx::alignment::VCentre }
    {
        init();
    }

    text_widget::~text_widget()
    {
    }

    neogfx::size_policy text_widget::size_policy() const
    {
        if (widget::has_size_policy())
            return widget::size_policy();
        return size_constraint::Minimum;
    }

    size text_widget::minimum_size(const optional_size& aAvailableSpace) const
    {
        if (widget::has_minimum_size())
            return widget::minimum_size(aAvailableSpace);
        else
        {
            size extent = units_converter(*this).to_device_units(text_extent().max(size_hint_extent()));
            size result = extent + units_converter(*this).to_device_units(margins().size());
            if (has_maximum_size())
            {
                result.cx = std::min(std::ceil(result.cx), maximum_size().cx);
                result.cy = std::min(std::ceil(result.cy), maximum_size().cy);
            }
            if (result.cx == 0.0 && (flags() & text_widget_flags::TakesSpaceWhenEmpty) == text_widget_flags::TakesSpaceWhenEmpty)
                result.cx = 1.0;
            return units_converter(*this).from_device_units(result);
        }
    }

    void text_widget::paint(i_graphics_context& aGraphicsContext) const
    {
        scoped_mnemonics sm(aGraphicsContext, service<i_keyboard>().is_key_pressed(ScanCode_LALT) || service<i_keyboard>().is_key_pressed(ScanCode_RALT));
        size textSize = text_extent();
        point textPosition;
        switch (iAlignment & neogfx::alignment::Horizontal)
        {
        case neogfx::alignment::Left:
        case neogfx::alignment::Justify:
            textPosition.x = 0.0;
            break;
        case neogfx::alignment::Centre:
            textPosition.x = std::floor((client_rect().width() - textSize.cx) / 2.0);
            break;
        case neogfx::alignment::Right:
            textPosition.x = std::floor((client_rect().width() - textSize.cx));
            break;
        default:
            break;
        }
        switch (iAlignment & neogfx::alignment::Vertical)
        {
        case neogfx::alignment::Top:
            textPosition.y = 0.0;
            break;
        case neogfx::alignment::VCentre:
            textPosition.y = std::floor((client_rect().height() - textSize.cy) / 2.0);
            break;
        case neogfx::alignment::Bottom:
            textPosition.y = std::floor((client_rect().height() - textSize.cy));
            break;
        default:
            break;
        }
        auto appearance = text_appearance();
        if (effectively_disabled())
            appearance = appearance.with_alpha(static_cast<color::component>(appearance.ink().alpha() * 0.25));
        if (appearance.effect() != std::nullopt)
            textPosition += size{ appearance.effect()->width() };
        if (multi_line())
            aGraphicsContext.draw_multiline_glyph_text(textPosition, glyph_text(), textSize.cx, appearance, iAlignment & neogfx::alignment::Horizontal);
        else
            aGraphicsContext.draw_glyph_text(textPosition, glyph_text(), appearance);
    }

    void text_widget::set_font(const optional_font& aFont)
    {
        widget::set_font(aFont);
        iTextExtent = std::nullopt;
        iSizeHintExtent = std::nullopt;
        iGlyphText = neogfx::glyph_text{};
    }

    bool text_widget::visible() const
    {
        if (iText.empty() && (iFlags & text_widget_flags::HideOnEmpty) == text_widget_flags::HideOnEmpty)
            return false;
        return widget::visible();
    }

    const std::string& text_widget::text() const
    {
        return iText;
    }

    void text_widget::set_text(const std::string& aText)
    {
        if (iText != aText)
        {
            size oldSize = minimum_size();
            iText = aText;
            iTextExtent = std::nullopt;
            iGlyphText = neogfx::glyph_text{};
            TextChanged.trigger();
            if (has_parent_layout())
                parent_layout().invalidate();
            if (oldSize != minimum_size())
            {
                TextGeometryChanged.trigger();
                if (has_managing_layout())
                    managing_layout().layout_items();
            }
            update();
        }
    }

    void text_widget::set_size_hint(const size_hint& aSizeHint)
    {
        if (iSizeHint != aSizeHint)
        {
            size oldSize = minimum_size();
            iSizeHint = aSizeHint;
            iSizeHintExtent = std::nullopt;
            if (has_parent_layout())
                parent_layout().invalidate();
            if (oldSize != minimum_size() && has_managing_layout())
                managing_layout().layout_items();
        }
    }

    bool text_widget::multi_line() const
    {
        return iType == text_widget_type::MultiLine;
    }

    text_widget_flags text_widget::flags() const
    {
        return iFlags;
    }
    
    void text_widget::set_flags(text_widget_flags aFlags)
    {
        iFlags = aFlags;
    }

    neogfx::alignment text_widget::alignment() const
    {
        return iAlignment;
    }

    void text_widget::set_alignment(neogfx::alignment aAlignment, bool aUpdateLayout)
    {
        if (iAlignment != aAlignment)
        {
            iAlignment = aAlignment;
            if (aUpdateLayout)
                layout_root(true);
        }
    }

    bool text_widget::has_text_color() const
    {
        return has_text_appearance() && text_appearance().ink() != neolib::none && std::holds_alternative<color>(text_appearance().ink());
    }

    color text_widget::text_color() const
    {
        if (has_text_color())
            return static_variant_cast<color>(iTextAppearance->ink());
        return service<i_app>().current_style().palette().text_color_for_widget(*this);
    }

    void text_widget::set_text_color(const optional_color& aTextColor)
    {
        if (has_text_appearance())
            set_text_appearance(neogfx::text_appearance{ aTextColor != std::nullopt ? *aTextColor : neogfx::text_color{}, iTextAppearance->paper(), iTextAppearance->effect() });
        else
            set_text_appearance(neogfx::text_appearance{ aTextColor != std::nullopt ? *aTextColor : neogfx::text_color{} });
    }

    bool text_widget::has_text_appearance() const
    {
        return iTextAppearance != std::nullopt;
    }

    text_appearance text_widget::text_appearance() const
    {
        if (has_text_appearance())
            return *iTextAppearance;
        return neogfx::text_appearance{ text_color() };
    }

    void text_widget::set_text_appearance(const optional_text_appearance& aTextAppearance)
    {
        iTextAppearance = aTextAppearance;
        update();
    }

    size text_widget::text_extent() const
    {
        if (iTextExtent != std::nullopt)
            return *iTextExtent;
        else if (!has_surface())
            return size{};
        graphics_context gc{ *this, graphics_context::type::Unattached };
        scoped_mnemonics sm{ gc, service<i_keyboard>().is_key_pressed(ScanCode_LALT) || service<i_keyboard>().is_key_pressed(ScanCode_RALT) };
        if (multi_line())
        {
            if (widget::has_minimum_size() && widget::minimum_size().cx != 0 && widget::minimum_size().cy == 0)
                iTextExtent = gc.multiline_glyph_text_extent(glyph_text(), widget::minimum_size().cx - margins().size().cx);
            else if (widget::has_maximum_size() && widget::maximum_size().cx != size::max_dimension())
                iTextExtent = gc.multiline_glyph_text_extent(glyph_text(), widget::maximum_size().cx - margins().size().cx);
            else
                iTextExtent = gc.multiline_glyph_text_extent(glyph_text(), 0);
        }
        else
            iTextExtent = gc.glyph_text_extent(glyph_text());
        if (iTextExtent->cy == 0.0)
            iTextExtent->cy = font().height();
        if (has_text_appearance() && text_appearance().effect() != std::nullopt)
            *iTextExtent += size{ text_appearance().effect().value().width() * 2.0 };
        return *iTextExtent;
    }

    size text_widget::size_hint_extent() const
    {
        if (iSizeHintExtent != std::nullopt)
            return *iSizeHintExtent;
        else if (!has_surface())
            return size{};
        else
        {
            graphics_context gc{ *this, graphics_context::type::Unattached };
            scoped_mnemonics sm{ gc, service<i_keyboard>().is_key_pressed(ScanCode_LALT) || service<i_keyboard>().is_key_pressed(ScanCode_RALT) };
            if (multi_line())
            {
                if (widget::has_minimum_size() && widget::minimum_size().cx != 0 && widget::minimum_size().cy == 0)
                    iSizeHintExtent = gc.multiline_text_extent(iSizeHint.primaryHint, font(), widget::minimum_size().cx - margins().size().cx).max(
                        gc.multiline_text_extent(iSizeHint.secondaryHint, font(), widget::minimum_size().cx - margins().size().cx));
                else if (widget::has_maximum_size() && widget::maximum_size().cx != size::max_dimension())
                    iSizeHintExtent = gc.multiline_text_extent(iSizeHint.primaryHint, font(), widget::maximum_size().cx - margins().size().cx).max(
                        gc.multiline_text_extent(iSizeHint.secondaryHint, font(), widget::maximum_size().cx - margins().size().cx));
                else
                    iSizeHintExtent = gc.multiline_text_extent(iSizeHint.primaryHint, font()).max(
                        gc.multiline_text_extent(iSizeHint.secondaryHint, font()));
            }
            else
                iSizeHintExtent = gc.text_extent(iSizeHint.primaryHint, font()).max(
                    gc.text_extent(iSizeHint.secondaryHint, font()));
        }
        if (iSizeHintExtent->cy == 0.0)
            iSizeHintExtent->cy = font().height();
        return *iSizeHintExtent;
    }

    void text_widget::init()
    {
        set_margins(neogfx::margins{ 0.0 });
        set_ignore_mouse_events(true);
        iSink += service<i_app>().current_style_changed([this](style_aspect aAspect)
        {
            if (!has_font() && (aAspect & style_aspect::Font) == style_aspect::Font)
            {
                iTextExtent = std::nullopt;
                iSizeHintExtent = std::nullopt;
                iGlyphText = neogfx::glyph_text{};
                if (has_parent_layout())
                    parent_layout().invalidate();
                update();
            }
        });
        iSink += service<i_rendering_engine>().subpixel_rendering_changed([this]()
        {
            iTextExtent = std::nullopt;
            iSizeHintExtent = std::nullopt;
            iGlyphText = neogfx::glyph_text{};
            if (has_parent_layout())
                parent_layout().invalidate();
            update();
        });
    }

    const neogfx::glyph_text& text_widget::glyph_text() const
    {
        if (iGlyphText.empty())
        {
            graphics_context gc{ *this, graphics_context::type::Unattached };
            scoped_mnemonics sm(gc, service<i_keyboard>().is_key_pressed(ScanCode_LALT) || service<i_keyboard>().is_key_pressed(ScanCode_RALT));
            iGlyphText = gc.to_glyph_text(iText, font());
        }
        return iGlyphText;
    }
}