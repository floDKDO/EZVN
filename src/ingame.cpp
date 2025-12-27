#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"
#include "game.h"

#include <iostream>
#include <algorithm>

//TODO : is_speaking etc. : modifier les characters dans characters_ ou modifier les Character* dans dialogues_ ??
//TODO : cr�er une fonction d'insertion qui incr�mente unique_id_ ?? => poserait s�rement probl�me dans le cas du premier dialogue dans insert_dialogue
//TODO : �viter de r�p�ter plein de fois les m�mes boucles for

InGame::InGame(Game& game, sdl::Renderer& renderer) 
	: GameState(game), unique_id_(0), current_unique_id_(unique_id_), current_unique_id_when_previous_(unique_id_), is_current_unique_id_saved_(false), 
	last_time_(0), textbox_(renderer), background_manager_(unique_id_, current_unique_id_, renderer), hide_ui_textbox_(false), renderer_(renderer), skip_mode_(false), auto_mode_(false), currently_playing_music_(nullptr), currently_playing_sound_({0, false, nullptr})
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


//Dialogues //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InGame::set_initial_dialogue()
{
	textbox_.show_initial_dialogue(dialogues_.begin()->second.t_, get_last_character_name(dialogues_.begin()->second.character_variable_));
}

//Dialogues
std::string InGame::get_last_character_name(const std::string_view character_variable)
{
	if(character_variable.empty())
	{
		return std::string(); //empty string
	}

	unsigned int initial_index_value;
	if(current_unique_id_ == 0)
	{
		initial_index_value = unique_id_; //il faut bien utiliser unique_id_ ici � la place de current_unique_id_, ce dernier valant toujours 0 pour les m�thodes insert_* car le jeu n'a pas encore r�ellement commenc� (= on est encore sur la toute premi�re ligne du script du joueur)
	}
	else
	{
		initial_index_value = current_unique_id_;
	}

	//std::cout << "get_last_character_name: " << character_variable << ", " << initial_index_value << ", " << unique_id_ << ", " << current_unique_id_ << std::endl;

	for(unsigned int i = initial_index_value; i != -1; --i)
	{
		if(characters_transforms_.count(i))
		{
			MyPair<Character::Editableproperties>& p_character = characters_transforms_.at(i);
			if(p_character.character_variable_ == character_variable)
			{
				return p_character.t_.name_;
			}
		}
	}

	//not found yet, get the one inside the "characters_" vector
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		if(p.character_variable_ == character_variable)
		{
			return p.t_->properties_.name_;
		}
	}
	//TODO : quid de ce cas ???
	std::cout << "CAS NON GERE !!!\n";
}

//Dialogues
void InGame::insert_dialogue(const std::string_view character_variable, const std::string_view dialogue)
{
	dialogues_.insert({unique_id_, {std::string(character_variable), dialogue}});

	static bool first_dialogue = true;
	if(first_dialogue)
	{
		set_initial_dialogue();
		first_dialogue = false;
	}

	unique_id_ += 1;
}

//Dialogues
void InGame::show_next_dialogue()
{
	if(dialogues_.count(current_unique_id_))
	{
		if(textbox_.text_.is_finished_) //emp�cher le spam d'espace
		{
			if((std::next(dialogues_.find(current_unique_id_), 1) != dialogues_.end()))
			{
				current_unique_id_ = std::next(dialogues_.find(current_unique_id_), 1)->first;
				is_current_unique_id_saved_ = false; //when we pass a dialogue, reset the mouse wheel dialogues
			}
			textbox_.show_new_dialogue(dialogues_.at(current_unique_id_).t_, get_last_character_name(dialogues_.at(current_unique_id_).character_variable_), skip_mode_, true);
		}
	}
}

