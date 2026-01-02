#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"
#include "game.h"

#include <iostream>
#include <algorithm>

//TODO : éviter de répéter plein de fois les mêmes boucles for
//TODO : "InGame::" devant les types pas nécessaire ?? (ex : InGame::AudioProperties <=> AudioProperties ??)
//TODO : ne pas stocker les editableProperties dans les Characters crées par create_character => ne doit contenir que des trucs fixes
//TODO : value_or("") ??

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game), current_dialogue_index_(-1), current_unique_id_(0), saved_when_prev_({false, current_unique_id_}),
	last_time_(0), textbox_(renderer), background_(0, 0, 0, 255), hide_ui_textbox_(false), renderer_(renderer), skip_mode_(false), auto_mode_(false), currently_playing_music_(nullptr), currently_playing_sound_({0, false, nullptr})
{
	build_ui_elements(renderer);
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
	if(textbox_.text_.is_finished_)
	{
		show_next_dialogue();
	}
}

void InGame::skip_function(Ui* ui)
{
	(void)ui;
	skip_mode_ = !skip_mode_;
}

void InGame::settings_function(Ui* ui)
{
	(void)ui;
	std::cout << "Clicked Settings!" << std::endl;
	game_.request_push_state(game_.settings_menu_.get());
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
	script_information_.push_back(InfoDialogue({std::string(character_variable), dialogue}));
	dialogues_indices_.push_back(script_information_.size() - 1); //= indice du dialogue inséré

	static bool first_dialogue = true;
	if(first_dialogue)
	{
		set_initial_dialogue();
		first_dialogue = false;
	}
}

Character* InGame::is_character_active(const std::string_view character_variable)
{
	if(active_characters_.count(std::string(character_variable))) //déjà dans active_characters_
	{
		return &(active_characters_.at(std::string(character_variable)));
	}
	else
	{
		return nullptr;
	}
}

void InGame::move_to_front(const std::string character_variable)
{
	int index = -1;
	for(int i = 0; i < draw_characters_order_.size(); ++i)
	{
		if(draw_characters_order_[i] == character_variable)
		{
			index = i;
			break;
		}
	}

	if(index == -1)
	{
		std::cerr << "Nothing was moved to the back...\n";
		return;
	}

	auto it = draw_characters_order_.begin() + index;
	std::rotate(draw_characters_order_.begin(), it, it + 1);
}

void InGame::show_character(const std::string_view character_variable, const std::optional<std::string> transform_name, const std::optional<int> zorder)
{
	CharacterDefinition* character_definition = get_character_definition(character_variable); //TODO : tester si différent de nullptr

	Character* character = is_character_active(character_variable);
	if(character == nullptr)
	{
		active_characters_.insert(std::make_pair(character_variable, Character(*character_definition, renderer_)));
		draw_characters_order_.push_back(std::string(character_variable));
		character = &active_characters_.at(std::string(character_variable));
	}
	else
	{
		//TODO : modifier l'ordre de draw_characters_order_
		//move_to_front(std::string(character_variable));
		std::cout << character->character_definition_->character_variable_ << " IS ALREADY IN THE VECTOR!" << std::endl;
	}

	Character::Editableproperties character_properties = character->properties_;
	character_properties.is_visible_ = true;

	if(zorder.has_value())
	{
		character_properties.zorder_ = zorder.value();
	}

	if(transform_name.has_value())
	{
		character_properties.transform_ = Transform(transform_name.value());
	}

	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

void InGame::show_character(const std::string_view character_variable, const std::optional<int> zorder)
{
	CharacterDefinition* character_definition = get_character_definition(character_variable); //TODO : tester si différent de nullptr
	
	Character* character = is_character_active(character_variable);
	if(character == nullptr)
	{
		active_characters_.insert(std::make_pair(character_variable, Character(*character_definition, renderer_)));
		draw_characters_order_.push_back(std::string(character_variable));
		character = &active_characters_.at(std::string(character_variable));
	}
	else
	{
		//TODO : modifier l'ordre de draw_characters_order_
		//move_to_front(std::string(character_variable));
		std::cout << character->character_definition_->character_variable_ << " IS ALREADY IN THE VECTOR!" << std::endl;
	}

	Character::Editableproperties character_properties = character->properties_;
	character_properties.is_visible_ = true;

	if(zorder.has_value())
	{
		character_properties.zorder_ = zorder.value();
	}

	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

void InGame::hide_character(const std::string_view character_variable)
{
	CharacterDefinition* character_definition = get_character_definition(character_variable); //TODO : tester si différent de nullptr

	Character* character = is_character_active(character_variable);
	if(character == nullptr)
	{
		std::cerr << "ERREUR: hide d'un personnage qui n'a pas encore été show()\n";
		exit(1);
	}

	Character::Editableproperties character_properties = character->properties_;
	character_properties.is_visible_ = false;

	script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
}

void InGame::rename_character(const std::string_view character_variable, const std::string_view new_character_name)
{
	script_information_.push_back(InfoRenameCharacter({std::string(character_variable), std::string(new_character_name)}));
}

//Background
void InGame::insert_background(const std::string_view background_path)
{
	script_information_.push_back(InfoBackground(Background(background_path, renderer_)));
}

//Background
void InGame::insert_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
	script_information_.push_back(InfoBackground(Background(r, g, b, a)));
}

//Sounds
void InGame::insert_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, int volume, int channel, bool loop)
{
	if(sound_path.empty()) //stop sound
	{
		script_information_.push_back(InfoSound(std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop, channel}, std::nullopt))); //TODO : std::make_pair a l'air obligatoire ici
	}
	else //play sound
	{
		script_information_.push_back(InfoSound(std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop, channel}, Sound(sound_path, channel, volume))));
	}
}

