#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"
#include "game.h"

#include <iostream>
#include <algorithm>

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

//TODO : inutile car inutilisée ??
void InGame::add_character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer)
{
	characters_.push_back(std::make_unique<Character>(name, character_path, renderer)); 
}

void InGame::add_character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path, const std::string_view namebox_path)
{
	characters_.push_back(std::make_unique<Character>(name, character_path, renderer, textbox_path, namebox_path));
}

Character* InGame::get_character(const std::string_view name)
{
	for(const std::unique_ptr<Character>& c : characters_)
	{
		if(c->name_ == name)
		{
			return c.get();
		}
	}
	return nullptr;
}

void InGame::insert_dialogue(const std::string_view character_name, const std::string_view dialogue)
{
	dialogues_.insert({unique_id_, {dialogue, get_character(character_name)}}); //TODO : make_pair ou accolades (= initializer list) ??
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

void InGame::insert_character(const std::string_view character_name, const TransformName transform_name, const int zorder)
{
	characters_transforms_.insert({unique_id_, {get_character(character_name), transform_name, zorder}}); //TODO : make_tuple ou accolades (= initializer list) ??
	unique_id_ += 1;
}

void InGame::insert_character(const std::string_view character_name, const TransformName transform_name)
{
	Character* character = get_character(character_name);

	int zorder = 0;
	for(unsigned int i = unique_id_; i != -1; --i)
	{
		if(characters_transforms_.count(i) && std::get<0>(characters_transforms_[i])->name_ == character_name)
		{
			zorder = std::get<2>(characters_transforms_[i]);
			break;
		}
	}
	characters_transforms_.insert({unique_id_, {character, transform_name, zorder}}); //TODO : make_tuple ou accolades (= initializer list) ??
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
			textbox_.show_new_dialogue(dialogues_[current_unique_id_].first, dialogues_[current_unique_id_].second, get_texttoggle("Skip")->is_checked_, true);
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
		textbox_.show_new_dialogue(dialogues_[current_unique_id_].first, dialogues_[current_unique_id_].second, false, false);
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
			show_next_dialogue();
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
	std::stable_sort(characters_.begin(), characters_.end(), [&](const std::unique_ptr<Character>& a, const std::unique_ptr<Character>& b) -> bool { return a->character_.zorder_ < b->character_.zorder_; });
	for(const std::unique_ptr<Character>& c : characters_)
	{
		c->draw(renderer); 
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
	//Pour l'autofocus
	if(dialogues_.count(current_unique_id_))
	{
		if(dialogues_[current_unique_id_].second != nullptr)
		{
			for(const std::unique_ptr<Character>& c : characters_)
			{
				if(c->name_ == dialogues_[current_unique_id_].second->name_)
				{
					c->is_speaking_ = true;
					if(!c->textbox_path_.empty())
					{
						textbox_.change_textbox(c->textbox_path_, c->namebox_path_, renderer_);
					}
				}
				else c->is_speaking_ = false;
			}
		}
		else //Narrator
		{
			for(const std::unique_ptr<Character>& c : characters_)
			{
				c->is_speaking_ = false;
			}
			textbox_.change_textbox(constants::textbox_image_, constants::namebox_image_, renderer_);
		}
	}

	for(const std::unique_ptr<Character>& c : characters_)
	{
		for(unsigned int i = current_unique_id_; i != -1; --i)
		{
			if(characters_transforms_.count(i) && std::get<0>(characters_transforms_[i])->name_ == c->name_)
			{
				c->set_transform(std::get<1>(characters_transforms_[i]));
				c->character_.zorder_ = std::get<2>(characters_transforms_[i]);
				break;
			}
		}
		c->update();
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