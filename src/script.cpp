#include "script.h"

Script::Script(sdl::Renderer& renderer)
	: renderer_(renderer), current_script_index_(0), previous_script_index_(0)
{}

//Character
std::optional<Character::Editableproperties> Script::get_last_character_properties(const std::string_view character_variable)
{
	for(size_t i = script_information_.size() - 1; i != -1; --i) //si script_information_.size() == 0, on n'entre pas dans cette boucle
	{
		if(std::holds_alternative<InfoCharacter>(script_information_[i]))
		{
			InfoCharacter& info_character = std::get<InfoCharacter>(script_information_[i]);
			if(info_character.character_variable_ == character_variable)
			{
				return info_character.t_;
			}
		}
	}
	return std::nullopt;
}

//Dialogues
void Script::insert_dialogue(const std::string_view character_variable, const std::string_view dialogue)
{
	script_information_.push_back(InfoDialogue({std::string(character_variable), std::string(dialogue)}));
}

//Character
void Script::show_character(const std::string_view character_variable, const std::optional<Character::Editableproperties> properties, const std::optional<std::string> transform_name, const std::optional<int> zorder)
{
	Character::Editableproperties character_properties;
	if(!properties.has_value())
	{
		character_properties = get_last_character_properties(character_variable).value();
	}
	else
	{
		character_properties = properties.value();
	}

	if(zorder.has_value())
	{
		character_properties.zorder_ = zorder.value();
	}

	if(transform_name.has_value())
	{
		character_properties.transform_name_ = transform_name.value();
	}

	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
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

	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.is_visible_ = false;

	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void Script::rename_character(const std::string_view character_variable, const std::string_view new_character_name)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.name_ = new_character_name;

	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void Script::insert_textbox(const std::string_view character_variable, const std::string_view textbox_path)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.textbox_path_ = textbox_path;
	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void Script::insert_namebox(const std::string_view character_variable, const std::string_view namebox_path)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.namebox_path_ = namebox_path;
	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

//Character
void Script::insert_namebox_text_color(const std::string_view character_variable, Color namebox_text_color)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable).value();
	character_properties.namebox_text_color_ = namebox_text_color;
	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
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