//Musics
void InGame::insert_music(const std::string_view music_path, int fadein_length, int fadeout_length, int volume, bool loop)
{
	if(music_path.empty()) //stop music
	{
		script_information_.push_back(InfoMusic(std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop}, std::nullopt))); //TODO : std::make_pair a l'air obligatoire ici
	}
	else //play music
	{
		script_information_.push_back(InfoMusic(std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop}, Music(music_path))));
	}
}

//Autofocus
void InGame::insert_autofocus(bool autofocus)
{
	script_information_.push_back(InfoAutofocus(autofocus));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Dialogues //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

InGame::InfoDialogue* InGame::get_current_dialogue()
{
	unsigned int index = dialogues_indices_[current_dialogue_index_];
	return std::get_if<InfoDialogue>(&script_information_[index]);
}

//Dialogues
void InGame::set_initial_dialogue()
{
	//ici, script_information_ n'a pas sa taille finale
	std::cout << "Initial dialogue\n";

	if(dialogues_indices_.size() > 0)
	{
		current_dialogue_index_ = 0;
		textbox_.show_initial_dialogue(get_current_dialogue()->t_, get_last_character_name(get_current_dialogue()->character_variable_).value());
	}
}

//Dialogues
std::optional<std::string> InGame::get_last_character_name(const std::string_view character_variable)
{
	if(character_variable.empty())
	{
		return std::string(); //empty string
	}

	for(size_t i = current_unique_id_; i != -1; --i) //si script_information_.size() == 0, on n'entre pas dans cette boucle
	{
		if(std::holds_alternative<InfoRenameCharacter>(script_information_[i]))
		{
			if(std::holds_alternative<InfoRenameCharacter>(script_information_[i]))
			{
				InfoRenameCharacter& p_character = std::get<InfoRenameCharacter>(script_information_[i]);
				if(p_character.character_variable_ == character_variable)
				{
					return p_character.t_;
				}
			}
		}
		else if(std::holds_alternative<InfoCharacter>(script_information_[i]))
		{
			InfoCharacter& info_character = std::get<InfoCharacter>(script_information_[i]);
			if(info_character.character_variable_ == character_variable)
			{
				return info_character.t_.name_;
			}
		}
	}

	std::cout << "NOT FOUND YET\n";

	//not found yet, get the one inside the "active_characters_" vector
	if(active_characters_.count(std::string(character_variable))) //déjà dans active_characters_
	{
		return active_characters_.at(std::string(character_variable)).properties_.name_;
	}

	return std::nullopt;
}

bool InGame::try_next_dialogue()
{
	return current_dialogue_index_ + 1 < dialogues_indices_.size();
}

bool InGame::try_prev_dialogue()
{
	return current_dialogue_index_ > 0;
}

//Dialogues
void InGame::set_next_dialogue_index()
{
	if(try_next_dialogue()) //TODO : garder ce test ou partir du principe que le test doit être réalisé avant l'appel à cette méthode => si je ne le garde pas, l'incrémentation pourra être réalisée directement sans cette méthode
	{
		current_dialogue_index_ += 1;
	}
}

//Dialogues
void InGame::set_prev_dialogue_index()
{
	if(try_prev_dialogue()) //TODO : garder ce test ou partir du principe que le test doit être réalisé avant l'appel à cette méthode
	{
		current_dialogue_index_ -= 1;
	}
}

//Dialogues
unsigned int InGame::get_current_dialogue_pos()
{
	return dialogues_indices_[current_dialogue_index_];
}

//Dialogues
unsigned int InGame::get_prev_dialogue_pos()
{
	return dialogues_indices_[current_dialogue_index_ - 1];
}

//Dialogues //TODO : pas utilisée
unsigned int InGame::get_next_dialogue_pos()
{
	return dialogues_indices_[current_dialogue_index_ + 1];
}

//Dialogues
void InGame::show_next_dialogue()
{
	//ici, script_information_ a sa taille finale
	if(textbox_.text_.is_finished_) //empêcher le spam d'espace
	{
		if(try_next_dialogue())
		{
			set_next_dialogue_index();
			current_unique_id_ = get_current_dialogue_pos();
			saved_when_prev_.is_saved_ = false; //when we pass a dialogue, reset the mouse wheel dialogues
		}

		textbox_.show_new_dialogue(get_current_dialogue()->t_, get_last_character_name(get_current_dialogue()->character_variable_).value_or(""), skip_mode_, true);
	}
}

//Dialogues
void InGame::show_dialogue_mouse_wheel(WhichDialogue which_dialogue)
{
	SDL_assert(which_dialogue == WhichDialogue::next || which_dialogue == WhichDialogue::previous);

	if(which_dialogue == WhichDialogue::next)
	{
		if(try_next_dialogue() && current_unique_id_ < saved_when_prev_.saved_value_)
		{
			set_next_dialogue_index();
			current_unique_id_ = get_current_dialogue_pos();
		}
	}
	else if(which_dialogue == WhichDialogue::previous)
	{
		if(!saved_when_prev_.is_saved_) //sauvegarder une seule fois 
		{
			saved_when_prev_.saved_value_ = get_current_dialogue_pos();
			saved_when_prev_.is_saved_ = true;
		}

		if(try_prev_dialogue())
		{
			set_prev_dialogue_index();
			current_unique_id_ = get_current_dialogue_pos();
		}

		//si on arrive sur un dialogue dont est associé un son, il faut le rejouer
		if(get_current_dialogue_pos() == currently_playing_sound_.associated_dialogue_pos_)
		{
			//faire cela uniquement si le son n'est pas en train de se jouer si on est sur le dialogue sinon il sera rejoué une fois sur le dialogue
			if(currently_playing_sound_.sound_ != nullptr && !sdl::Chunk::playing(currently_playing_sound_.sound_->channel_))
			{
				currently_playing_sound_.played_ = false;
			}
		}
	}
	textbox_.show_new_dialogue(get_current_dialogue()->t_, get_last_character_name(get_current_dialogue()->character_variable_).value_or(""), skip_mode_, false);
}
////////////////////////////////////////////////////////////////////////////////////


//Character//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InGame::add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const SDL_Color namebox_font_color, const std::string_view textbox_path, const std::string_view namebox_path)
{
	character_definitions_.push_back({character_variable, character_name, character_path, namebox_font_color, textbox_path, namebox_path});
}

CharacterDefinition* InGame::get_character_definition(const std::string_view character_variable)
{
	for(CharacterDefinition& c : character_definitions_)
	{
		if(c.character_variable_ == character_variable)
		{
			return &c;
		}
	}
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Background/////////////////////////////////////////////////////////////////////////////////// 
void InGame::change_background(const Background& b)
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
		background_.color_ = {b.color_.r, b.color_.g, b.color_.b, b.color_.a};
	}
}
////////////////////////////////////////////////////////////////////////////////////////////

