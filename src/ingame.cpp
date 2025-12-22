#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"
#include "game.h"

#include <iostream>
#include <algorithm>

//TODO : is_speaking etc. : modifier les characters dans characters_ ou modifier les Character* dans dialogues_ ??
//TODO : créer une fonction d'insertion qui incrémente unique_id_

InGame::InGame(Game& game, sdl::Renderer& renderer) 
	: GameState(game), unique_id_(0), current_unique_id_(unique_id_), current_unique_id_when_previous_(unique_id_), is_current_unique_id_saved_(false), 
	last_time_(0), textbox_(renderer), background_(0, 0, 0, 255), hide_ui_textbox_(false), renderer_(renderer)
{
	build_ui_elements(renderer);
}

void InGame::build_ui_elements(sdl::Renderer& renderer)
{
	int x_textbutton = textbox_.textbox_.position_.x + constants::textbox_ui_elements_x_delta_;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h + constants::textbox_ui_elements_y_delta_;

	ui_manager_.add_element(std::make_unique<TextButton>("History", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_manager_.ui_elements_[0].get())->text_.get_width_text() + constants::textbox_ui_elements_x_spacing_;
	ui_manager_.add_element(std::make_unique<TextToggle>("Skip", x_textbutton, y_textbutton, false, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextToggleKind::ON_TEXTBOX));
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

void InGame::set_initial_dialogue()
{
	//std::cout << "Initial dialogue: " << dialogues_.begin()->second.second << ",+ " << get_last_character_name(dialogues_.begin()->second.second) << ",/ " << unique_id_ << ",* " << current_unique_id_ << std::endl;
	textbox_.show_initial_dialogue(dialogues_.begin()->second.first, get_last_character_name(dialogues_.begin()->second.second, true)); //TODO: ici, il faudrait utiliser current_unique_id_ mais pas possible pour l'instant...
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

std::string InGame::get_last_character_name(const std::string_view character_variable, const bool is_first_dialogue)
{
	if(character_variable.empty())
	{
		return "";
	}

	unsigned int initial_index_value;
	if(current_unique_id_ == 0 && !is_first_dialogue)
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

			//TODO : affiche "" (= Narrator), ce qui n'est pas logique => vérifier que dialogues_[current_unique_id_].second.first != "" ??
			//if(dialogues_[current_unique_id_].second.first.empty())

			//for(unsigned int i = current_unique_id_; i != -1; --i)
			//{
			//	if(dialogues_.count(i))
			//	{
			//		std::cout << "Nom: " << dialogues_[i].first << std::endl;
			//	}
			//}

			textbox_.show_new_dialogue(dialogues_[current_unique_id_].first, get_last_character_name(dialogues_[current_unique_id_].second), get_texttoggle("Skip")->is_checked_, true);
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
		textbox_.show_new_dialogue(dialogues_[current_unique_id_].first, get_last_character_name(dialogues_[current_unique_id_].second), get_texttoggle("Skip")->is_checked_, false);
	}
}

void InGame::auto_function(Ui* ui)
{
	(void)ui;
	if(textbox_.text_.is_finished_)
	{
		show_next_dialogue();
	}
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

void InGame::handle_events(const SDL_Event& e)
{
	if(!hide_ui_textbox_)
	{
		//condition placée en premier pour que le scroll de la mouse wheel sur un textbutton fonctionne
		if(e.type == SDL_MOUSEWHEEL)
		{
			if(e.wheel.y > 0) //scroll vers l'avant => reculer d'un dialogue
			{
				get_texttoggle("Skip")->is_checked_ = false;
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
	for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
	{
		pair.second->draw(renderer); 
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
	/*for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(dialogues_.count(current_unique_id_) && dialogues_[current_unique_id_].second != nullptr)
		{
			std::cout << current_unique_id_ <<  ", Nom: " << dialogues_[current_unique_id_].second->properties_.name_ << ", transfo: " << dialogues_[current_unique_id_].second->properties_.transform_.transform_name_ << ", is_speaking_: " << std::boolalpha << dialogues_[current_unique_id_].second->is_speaking_ << ", zorder_: " << dialogues_[current_unique_id_].second->properties_.zorder_ << std::endl;
		}
	}*/
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
					if(!pair.second->properties_.textbox_path_.empty())
					{
						textbox_.change_textbox(pair.second->properties_.textbox_path_, pair.second->properties_.namebox_path_, renderer_);
					}
				}
				else
				{
					pair.second->is_speaking_ = false;
				}
			}
		}
		else //Narrator
		{
			for(const std::pair<std::string, std::unique_ptr<Character>>& pair : characters_)
			{
				pair.second->is_speaking_ = false;
			}
			textbox_.change_textbox(constants::textbox_image_, constants::namebox_image_, renderer_);
		}
	}

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

	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(backgrounds_.count(i))
		{
			change_background(backgrounds_.at(i)); 
			break;
		}
	}

	if(!dialogues_.count(current_unique_id_) && current_unique_id_ <= std::prev(dialogues_.end())->first) //ne pas incrémenter au-delà la clef max
	{
		//std::cout << current_unique_id_ << std::endl;
		current_unique_id_ += 1;
	}

	if(get_texttoggle("Skip")->is_checked_)
	{
		show_next_dialogue();
	}

	if(get_texttoggle("Auto")->is_checked_)
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