#include "ingame.h"
#include "transform.h"
#include "constants.h"
#include "game.h"
#include "RAII_SDL2/channel.h"

#include <iostream>

//TODO : éviter de répéter plein de fois les mêmes boucles for
//TODO : le code des dialogues devra être modifié quand il y aura l'ajout de pauses, animations d'images, choice menus etc.

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game), init_(false), /*should_continue_advancing_(false),*/ /*current_dialogue_index_(size_t(1)),*/ character_manager_(renderer), which_dialogue_from_where_({Script::Where::none, false, false}),
	skip_mode_(false), auto_mode_(false), autofocus_(constants::default_autofocus), last_time_(0), background_(Color::from_rgba8(0, 0, 0)), textbox_(renderer), hide_ui_textbox_(false), currently_playing_sound_({{}, 0, false, nullptr}), currently_playing_music_({{}, nullptr}),
	/*background_changed_(false), autofocus_changed_(false), music_changed_(false), sound_changed_(false),*/ renderer_(renderer)
{
	build_ui_elements(renderer); 
	character_manager_.create_narrator();
}

void InGame::build_ui_elements(sdl::Renderer& renderer)
{
	std::unique_ptr<Ui> history_ui = std::make_unique<TextButton>("History", 0, 0, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX);
	history_button_ = dynamic_cast<TextButton*>(history_ui.get());
	ui_manager_.add_element(std::move(history_ui));

	std::unique_ptr<Ui> skip_ui = std::make_unique<TextToggle>("Skip", 0, 0, false, renderer, std::bind(&InGame::skip_function, this, std::placeholders::_1), TextToggleKind::ON_TEXTBOX);
	skip_toggle_ = dynamic_cast<TextToggle*>(skip_ui.get());
	ui_manager_.add_element(std::move(skip_ui));

	std::unique_ptr<Ui> auto_ui = std::make_unique<TextToggle>("Auto", 0, 0, false, renderer, std::bind(&InGame::auto_function, this, std::placeholders::_1), TextToggleKind::ON_TEXTBOX);
	auto_toggle_ = dynamic_cast<TextToggle*>(auto_ui.get());
	ui_manager_.add_element(std::move(auto_ui));

	std::unique_ptr<Ui> save_ui = std::make_unique<TextButton>("Save", 0, 0, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX);
	save_button_ = dynamic_cast<TextButton*>(save_ui.get());
	ui_manager_.add_element(std::move(save_ui));

	std::unique_ptr<Ui> load_ui = std::make_unique<TextButton>("Load", 0, 0, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX);
	load_button_ = dynamic_cast<TextButton*>(load_ui.get());
	ui_manager_.add_element(std::move(load_ui));

	std::unique_ptr<Ui> settings_ui = std::make_unique<TextButton>("Settings", 0, 0, renderer, std::bind(&InGame::settings_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX);
	settings_button_ = dynamic_cast<TextButton*>(settings_ui.get());
	ui_manager_.add_element(std::move(settings_ui));

	set_position_ui_textbox("bottom"); //TODO : hardcodé

	ui_manager_.set_elements();
}

void InGame::set_position_ui_textbox(std::string_view where)
{
	textbox_.set_textbox_position(where);

	int x_textbutton = textbox_.textbox_.position_.x + constants::textbox_ui_elements_x_delta_;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h + constants::textbox_ui_elements_y_delta_;

	history_button_->text_.position_.x = x_textbutton;
	history_button_->text_.position_.y = y_textbutton;
	x_textbutton += history_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	skip_toggle_->text_.position_.x = x_textbutton;
	skip_toggle_->text_.position_.y = y_textbutton;
	x_textbutton += skip_toggle_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	auto_toggle_->text_.position_.x = x_textbutton;
	auto_toggle_->text_.position_.y = y_textbutton;
	x_textbutton += auto_toggle_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	save_button_->text_.position_.x = x_textbutton;
	save_button_->text_.position_.y = y_textbutton;
	x_textbutton += save_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	load_button_->text_.position_.x = x_textbutton;
	load_button_->text_.position_.y = y_textbutton;
	x_textbutton += load_button_->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;

	settings_button_->text_.position_.x = x_textbutton;
	settings_button_->text_.position_.y = y_textbutton;
}

//Fonctions de callback///////////////////////////////////////////
void InGame::auto_function([[maybe_unused]] Ui* ui)
{
	auto_mode_ = !auto_mode_;
}

void InGame::skip_function([[maybe_unused]] Ui* ui)
{
	skip_mode_ = !skip_mode_;
}

void InGame::settings_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed Settings!" << std::endl;
	game_.request_push_state(constants::settings_menu_unique_id_);
}

