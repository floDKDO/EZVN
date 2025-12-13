#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"

#include <iostream>
#include <algorithm>

InGame::InGame(Game& game, sdl::Renderer& renderer) //TODO : ne rien afficher si pas de background
	: GameState(game), unique_id_(0), current_unique_id_(unique_id_), current_unique_id_when_previous_(unique_id_), is_current_unique_id_saved_(false), textbox_(renderer), hide_ui_textbox_(false), renderer_(renderer)
{
	build_ui_elements(renderer);
}

void InGame::build_ui_elements(sdl::Renderer& renderer)
{
	int x_textbutton = textbox_.textbox_.position_.x + constants::textbox_textbuttons_x_delta_;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h + constants::textbox_textbuttons_y_delta_;

	ui_elements_.reserve(10);

	ui_elements_.push_back(std::make_unique<TextButton>("History", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements_[0].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements_.push_back(std::make_unique<TextButton>("Skip", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements_[1].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements_.push_back(std::make_unique<TextButton>("Auto", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements_[2].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements_.push_back(std::make_unique<TextButton>("Save", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements_[3].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements_.push_back(std::make_unique<TextButton>("Load", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements_[4].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements_.push_back(std::make_unique<TextButton>("Settings", x_textbutton, y_textbutton, renderer, std::bind(&InGame::temp_function, this, std::placeholders::_1), TextButtonKind::ON_TEXTBOX));
}

void InGame::add_character(const std::string_view name, const std::string_view character_path, sdl::Renderer& renderer)
{
	characters_.push_back(std::make_unique<Character>(name, character_path, renderer)); 
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
	dialogues_.insert({unique_id_, {dialogue, get_character(character_name)}});
	unique_id_ += 1;
}

void InGame::insert_background(const std::string_view background_path)
{
	backgrounds_.insert({unique_id_, background_path});
	unique_id_ += 1;
}

void InGame::insert_character(const std::string_view character_name, const TransformName transform_name, const int zorder)
{
	characters_transforms_.insert({unique_id_, {get_character(character_name), transform_name, zorder}});
	unique_id_ += 1;
}

void InGame::change_background(const std::string_view background_path)
{
	if(background_path.empty())
	{
		background_.reset(); 
	}
	else
	{
		if(background_ == nullptr)
		{
			background_ = std::make_unique<Image>(background_path, 0, 0, renderer_);
		}
		else
		{
			background_->init_image(background_path, 0, 0, renderer_);
		}
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
			textbox_.show_new_dialogue(dialogues_[current_unique_id_].first, dialogues_[current_unique_id_].second);
		}
	}
}

void InGame::show_dialogue_mouse_wheel(WhichDialogue which_dialogue)
{
	SDL_assert(which_dialogue == WhichDialogue::next || which_dialogue == WhichDialogue::previous);

	if(dialogues_.count(current_unique_id_))
	{
		if(textbox_.text_.is_finished_) //empêcher le spam d'espace
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
			textbox_.show_new_dialogue(dialogues_[current_unique_id_].first, dialogues_[current_unique_id_].second);
		}
	}
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
		for(const std::unique_ptr<Ui>& ui : ui_elements_)
		{
			ui->handle_events(e);

			if(ui->is_mouse_on_ui()) //si collision avec un textbutton, ne pas gérer les événements de la Textbox (= ne pas passer au prochain dialogue)
			{
				return;
			}
		}

		if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
		|| (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT))
		{
			show_next_dialogue();
		}
		else if(e.type == SDL_MOUSEWHEEL) 
		{
			if(e.wheel.y > 0) //scroll vers l'avant => reculer d'un dialogue
			{
				show_dialogue_mouse_wheel(WhichDialogue::previous);
			}
			else //scroll vers l'arrière => avancer d'un dialogue
			{
				show_dialogue_mouse_wheel(WhichDialogue::next);
			}
		}
		//textbox_.handle_events(e);
	}

	if(e.type == SDL_MOUSEBUTTONDOWN)
	{
		if(e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE)
		{
			hide_ui_textbox_ = !hide_ui_textbox_;
		}
	}
}

void InGame::draw(sdl::Renderer& renderer)
{
	if(background_ != nullptr)
	{
		background_->draw(renderer);
	}

	//TODO : coûteux car réalisé à chaque tour de boucle...
	std::stable_sort(characters_.begin(), characters_.end(), [&](const std::unique_ptr<Character>& a, const std::unique_ptr<Character>& b) -> bool { return a->character_.zorder_ < b->character_.zorder_; });
	for(const std::unique_ptr<Character>& c : characters_)
	{
		//std::cout << "********************************************************************************\n";
		c->draw(renderer); 
	}

	if(!hide_ui_textbox_)
	{
		textbox_.draw(renderer);
		for(const std::unique_ptr<Ui>& ui : ui_elements_)
		{
			ui->draw(renderer);
		}
	}
}

void InGame::update()
{
	for(const std::unique_ptr<Character>& c : characters_)
	{
		for(unsigned int i = current_unique_id_; i != -1; --i)
		{
			//std::cout << "I : " << i << std::endl;
			if(characters_transforms_.count(i) && std::get<0>(characters_transforms_[i])->name_ == c->name_)
			{
				//std::cout << "=========================================================================\n";
				c->set_transform(std::get<1>(characters_transforms_[i]));
				c->character_.zorder_ = std::get<2>(characters_transforms_[i]);
				break;
			}
		}
	}

	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		//std::cout << "I : " << i << std::endl;
		if(backgrounds_.count(i))
		{
			//std::cout << "=========================================================================\n";
			change_background(backgrounds_[i]);
			break;
		}
	}

	if(!dialogues_.count(current_unique_id_) && current_unique_id_ <= (--dialogues_.end())->first) //ne pas incrémenter au-delà la clef max
	{
		current_unique_id_ += 1;
		//std::cout << "CURRENT_LINE: " << current_unique_id_ << std::endl;
		//std::cout << std::boolalpha << textbox_.text_.is_finished_ << std::endl;
	}

	if(!hide_ui_textbox_)
	{
		textbox_.update();
		for(const std::unique_ptr<Ui>& ui : ui_elements_)
		{
			ui->update();
		}

		for(const std::unique_ptr<Character>& c : characters_)
		{
			c->t_.show_transform(c->t_.transform_name_, c->character_);
		}
	}
}