void InGame::handle_events(const SDL_Event& e)
{
	if(!hide_ui_textbox_)
	{
		//Dialogues
		// 
		//condition placée en premier pour que le scroll de la mouse wheel sur un textbutton fonctionne
		if(e.type == SDL_MOUSEWHEEL)
		{
			if(e.wheel.y > 0) //scroll vers l'avant => reculer d'un dialogue
			{
				get_texttoggle("Skip")->change_checked(false);
				show_dialogue_mouse_wheel(WhichDialogue::previous);
			}
			else //scroll vers l'arrière => avancer d'un dialogue
			{
				show_dialogue_mouse_wheel(WhichDialogue::next);
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
			if(textbox_.text_.is_finished_)
			{
				show_next_dialogue();
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
}


void InGame::draw_characters(sdl::Renderer& renderer)
{
	//Characters
	std::stable_sort(draw_characters_order_.begin(), draw_characters_order_.end(), 
		[&](const std::string& a, const std::string& b) -> bool 
	{ 
		return active_characters_.at(a).properties_.zorder_ < active_characters_.at(b).properties_.zorder_;
	});

	//std::cout << "\n";
	for(std::pair<const std::string, Character>& p : active_characters_)
	{
		for(unsigned int i = current_unique_id_; i != -1; --i)
		{
			if(std::holds_alternative<InfoCharacter>(script_information_[i]))
			{
				InfoCharacter& p_character = std::get<InfoCharacter>(script_information_[i]);
				if(!(p_character.character_variable_.empty()) && p.first == p_character.character_variable_)
				{
					p.second.draw(renderer);
					//std::cout << p_character.character_variable_ << ", " << std::boolalpha << p.second.properties_.is_visible_ << std::endl;
					break;
				}
			}
		}
	}
	//std::cout << "\n";
}

void InGame::draw(sdl::Renderer& renderer)
{
	if(script_information_.size() > 0)
	{
		//Backgrounds
		background_.draw(renderer);
		///////////////////////////

		draw_characters(renderer);
	}

	if(!hide_ui_textbox_)
	{
		textbox_.draw(renderer);
		ui_manager_.draw(renderer);
	}
}


void InGame::update_backgrounds()
{
	//Backgrounds
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(std::holds_alternative<InfoBackground>(script_information_[i]))
		{
			change_background(std::get<InfoBackground>(script_information_[i]));
			break;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update_characters()
{
	//Characters
	for(std::pair<const std::string, Character>& p : active_characters_)
	{
		for(unsigned int i = current_unique_id_; i != -1; --i)
		{
			if(std::holds_alternative<InfoRenameCharacter>(script_information_[i]))
			{
				InfoRenameCharacter& p_character = std::get<InfoRenameCharacter>(script_information_[i]);
				if(p.first == p_character.character_variable_)
				{
					//std::cout << "Rename: " << p.second.properties_.name_ << ", " << p_character.t_ << std::endl;

					p.second.properties_.name_ = p_character.t_;
				}
			}
			else if(std::holds_alternative<InfoCharacter>(script_information_[i]))
			{
				InfoCharacter& p_character = std::get<InfoCharacter>(script_information_[i]);
				if(p.first == p_character.character_variable_)
				{
					if(!p_character.t_.transform_.transform_name_.empty())
					{
						p.second.set_transform(p_character.t_.transform_.transform_name_);
					}

					//std::cout << "Name: " << p.second.properties_.name_ << ", " << p.second.properties_.transform_.transform_name_ << ", " << p_character.t_.transform_.transform_name_ << std::endl;

					p.second.properties_.zorder_ = p_character.t_.zorder_;
					//p.second.properties_.name_ = p_character.t_.name_;
					break;
				}
			}
		}
		p.second.update();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update_autofocus()
{
	//TODO : positionner is_speaking_ dans show_new_dialogue() ??

	//Pour l'autofocus
	bool autofocus = false;
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(std::holds_alternative<InfoAutofocus>(script_information_[i]))
		{
			if(std::get<InfoAutofocus>(script_information_[i]))
			{
				autofocus = true;
				break;
			}
			else
			{
				return;
			}
		}
	}

	//Ne marche que si autofocus vaut par défaut false (constructeur et variable "autofocus")
	if(autofocus == false)
	{
		return;
	}

	std::optional<std::string> last_character_name = get_last_character_name(get_current_dialogue()->character_variable_);
	if(last_character_name.has_value() && !last_character_name.value().empty())
	{
		for(std::pair<const std::string, Character>& p : active_characters_)
		{
			if(p.second.character_definition_->character_variable_ == get_current_dialogue()->character_variable_)
			{
				p.second.properties_.is_speaking_ = true;
				p.second.properties_.zorder_ = 10;
			}
			else
			{
				p.second.properties_.is_speaking_ = false;
				p.second.properties_.zorder_ = 5;
			}
		}
	}
	else //Narrator
	{
		for(std::pair<const std::string, Character>& p : active_characters_)
		{
			p.second.properties_.is_speaking_ = false;
			p.second.properties_.zorder_ = 5;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update_skip_auto_modes()
{
	//Dialogues//////////////////////////////////////////////////////////////////////////////////////
	if(skip_mode_)
	{
		show_next_dialogue();
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
				show_next_dialogue();
				last_time_ = SDL_GetTicks64();
			}
		}
	}
}

void InGame::update_music()
{
	//Musics
	//TODO : regarder s'il faut modifier des choses par rapport à la boucle des sons en-dessous
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(std::holds_alternative<InfoMusic>(script_information_[i]))
		{
			InfoMusic& music_pair = std::get<InfoMusic>(script_information_[i]);
			InGame::AudioProperties& music_properties = music_pair.first;

			if(music_pair.second.has_value()) //play music
			{
				Music& music = music_pair.second.value();

				if(currently_playing_music_ == &music)
				{
					if(!sdl::Music::playing())
					{
						music.change_volume(music_properties.volume);
						music.play_music(music_properties.loop, music_properties.fadein_length);
					}
				}
				else //si une musique est déjà en train de se jouer, il faut stopper (avec ou sans fadeout) la musique courante avant de jouer la nouvelle
				{
					sdl::Music::fade_out(music_properties.fadeout_length);
					currently_playing_music_ = &music;
				}
			}
			else // stop music
			{
				if(sdl::Music::playing())
				{
					sdl::Music::fade_out(music_properties.fadeout_length);
				}
			}
			break;
		}

		//aucune musique trouvée => stopper l'éventuelle musique qui était en train de se jouer
		if(i == 0)
		{
			//si scroll en arrière et aucune musique => fadeout de 1.5 secondes (valeur constante)
			if(sdl::Music::playing())
			{
				sdl::Music::fade_out(constants::fadeout_length_scroll_back_);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update_sounds()
{
	//Sounds
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(std::holds_alternative<InfoSound>(script_information_[i]))
		{
			InfoSound& sound_pair = std::get<InfoSound>(script_information_[i]);
			InGame::AudioProperties& sound_properties = sound_pair.first;

			if(sound_pair.second.has_value()) //play sound
			{
				Sound& sound = sound_pair.second.value();

				if(currently_playing_sound_.sound_ == &sound)
				{
					if(!sdl::Chunk::playing(sound_properties.channel)
					&& ((!sound_properties.loop && !currently_playing_sound_.played_) || sound_properties.loop)) //ne pas rejouer un son qui a déjà été joué s'il ne doit pas être joué en boucle
					{
						//sound.change_volume(sound_properties.volume); //TODO
						sound.play_sound(sound_properties.loop, sound_properties.fadein_length);
						currently_playing_sound_ = {get_current_dialogue_pos(), true, &sound}; 
					}
				}
				else
				{
					//nouveau son à jouer

					//cas d'un scroll arrière et un son était en train de se jouer => l'arrêter
					if(currently_playing_sound_.sound_ && currently_playing_sound_.associated_dialogue_pos_ > i)
					{
						if(sdl::Chunk::playing(currently_playing_sound_.sound_->channel_))
						{
							sdl::Chunk::halt_channel(currently_playing_sound_.sound_->channel_);
						}
					}
					else
					{
						if(sdl::Chunk::playing(sound_properties.channel))
						{
							sdl::Chunk::fade_out(sound_properties.channel, sound_properties.fadeout_length);
						}
					}

					if(current_dialogue_index_ > 0 && i <= get_prev_dialogue_pos()) //on est au moins un dialogue avant le dialogue courant
					{
						currently_playing_sound_ = {get_current_dialogue_pos(), false, nullptr};
					}
					else
					{
						currently_playing_sound_ = {get_current_dialogue_pos(), false, &sound};
					}
				}
			}
			else // stop sound
			{
				if(sdl::Chunk::playing(sound_properties.channel))
				{
					sdl::Chunk::fade_out(sound_properties.channel, sound_properties.fadeout_length);
				}
			}
			break;
		}

		if(i == 0)
		{
			for(auto& s : script_information_)
			{
				if(std::holds_alternative<InfoSound>(s))
				{
					InfoSound& sound_pair = std::get<InfoSound>(s);
					if(sound_pair.second.has_value())
					{
						Sound& sound = sound_pair.second.value();

						if(sdl::Chunk::playing(sound.channel_))
						{
							sdl::Chunk::halt_channel(sound.channel_);
						}
					}
				}
			}
			currently_playing_sound_ = {0, false, nullptr};
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update_textbox()
{
	std::optional<std::string> last_character_name = get_last_character_name(get_current_dialogue()->character_variable_);
	if(last_character_name.has_value() && !last_character_name.value().empty())
	{
		for(std::pair<const std::string, Character>& p : active_characters_)
		{
			if(p.second.character_definition_->character_variable_ == get_current_dialogue()->character_variable_)
			{
				if(!p.second.properties_.textbox_path_.empty()) //TODO : tester aussi la namebox ??
				{
					textbox_.change_textbox(p.second.properties_.textbox_path_, p.second.properties_.namebox_path_, renderer_);
				}
			}
		}
	}
	else //Narrator
	{
		textbox_.change_textbox(constants::textbox_image_, constants::namebox_image_, renderer_);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update()
{
	//TODO : attention, ne marchera plus si je modifie la valeur initiale de current_unique_id_
	if(/*current_unique_id_ == 0 &&*/ current_dialogue_index_ == 0)
	{
		current_unique_id_ = get_current_dialogue_pos();
	}

	if(script_information_.size() > 0)
	{
		//update2();

		/*for(std::string s : draw_characters_order_)
		{
			std::cout << s << std::endl;
		}*/

		/*for(int i = 0; i < script_information_.size(); ++i)
		{
			if(std::holds_alternative<InfoCharacter>(script_information_[i]))
			{
				std::cout << "Transfo: " << std::get<InfoCharacter>(script_information_[i]).t_.transform_.transform_name_ << " " << std::boolalpha << std::get<InfoCharacter>(script_information_[i]).t_.is_speaking_ << std::endl;
			}
		}*/

		/*for(std::pair<const std::string, Character>& p : active_characters_)
		{
			std::cout << p.second.properties_.name_ << ", " << p.second.properties_.zorder_ << std::endl;
			std::cout << p.second.properties_.name_ << ", " << std::boolalpha << p.second.properties_.is_visible_ << std::endl;
		}*/

		update_backgrounds();

		update_characters();

		update_autofocus();

		update_music();

		update_sounds();

		update_skip_auto_modes();

		update_textbox();
	}

	if(currently_playing_music_ != nullptr)
	{
		currently_playing_music_->update();
	}

	if(!hide_ui_textbox_)
	{
		textbox_.update();
		ui_manager_.update();
	}
}

//TODO : utiliser std::visit ??
//TODO : ne pas oublier les cas "i = 0" pour les sons et la musique
void InGame::update2()
{
	/*for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(std::holds_alternative<InfoBackground>(script_information_[i]))
		{
			InfoBackground& info_background = std::get<InfoBackground>(script_information_[i]);
			change_background(info_background);
			break; //TODO : nécessaire pour tous les cas ??
		}
		else if(std::holds_alternative<InfoAutofocus>(script_information_[i]))
		{
			InfoAutofocus& info_autofocus = std::get<InfoAutofocus>(script_information_[i]);
			if(info_autofocus)
			{
				std::optional<std::string> last_character_name = get_last_character_name(get_current_dialogue()->character_variable_);
				if(last_character_name.has_value() && !last_character_name.value().empty())
				{
					for(const MyPair<std::unique_ptr<Character>>& p : character_definitions_)
					{
						if(p.character_variable_ == get_current_dialogue()->character_variable_)
						{
							p.t_->properties_.is_speaking_ = true;
							p.t_->properties_.zorder_ = 10;
						}
						else
						{
							p.t_->properties_.is_speaking_ = false;
							p.t_->properties_.zorder_ = 5;
						}
					}
				}
				else //Narrator
				{
					for(const MyPair<std::unique_ptr<Character>>& p : character_definitions_)
					{
						p.t_->properties_.is_speaking_ = false;
						p.t_->properties_.zorder_ = 5;
					}
				}
			}
			break;
		}
		else if(std::holds_alternative<InfoMusic>(script_information_[i]))
		{
			InfoMusic& info_music = std::get<InfoMusic>(script_information_[i]);
			InGame::AudioProperties& music_properties = info_music.first;

			if(info_music.second.has_value()) //play music
			{
				Music& music = info_music.second.value();
				if(currently_playing_music_ == &music)
				{
					if(!sdl::Music::playing())
					{
						music.change_volume(music_properties.volume);
						music.play_music(music_properties.loop, music_properties.fadein_length);
					}
				}
				else //si une musique est déjà en train de se jouer, il faut stopper (avec ou sans fadeout) la musique courante avant de jouer la nouvelle
				{
					sdl::Music::fade_out(music_properties.fadeout_length);
					currently_playing_music_ = &music;
				}
			}
			else //stop music
			{
				if(sdl::Music::playing())
				{
					sdl::Music::fade_out(music_properties.fadeout_length);
				}
			}
		}
		else if(std::holds_alternative<InfoSound>(script_information_[i]))
		{
			InfoSound& info_sound = std::get<InfoSound>(script_information_[i]);
			InGame::AudioProperties& sound_properties = info_sound.first;

			if(info_sound.second.has_value()) //play sound
			{
				Sound& sound = info_sound.second.value();

				if(currently_playing_sound_.sound_ == &sound)
				{
					if(!sdl::Chunk::playing(sound_properties.channel)
					&& ((!sound_properties.loop && !currently_playing_sound_.played_) || sound_properties.loop)) //ne pas rejouer un son qui a déjà été joué s'il ne doit pas être joué en boucle
					{
						//sound.change_volume(sound_properties.volume); //TODO
						sound.play_sound(sound_properties.loop, sound_properties.fadein_length);
						currently_playing_sound_ = {get_current_dialogue_pos(), true, &sound};
					}
				}
				else
				{
					//nouveau son à jouer

					//cas d'un scroll arrière et un son était en train de se jouer => l'arrêter
					if(currently_playing_sound_.sound_ && currently_playing_sound_.associated_dialogue_pos_ > i)
					{
						if(sdl::Chunk::playing(currently_playing_sound_.sound_->channel_))
						{
							sdl::Chunk::halt_channel(currently_playing_sound_.sound_->channel_);
						}
					}
					else
					{
						if(sdl::Chunk::playing(sound_properties.channel))
						{
							sdl::Chunk::fade_out(sound_properties.channel, sound_properties.fadeout_length);
						}
					}

					if(current_dialogue_index_ > 0 && i <= get_prev_dialogue_pos()) //on est au moins un dialogue avant le dialogue courant
					{
						currently_playing_sound_ = {get_current_dialogue_pos(), false, nullptr};
					}
					else
					{
						currently_playing_sound_ = {get_current_dialogue_pos(), false, &sound};
					}
				}
			}
			else //stop sound
			{
				if(sdl::Chunk::playing(sound_properties.channel))
				{
					sdl::Chunk::fade_out(sound_properties.channel, sound_properties.fadeout_length);
				}
			}
		}
		else if(std::holds_alternative<InfoCharacter>(script_information_[i]))
		{
			InfoCharacter& info_character = std::get<InfoCharacter>(script_information_[i]);

			for(const MyPair<std::unique_ptr<Character>>& p : character_definitions_)
			{
				if(info_character.character_variable_ == p.character_variable_)
				{
					p.t_->set_transform(info_character.t_.transform_.transform_name_);
					//p.t_->properties_.zorder_ = info_character.t_.zorder_;
					p.t_->properties_.name_ = info_character.t_.name_;
				}
				p.t_->update();
			}
		}
		else if(std::holds_alternative<InfoDialogue>(script_information_[i]))
		{
			InfoDialogue& info_dialogue = std::get<InfoDialogue>(script_information_[i]);
		}
	}*/
}
