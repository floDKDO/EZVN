#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"
#include "game.h"

#include <iostream>
#include <algorithm>

//TODO : is_speaking etc. : modifier les characters dans characters_ ou modifier les Character* dans dialogues_ ??
//TODO : créer une fonction d'insertion qui incrémente unique_id_ ?? => poserait sûrement problème dans le cas du premier dialogue dans insert_dialogue
//TODO : utiliser des variables à la place de map.first.second. ... pour rendre le code plus lisible

InGame::InGame(Game& game, sdl::Renderer& renderer) 
	: GameState(game), unique_id_(0), current_unique_id_(unique_id_), current_unique_id_when_previous_(unique_id_), is_current_unique_id_saved_(false), 
	last_time_(0), textbox_(renderer), background_(0, 0, 0, 255), hide_ui_textbox_(false), renderer_(renderer), skip_mode_(false), auto_mode_(false), music_(nullptr), sound_(nullptr)
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

void InGame::set_initial_dialogue()
{
	textbox_.show_initial_dialogue(dialogues_.begin()->second.first, get_last_character_name(dialogues_.begin()->second.second)); 
}

void InGame::add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path)
{
	characters_.push_back({std::string(character_variable), std::make_unique<Character>(character_name, character_path, renderer, textbox_path, namebox_path)});
}

Character::Editableproperties InGame::get_last_character_properties(const std::string_view character_variable)
{
	//TODO : que faire quand character_variable = "" ??

	for(unsigned int i = unique_id_; i != -1; --i) //il faut bien utiliser unique_id_ ici à la place de current_unique_id_, ce dernier valant toujours 0 pour les méthodes insert_* car le jeu n'a pas encore réellement commencé (= on est encore sur la toute première ligne du script du joueur)
	{
		if(characters_transforms_.count(i) && characters_transforms_.at(i).first == character_variable)
		{
			return characters_transforms_.at(i).second;
		}
	}

	//not found yet, get the one inside the "characters_" vector
	for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
	{
		if(pair.first == character_variable)
		{
			return pair.second->properties_;
		}
	}
	//TODO : quid de ce cas ???
	std::cout << "CAS NON GERE !!!\n";
}

std::string InGame::get_last_character_name(const std::string_view character_variable)
{
	if(character_variable.empty())
	{
		return "";
	}

	unsigned int initial_index_value;
	if(current_unique_id_ == 0)
	{
		initial_index_value = unique_id_; //il faut bien utiliser unique_id_ ici à la place de current_unique_id_, ce dernier valant toujours 0 pour les méthodes insert_* car le jeu n'a pas encore réellement commencé (= on est encore sur la toute première ligne du script du joueur)
	}
	else
	{
		initial_index_value = current_unique_id_;
	}

	//std::cout << "get_last_character_name: " << character_variable << ", " << initial_index_value << ", " << unique_id_ << ", " << current_unique_id_ << std::endl;

	for(unsigned int i = initial_index_value; i != -1; --i)
	{
		if(characters_transforms_.count(i) && characters_transforms_.at(i).first == character_variable)
		{
			return characters_transforms_.at(i).second.name_;
		}
	}

	//not found yet, get the one inside the "characters_" vector
	for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
	{
		if(pair.first == character_variable)
		{
			return pair.second->properties_.name_;
		}
	}
	//TODO : quid de ce cas ???
	std::cout << "CAS NON GERE !!!\n";
}

void InGame::insert_dialogue(const std::string_view character_variable, const std::string_view dialogue)
{
	if(character_variable == "")
	{
		dialogues_.insert({unique_id_, {dialogue, std::string(character_variable)}}); 
	}
	else
	{
		dialogues_.insert({unique_id_, {dialogue, std::string(character_variable)}});
	}

	static bool first_dialogue = true;
	if(first_dialogue)
	{
		set_initial_dialogue();
		first_dialogue = false;
	}

	unique_id_ += 1;
}

void InGame::insert_background(const std::string_view background_path)
{
	backgrounds_.insert({unique_id_, Background(background_path, renderer_)}); 
	unique_id_ += 1;
}

void InGame::insert_background(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
	backgrounds_.insert({unique_id_, Background(r, g, b, a)}); 
	unique_id_ += 1;
}

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

void InGame::insert_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, int volume, int channel, bool loop)
{
	if(sound_path.empty()) //stop music
	{
		sounds_.insert({unique_id_, std::make_pair(SoundProperties{fadein_length, fadeout_length, volume, loop, channel}, std::nullopt)}); //TODO : std::make_pair a l'air obligatoire ici
	}
	else //play music
	{
		sounds_.insert({unique_id_, std::make_pair(SoundProperties{fadein_length, fadeout_length, volume, loop, channel}, Sound(sound_path, channel))});
	}
	unique_id_ += 1;
}

void InGame::insert_music(const std::string_view music_path, int fadein_length, int fadeout_length, int volume, bool loop)
{
	if(music_path.empty()) //stop music
	{
		musics_.insert({unique_id_, std::make_pair(MusicProperties{fadein_length, fadeout_length, volume, loop}, std::nullopt)}); //TODO : std::make_pair a l'air obligatoire ici
	}
	else //play music
	{
		musics_.insert({unique_id_, std::make_pair(MusicProperties{fadein_length, fadeout_length, volume, loop}, Music(music_path))});
	}
	unique_id_ += 1;
}

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

