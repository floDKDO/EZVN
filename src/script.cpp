#include "script.h"

Script::Script(sdl::Renderer& renderer)
	: renderer_(renderer)
{
	add_character(constants::default_narrator_name_, "", constants::namebox_text_color_, constants::textbox_image_, constants::namebox_image_);
}

void Script::insert_dialogue(std::string_view character_variable, std::string_view dialogue)
{
	InfoTextbox info_textbox;
	info_textbox.character_variable_ = character_variable;
	info_textbox.t_.textbox_command_kind_ = TextboxCommandKind::DIALOGUE;
	info_textbox.t_.textbox_command_value_ = std::string(dialogue);

	script_information_.push_back(InfoTextbox(info_textbox));
}

void Script::show_character(std::string_view character_variable, std::optional<std::string_view> composite_image_name, std::optional<std::string_view> transform_name, std::optional<unsigned int> zorder)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;

	if(composite_image_name.has_value())
	{
		info_character.t_.insert({CharacterCommandKind::COMPOSITE_IMAGE, std::string(composite_image_name.value())});
	}

	if(transform_name.has_value())
	{
		info_character.t_.insert({CharacterCommandKind::TRANSFORM_NAME, std::string(transform_name.value())});
	}

	if(zorder.has_value())
	{
		info_character.t_.insert({CharacterCommandKind::ZORDER, zorder.value()});
	}

	info_character.t_.insert({CharacterCommandKind::IS_VISIBLE, true});

	script_information_.push_back(InfoCharacter(info_character));
}

void Script::hide_character(std::string_view character_variable)
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

void Script::rename_character(std::string_view character_variable, std::string_view new_character_name)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::NAME, std::string(new_character_name)});
	script_information_.push_back(InfoCharacter(info_character));
}

void Script::add_character(std::string_view character_variable, std::string_view character_name, Color namebox_text_color, std::string_view textbox_path, std::string_view namebox_path)
{
	character_definitions_.insert(std::make_pair(std::string(character_variable), CharacterDefinition{character_variable, character_name, namebox_text_color, textbox_path, namebox_path}));
}

void Script::add_composite_image(std::string_view character_variable, std::string_view composite_image_name, int w, int h, std::initializer_list<ImageInfo> images_info)
{
	CharacterDefinition& character_definition = character_definitions_.at(std::string(character_variable));
	character_definition.composite_images_.insert(std::make_pair(composite_image_name, CompositeImage(composite_image_name, w, h, images_info)));
}

void Script::insert_textbox(std::string_view character_variable, std::string_view textbox_path)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::TEXTBOX_PATH, std::string(textbox_path)});
	script_information_.push_back(InfoCharacter(info_character));
}

void Script::insert_namebox(std::string_view character_variable, std::string_view namebox_path)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::NAMEBOX_PATH, std::string(namebox_path)});
	script_information_.push_back(InfoCharacter(info_character));
}

void Script::insert_namebox_text_color(std::string_view character_variable, Color namebox_text_color)
{
	InfoCharacter info_character;
	info_character.character_variable_ = character_variable;
	info_character.t_.insert({CharacterCommandKind::NAMEBOX_TEXT_COLOR, namebox_text_color});
	script_information_.push_back(InfoCharacter(info_character));
}

void Script::move_textbox(std::string_view where)
{
	InfoTextbox info_textbox;
	info_textbox.character_variable_ = constants::default_narrator_name_; //TODO : garder Narrator ??
	info_textbox.t_.textbox_command_kind_ = TextboxCommandKind::MOVE_TEXTBOX;
	info_textbox.t_.textbox_command_value_ = std::string(where);

	script_information_.push_back(InfoTextbox(info_textbox));
}

void Script::insert_background(std::string_view background_path, std::string_view transition_name, std::optional<int> length_to_black, std::optional<int> length_black, std::optional<int> length_to_clear)
{
	if(length_to_black.has_value() && length_black.has_value() && length_to_clear.has_value())
	{
		script_information_.push_back(InfoTransition(transition_name, length_to_black.value(), length_black.value(), length_to_clear.value()));
	}
	else
	{
		script_information_.push_back(InfoTransition(transition_name));
	}
	script_information_.push_back(InfoBackground(Background(background_path, renderer_)));
}

void Script::insert_background(std::string_view background_path)
{
	script_information_.push_back(InfoBackground(Background(background_path, renderer_)));
}

void Script::insert_background(Color color, std::string_view transition_name, std::optional<int> length_to_black, std::optional<int> length_black, std::optional<int> length_to_clear)
{
	if(length_to_black.has_value() && length_black.has_value() && length_to_clear.has_value())
	{
		script_information_.push_back(InfoTransition(transition_name, length_to_black.value(), length_black.value(), length_to_clear.value()));
	}
	else
	{
		script_information_.push_back(InfoTransition(transition_name));
	}
	script_information_.push_back(InfoBackground(Background(color)));
}

void Script::insert_background(Color color)
{
	script_information_.push_back(InfoBackground(Background(color)));
}

void Script::play_sound(std::string_view sound_path, int fadein_length, int fadeout_length, float volume_multiplier, int channel, bool loop)
{
	script_information_.push_back(InfoSound(std::make_pair(AudioProperties{fadein_length, fadeout_length, loop, channel}, Sound{sdl::Chunk(sound_path), volume_multiplier}))); //TODO : std::make_pair a l'air obligatoire ici
}

void Script::stop_sound(int fadeout_length, int channel)
{
	//TODO : hardcodé
	script_information_.push_back(InfoSound(std::make_pair(AudioProperties{0, fadeout_length, false, channel}, std::nullopt))); //TODO : std::make_pair a l'air obligatoire ici
}

void Script::play_music(std::string_view music_path, int fadein_length, int fadeout_length, float volume_multiplier, bool loop)
{
	script_information_.push_back(InfoMusic(std::make_pair(AudioProperties{fadein_length, fadeout_length, loop}, Music{sdl::Music(music_path), volume_multiplier}))); //TODO : std::make_pair a l'air obligatoire ici
}

void Script::stop_music(int fadeout_length)
{
	//TODO : hardcodé
	script_information_.push_back(InfoMusic(std::make_pair(AudioProperties{0, fadeout_length, true}, std::nullopt))); //TODO : std::make_pair a l'air obligatoire ici
}

void Script::insert_choice_menu(std::string_view character_variable, std::string_view dialogue, std::initializer_list<std::string> texts)
{
	script_information_.push_back(InfoChoiceMenu(texts));

	InfoTextbox info_textbox;
	info_textbox.character_variable_ = character_variable;
	info_textbox.t_.textbox_command_kind_ = TextboxCommandKind::DIALOGUE;
	info_textbox.t_.textbox_command_value_ = std::string(dialogue);
	script_information_.push_back(InfoTextbox(info_textbox));
}

void Script::insert_autofocus(bool autofocus)
{
	script_information_.push_back(InfoAutofocus({AutofocusCommandKind::AUTOFOCUS, autofocus}));
}

void Script::insert_autozorder(bool autozorder)
{
	script_information_.push_back(InfoAutofocus({AutofocusCommandKind::AUTOZORDER, autozorder}));
}