//Dialogues
void InGame::show_dialogue_mouse_wheel(WhichDialogue which_dialogue)
{
	SDL_assert(which_dialogue == WhichDialogue::next || which_dialogue == WhichDialogue::previous);

	if(dialogues_.count(current_unique_id_))
	{
		if(which_dialogue == WhichDialogue::next)
		{
			if((std::next(dialogues_.find(current_unique_id_), 1) != dialogues_.end())
			&& (is_current_unique_id_saved_ && current_unique_id_ < current_unique_id_when_previous_))
			{
				current_unique_id_ = std::next(dialogues_.find(current_unique_id_), 1)->first;
			}
		}
		else if(which_dialogue == WhichDialogue::previous)
		{
			if(!is_current_unique_id_saved_)
			{
				current_unique_id_when_previous_ = current_unique_id_;
				is_current_unique_id_saved_ = true;
			}

			if(dialogues_.find(current_unique_id_) != dialogues_.begin())
			{
				current_unique_id_ = std::prev(dialogues_.find(current_unique_id_), 1)->first;
			}

			//si on arrive sur le dialogue dont est associ� un son, il faut le rejouer
			if(current_unique_id_ == dialogues_.upper_bound(currently_playing_sound_.key_)->first)
			{
				currently_playing_sound_.played_ = false;
			}
		}
		textbox_.show_new_dialogue(dialogues_.at(current_unique_id_).t_, get_last_character_name(dialogues_.at(current_unique_id_).character_variable_), skip_mode_, false);
	}
}
////////////////////////////////////////////////////////////////////////////////////


//Character//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InGame::add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path)
{
	characters_.push_back({std::string(character_variable), std::make_unique<Character>(character_name, character_path, renderer, textbox_path, namebox_path)});
}

//Character
Character::Editableproperties InGame::get_last_character_properties(const std::string_view character_variable)
{
	//TODO : que faire quand character_variable = "" ?? => erreur

	for(unsigned int i = unique_id_; i != -1; --i) //il faut bien utiliser unique_id_ ici � la place de current_unique_id_, ce dernier valant toujours 0 pour les m�thodes insert_* car le jeu n'a pas encore r�ellement commenc� (= on est encore sur la toute premi�re ligne du script du joueur)
	{
		if(characters_transforms_.count(i))
		{
			MyPair<Character::Editableproperties>& p_character = characters_transforms_.at(i);
			if(p_character.character_variable_ == character_variable)
			{
				return p_character.t_;
			}
		}
	}

	//not found yet, get the one inside the "characters_" vector
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		if(p.character_variable_ == character_variable)
		{
			return p.t_->properties_;
		}
	}
	//TODO : quid de ce cas ???
	std::cout << "CAS NON GERE !!!\n";
}

//Character
void InGame::insert_character(const std::string_view character_variable, const std::optional<std::string> new_character_name, const std::optional<std::string> transform_name, const std::optional<int> zorder)
{
	Character::Editableproperties character_properties = get_last_character_properties(character_variable);
	if(new_character_name.has_value())
	{
		character_properties.name_ = new_character_name.value();
	}
	if(transform_name.has_value())
	{
		character_properties.transform_.transform_name_ = transform_name.value();
	}
	if(zorder.has_value())
	{
		character_properties.zorder_ = zorder.value();
	}

	characters_transforms_.insert({unique_id_, {std::string(character_variable), character_properties}});
	unique_id_ += 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Sounds////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InGame::insert_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, int volume, int channel, bool loop)
{
	if(sound_path.empty()) //stop music
	{
		sounds_.insert({unique_id_, std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop, channel}, std::nullopt)}); //TODO : std::make_pair a l'air obligatoire ici
	}
	else //play music
	{
		sounds_.insert({unique_id_, std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop, channel}, Sound(sound_path, channel, volume))});
	}
	unique_id_ += 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


//Musics/////////////////////////////////////////////////////////////////////////////////////////////
void InGame::insert_music(const std::string_view music_path, int fadein_length, int fadeout_length, int volume, bool loop)
{
	if(music_path.empty()) //stop music
	{
		musics_.insert({unique_id_, std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop}, std::nullopt)}); //TODO : std::make_pair a l'air obligatoire ici
	}
	else //play music
	{
		musics_.insert({unique_id_, std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop}, Music(music_path))});
	}
	unique_id_ += 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


void InGame::insert_autofocus(bool autofocus)
{
	autofocus_.insert({unique_id_, autofocus});
	unique_id_ += 1;
}