void InGame::show_next_dialogue()
{
	if(dialogues_.count(current_unique_id_))
	{
		if(textbox_.text_.is_finished_) //empêcher le spam d'espace
		{
			if((std::next(dialogues_.find(current_unique_id_), 1) != dialogues_.end()))
			{
				current_unique_id_ = std::next(dialogues_.find(current_unique_id_), 1)->first;
				is_current_unique_id_saved_ = false; //when we pass a dialogue, reset the mouse wheel dialogues
			}

			//std::cout << "DANS NEXT DIALOGUE: " << current_unique_id_ << std::endl;
			//for(unsigned int i = current_unique_id_; i != -1; --i)
			//{
			//	if(characters_transforms_.count(i) && dialogues_[current_unique_id_].second.first == characters_transforms_.at(i).first)
			//	{
			//		std::cout << "i: " << i << ", " << characters_transforms_.at(i).second.name_ << ", " << dialogues_[current_unique_id_].second.second << std::endl;
			//		break;
			//	}
			//}

			//for(unsigned int i = current_unique_id_; i != -1; --i)
			//{
			//	if(dialogues_.count(i))
			//	{
			//		std::cout << "Nom: " << dialogues_[i].first << std::endl;
			//	}
			//}

			textbox_.show_new_dialogue(dialogues_[current_unique_id_].first, get_last_character_name(dialogues_[current_unique_id_].second), skip_mode_, true);
		}
	}
}

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
		}
		textbox_.show_new_dialogue(dialogues_[current_unique_id_].first, get_last_character_name(dialogues_[current_unique_id_].second), skip_mode_, false);
	}
}

void InGame::handle_events(const SDL_Event& e)
{
	if(!hide_ui_textbox_)
	{
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

void InGame::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);

	//TODO : coûteux car réalisé à chaque tour de boucle...
	std::stable_sort(characters_.begin(), characters_.end(), [&](const std::pair<std::string, std::unique_ptr<Character>>& a, const std::pair<std::string, std::unique_ptr<Character>>& b) -> bool { return a.second->properties_.zorder_ < b.second->properties_.zorder_; });

	//TODO : coûteux
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(characters_transforms_.count(i) && !(characters_transforms_.at(i).first.empty()))
		{
			//std::cout << characters_transforms_.at(i).first << std::endl;
			for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
			{
				if(pair.first == characters_transforms_.at(i).first)
				{
					pair.second->draw(renderer);
				}
			}
		}
	}

	if(!hide_ui_textbox_)
	{
		textbox_.draw(renderer);
		ui_manager_.draw(renderer);
	}
}

