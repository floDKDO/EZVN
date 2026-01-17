#include "script.h"

Script::Script(sdl::Renderer& renderer)
	: current_script_index_(0), previous_script_index_(current_script_index_), script_index_when_prev_({false, current_script_index_}), renderer_(renderer)
{}

//Dialogues
void Script::insert_dialogue(const std::string_view character_variable, const std::string_view dialogue)
{
	script_information_.push_back(InfoDialogue({std::string(character_variable), std::string(dialogue)}));
}

//Character
void Script::show_character(const std::string_view character_variable, const std::optional<std::string> transform_name, const std::optional<int> zorder)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;

	if(zorder.has_value())
	{
		info_character.t_.insert({CharacterCommandKind::ZORDER, zorder.value()});
	}

	if(transform_name.has_value())
	{
		info_character.t_.insert({CharacterCommandKind::TRANSFORM_NAME, transform_name.value()});
	}

	info_character.t_.insert({CharacterCommandKind::IS_VISIBLE, true});

	script_information_.push_back(InfoCharacter(info_character));
}

//Character
void Script::hide_character(const std::string_view character_variable)
{
	//TODO : garder ??
	/*Character* character = is_character_active(character_variable);
	if(character == nullptr)
	{
		std::cerr << "ERREUR: hide d'un personnage qui n'a pas encore été show()\n";
		exit(1);
	}*/

	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::IS_VISIBLE, false});
	script_information_.push_back(InfoCharacter(info_character));
}

//Character
void Script::rename_character(const std::string_view character_variable, const std::string_view new_character_name)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::NAME, std::string(new_character_name)});
	script_information_.push_back(InfoCharacter(info_character));
}

//Character
void Script::insert_textbox(const std::string_view character_variable, const std::string_view textbox_path)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::TEXTBOX_PATH, std::string(textbox_path)});
	script_information_.push_back(InfoCharacter(info_character));
}

//Character
void Script::insert_namebox(const std::string_view character_variable, const std::string_view namebox_path)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::NAMEBOX_PATH, std::string(namebox_path)});
	script_information_.push_back(InfoCharacter(info_character));
}

//Character
void Script::insert_namebox_text_color(const std::string_view character_variable, Color namebox_text_color)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::NAMEBOX_TEXT_COLOR, namebox_text_color});
	script_information_.push_back(InfoCharacter(info_character));
}

//Textbox
void Script::move_textbox(const std::string_view where)
{
	script_information_.push_back(InfoTextbox(std::string(where)));
}

//Background
void Script::insert_background(const std::string_view background_path)
{
	script_information_.push_back(InfoBackground(Background(background_path, renderer_)));
}

//Background
void Script::insert_background(Color color)
{
	script_information_.push_back(InfoBackground(Background(color)));
}

//Sounds
void Script::play_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, float volume_multiplier, int channel, bool loop)
{
	script_information_.push_back(InfoSound(std::make_pair(AudioProperties{fadein_length, fadeout_length, loop, channel}, Sound{sdl::Chunk(sound_path), volume_multiplier}))); //TODO : std::make_pair a l'air obligatoire ici
}

//Sounds
void Script::stop_sound(int fadeout_length, int channel)
{
	//TODO : hardcodé
	script_information_.push_back(InfoSound(std::make_pair(AudioProperties{0, fadeout_length, false, channel}, std::nullopt))); //TODO : std::make_pair a l'air obligatoire ici
}

//Musics
void Script::play_music(const std::string_view music_path, int fadein_length, int fadeout_length, float volume_multiplier, bool loop)
{
	script_information_.push_back(InfoMusic(std::make_pair(AudioProperties{fadein_length, fadeout_length, loop}, Music{sdl::Music(music_path), volume_multiplier}))); //TODO : std::make_pair a l'air obligatoire ici
}

//Musics
void Script::stop_music(int fadeout_length)
{
	//TODO : hardcodé
	script_information_.push_back(InfoMusic(std::make_pair(AudioProperties{0, fadeout_length, true}, std::nullopt))); //TODO : std::make_pair a l'air obligatoire ici
}

//Autofocus
void Script::insert_autofocus(bool autofocus)
{
	script_information_.push_back(InfoAutofocus(autofocus));
}

void Script::increment_script_index()
{
	if(script_information_.size() > current_script_index_ + 1)
	{
		current_script_index_ += 1;
	}
}

void Script::decrement_script_index()
{
	if(current_script_index_ > 0)
	{
		current_script_index_ -= 1;
	}
}

bool Script::is_current_script_index_a_dialogue()
{
	return std::holds_alternative<Script::InfoDialogue>(script_information_[current_script_index_]);
}

bool Script::move_dialogue(Where where, bool is_from_mouse_wheel_)
{
	//std::cout << "AVANT: " << current_script_index_ << ", " << previous_script_index_ << std::endl;

	if(where == Where::next)
	{
		if(is_from_mouse_wheel_ && current_script_index_ < script_index_when_prev_.saved_script_index_)
		{
			increment_script_index();
		}
		else if(!is_from_mouse_wheel_)
		{
			script_index_when_prev_.is_saved_ = false; //when we pass a dialogue, reset the mouse wheel dialogues
			increment_script_index();
		}
	}
	else if(where == Where::prev)
	{
		if(!script_index_when_prev_.is_saved_) //sauvegarder une seule fois 
		{
			script_index_when_prev_.saved_script_index_ = current_script_index_;
			script_index_when_prev_.is_saved_ = true;
		}

		decrement_script_index();
		
		if(current_script_index_ == 0 && !is_current_script_index_a_dialogue())
		{
			current_script_index_ = previous_script_index_; //on est allé en arrière sans croiser de dialogue => revenir au tout premier dialogue
		}
	}
	//std::cout << "APRES: " << current_script_index_ << ", " << previous_script_index_ << std::endl;

	bool is_dialogue = is_current_script_index_a_dialogue();
	if(is_dialogue)
	{
		previous_script_index_ = current_script_index_;
	}
	return is_dialogue;
}