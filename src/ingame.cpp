#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"
#include "game.h"

#include <iostream>
#include <algorithm>

//TODO : is_speaking etc. : modifier les characters dans characters_ ou modifier les Character* dans dialogues_ ??
//TODO : éviter de répéter plein de fois les mêmes boucles for
//TODO : "InGame::" devant les types pas nécessaire ?? (ex : InGame::AudioProperties <=> AudioProperties ??)

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game), current_unique_id_(0), current_unique_id_when_previous_(current_unique_id_), is_current_unique_id_saved_(false),
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

InGame::InfoDialogue* InGame::get_first_dialogue()
{
	for(unsigned int i = 0; i < script_information_.size(); ++i)
	{
		if(std::holds_alternative<InfoDialogue>(script_information_[i]))
		{
			return std::get_if<InfoDialogue>(&script_information_[i]);
		}
	}
	return nullptr;
}

InGame::InfoDialogue* InGame::get_dialogue(unsigned int current_unique_id)
{
	return std::get_if<InfoDialogue>(&script_information_[current_unique_id]);
}

InGame::InfoDialogue* InGame::get_next_dialogue(unsigned int current_unique_id)
{
	for(unsigned int i = current_unique_id + 1; i < script_information_.size(); ++i)
	{
		if(std::holds_alternative<InfoDialogue>(script_information_[i]))
		{
			return std::get_if<InfoDialogue>(&script_information_[i]);
		}
	}
	return nullptr;
}

std::optional<unsigned int> InGame::get_id_next_dialogue(unsigned int current_unique_id)
{
	for(unsigned int i = current_unique_id + 1; i < script_information_.size(); ++i)
	{
		if(std::holds_alternative<InfoDialogue>(script_information_[i]))
		{
			return i;
		}
	}
	return std::nullopt;
}

InGame::InfoDialogue* InGame::get_prev_dialogue(unsigned int current_unique_id)
{
	for(unsigned int i = current_unique_id - 1; i != -1; --i)
	{
		if(std::holds_alternative<InfoDialogue>(script_information_[i]))
		{
			return std::get_if<InfoDialogue>(&script_information_[i]);
		}
	}
	return nullptr;
}

std::optional<unsigned int> InGame::get_id_prev_dialogue(unsigned int current_unique_id)
{
	for(unsigned int i = current_unique_id - 1; i != -1; --i)
	{
		if(std::holds_alternative<InfoDialogue>(script_information_[i]))
		{
			return i;
		}
	}
	return std::nullopt;
}

//Dialogues //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InGame::set_initial_dialogue()
{
	if(get_first_dialogue() != nullptr && get_last_character_name(get_first_dialogue()->character_variable_).has_value())
	{
		textbox_.show_initial_dialogue(get_first_dialogue()->t_, get_last_character_name(get_first_dialogue()->character_variable_).value());
	}
}

