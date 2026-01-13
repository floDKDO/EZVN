#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"
#include "game.h"
#include "RAII_SDL2/channel.h"

#include <iostream>
#include <algorithm>

//TODO : éviter de répéter plein de fois les mêmes boucles for
//TODO : le code des dialogues devra être modifié quand il y aura l'ajout de pauses, animations d'images, choice menus etc.

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game), /*current_dialogue_index_(size_t(1)),*/ script_({0, 0}), which_dialogue_from_where_({WhichDialogue::next, false, false}),
	skip_mode_(false), auto_mode_(false), autofocus_(constants::default_autofocus), last_time_(0), background_(Color::from_rgba8(0, 0, 0)), textbox_(renderer), hide_ui_textbox_(false), currently_playing_sound_({{}, 0, false, nullptr}), currently_playing_music_({{}, nullptr}),
	/*background_changed_(false), autofocus_changed_(false), music_changed_(false), sound_changed_(false),*/ renderer_(renderer)
{
	build_ui_elements(renderer); 
	create_narrator();
}

void InGame::build_ui_elements(sdl::Renderer& renderer)
{
	int x_textbutton = textbox_.textbox_.position_.x + constants::textbox_ui_elements_x_delta_;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h + constants::textbox_ui_elements_y_delta_;

	ui_manager_.add_element(std::make_unique<TextButton>("History", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_manager_.ui_elements_[0].get())->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;
	ui_manager_.add_element(std::make_unique<TextToggle>("Skip", x_textbutton, y_textbutton, false, renderer, std::bind(&InGame::skip_function, this, std::placeholders::_1), TextToggleKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextToggle*>(ui_manager_.ui_elements_[1].get())->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;
	ui_manager_.add_element(std::make_unique<TextToggle>("Auto", x_textbutton, y_textbutton, false, renderer, std::bind(&InGame::auto_function, this, std::placeholders::_1), TextToggleKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextToggle*>(ui_manager_.ui_elements_[2].get())->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;
	ui_manager_.add_element(std::make_unique<TextButton>("Save", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_manager_.ui_elements_[3].get())->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;
	ui_manager_.add_element(std::make_unique<TextButton>("Load", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_manager_.ui_elements_[4].get())->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;
	ui_manager_.add_element(std::make_unique<TextButton>("Settings", x_textbutton, y_textbutton, renderer, std::bind(&InGame::settings_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));

	ui_manager_.set_elements();
}

TextToggle* InGame::get_texttoggle(const std::string_view text)
{
	for(Ui* ui : ui_manager_.navigation_list_)
	{
		TextToggle* texttoggle = dynamic_cast<TextToggle*>(ui);
		if(texttoggle != nullptr && texttoggle->text_.text_ == text)
		{
			return texttoggle;
		}
	}
	return nullptr;
}

//Fonctions de callback///////////////////////////////////////////
void InGame::auto_function(Ui* ui)
{
	(void)ui;
	auto_mode_ = !auto_mode_;
}

void InGame::skip_function(Ui* ui)
{
	(void)ui;
	skip_mode_ = !skip_mode_;
}

void InGame::settings_function(Ui* ui)
{
	(void)ui;
	std::cout << "Pressed Settings!" << std::endl;
	game_.request_push_state(constants::settings_menu_unique_id_);
}

void InGame::temp_function(Ui* ui)
{
	(void)ui;
	std::cout << "Pressed!\n";
}
//////////////////////////////////////////////////////////////////


//Insert* ////////////////////////////////////////////////////////////////////////////////////////////////

//Dialogues
void InGame::insert_dialogue(const std::string_view character_variable, const std::string_view dialogue)
{
	script_.script_information_.push_back(InfoDialogue({std::string(character_variable), std::string(dialogue)}));
}

//Character
Character::Editableproperties InGame::show_character_prologue(const std::string_view character_variable)
{
	const std::string key(character_variable);

	CharacterDefinition& character_definition = character_definitions_.at(key);
	Character::Editableproperties character_properties;

	if(is_character_active(character_variable) == nullptr)
	{
		active_characters_.insert(std::make_pair(key, Character(character_definition, renderer_)));
		draw_characters_order_.push_back(key);
		character_properties = active_characters_.at(key).properties_; //=> n'est pas encore dans script_information_
	}
	else
	{
		character_properties = get_last_character_properties(character_variable).value();
	}
	character_properties.is_visible_ = true;

	return character_properties;
}

//Character
void InGame::show_character(const std::string_view character_variable, const std::optional<std::string> transform_name, const std::optional<int> zorder)
{
	Character::Editableproperties character_properties = show_character_prologue(character_variable);

	if(zorder.has_value())
	{
		character_properties.zorder_ = zorder.value();
	}

	if(transform_name.has_value())
	{
		character_properties.transform_name_ = transform_name.value();
	}

	script_.script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void InGame::hide_character(const std::string_view character_variable)
{
	Character* character = is_character_active(character_variable);
	if(character == nullptr)
	{
		std::cerr << "ERREUR: hide d'un personnage qui n'a pas encore été show()\n";
		exit(1);
	}

	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.is_visible_ = false;

	script_.script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void InGame::rename_character(const std::string_view character_variable, const std::string_view new_character_name)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.name_ = new_character_name;

	script_.script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void InGame::insert_textbox(const std::string_view character_variable, const std::string_view textbox_path)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.textbox_path_ = textbox_path;
	script_.script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void InGame::insert_namebox(const std::string_view character_variable, const std::string_view namebox_path)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.namebox_path_ = namebox_path;
	script_.script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void InGame::insert_namebox_text_color(const std::string_view character_variable, Color namebox_text_color)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.namebox_text_color_ = namebox_text_color;
	script_.script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Background
void InGame::insert_background(const std::string_view background_path)
{
	script_.script_information_.push_back(InfoBackground(Background(background_path, renderer_)));
}

//Background
void InGame::insert_background(Color color)
{
	script_.script_information_.push_back(InfoBackground(Background(color)));
}

//Sounds
void InGame::play_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, float volume_multiplier, int channel, bool loop)
{
	script_.script_information_.push_back(InfoSound(std::make_pair(AudioProperties{fadein_length, fadeout_length, loop, channel}, Sound{sdl::Chunk(sound_path), volume_multiplier}))); //TODO : std::make_pair a l'air obligatoire ici
}

//Sounds
void InGame::stop_sound(int fadeout_length, int channel)
{
	//TODO : hardcodé
	script_.script_information_.push_back(InfoSound(std::make_pair(AudioProperties{0, fadeout_length, false, channel}, std::nullopt))); //TODO : std::make_pair a l'air obligatoire ici
}

//Musics
void InGame::play_music(const std::string_view music_path, int fadein_length, int fadeout_length, float volume_multiplier, bool loop)
{
	script_.script_information_.push_back(InfoMusic(std::make_pair(AudioProperties{fadein_length, fadeout_length, loop}, Music{sdl::Music(music_path), volume_multiplier}))); //TODO : std::make_pair a l'air obligatoire ici
}

//Musics
void InGame::stop_music(int fadeout_length)
{
	//TODO : hardcodé
	script_.script_information_.push_back(InfoMusic(std::make_pair(AudioProperties{0, fadeout_length, true}, std::nullopt))); //TODO : std::make_pair a l'air obligatoire ici
}

//Autofocus
void InGame::insert_autofocus(bool autofocus)
{
	script_.script_information_.push_back(InfoAutofocus(autofocus));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Character//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Character
void InGame::add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, Color namebox_text_color, const std::string_view textbox_path, const std::string_view namebox_path)
{
	character_definitions_.insert(std::make_pair(std::string(character_variable), CharacterDefinition{character_variable, character_name, character_path, namebox_text_color, textbox_path, namebox_path}));
}

//Character
void InGame::create_narrator()
{
	add_character("Narrator", "", "", constants::namebox_text_color_, constants::textbox_image_, constants::namebox_image_);
	CharacterDefinition& character_definition = character_definitions_.at("Narrator");
	active_characters_.insert(std::make_pair("Narrator", Character(character_definition, renderer_)));
}

//Character
Character* InGame::is_character_active(const std::string_view character_variable)
{
	const std::string key(character_variable);
	if(active_characters_.count(key)) //déjà dans active_characters_
	{
		//std::cout << "(is_character_active) " << character_variable << " is already in active_characters_\n";
		return &(active_characters_.at(key));
	}
	else
	{
		//std::cout << "(is_character_active) " << character_variable << " is not in active_characters_\n";
		return nullptr;
	}
}

//Character
std::optional<Character::Editableproperties> InGame::get_last_character_properties(const std::string_view character_variable)
{
	for(size_t i = script_.script_information_.size() - 1; i != -1; --i) //si script_information_.size() == 0, on n'entre pas dans cette boucle
	{
		if(std::holds_alternative<InfoCharacter>(script_.script_information_[i]))
		{
			InfoCharacter& info_character = std::get<InfoCharacter>(script_.script_information_[i]);
			if(info_character.character_variable_ == character_variable)
			{
				return info_character.t_;
			}
		}
	}
	return std::nullopt;
}

//Character
void InGame::draw_characters(sdl::Renderer& renderer)
{
	//Characters
	std::stable_sort(draw_characters_order_.begin(), draw_characters_order_.end(),
		[&](const std::string_view a, const std::string_view b) -> bool
	{
		return active_characters_.at(std::string(a)).properties_.zorder_ < active_characters_.at(std::string(b)).properties_.zorder_;
	});

	/*std::cout << "\n";
	for(const std::string_view s : draw_characters_order_)
	{
		std::cout << s << std::endl;
	}
	std::cout << "\n";*/

	//std::cout << "\n";
	for(const std::string_view s : draw_characters_order_)
	{
		for(size_t i = script_.current_script_index_; i != -1; --i)
		{
			if(std::holds_alternative<InfoCharacter>(script_.script_information_[i]))
			{
				InfoCharacter& p_character = std::get<InfoCharacter>(script_.script_information_[i]);
				if(s == p_character.character_variable_)
				{
					active_characters_.at(std::string(s)).draw(renderer);
					//std::cout << p_character.character_variable_ << ", " << std::boolalpha << value_character.properties_.is_visible_ << std::endl;
					break;
				}
			}
		}
	}
	//std::cout << "\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Background/////////////////////////////////////////////////////////////////////////////////// 
void InGame::change_background(const InfoBackground& b)
{
	if(b.image_ != nullptr)
	{
		if(background_.image_ != nullptr)
		{
			background_.image_->init_image(b.image_->path_, 0, 0, renderer_);
		}
		else
		{
			background_ = Background(b.image_->path_, renderer_);
		}
	}
	else
	{
		background_.image_.reset();
		background_.color_ = b.color_;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////

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
				get_texttoggle("Skip")->change_checked(false);
				skip_mode_ = false;

				which_dialogue_from_where_ = {WhichDialogue::prev, true, false};
			}
			else //scroll vers l'arrière => avancer d'un dialogue
			{
				which_dialogue_from_where_ = {WhichDialogue::next, true, false};
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
			get_texttoggle("Auto")->change_checked(false);
			auto_mode_ = false;

			if(textbox_.text_.is_finished_)
			{
				which_dialogue_from_where_ = {WhichDialogue::next, false, true};
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

	if(script_.script_information_.size() > 0)
	{
		draw_characters(renderer);
	}

	if(!hide_ui_textbox_)
	{
		textbox_.draw(renderer);
		ui_manager_.draw(renderer);
	}
}

//void InGame::update_current_script_index_dialogue()
//{
//	if(which_dialogue_from_where_.which_dialogue_ == WhichDialogue::none)
//	{
//		return;
//	}
//
//	if(!which_dialogue_from_where_.is_from_mouse_wheel_)
//	{
//		show_next_dialogue();
//	}
//	else
//	{
//		show_dialogue_mouse_wheel();
//	}
//}

void InGame::update_backgrounds(const InfoBackground& info_background)
{
	//Backgrounds
	change_background(info_background);
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update_characters(const InfoCharacter& info_character)
{
	//Characters
	const std::string& character_variable = info_character.character_variable_;

	Character& character = active_characters_.at(character_variable);
	character.set_transform(info_character.t_.transform_name_);
	character.properties_.zorder_ = info_character.t_.zorder_;
	character.properties_.name_ = info_character.t_.name_;
	character.properties_.textbox_path_ = info_character.t_.textbox_path_;
	character.properties_.namebox_path_ = info_character.t_.namebox_path_;
	character.properties_.namebox_text_color_ = info_character.t_.namebox_text_color_;
}

void InGame::update_autofocus(const InfoAutofocus& info_autofocus)
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
		which_dialogue_from_where_ = {WhichDialogue::next, false, false};
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
				which_dialogue_from_where_ = {WhichDialogue::next, false, true};
				//show_next_dialogue();

				last_time_ = SDL_GetTicks64();
			}
		}
	}
}

void InGame::update_music(InfoMusic& info_music)
{
	//Musics
	const InGame::AudioProperties& music_properties = info_music.first;
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
	for(auto& s : script_.script_information_)
	{
		if(std::holds_alternative<InfoSound>(s))
		{
			InfoSound& sound_pair = std::get<InfoSound>(s);
			if(sound_pair.second.has_value())
			{
				InGame::AudioProperties& sound_properties = sound_pair.first;
				if(sdl::channel::playing(sound_properties.channel_))
				{
					game_.audio_manager_.halt_channel(sound_properties.channel_);
				}
			}
		}
	}
}

//Sounds
void InGame::update_sounds(InfoSound& info_sound, size_t i)
{
	//if(!sound_changed_)
	{
		InGame::AudioProperties& sound_properties = info_sound.first;

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
void InGame::update_textbox()
{

}

//Dialogues
void InGame::update_dialogue(InfoDialogue& info_dialogue)
{
	if(which_dialogue_from_where_.which_dialogue_ == WhichDialogue::none)
	{
		return;
	}

	//std::cout << "ICI: " << info_dialogue.t_  << ", " << info_dialogue.character_variable_ << ", " << active_characters_.at(info_dialogue.character_variable_).properties_.name_ << std::endl;

	//std::cout << "*************************PERSO: " << info_dialogue.character_variable_ << ", texte: " << info_dialogue.t_ << std::endl;

	textbox_.show_new_dialogue(info_dialogue.t_, active_characters_.at(info_dialogue.character_variable_).properties_.name_, skip_mode_, which_dialogue_from_where_.wait_for_end_of_dialogue_);

	textbox_.change_textbox(active_characters_.at(info_dialogue.character_variable_).properties_.textbox_path_, renderer_);
	textbox_.change_namebox(active_characters_.at(info_dialogue.character_variable_).properties_.namebox_path_, renderer_);
	textbox_.change_namebox_text_color(active_characters_.at(info_dialogue.character_variable_).properties_.namebox_text_color_);

	//which_dialogue_from_where_ = {WhichDialogue::none, false, false};
}

void InGame::update_characters_dialogue(InfoDialogue& info_dialogue)
{
	if(which_dialogue_from_where_.which_dialogue_ == WhichDialogue::none)
	{
		return;
	}

	for(auto& [key_character_variable, value_character] : active_characters_)
	{
		value_character.properties_.is_speaking_ = false;
	}

	//std::cout << "PERSO: " << info_dialogue.character_variable_ << ", texte: " << info_dialogue.t_ << std::endl;

	Character& character = active_characters_.at(info_dialogue.character_variable_);
	character.properties_.is_speaking_ = true;
}

void InGame::update()
{
	//std::cout << "AUTOFOCUS: " << autofocus_ << std::endl;
	update2();
}

bool InGame::move_dialogue()
{
	//std::cout << "DEDANS (next): " << current_script_index_  << ", " << std::boolalpha << (which_dialogue_from_where_.which_dialogue_ == WhichDialogue::next) << ", " << (which_dialogue_from_where_.which_dialogue_ == WhichDialogue::prev) << std::endl;
	if(which_dialogue_from_where_.which_dialogue_ == WhichDialogue::next)
	{
		if(script_.script_information_.size() > script_.current_script_index_ + 1)
		{
			script_.current_script_index_ += 1;
		}
	}
	else if(which_dialogue_from_where_.which_dialogue_ == WhichDialogue::prev)
	{
		if(script_.current_script_index_ > 0)
		{
			script_.current_script_index_ -= 1;
		}
	}
	return(!std::holds_alternative<InfoDialogue>(script_.script_information_[script_.current_script_index_]));
}


//TODO : utiliser std::visit ??
void InGame::update2()
{
	//TODO : enregistrer les choses courantes
	static bool should_continue_advancing = false;

	//if(should_continue_advancing)
	//{
	//	//should_continue_advancing = advance_to_next_dialogue();
	//}
	//else
	//{
	//	which_dialogue_from_where_ = {WhichDialogue::none, false, false};
	//}
	
	if(std::holds_alternative<InfoBackground>(script_.script_information_[script_.current_script_index_]))
	{
		update_backgrounds(std::get<InfoBackground>(script_.script_information_[script_.current_script_index_]));
	}
	else if(std::holds_alternative<InfoCharacter>(script_.script_information_[script_.current_script_index_]))
	{
		update_characters(std::get<InfoCharacter>(script_.script_information_[script_.current_script_index_]));
	}
	else if(std::holds_alternative<InfoAutofocus>(script_.script_information_[script_.current_script_index_]))
	{
		update_autofocus(std::get<InfoAutofocus>(script_.script_information_[script_.current_script_index_]));
	}
	else if(std::holds_alternative<InfoMusic>(script_.script_information_[script_.current_script_index_]))
	{
		update_music(std::get<InfoMusic>(script_.script_information_[script_.current_script_index_]));
	}
	else if(std::holds_alternative<InfoSound>(script_.script_information_[script_.current_script_index_]))
	{
		update_sounds(std::get<InfoSound>(script_.script_information_[script_.current_script_index_]), script_.current_script_index_);
	}
	else if(std::holds_alternative<InfoDialogue>(script_.script_information_[script_.current_script_index_]))
	{
		update_dialogue(std::get<InfoDialogue>(script_.script_information_[script_.current_script_index_]));
		update_characters_dialogue(std::get<InfoDialogue>(script_.script_information_[script_.current_script_index_]));
	}

	if(move_dialogue())
	{
		should_continue_advancing = true;
		//std::cout << "SHOULD: " << std::boolalpha << should_continue_advancing << std::endl;
	}
	else
	{
		which_dialogue_from_where_ = {WhichDialogue::none, false, false};
		should_continue_advancing = false;
		//std::cout << "SHOULD: " << std::boolalpha << should_continue_advancing << std::endl;
	}

	update_skip_auto_modes();

	if(!hide_ui_textbox_)
	{
		ui_manager_.update();
		textbox_.update();
	}

	//update_textbox();

	//Characters
	for(auto& [key_character_variable, value_character] : active_characters_)
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
