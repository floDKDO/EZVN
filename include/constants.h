#pragma once

#include "Color/color.h"

#include <string_view>

namespace constants
{

inline constexpr int window_width_ = 1280;
inline constexpr int window_height_ = 720;

inline constexpr std::string_view persistent_variables_file_ = "persistent_variables.json";

inline constexpr unsigned int default_zorder_ = 0;
inline constexpr bool default_autofocus_ = false;
inline constexpr bool default_autozorder_ = false;
inline constexpr int default_minimum_character_for_autofocus_ = 2;
inline constexpr std::string_view default_textbox_position_ = "bottom";
inline constexpr std::string_view default_menu_background_ = "resources/img/backgrounds/night.png";
inline constexpr std::string_view default_narrator_name_ = "Narrator";
inline constexpr int default_fade_transition_length_ = 500; //ms

inline constexpr int text_outline_size_ = 1;
inline Color text_outline_color_ = Color::from_rgba8(255, 205, 230);

inline constexpr std::string_view button_normal_ = "resources/img/gui/button_normal.png";
inline constexpr std::string_view button_selected_ = "resources/img/gui/button_selected.png";
inline constexpr std::string_view button_pressed_ = "resources/img/gui/button_pressed.png";
inline constexpr std::string_view choice_button_normal_ = "resources/img/gui/choice_button_normal.png";
inline constexpr std::string_view choice_button_selected_ = "resources/img/gui/choice_button_selected.png";
inline constexpr std::string_view choice_button_pressed_ = "resources/img/gui/choice_button_selected.png"; //identique à selected
inline constexpr std::string_view button_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int button_text_size_ = 30;
inline constexpr int choice_button_text_size_ = 25;
inline const Color button_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr int checkboxgroup_y_spacing_ = 150;
inline constexpr int texttogglegroup_y_spacing_ = 60;

//TODO : inutilisés
/*inline constexpr int checkablegroup_top_text_y_delta_ = -70;
inline constexpr std::string_view checkablegroup_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int checkablegroup_text_size_ = 40;
inline const Color checkablegroup_text_color_ = Color::from_rgba8(255, 255, 255);*/

inline constexpr std::string_view checkbox_normal_ = "resources/img/gui/checkbox_normal.png";
inline constexpr std::string_view checkbox_selected_ = "resources/img/gui/checkbox_selected.png";
inline constexpr std::string_view checkbox_pressed_ = "resources/img/gui/checkbox_pressed.png";
inline constexpr std::string_view checkbox_check_mark_ = "resources/img/gui/check_mark.png";
inline constexpr std::string_view checkbox_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int checkbox_text_x_delta_ = 10;
inline constexpr int checkbox_text_size_ = 30;
inline const Color checkbox_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr int inputfield_container_width_ = 250;
inline constexpr int inputfield_container_height_ = 45;
inline const Color inputfield_container_color_ = Color::from_rgba8(255, 255, 255);
inline const Color inputfield_outline_normal_color_ = Color::from_rgba8(174, 84, 135);
inline const Color inputfield_outline_selected_color_ = Color::from_rgba8(255, 147, 210);
inline const Color inputfield_outline_pressed_color_ = Color::from_rgba8(255, 191, 228);
inline constexpr std::string_view inputfield_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int inputfield_text_x_delta_ = 7;
inline constexpr int inputfield_text_y_delta_ = 5;
inline constexpr int inputfield_text_size_ = 30;
inline const Color inputfield_text_color_ = Color::from_rgba8(0, 0, 0);
inline const Color inputfield_placeholder_text_color_ = Color::from_rgba8(204, 204, 204);
inline constexpr int inputfield_caret_blink_time_ = 300;
inline constexpr int inputfield_idle_time_ = 500;

inline constexpr std::string_view confirmationpopup_frame_ = "resources/img/gui/confirmation_pop_up.png";
inline constexpr int confirmationpopup_width_ = 411;
inline constexpr std::string_view confirmationpopup_background_ = "resources/img/gui/confirm.png";
inline constexpr std::string_view confirmationpopup_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int confirmationpopup_text_size_ = 22;
inline constexpr int confirmationpopup_text_y_delta_ = 40;
inline constexpr int confirmationpopup_buttons_x_delta_ = 120;
inline constexpr int confirmationpopup_buttons_y_delta_ = 100;
inline const Color confirmationpopup_text_color_ = Color::from_rgba8(255, 255, 255);
inline constexpr std::string_view confirmationpopup_message_ = "Are you sure you want to quit?";

inline constexpr int slider_container_width_ = 300;
inline constexpr int slider_container_height_ = 18;
inline const Color slider_container_color_ = Color::from_rgba8(255, 255, 255);
inline const Color slider_container_outline_color_ = Color::from_rgba8(174, 84, 135);
inline constexpr int slider_handle_size_ = 30;
inline const Color slider_handle_normal_color_ = Color::from_rgba8(174, 84, 135);
inline const Color slider_handle_selected_color_ = Color::from_rgba8(255, 147, 210);
inline const Color slider_handle_pressed_color_ = Color::from_rgba8(255, 191, 228);
inline constexpr int slider_handle_y_delta_ = -(slider_handle_size_ - slider_container_height_) / 2;
inline const Color slider_handle_outline_color_ = Color::from_rgba8(255, 255, 255);
inline constexpr std::string_view slider_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int slider_text_y_delta_ = -60;
inline constexpr int slider_text_size_ = 40;
inline const Color slider_text_color_ = Color::from_rgba8(174, 84, 135);
inline constexpr int slider_step_count_ = 10;

inline constexpr std::string_view scrollbar_up_arrow_normal_ = "resources/img/gui/triangle_scrollbar_normal.png";
inline constexpr std::string_view scrollbar_up_arrow_selected_ = "resources/img/gui/triangle_scrollbar_selected.png";
inline constexpr std::string_view scrollbar_up_arrow_pressed_ = "resources/img/gui/triangle_scrollbar_selected.png"; //identique à selected
inline constexpr std::string_view scrollbar_down_arrow_normal_ = "resources/img/gui/triangle_scrollbar_reversed_normal.png";
inline constexpr std::string_view scrollbar_down_arrow_selected_ = "resources/img/gui/triangle_scrollbar_reversed_selected.png";
inline constexpr std::string_view scrollbar_down_arrow_pressed_ = "resources/img/gui/triangle_scrollbar_reversed_selected.png"; //identique à selected
inline constexpr int scrollbar_container_width_ = 18;
inline constexpr int scrollbar_handle_width_ = scrollbar_container_width_ - 2;
inline const Color scrollbar_container_color_ = Color::from_rgba8(255, 255, 255);
inline const Color scrollbar_container_outline_color_ = Color::from_rgba8(174, 84, 135);
inline const Color scrollbar_handle_normal_color_ = Color::from_rgba8(174, 84, 135);
inline const Color scrollbar_handle_selected_color_ = Color::from_rgba8(255, 147, 210);
inline const Color scrollbar_handle_pressed_color_ = Color::from_rgba8(255, 191, 228);
inline const Color scrollbar_handle_outline_color_ = Color::from_rgba8(255, 255, 255);
inline constexpr int scrollbar_step_count_ = 10;

inline const Color textbutton_normal_color_ = Color::from_rgba8(174, 84, 135);
inline const Color textbutton_selected_color_ = Color::from_rgba8(255, 147, 210);
inline const Color textbutton_pressed_color_ = Color::from_rgba8(255, 191, 228);
inline constexpr std::string_view textbutton_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int textbutton_text_size_ = 30;

inline const Color texttoggle_normal_unchecked_color_ = Color::from_rgba8(174, 84, 135);
inline const Color texttoggle_normal_checked_color_ = Color::from_rgba8(255, 255, 255);
inline const Color texttoggle_selected_checked_color_ = Color::from_rgba8(255, 191, 228);
inline const Color texttoggle_selected_unchecked_color_ = Color::from_rgba8(255, 147, 210);
inline const Color texttoggle_pressed_checked_color_ = Color::from_rgba8(255, 178, 223);
inline const Color texttoggle_pressed_unchecked_color_ = Color::from_rgba8(255, 178, 223);

inline constexpr std::string_view texttoggle_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int texttoggle_text_size_ = 30;

inline constexpr int textbox_y_delta_ = -6;
inline constexpr std::string_view textbox_image_ = "resources/img/gui/textbox.png";
inline constexpr std::string_view textbox_font_ = "resources/fonts/Aller_Rg.ttf";
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
inline constexpr std::string_view textbox_end_dialogue_indicator_ = "resources/img/gui/textbox_end_dialogue_indicator.png";

inline constexpr int ui_group_y_spacing_at_top_ = 80;
inline constexpr int ui_group_spacing_between_ui_ = 20;

inline constexpr int namebox_textbox_x_delta_ = 37;
inline constexpr std::string_view namebox_image_ = "resources/img/gui/namebox.png";
inline constexpr std::string_view namebox_font_ = "resources/fonts/Aller_Rg.ttf";
inline constexpr int namebox_text_size_ = 30;
inline const Color namebox_text_color_ = Color::from_rgba8(255, 255, 255);

inline constexpr int choice_menu_y_delta_ = 180; //from the top of the screen
inline constexpr int buttongroup_y_spacing_ = 60; //= choice menu
inline constexpr int buttongroup_width_ = 400; //= choice menu

inline constexpr int history_menu_y_spacing_between_dialogues_ = 20;
inline constexpr int history_menu_y_spacing_at_top_ = 20;
inline constexpr int history_menu_x_spacing_character_name_ = 380;
inline constexpr int history_menu_x_spacing_dialogue_ = 500;

inline constexpr std::string_view window_icon_ = "resources/img/gui/window_icon.png";
inline constexpr std::string_view game_name_ = "EZVN";

inline constexpr std::string_view ui_sound_select_ = "resources/sounds/ui_select.ogg";
inline constexpr std::string_view ui_sound_press_ = "resources/sounds/ui_press.ogg";

inline constexpr int fadeout_length_scroll_back_ = 1500; //ms

inline constexpr std::string_view main_menu_unique_id_ = "Main";
inline constexpr std::string_view settings_menu_unique_id_ = "Settings";
inline constexpr std::string_view ingame_unique_id_ = "InGame";
inline constexpr std::string_view history_menu_unique_id_ = "History";

}