//TODO : créer des fonctions pour chaque bloc
void InGame::update()
{
	//TODO : pour debug
	//std::cout << current_unique_id_ << std::endl;

	//std::cout << "DEBUT\n";
	/*for(auto it = dialogues_.begin(); it != dialogues_.end(); ++it)
	{
		if(dialogues_.count(it->first))
		{
			std::cout << it->first << std::endl;
		}
	}*/
	/*for(auto it = backgrounds_.begin(); it != backgrounds_.end(); ++it)
	{
		if(backgrounds_.count(it->first))
		{
			std::cout << it->first << std::endl;
		}
	}*/
	/*for(auto it = characters_transforms_.begin(); it != characters_transforms_.end(); ++it)
	{
		if(characters_transforms_.count(it->first))
		{
			std::cout << it->first << std::endl;
		}
	}*/
	//std::cout << "FIN\n\n";

	/*for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
	{
		std::cout << "Nom: " << pair.second->properties_.name_ << ", transfo: " << pair.second->properties_.transform_.transform_name_ << ", is_speaking_: " << std::boolalpha << pair.second->is_speaking_ << ", zorder_: " << pair.second->character_.zorder_ << std::endl;
	}*/

	//TODO : pour transform et zorder de l'autofocus, utiliser characters_transforms_
	//TODO : positionner is_speaking_ dans show_new_dialogue() ?? Si non, aucun intérêt de passer un Character* => une string suffirait

	//Pour l'autofocus (et changement de textbox/namebox)
	if(dialogues_.count(current_unique_id_))
	{
		if(!(get_last_character_name(dialogues_[current_unique_id_].second).empty()))
		{
			for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
			{
				//std::cout << pair.first << ", " << dialogues_[current_unique_id_].second.first << ", " << dialogues_[current_unique_id_].second.second << std::endl;
				if(pair.first == dialogues_[current_unique_id_].second)
				{
					pair.second->is_speaking_ = true;
					//pair.second->properties_.zorder_ = 10;
					if(!pair.second->properties_.textbox_path_.empty())
					{
						textbox_.change_textbox(pair.second->properties_.textbox_path_, pair.second->properties_.namebox_path_, renderer_);
					}
				}
				else
				{
					pair.second->is_speaking_ = false;
					//pair.second->properties_.zorder_ = 5;
				}
			}
		}
		else //Narrator
		{
			for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
			{
				pair.second->is_speaking_ = false;
				//pair.second->properties_.zorder_ = 5;
			}
			textbox_.change_textbox(constants::textbox_image_, constants::namebox_image_, renderer_);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	//Personnages
	for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
	{
		for(unsigned int i = current_unique_id_; i != -1; --i)
		{
			/*if(characters_transforms_.count(i))
				std::cout << characters_transforms_.at(i).name_ << ", " << pair.second->properties_.name_ << std::endl;*/

			if(characters_transforms_.count(i) && characters_transforms_.at(i).first == pair.first)
			{
				pair.second->set_transform(characters_transforms_.at(i).second.transform_.transform_name_);
				pair.second->properties_.zorder_ = characters_transforms_.at(i).second.zorder_;
				//std::cout << pair.second->properties_.name_ << ", " << characters_transforms_.at(i).second.name_ << std::endl;
				pair.second->properties_.name_ = characters_transforms_.at(i).second.name_;
				break;
			}
		}
		pair.second->update();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	//Backgrounds
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(backgrounds_.count(i))
		{
			change_background(backgrounds_.at(i)); 
			break;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	//Musiques
	//TODO : regarder s'il faut modifier des choses par rapport à la boucle des sons en-dessous
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(musics_.count(i))
		{
			//TODO : comme il y a que des break, possible de mettre un seul break à la fin du if ci-dessus ??
			if(musics_.at(i).second.has_value()) //play music
			{
				if(music_ == &musics_.at(i).second.value())
				{
					musics_.at(i).second.value().change_volume(musics_.at(i).first.volume);
					musics_.at(i).second.value().play_music(musics_.at(i).first.loop, musics_.at(i).first.fadein_length);
					break;
				}
				else
				{
					//si une musique est déjà en train de se jouer, il faut la stopper
					//deux musiques consécutives => stopper (avec ou sans fadeout) la musique courante
					if(sdl::Music::playing())
					{
						sdl::Music::fade_out(musics_.at(i).first.fadeout_length);
					}
					music_ = &musics_.at(i).second.value();
					break;
				}
			}
			else // stop music
			{
				if(sdl::Music::playing())
				{
					sdl::Music::fade_out(musics_.at(i).first.fadeout_length);
					break;
				}
			}
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

	//Sons
	//TODO : pose problème quand on appuie sur Play => le son n'est pas joué entièrement !!
	//TODO : tester fadein/fadeout sur le même channel et sur des channels différents
	//TODO : éventuellement utiliser Mix_ReserveChannels pour réserver des channels aux éléments d'UI ??
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(sounds_.count(i))
		{
			//TODO : comme il y a que des break, possible de mettre un seul break à la fin du if ci-dessus ??
			if(sounds_.at(i).second.has_value()) //play sound
			{
				if(sound_ == &sounds_.at(i).second.value())
				{
					std::cout << "SOUND OK1\n";
					sounds_.at(i).second.value().change_volume(sounds_.at(i).first.volume);
					if(!sdl::Chunk::playing(sounds_.at(i).first.channel))
					{
						sounds_.at(i).second.value().play_sound(sounds_.at(i).first.loop, sounds_.at(i).first.fadein_length);
					}
					break;
				}
				else
				{
					std::cout << "SOUND OK2\n";

					/*if(sound_)
					{
						std::cout << sound_->channel_ << " et " << sounds_.at(i).first.channel << std::endl;
					}*/

					//cas d'un scroll arrière et un son était en train de se jouer => l'arrêter
					if(sound_ && i < current_i_of_sound_)
					{
						if(sdl::Chunk::playing(sound_->channel_))
						{
							sdl::Chunk::halt_channel(sound_->channel_);
						}
					}

					if(sdl::Chunk::playing(sounds_.at(i).first.channel))
					{
						sdl::Chunk::fade_out(sounds_.at(i).first.channel, sounds_.at(i).first.fadeout_length);
					}
					sound_ = &sounds_.at(i).second.value();
					current_i_of_sound_ = i;
					break;
				}
			}
			else // stop sound
			{
				if(sdl::Chunk::playing(sounds_.at(i).first.channel))
				{
					sdl::Chunk::fade_out(sounds_.at(i).first.channel, sounds_.at(i).first.fadeout_length);
					break;
				}
			}
		}

		//TODO : peut-être ajouter la condition current_unique_id_ > 0 ??
		//TODO : cette condition est fausse/inutile pour les sons ??
		if(i == 0)
		{
			if(sdl::Chunk::playing(-1))
			{
				sdl::Chunk::halt_channel(-1);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	if(!dialogues_.count(current_unique_id_) && current_unique_id_ <= std::prev(dialogues_.end())->first) //ne pas incrémenter au-delà la clef max
	{
		//std::cout << current_unique_id_ << std::endl;
		//current_unique_id_ += 1; //TODO : toujours utile ??
	}

	if(skip_mode_)
	{
		show_next_dialogue();
	}

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

	if(!hide_ui_textbox_)
	{
		textbox_.update();
		ui_manager_.update();
	}
}