//Dialogues
std::optional<std::string> InGame::get_last_character_name(const std::string_view character_variable)
{
	//TODO : narrateur ou erreur car character_variable vide => est-ce que le narrateur a une character_variable vide ??
	if(character_variable.empty())
	{
		return std::string(); //empty string
	}

	for(size_t i = script_information_.size() - 1; i != -1; --i) //si script_information_.size() == 0, on n'entre pas dans cette boucle
	{
		if(std::holds_alternative<InfoCharacter>(script_information_[i]))
		{
			InfoCharacter& info_character = std::get<InfoCharacter>(script_information_[i]);
			if(info_character.character_variable_ == character_variable)
			{
				return info_character.t_.name_;
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

	return std::nullopt;
}

//Dialogues
void InGame::insert_dialogue(const std::string_view character_variable, const std::string_view dialogue)
{
	script_information_.push_back(InfoDialogue({std::string(character_variable), dialogue}));

	static bool first_dialogue = true;
	if(first_dialogue)
	{
		set_initial_dialogue();
		first_dialogue = false;
	}
}

//Dialogues
void InGame::show_next_dialogue()
{
	InfoDialogue* info_dialogue = get_dialogue(current_unique_id_);
	if(info_dialogue != nullptr)
	{
		if(textbox_.text_.is_finished_) //empêcher le spam d'espace
		{
			if(get_next_dialogue(current_unique_id_) != nullptr && get_id_next_dialogue(current_unique_id_).has_value())
			{
				current_unique_id_ = get_id_next_dialogue(current_unique_id_).value();
				is_current_unique_id_saved_ = false; //when we pass a dialogue, reset the mouse wheel dialogues
			}
			if(get_last_character_name(get_dialogue(current_unique_id_)->character_variable_).has_value())
			{
				textbox_.show_new_dialogue(get_dialogue(current_unique_id_)->t_, get_last_character_name(get_dialogue(current_unique_id_)->character_variable_).value(), skip_mode_, true);
			}
		}
	}
}

//Dialogues
void InGame::show_dialogue_mouse_wheel(WhichDialogue which_dialogue)
{
	SDL_assert(which_dialogue == WhichDialogue::next || which_dialogue == WhichDialogue::previous);

	if(get_dialogue(current_unique_id_) != nullptr)
	{
		if(which_dialogue == WhichDialogue::next)
		{
			if(get_next_dialogue(current_unique_id_) != nullptr && get_id_next_dialogue(current_unique_id_).has_value()
			&& (is_current_unique_id_saved_ && current_unique_id_ < current_unique_id_when_previous_))
			{
				current_unique_id_ = get_id_next_dialogue(current_unique_id_).value();
			}
		}
		else if(which_dialogue == WhichDialogue::previous)
		{
			if(!is_current_unique_id_saved_)
			{
				current_unique_id_when_previous_ = current_unique_id_;
				is_current_unique_id_saved_ = true;
			}

			if(get_dialogue(current_unique_id_) != get_first_dialogue() && get_id_prev_dialogue(current_unique_id_).has_value())
			{
				current_unique_id_ = get_id_prev_dialogue(current_unique_id_).value();
			}

			//si on arrive sur le dialogue dont est associé un son, il faut le rejouer
			if(get_id_next_dialogue(currently_playing_sound_.key_).has_value() && current_unique_id_ == get_id_next_dialogue(currently_playing_sound_.key_).value())
			{
				currently_playing_sound_.played_ = false;
			}
		}
		if(get_last_character_name(get_dialogue(current_unique_id_)->character_variable_).has_value())
		{
			textbox_.show_new_dialogue(get_dialogue(current_unique_id_)->t_, get_last_character_name(get_dialogue(current_unique_id_)->character_variable_).value(), skip_mode_, false);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////


//Character//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InGame::add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path)
{
	characters_.push_back({std::string(character_variable), std::make_unique<Character>(character_name, character_path, renderer, textbox_path, namebox_path)});
}

//Character
std::optional<Character::Editableproperties> InGame::get_last_character_properties(const std::string_view character_variable)
{
	//TODO : que faire quand character_variable = "" ?? => erreur

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

	//not found yet, get the one inside the "characters_" vector
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		if(p.character_variable_ == character_variable)
		{
			return p.t_->properties_;
		}
	}
	
	return std::nullopt;
}

//Character
void InGame::insert_character(const std::string_view character_variable, const std::optional<std::string> new_character_name, const std::optional<std::string> transform_name, const std::optional<int> zorder)
{
	std::optional<Character::Editableproperties> character_properties_opt = get_last_character_properties(character_variable);

	if(character_properties_opt.has_value())
	{
		Character::Editableproperties character_properties = character_properties_opt.value();
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

		script_information_.push_back(InfoCharacter({std::string(character_variable), character_properties}));
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Background/////////////////////////////////////////////////////////////////////////////////// 
void InGame::insert_background(const std::string_view background_path)
{
	script_information_.push_back(InfoBackground(Background(background_path, renderer_)));
}

//Background
void InGame::insert_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
	script_information_.push_back(InfoBackground(Background(r, g, b, a)));
}

//Background
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


//Sounds////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////


//Musics/////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////


void InGame::insert_autofocus(bool autofocus)
{
	script_information_.push_back(InfoAutofocus(autofocus));
}

void InGame::handle_events(const SDL_Event& e)
{
	if(!hide_ui_textbox_)
	{
		//Dialogues
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

	/*std::cout << "AVANT TRI\n";
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		std::cout << p.t_->properties_.name_ << ", zorder: " << p.t_->properties_.zorder_ << std::endl;
	}*/

	//TODO : coûteux car réalisé à chaque tour de boucle...
	std::stable_sort(characters_.begin(), characters_.end(), [&](const MyPair<std::unique_ptr<Character>>& a, const MyPair<std::unique_ptr<Character>>& b) -> bool { return a.t_->properties_.zorder_ < b.t_->properties_.zorder_; });

	/*std::cout << "\nAPRES TRI\n";
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		std::cout << p.t_->properties_.name_ << ", zorder: " << p.t_->properties_.zorder_ << std::endl;
	}*/

	//TODO : coûteux
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(std::holds_alternative<InfoCharacter>(script_information_[i]))
		{
			InfoCharacter& p_character = std::get<InfoCharacter>(script_information_[i]);
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
	//Backgrounds
	background_.draw(renderer);
	///////////////////////////

	draw_characters(renderer);

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
	for(const MyPair<std::unique_ptr<Character>>& p : characters_)
	{
		for(unsigned int i = current_unique_id_; i != -1; --i)
		{
			if(std::holds_alternative<InfoCharacter>(script_information_[i]))
			{
				InfoCharacter& p_character = std::get<InfoCharacter>(script_information_[i]);
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
	//TODO : positionner is_speaking_ dans show_new_dialogue() ?? Si non, aucun intérêt de passer un Character* => une string suffirait

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

	if(get_dialogue(current_unique_id_) != nullptr)
	{
		std::optional<std::string> last_character_name = get_last_character_name(get_dialogue(current_unique_id_)->character_variable_);
		if(last_character_name.has_value() && !last_character_name.value().empty())
		{
			for(const MyPair<std::unique_ptr<Character>>& p : characters_)
			{
				if(p.character_variable_ == get_dialogue(current_unique_id_)->character_variable_)
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
						currently_playing_sound_ = {i, true, &sound};
					}
				}
				else
				{
					//nouveau son à jouer

					//cas d'un scroll arrière et un son était en train de se jouer => l'arrêter
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

					if(get_dialogue(current_unique_id_) != get_first_dialogue() && get_id_prev_dialogue(current_unique_id_).has_value()
					&& i <= get_id_prev_dialogue(current_unique_id_).value()) //on est au moins un dialogue avant le dialogue courant
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
			for(auto& e : script_information_)
			{
				if(std::holds_alternative<InfoSound>(e))
				{
					InfoSound& sound_pair = std::get<InfoSound>(e);
					if(sound_pair.second.has_value())
					{
						Sound& sound = sound_pair.second.value();

						if(sdl::Chunk::playing(sound.channel_))
						{
							std::cout << "HALT\n";
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
	if(get_dialogue(current_unique_id_) != nullptr)
	{
		std::optional<std::string> last_character_name = get_last_character_name(get_dialogue(current_unique_id_)->character_variable_);
		if(last_character_name.has_value() && !last_character_name.value().empty())
		{
			for(const MyPair<std::unique_ptr<Character>>& p : characters_)
			{
				if(p.character_variable_ == get_dialogue(current_unique_id_)->character_variable_)
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

//TODO : utiliser std::visit ??
//TODO : ne pas oublier les cas "i = 0" pour les sons et la musique
//void InGame::update()
//{
//	for(unsigned int i = current_unique_id_; i != -1; --i)
//	{
//		if(std::holds_alternative<InfoBackground>(script_information_[i]))
//		{
//			InfoBackground& info_background = std::get<InfoBackground>(script_information_[i]);
//			change_background(info_background);
//			break; //TODO : nécessaire pour tous les cas ??
//		}
//		else if(std::holds_alternative<InfoAutofocus>(script_information_[i]))
//		{
//			InfoAutofocus& info_autofocus = std::get<InfoAutofocus>(script_information_[i]);
//			if(info_autofocus)
//			{
//				update_autofocus();
//			}
//			break;
//		}
//		else if(std::holds_alternative<InfoMusic>(script_information_[i]))
//		{
//			InfoMusic& info_music = std::get<InfoMusic>(script_information_[i]);
//			InGame::AudioProperties& music_properties = info_music.first;
//
//			if(info_music.second.has_value()) //play music
//			{
//				Music& music = info_music.second.value();
//				if(currently_playing_music_ == &music)
//				{
//					if(!sdl::Music::playing())
//					{
//						music.change_volume(music_properties.volume);
//						music.play_music(music_properties.loop, music_properties.fadein_length);
//					}
//				}
//				else //si une musique est déjà en train de se jouer, il faut stopper (avec ou sans fadeout) la musique courante avant de jouer la nouvelle
//				{
//					sdl::Music::fade_out(music_properties.fadeout_length);
//					currently_playing_music_ = &music;
//				}
//			}
//			else //stop music
//			{
//				if(sdl::Music::playing())
//				{
//					sdl::Music::fade_out(music_properties.fadeout_length);
//				}
//			}
//		}
//		else if(std::holds_alternative<InfoSound>(script_information_[i]))
//		{
//			InfoSound& info_sound = std::get<InfoSound>(script_information_[i]);
//			InGame::AudioProperties& sound_properties = info_sound.first;
//
//			if(info_sound.second.has_value()) //play sound
//			{
//				Sound& sound = info_sound.second.value();
//
//				if(currently_playing_sound_.sound_ == &sound)
//				{
//					if(!sdl::Chunk::playing(sound_properties.channel)
//					&& ((!sound_properties.loop && !currently_playing_sound_.played_) || sound_properties.loop)) //ne pas rejouer un son qui a déjà été joué s'il ne doit pas être joué en boucle
//					{
//						//sound.change_volume(sound_properties.volume); //TODO
//						sound.play_sound(sound_properties.loop, sound_properties.fadein_length);
//						currently_playing_sound_ = {i, true, &sound};
//					}
//				}
//				else
//				{
//					//nouveau son à jouer
//
//					//cas d'un scroll arrière et un son était en train de se jouer => l'arrêter
//					if(currently_playing_sound_.sound_ && currently_playing_sound_.key_ > i)
//					{
//						if(sdl::Chunk::playing(currently_playing_sound_.sound_->channel_))
//						{
//							sdl::Chunk::halt_channel(currently_playing_sound_.sound_->channel_);
//						}
//					}
//					else
//					{
//						if(sdl::Chunk::playing(sound_properties.channel))
//						{
//							sdl::Chunk::fade_out(sound_properties.channel, sound_properties.fadeout_length);
//						}
//					}
//
//					if(/*dialogues_.find(current_unique_id_) != dialogues_.begin()
//					&&*/ i <= get_id_prev_dialogue(current_unique_id_)) //on est au moins un dialogue avant le dialogue courant
//					{
//						currently_playing_sound_ = {i, false, nullptr};
//					}
//					else
//					{
//						currently_playing_sound_ = {i, false, &sound};
//					}
//				}
//			}
//			else //stop sound
//			{
//				if(sdl::Chunk::playing(sound_properties.channel))
//				{
//					sdl::Chunk::fade_out(sound_properties.channel, sound_properties.fadeout_length);
//				}
//			}
//		}
//		else if(std::holds_alternative<InfoCharacter>(script_information_[i]))
//		{
//			InfoCharacter& info_character = std::get<InfoCharacter>(script_information_[i]);
//
//			for(const MyPair<std::unique_ptr<Character>>& p : characters_)
//			{
//				if(info_character.character_variable_ == p.character_variable_)
//				{
//					p.t_->set_transform(info_character.t_.transform_.transform_name_);
//					//p.t_->properties_.zorder_ = info_character.t_.zorder_;
//					p.t_->properties_.name_ = info_character.t_.name_;
//				}
//				p.t_->update();
//			}
//		}
//		else if(std::holds_alternative<InfoDialogue>(script_information_[i]))
//		{
//			InfoDialogue& info_dialogue = std::get<InfoDialogue>(script_information_[i]);
//		}
//	}
//}

void InGame::update()
{
	update_backgrounds();

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
