// text_edit.hpp
/*
neoGFX Resource Compiler
Copyright(C) 2019 Leigh Johnston

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
#include <neogfx/core/color.hpp>
#include <neogfx/tools/nrc/ui_element.hpp>

namespace neogfx::nrc
{
    class text_edit : public ui_element<>
    {
    public:
        text_edit(const i_ui_element_parser& aParser, i_ui_element& aParent, ui_element_type aElementType = ui_element_type::TextEdit) :
            ui_element<>{ aParser, aParent, aElementType }
        {
            add_data_names({ "tab_stop_hint", "text_color" });
        }
    public:
        const neolib::i_string& header() const override
        {
            static const neolib::string sHeader = "neogfx/gui/widget/text_edit.hpp";
            return sHeader;
        }
    public:
        void parse(const neolib::i_string& aName, const data_t& aData) override
        {
            if (aName == "tab_stop_hint")
                iTabStopHint = aData.get<neolib::i_string>();
            else if (aName == "text_color")
                iTextColor = get_color(aData);
            else if (aName == "background_color")
                iBackgroundColor = get_color(aData);
            else
                ui_element<>::parse(aName, aData);
        }
        void parse(const neolib::i_string& aName, const array_data_t& aData) override
        {
            if (aName == "text_color")
                iTextColor = get_color_or_gradient(aData);
            else if (aName == "background_color")
                iBackgroundColor = get_color_or_gradient(aData);
            else
                ui_element<>::parse(aName, aData);
        }
    protected:
        void emit() const override
        {
        }
        void emit_preamble() const override
        {
            if (type() == ui_element_type::TextEdit)
                emit("  text_edit %1%;\n", id());
            ui_element<>::emit_preamble();
        }
        void emit_ctor() const override
        {
            ui_element<>::emit_generic_ctor();
            ui_element<>::emit_ctor();
        }
        void emit_body() const override
        {
            ui_element<>::emit_body();
            if (iTabStopHint)
                emit("   %1%.set_tab_stop_hint(\"%2%\");\n", id(), *iTabStopHint);
            if (iTextColor || iBackgroundColor)
                emit("   text_edit::style %1%DefaultStyle;\n", id());
            if (iTextColor)
            {
                if (std::holds_alternative<color>(*iTextColor))
                    emit("   %1%DefaultStyle.set_text_color(color{ %2% });\n", id(), std::get<color>(*iTextColor));
                else
                    emit("   %1%DefaultStyle.set_text_color(gradient{ %2% });\n", id(), std::get<gradient>(*iTextColor));
            }
            if (iBackgroundColor)
            {
                if (std::holds_alternative<color>(*iBackgroundColor))
                    emit("   %1%DefaultStyle.set_background_color(color{ %2% });\n", id(), std::get<color>(*iBackgroundColor));
                else
                    emit("   %1%DefaultStyle.set_background_color(gradient{ %2% });\n", id(), std::get<gradient>(*iBackgroundColor));
            }
            if (iTextColor || iBackgroundColor)
                emit("   %1%.set_default_style(%1%DefaultStyle);\n", id());
        }
    protected:
        using ui_element<>::emit;
    private:
        std::optional<string> iTabStopHint;
        std::optional<color_or_gradient> iTextColor;
        std::optional<color_or_gradient> iBackgroundColor;
    };
}