void InGame::handle_events(const SDL_Event& e)
{
	if(!hide_ui_textbox_)
	{
		//Dialogues
		//condition plac�e en premier pour que le scroll de la mouse wheel sur un textbutton fonctionne
		if(e.type == SDL_MOUSEWHEEL)
		{
			if(e.wheel.y > 0) //scroll vers l'avant => reculer d'un dialogue
			{
				get_texttoggle("Skip")->change_checked(false);
				show_dialogue_mouse_wheel(WhichDialogue::previous);
			}
			else //scroll vers l'arri�re => avancer d'un dialogue
			{
				show_dialogue_mouse_wheel(WhichDialogue::next);
			}
		}

		ui_manager_.handle_events(e);
		if(ui_manager_.is_mouse_on_ui_)
		{
			return; //si collision avec un textbutton, ne pas g�rer les �v�nements "clic" et "espace" de la Textbox (= ne pas passer au prochain dialogue)
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
				textbox_.text_.is_animated_ = false; //afficher le dialogue en entier apr�s un clic / touche espace sur un dialogue en train de s'afficher
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
	
	/*std::cout << "AVANT TRI\n";
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		std::cout << p.t_->properties_.name_ << ", zorder: " << p.t_->properties_.zorder_ << std::endl;
	}*/

	//TODO : co�teux car r�alis� � chaque tour de boucle...
	std::stable_sort(characters_.begin(), characters_.end(), [&](const MyPair<std::unique_ptr<Character>>& a, const MyPair<std::unique_ptr<Character>>& b) -> bool { return a.t_->properties_.zorder_ < b.t_->properties_.zorder_; });
	
	/*std::cout << "\nAPRES TRI\n";
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		std::cout << p.t_->properties_.name_ << ", zorder: " << p.t_->properties_.zorder_ << std::endl;
	}*/

	//TODO : co�teux
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(characters_transforms_.count(i))
		{
			MyPair<Character::Editableproperties>& p_character = characters_transforms_.at(i);
			if(!(p_character.character_variable_.empty()))
			{
				//std::cout << "\n";
				for(const MyPair<std::unique_ptr<Character>>& p : characters_)
				{
					//std::cout << p.character_variable_ << " et " << p_character.character_variable_ << std::endl;
					if(p.character_variable_ == p_character.character_variable_) //TODO : il faut commenter cette ligne pour que l'autozorder fonctionne...
					{
						//std::cout << p.character_variable_ << std::endl;
						p.t_->draw(renderer);
					}
				}
				//std::cout << "\n";
			}
		}
	}
	///////////////////////////////////////////////////////////////////
}

void InGame::draw(sdl::Renderer& renderer)
{
	background_manager_.draw(); //TODO : cohérence => paramètre renderer

	draw_characters(renderer);

	if(!hide_ui_textbox_)
	{
		textbox_.draw(renderer);
		ui_manager_.draw(renderer);
	}
}

void InGame::update_characters()
{
	//Characters
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		for(unsigned int i = current_unique_id_; i != -1; --i)
		{
			if(characters_transforms_.count(i))
			{
				MyPair<Character::Editableproperties>& p_character = characters_transforms_.at(i);
				if(p_character.character_variable_ == p.character_variable_)
				{
					p.t_->set_transform(p_character.t_.transform_.transform_name_);
					//p.t_->properties_.zorder_ = p_character.t_.zorder_;
					p.t_->properties_.name_ = p_character.t_.name_;
					break;
				}
			}
		}
		//std::cout << p.t_->properties_.name_ << ", zorder: " << p.t_->properties_.zorder_ << std::endl;
		p.t_->update();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update_autofocus()
{
	//TODO : pour transform et zorder de l'autofocus, utiliser characters_transforms_
	//TODO : positionner is_speaking_ dans show_new_dialogue() ?? Si non, aucun int�r�t de passer un Character* => une string suffirait

	//Pour l'autofocus
	bool autofocus = false;
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(autofocus_.count(i))
		{
			if(autofocus_.at(i))
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

	//Ne marche que si autofocus vaut par d�faut false (constructeur et variable "autofocus")
	if(autofocus == false)
	{
		return;
	}

	if(dialogues_.count(current_unique_id_))
	{
		if(!(get_last_character_name(dialogues_.at(current_unique_id_).character_variable_).empty()))
		{
			for(const MyPair<std::unique_ptr<Character>>& p : characters_)
			{
				//std::cout << pair.first << ", " << dialogues_.at(current_unique_id_).second.second << ", " << dialogues_.at(current_unique_id_).second.first << std::endl;
				if(p.character_variable_ == dialogues_.at(current_unique_id_).character_variable_)
				{
					p.t_->is_speaking_ = true;
					p.t_->properties_.zorder_ = 10;
				}
				else
				{
					p.t_->is_speaking_ = false;
					p.t_->properties_.zorder_ = 5;
				}
			}
		}
		else //Narrator
		{
			for(const MyPair<std::unique_ptr<Character>>& p : characters_)
			{
				p.t_->is_speaking_ = false;
				p.t_->properties_.zorder_ = 5;
			}
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
	//TODO : regarder s'il faut modifier des choses par rapport � la boucle des sons en-dessous
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(musics_.count(i))
		{
			std::pair<AudioProperties, std::optional<Music>>& music_pair = musics_.at(i);
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
				else //si une musique est d�j� en train de se jouer, il faut stopper (avec ou sans fadeout) la musique courante avant de jouer la nouvelle
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

		//aucune musique trouv�e => stopper l'�ventuelle musique qui �tait en train de se jouer
		if(i == 0)
		{
			//si scroll en arri�re et aucune musique => fadeout de 1.5 secondes (valeur constante)
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
		if(sounds_.count(i))
		{
			std::pair<AudioProperties, std::optional<Sound>>& sound_pair = sounds_.at(i);
			InGame::AudioProperties& sound_properties = sound_pair.first;

			if(sound_pair.second.has_value()) //play sound
			{
				Sound& sound = sound_pair.second.value();

				if(currently_playing_sound_.sound_ == &sound)
				{
					if(!sdl::Chunk::playing(sound_properties.channel)
					&& ((!sound_properties.loop && !currently_playing_sound_.played_) || sound_properties.loop)) //ne pas rejouer un son qui a d�j� �t� jou� s'il ne doit pas �tre jou� en boucle
					{
						//sound.change_volume(sound_properties.volume); //TODO
						sound.play_sound(sound_properties.loop, sound_properties.fadein_length);
						currently_playing_sound_ = {i, true, &sound};
					}
				}
				else
				{
					//nouveau son � jouer

					//cas d'un scroll arri�re et un son �tait en train de se jouer => l'arr�ter
					if(currently_playing_sound_.sound_ && currently_playing_sound_.key_ > i)
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

					if(dialogues_.find(current_unique_id_) != dialogues_.begin()
					&& i <= std::prev(dialogues_.find(current_unique_id_))->first) //on est au moins un dialogue avant le dialogue courant
					{
						currently_playing_sound_ = {i, false, nullptr};
					}
					else
					{
						currently_playing_sound_ = {i, false, &sound};
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
			for(auto& p : sounds_)
			{
				std::pair<AudioProperties, std::optional<Sound>>& sound_pair = p.second;
				if(sound_pair.second.has_value())
				{
					Sound& sound = p.second.second.value();

					if(sdl::Chunk::playing(sound.channel_))
					{
						std::cout << "HALT\n";
						sdl::Chunk::halt_channel(sound.channel_);
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
	if(dialogues_.count(current_unique_id_))
	{
		if(!(get_last_character_name(dialogues_.at(current_unique_id_).character_variable_).empty()))
		{
			for(const MyPair<std::unique_ptr<Character>>& p : characters_)
			{
				if(p.character_variable_ == dialogues_.at(current_unique_id_).character_variable_)
				{
					if(!p.t_->properties_.textbox_path_.empty())
					{
						textbox_.change_textbox(p.t_->properties_.textbox_path_, p.t_->properties_.namebox_path_, renderer_);
					}
				}
			}
		}
		else //Narrator
		{
			textbox_.change_textbox(constants::textbox_image_, constants::namebox_image_, renderer_);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void InGame::update()
{
	background_manager_.update();

	update_characters();

	update_autofocus();

	update_music();

	update_sounds();

	update_skip_auto_modes();

	update_textbox();

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