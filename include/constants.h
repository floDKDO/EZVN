#pragma once

#include "color.h"

#include <string_view>

//TODO : centrer les boutons du menu principal
//TODO : text_.update() partout

namespace constants
{

inline constexpr int window_width_ = 1280;
inline constexpr int window_height_ = 720;

inline constexpr int default_zorder_ = 0;
inline constexpr bool default_autofocus = false;

inline constexpr int text_outline_size_ = 1;
inline Color text_outline_color_ = Color::from_rgba8(255, 205, 230);

inline constexpr std::string_view button_normal_ = "img/gui/button_normal.png";
inline constexpr std::string_view button_selected_ = "img/gui/button_selected.png";
inline constexpr std::string_view button_clicked_ = "img/gui/button_clicked.png";
inline constexpr std::string_view button_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int button_text_size_ = 30;
inline const Color button_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr int checkboxgroup_y_spacing_ = 150;
inline constexpr int texttogglegroup_y_spacing_ = 60;

inline constexpr int checkablegroup_top_text_y_delta_ = -70;
inline constexpr std::string_view checkablegroup_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int checkablegroup_text_size_ = 40;
inline const Color checkablegroup_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr std::string_view checkbox_normal_ = "img/gui/checkbox_normal.png";
inline constexpr std::string_view checkbox_selected_ = "img/gui/checkbox_selected.png";
inline constexpr std::string_view checkbox_clicked_ = "img/gui/checkbox_clicked.png";
inline constexpr std::string_view checkbox_check_mark_ = "img/gui/check_mark.png";
inline constexpr std::string_view checkbox_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int checkbox_text_x_delta_ = 10;
inline constexpr int checkbox_text_size_ = 30;
inline const Color checkbox_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr int inputfield_container_width_ = 250;
inline constexpr int inputfield_container_height_ = 45;
inline const Color inputfield_container_color_ = Color::from_rgba8(255, 255, 255);
inline const Color inputfield_outline_normal_color_ = Color::from_rgba8(174, 84, 135);
inline const Color inputfield_outline_selected_color_ = Color::from_rgba8(221, 108, 172);
inline const Color inputfield_outline_clicked_color_ = Color::from_rgba8(255, 124, 198);
inline constexpr std::string_view inputfield_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int inputfield_text_x_delta_ = 7;
inline constexpr int inputfield_text_y_delta_ = 5;
inline constexpr int inputfield_text_size_ = 30;
inline const Color inputfield_text_color_ = Color::from_rgba8(0, 0, 0);
inline const Color inputfield_placeholder_text_color_ = Color::from_rgba8(204, 204, 204);
inline constexpr int inputfield_caret_blink_time_ = 300;
inline constexpr int inputfield_idle_time_ = 500;

inline constexpr std::string_view confirmationpopup_frame_ = "img/GUI/confirmation_pop_up.png";
inline constexpr int confirmationpopup_width_ = 411;
inline constexpr std::string_view confirmationpopup_background_ = "img/GUI/confirm.png";
inline constexpr std::string_view confirmationpopup_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int confirmationpopup_text_size_ = 20;
inline constexpr int confirmationpopup_text_x_delta_ = 30;
inline const Color confirmationpopup_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr int slider_container_width_ = 300;
inline constexpr int slider_container_height_ = 18;
inline const Color slider_container_color_ = Color::from_rgba8(255, 255, 255);
inline const Color slider_container_outline_color_ = Color::from_rgba8(174, 84, 135);
inline constexpr int slider_handle_size_ = 30;
inline const Color slider_handle_normal_color_ = Color::from_rgba8(174, 84, 135);
inline const Color slider_handle_selected_color_ = Color::from_rgba8(221, 108, 172);
inline const Color slider_handle_clicked_color_ = Color::from_rgba8(255, 124, 198);
inline constexpr int slider_handle_y_delta_ = -(slider_handle_size_ - slider_container_height_) / 2;
inline const Color slider_handle_outline_color_ = Color::from_rgba8(255, 255, 255);
inline constexpr std::string_view slider_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int slider_text_y_delta_ = -60;
inline constexpr int slider_text_size_ = 40;
inline const Color slider_text_color_ = Color::from_rgba8(174, 84, 135);
inline constexpr int slider_step_count_ = 10;

inline const Color textbutton_normal_color_ = Color::from_rgba8(174, 84, 135);
inline const Color textbutton_selected_color_ = Color::from_rgba8(210, 210, 210);
inline const Color textbutton_clicked_color_ = Color::from_rgba8(255, 255, 255);
inline constexpr std::string_view textbutton_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int textbutton_text_size_ = 30;

inline const Color texttoggle_normal_unchecked_color_ = Color::from_rgba8(174, 84, 135);
inline const Color texttoggle_normal_checked_color_ = Color::from_rgba8(255, 255, 255);
inline const Color texttoggle_selected_checked_color_ = Color::from_rgba8(230, 230, 230);
inline const Color texttoggle_selected_unchecked_color_ = Color::from_rgba8(210, 210, 210);
inline const Color texttoggle_clicked_checked_color_ = Color::from_rgba8(255, 255, 255);
inline const Color texttoggle_clicked_unchecked_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr std::string_view texttoggle_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int texttoggle_text_size_ = 30;

inline constexpr int textbox_y_delta_ = -6;
inline constexpr std::string_view textbox_image_ = "img/gui/textbox.png";
inline constexpr std::string_view textbox_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int textbox_text_size_ = 20;
inline const Color textbox_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr int textbox_ui_elements_text_size_ = 13;
inline constexpr int textbox_ui_elements_x_delta_ = 283;
inline constexpr int textbox_ui_elements_y_delta_ = -30;
inline constexpr int textbox_ui_elements_x_spacing_ = 9;

inline constexpr int textbox_text_x_delta_ = 30;
inline constexpr int textbox_text_y_delta_ = 25;

inline constexpr int textbox_end_dialogue_indicator_x_delta_ = -30;
inline constexpr int textbox_end_dialogue_indicator_y_delta_ = 115;
inline constexpr std::string_view textbox_end_dialogue_indicator_ = "img/gui/textbox_end_dialogue_indicator.png";

inline constexpr int namebox_textbox_x_delta_ = 37;
inline constexpr std::string_view namebox_image_ = "img/gui/namebox.png";
inline constexpr std::string_view namebox_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int namebox_text_size_ = 30;
inline const Color namebox_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr int buttongroup_y_spacing_ = 60; //= choice menu
inline constexpr int buttongroup_width_ = 400; //= choice menu

inline constexpr std::string_view window_icon_ = "img/gui/window_icon.png";
inline constexpr std::string_view game_name_ = "EZVN";

inline constexpr std::string_view sound_hover_ = "sounds/hover.ogg";
inline constexpr std::string_view sound_click_ = "sounds/click.ogg";

inline constexpr int fadeout_length_scroll_back_ = 1500; //ms

}