void InGame::temp_function([[maybe_unused]] Ui* ui)
{
	std::cout << "Pressed!\n";
}
//////////////////////////////////////////////////////////////////

void InGame::handle_events(const SDL_Event& e)
{
	if(!hide_ui_textbox_)
	{
		//Dialogues
		if(e.type == SDL_MOUSEWHEEL) //condition placée en premier pour que le scroll de la mouse wheel sur un textbutton fonctionne
		{
			if(e.wheel.y > 0) //scroll vers l'avant => reculer d'un dialogue
			{
				//annuler le mode skip
				skip_toggle_->change_checked(false);
				skip_mode_ = false;

				which_dialogue_from_where_ = {Script::Where::prev, true, false};
			}
			else //scroll vers l'arrière => avancer d'un dialogue
			{
				which_dialogue_from_where_ = {Script::Where::next, true, false};
			}
		}

		ui_manager_.handle_events(e);
		if(ui_manager_.is_mouse_on_ui_)
		{
			return; //si collision avec un textbutton, ne pas gérer les événements "clic" et "espace" de la Textbox (= ne pas passer au prochain dialogue)
		}

		//Dialogues
		if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
		|| (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT))
		{
			//annuler le mode auto
			auto_toggle_->change_checked(false);
			auto_mode_ = false;

			if(textbox_.text_.is_finished_)
			{
				which_dialogue_from_where_ = {Script::Where::next, false, true};
			}
			else
			{
				textbox_.text_.is_animated_ = false; //afficher le dialogue en entier après un clic / touche espace sur un dialogue en train de s'afficher
			}
		}
	}

	if(e.type == SDL_MOUSEBUTTONDOWN && (e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE))
	{
		hide_ui_textbox_ = !hide_ui_textbox_;
	}

	if(e.type == AudioManager::END_MUSIC_EVENT_)
	{
		if(currently_playing_music_.music_ != nullptr)
		{
			game_.audio_manager_.fade_in_music(*currently_playing_music_.music_, currently_playing_music_.audio_properties_.loop_, currently_playing_music_.audio_properties_.fadein_length_);
		}
	}
}

void InGame::draw(sdl::Renderer& renderer)
{
	//Backgrounds
	background_.draw(renderer);
	///////////////////////////

	if(game_.script_.script_information_.size() > 0)
	{
		character_manager_.draw(renderer);
	}

	if(!hide_ui_textbox_)
	{
		textbox_.draw(renderer);
		ui_manager_.draw(renderer);
	}
}

void InGame::update_backgrounds(const Script::InfoBackground& info_background)
{
	//Backgrounds
	if(info_background.image_ != nullptr)
	{
		if(background_.image_ != nullptr)
		{
			background_.image_->init_image(info_background.image_->path_, 0, 0, renderer_);
		}
		else
		{
			background_ = Background(info_background.image_->path_, renderer_);
		}
	}
	else
	{
		background_.image_.reset();
		background_.color_ = info_background.color_;
	}
}

void InGame::update_autofocus(const Script::InfoAutofocus& info_autofocus)
{
	////if(!autofocus_changed_)
	//{
	//	if(info_autofocus || constants::default_autofocus)
	//	{
	//		/*for(auto& [key_character_variable, value_character] : active_characters_)
	//		{
	//			if(value_character.character_definition_->character_variable_ == textbox_.)
	//			{
	//				value_character.properties_.is_speaking_ = true;
	//				value_character.properties_.zorder_ = 10;
	//			}
	//			else
	//			{
	//				value_character.properties_.is_speaking_ = false;
	//				value_character.properties_.zorder_ = 5;
	//			}
	//		}*/
	//	}
	//	//autofocus_changed_ = true;
	//}
	autofocus_ = info_autofocus;
}

void InGame::update_skip_auto_modes()
{
	//Dialogues//////////////////////////////////////////////////////////////////////////////////////
	if(skip_mode_)
	{
		which_dialogue_from_where_ = {Script::Where::next, false, false};
		//show_next_dialogue(); 
	}

	//Dialogues
	if(auto_mode_)
	{
		if(textbox_.text_.is_finished_)
		{
			Uint64 now = SDL_GetTicks64();
			if(last_time_ == 0)
			{
				last_time_ = now;
			}

			if(now > last_time_ + textbox_.get_text_delay())
			{
				which_dialogue_from_where_ = {Script::Where::next, false, true};
				//show_next_dialogue();

				last_time_ = SDL_GetTicks64();
			}
		}
	}
}

