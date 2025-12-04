#pragma once

#include <SDL2/SDL.h>
#include <string>

//TODO : normaliser la taille des textes
//TODO : centrer les boutons du menu principal

namespace constants
{

inline constexpr int window_width_ = 1280;
inline constexpr int window_height_ = 720;

inline constexpr int text_outline_size_ = 1;
inline constexpr SDL_Color text_outline_color_ = {255, 205, 230, 255};

inline const std::string button_normal_ = "img/gui/button_normal.png";
inline const std::string button_selected_ = "img/gui/button_selected.png";
inline const std::string button_clicked_ = "img/gui/button_clicked.png";
inline const std::string button_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int button_text_size_ = 30;
inline constexpr SDL_Color button_text_color_ = {255, 255, 255, 255};

inline constexpr int checkboxgroup_y_spacing_ = 150;
inline constexpr int texttogglegroup_y_spacing_ = 100;

inline constexpr int checkablegroup_top_text_y_delta_ = -100;
inline const std::string checkablegroup_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int checkablegroup_text_size_ = 50;
inline constexpr SDL_Color checkablegroup_text_color_ = {255, 255, 255, 255};

inline const std::string checkbox_normal_ = "img/gui/checkbox_normal.png";
inline const std::string checkbox_selected_ = "img/gui/checkbox_selected.png";
inline const std::string checkbox_clicked_ = "img/gui/checkbox_clicked.png";
inline const std::string checkbox_check_mark_ = "img/gui/check_mark.png";
inline const std::string checkbox_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int checkbox_text_x_delta_ = 10;
inline constexpr int checkbox_text_size_ = 30;
inline constexpr SDL_Color checkbox_text_color_ = {255, 255, 255, 255};

inline const std::string inputfield_container_ = "img/gui/inputfield_container.png";
inline const std::string inputfield_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int inputfield_text_x_delta_ = 7;
inline constexpr int inputfield_text_size_ = 50;
inline constexpr SDL_Color inputfield_text_color_ = {0, 0, 0, 255};
inline constexpr SDL_Color inputfield_placeholder_text_color_ = {204, 204, 204, 255};

inline const std::string popupconfirmation_frame_ = "img/GUI/pop_up_confirmation.png";
inline constexpr int popupconfirmation_width_ = 411;
inline const std::string popupconfirmation_background = "img/GUI/confirm.png";
inline const std::string popupconfirmation_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int popupconfirmation_text_size_ = 20;
inline constexpr int popupconfirmation_text_x_delta_ = 30;
inline constexpr SDL_Color popupconfirmation_text_color_ = {255, 255, 255, 255};

inline const std::string slider_container_ = "img/gui/slider_container.png";
inline const std::string slider_handle_ = "img/gui/slider_handle.png";
inline constexpr int slider_handle_y_delta_ = -5;
inline const std::string slider_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int slider_text_y_delta_ = -70;
inline constexpr int slider_text_size_ = 50;
inline constexpr SDL_Color slider_text_color_ = {255, 255, 255, 255};

inline constexpr SDL_Color textbutton_normal_color_ = {255, 255, 255, 255};
inline constexpr SDL_Color textbutton_selected_color_ = {255, 0, 0, 255};
inline constexpr SDL_Color textbutton_clicked_color_ = {255, 0, 0, 255};
inline const std::string textbutton_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int textbutton_text_size_ = 30;
inline constexpr SDL_Color textbutton_text_color_ = {255, 255, 255, 255};

inline constexpr SDL_Color texttoggle_unchecked_color_ = {255, 255, 255, 255};
inline constexpr SDL_Color texttoggle_selected_color_ = {255, 0, 0, 255};
inline constexpr SDL_Color texttoggle_checked_color_ = {255, 0, 0, 255};
inline const std::string texttoggle_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int texttoggle_text_size_ = 50;

inline constexpr int textbox_y_delta_ = -6;
inline const std::string textbox_image_ = "img/gui/textbox.png";
inline const std::string textbox_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int textbox_text_size_ = 20;
inline constexpr SDL_Color textbox_text_color_ = {255, 255, 255, 255};

inline constexpr int textbox_textbuttons_text_size_ = 13;
inline constexpr int textbox_textbuttons_x_delta_ = 283;
inline constexpr int textbox_textbuttons_y_delta_ = -28;
inline constexpr int textbox_textbuttons_x_spacing_ = 9;

inline constexpr int textbox_text_x_delta_ = 30;
inline constexpr int textbox_text_y_delta_ = 25;

inline constexpr int textbox_end_dialogue_indicator_x_delta_ = -30;
inline constexpr int textbox_end_dialogue_indicator_y_delta_ = 115;
inline const std::string textbox_end_dialogue_indicator_ = "img/gui/textbox_end_dialogue_indicator.png";

inline constexpr int namebox_textbox_x_delta_ = 37;
inline const std::string namebox_image_ = "img/gui/namebox.png";
inline const std::string namebox_font_ = "fonts/Aller_Rg.ttf";
inline constexpr int namebox_text_size_ = 30;
inline constexpr SDL_Color namebox_text_color_ = {255, 255, 255, 255};

inline constexpr int buttongroup_y_spacing_ = 60; //= choice menu
inline constexpr int buttongroup_width_ = 400; //= choice menu

inline const std::string window_icon_ = "img/gui/window_icon.png"; 
inline const std::string game_name_ = "EZVN";

inline const std::string sound_hover_ = "sounds/hover.ogg";
inline const std::string sound_click_ = "sounds/click.ogg";

}