void InGame::update_music(Script::InfoMusic& info_music)
{
	//Musics
	const Script::AudioProperties& music_properties = info_music.first;
	if(info_music.second.has_value()) //play music
	{
		Music& music = info_music.second.value();
		if(!sdl::music::playing())
		{
			game_.audio_manager_.fade_in_music(music, music_properties.loop_, music_properties.fadein_length_);
		}
		else if(currently_playing_music_.music_ != &music)
		{
			std::cout << "FADEOUT\n";
			game_.audio_manager_.fade_out_music(music_properties.fadeout_length_);
		}
		currently_playing_music_ = {music_properties, &music};
	}
	else //stop music
	{
		game_.audio_manager_.fade_out_music(music_properties.fadeout_length_);
		currently_playing_music_.music_ = nullptr;
	}
}

//Sounds
void InGame::halt_all_sounds()
{
	for(auto& s : game_.script_.script_information_)
	{
		if(std::holds_alternative<Script::InfoSound>(s))
		{
			Script::InfoSound& sound_pair = std::get<Script::InfoSound>(s);
			if(sound_pair.second.has_value())
			{
				Script::AudioProperties& sound_properties = sound_pair.first;
				if(sdl::channel::playing(sound_properties.channel_))
				{
					game_.audio_manager_.halt_channel(sound_properties.channel_);
				}
			}
		}
	}
}

//Sounds
void InGame::update_sounds(Script::InfoSound& info_sound, size_t i)
{
	//if(!sound_changed_)
	{
		Script::AudioProperties& sound_properties = info_sound.first;

		if(info_sound.second.has_value()) //play sound
		{
			Sound& sound = info_sound.second.value();

			if(currently_playing_sound_.sound_ == &sound)
			{
				if(!sdl::channel::playing(sound_properties.channel_)
				&& ((!sound_properties.loop_ && !currently_playing_sound_.played_) || sound_properties.loop_)) //ne pas rejouer un son qui a déjà été joué s'il ne doit pas être joué en boucle
				{
					//currently_playing_sound_ = {sound_properties, /*get_current_script_index()*/, true, &sound};
				}
				//sound_changed_ = true;
			}
			else
			{
				//nouveau son à jouer

				//cas d'un scroll arrière et un son était en train de se jouer => l'arrêter
				if(currently_playing_sound_.sound_ && currently_playing_sound_.associated_script_index_ > i) //TODO : remplacer i par current_script_index_ 
				{
					if(sdl::channel::playing(currently_playing_sound_.audio_properties_.channel_))
					{
						game_.audio_manager_.halt_channel(currently_playing_sound_.audio_properties_.channel_);
					}
				}
				else
				{
					if(sdl::channel::playing(sound_properties.channel_))
					{
						game_.audio_manager_.fade_out_sound(sound_properties.channel_, sound_properties.fadeout_length_);
					}
				}

				//TODO : remplacer i par current_script_index_ 
				//if(current_dialogue_index_ > 0 && i <= get_prev_script_index()) //on est au moins un dialogue avant le dialogue courant
				//{
				//	currently_playing_sound_ = {sound_properties, get_current_script_index(), false, nullptr};
				//}
				//else
				//{
				//	currently_playing_sound_ = {sound_properties, get_current_script_index(), false, &sound};
				//}
			}
		}
		else //stop sound
		{
			if(sdl::channel::playing(sound_properties.channel_))
			{
				game_.audio_manager_.fade_out_sound(sound_properties.channel_, sound_properties.fadeout_length_);
			}
			//sound_changed_ = true;
		}
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////

//Textbox
void InGame::update_textbox(Script::InfoTextbox& info_textbox)
{
	set_position_ui_textbox(info_textbox);
}

//Dialogues
void InGame::update_dialogue(Script::InfoDialogue& info_dialogue, const Character& character)
{
	if(which_dialogue_from_where_.which_dialogue_ == Script::Where::none && !textbox_.is_first_dialogue_)
	{
		return;
	}

	std::cout << "ICI: " << info_dialogue.t_  << ", " << game_.script_.current_script_index_ << ", " << info_dialogue.character_variable_ << ", " << character.properties_.name_ << std::endl;

	//std::cout << "*************************PERSO: " << info_dialogue.character_variable_ << ", texte: " << info_dialogue.t_ << std::endl;
	textbox_.show_new_dialogue(info_dialogue.t_, character.properties_.name_, skip_mode_, which_dialogue_from_where_.wait_for_end_of_dialogue_);
	textbox_.change_textbox(character.properties_.textbox_path_, renderer_);
	textbox_.change_namebox(character.properties_.namebox_path_, renderer_);
	textbox_.change_namebox_text_color(character.properties_.namebox_text_color_);
	//which_dialogue_from_where_ = {WhichDialogue::none, false, false};
}

void InGame::update_characters_dialogue(Script::InfoDialogue& info_dialogue)
{
	if(which_dialogue_from_where_.which_dialogue_ == Script::Where::none) //TODO : ajouter && !textbox_.is_first_dialogue_ ??
	{
		return;
	}

	//TODO : attention : conflit avec l'autofocus
	/*for(auto& [key_character_variable, value_character] : character_manager_.active_characters_)
	{
		value_character.properties_.is_speaking_ = false;
	}*/

	//std::cout << "PERSO: " << info_dialogue.character_variable_ << ", texte: " << info_dialogue.t_ << std::endl;

	//Character& character = character_manager_.active_characters_.at(info_dialogue.character_variable_);
	/*character.properties_.is_speaking_ = true;*/ //TODO : attention : conflit avec l'autofocus
}

//TODO : utiliser std::visit ??
void InGame::update()
{
	//TODO : enregistrer les choses courantes
	/*if(!game_.script_.move_dialogue(which_dialogue_from_where_.which_dialogue_))
	{
		//should_continue_advancing_ = true;
	}*/
	//else
	//{
	//	should_continue_advancing_ = false;
	//	//which_dialogue_from_where_ = {Script::Where::none, false, false};
	//}

	game_.script_.move_dialogue(which_dialogue_from_where_.which_dialogue_, which_dialogue_from_where_.is_from_mouse_wheel_);

	if(!init_)
	{
		if(!std::holds_alternative<Script::InfoDialogue>(game_.script_.script_information_[game_.script_.current_script_index_]))
		{
			//should_continue_advancing_ = true;
			which_dialogue_from_where_ = {Script::Where::next, false, false};
		}
		else
		{
			//should_continue_advancing_ = false;
			//which_dialogue_from_where_ = {Script::Where::none, false, false}; //pas utile car réalisé dans le if de Script::InfoDialogue
			init_ = true;
		}
	}

	std::cout << "CURRENT: " << game_.script_.current_script_index_ << std::endl;

	Script::ScriptInformation& current_script_information = game_.script_.script_information_[game_.script_.current_script_index_];
	if(std::holds_alternative<Script::InfoBackground>(current_script_information))
	{
		update_backgrounds(std::get<Script::InfoBackground>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoCharacter>(current_script_information))
	{
		character_manager_.update(std::get<Script::InfoCharacter>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoAutofocus>(current_script_information))
	{
		update_autofocus(std::get<Script::InfoAutofocus>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoMusic>(current_script_information))
	{
		update_music(std::get<Script::InfoMusic>(current_script_information));
	}
	else if(std::holds_alternative<Script::InfoSound>(current_script_information))
	{
		update_sounds(std::get<Script::InfoSound>(current_script_information), game_.script_.current_script_index_);
	}
	else if(std::holds_alternative<Script::InfoDialogue>(current_script_information))
	{
		Script::InfoDialogue& info_dialogue = std::get<Script::InfoDialogue>(current_script_information);
		update_dialogue(info_dialogue, character_manager_.active_characters_.at(info_dialogue.character_variable_));
		update_characters_dialogue(info_dialogue);
		which_dialogue_from_where_ = {Script::Where::none, false, false};
	}
	else if(std::holds_alternative<Script::InfoTextbox>(current_script_information))
	{
		update_textbox(std::get<Script::InfoTextbox>(current_script_information));
	}

	update_skip_auto_modes();

	if(!hide_ui_textbox_)
	{
		ui_manager_.update();
		textbox_.update();
	}

	//update_textbox();

	//Characters
	for(auto& [key_character_variable, value_character] : character_manager_.active_characters_)
	{
		value_character.update();
	}
	//////////////////////////////

	//Musics
	//aucune musique trouvée => stopper l'éventuelle musique qui était en train de se jouer
	//si scroll en arrière et aucune musique => fadeout de 1.5 secondes (valeur constante)
	//if(!music_changed_)
	//{
	//	//TODO : même code que dans update_music() (sauf l'argument de fadeout)
	//	game_.audio_manager_.fade_out_music(constants::fadeout_length_scroll_back_);
	//	currently_playing_music_.music_ = nullptr;
	//}
	//////////////////////////////

	//Sounds
	/*if(!sound_changed_)
	{
		halt_all_sounds();
		currently_playing_sound_ = {{}, 0, false, nullptr};
	}*/
	//////////////////////////////
}