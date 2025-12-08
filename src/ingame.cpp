#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"

#include <iostream>
#include <algorithm>

InGame::InGame(Game& game, sdl::Renderer& renderer) //TODO : ne rien afficher si pas de background
	: GameState(game), counter_(1), current_line_(0), textbox_(renderer), hide_ui_textbox_(false)
{
	build_ui_elements(renderer);

	//textbox_.text_.text_ = "Come on PLAYER! Maybe literature isn\'t that boring."; //TODO : il faut retirer cela !!

	/*characters_.push_back(std::make_unique<Character>("Sayori", "img/characters/sayori.png", renderer, 1));
	characters_.push_back(std::make_unique<Character>("Monika", "img/characters/monika.png", renderer, 4));
	characters_.push_back(std::make_unique<Character>("Natsuki", "img/characters/natsuki.png", renderer, 3));
	characters_.push_back(std::make_unique<Character>("Yuri", "img/characters/yuri.png", renderer, 2));*/

	//std::stable_sort(characters_.begin(), characters_.end(), [&](const std::unique_ptr<Character>& a, const std::unique_ptr<Character>& b) -> bool { return a->character_.zorder_ < b->character_.zorder_; });
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

void InGame::add_character(const std::string name, const std::string character_path, sdl::Renderer& renderer)
{
	characters_.push_back(std::make_unique<Character>(name, character_path, renderer, 1));
}

Character* InGame::get_character(const std::string name) 
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

void InGame::change_background(const std::string background_path, sdl::Renderer& renderer)
{
	//TODO
	if(background_path.empty())
	{
		background_.reset(); //TODO : juste ??
	}
	else
	{
		if(background_ == nullptr)
		{
			background_ = std::make_unique<Image>(background_path, 0, 0, renderer);
		}
		else
		{
			background_->change_image(background_path, 0, 0, renderer);
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

			if(ui->is_mouse_on_ui() != Ui::MOUSE_NOT_ON_ANY_UI_) //si collision avec un textbutton, ne pas gérer les événements de la Textbox (= ne pas passer au prochain dialogue)
			{
				return;
			}
		}

		if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
		|| (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT))
		{
			if(dialogues_.find(current_line_) == dialogues_.begin()) //tout premier dialogue
			{
				//std::cout << "FIRSTTTTTRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n";
				current_line_ = std::next(dialogues_.begin(), counter_)->first; //TODO : tester en incrémentant de counter_ au lieu d'utiliser std::next
				//std::cout << "MODIF DE CURRENT_LINE1: " << current_line_ << std::endl;
				textbox_.show_new_dialogue(dialogues_[current_line_].first, dialogues_[current_line_].second);
				return;
			}

			if(dialogues_.count(current_line_))
			{
				//std::cout << "--CURRENT_LINE: " << current_line_ << std::endl;
				
				if(textbox_.text_.is_finished_) //empêcher le spam d'espace
				{
					//std::cout << "DEDANS\n";
					if(std::next(dialogues_.begin(), counter_ + 1) != dialogues_.end())
					{
						counter_ += 1;
					}
					current_line_ = std::next(dialogues_.begin(), counter_)->first;
					textbox_.show_new_dialogue(dialogues_[current_line_].first, dialogues_[current_line_].second);
					//current_line_ += 1; //ne pas incrémenter dans le cas du tout premier dialogue
				}
			}
		}
		//textbox_.handle_events(e);
	}

	if(e.type == SDL_MOUSEBUTTONDOWN)
	{
		if(e.button.button == SDL_BUTTON_RIGHT)
		{
			hide_ui_textbox_ = !hide_ui_textbox_;
		}
	}
}

void InGame::draw(sdl::Renderer& renderer)
{
	for(unsigned int i = current_line_; i > 0; --i)
	{
		//std::cout << "I : " << i << std::endl;
		if(backgrounds_.count(i))
		{
			//std::cout << "=========================================================================\n";
			change_background(backgrounds_[i], renderer);
			break;
		}
	}

	if(background_ != nullptr)
	{
		background_->draw(renderer);
	}

	for(std::unique_ptr<Character> const& c : characters_)
	{
		//std::cout << "********************************************************************************\n";
		c->draw(renderer); //TODO : draw avant que la transfo ait commencée...
		//TODO : chercher si "c" est dans characters_transforms_ et si oui, afficher sa dernière transfo selon la ligne (ligne = valeur max de l'indice dans characters_transforms_ ??)
		//c->draw(renderer); //TODO : affichage selon ligne (dernière transfo de chaque personnage)
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
	//TODO : ne fonctionnera plus avec le vecteur de personnages
	//std::cout << current_line_ << " et " << dialogues_.count(current_line_) << std::endl;

	for(std::unique_ptr<Character> const& c : characters_)
	{
		for(unsigned int i = current_line_; i > 0; --i)
		{
			//std::cout << "I : " << i << std::endl;
			if(characters_transforms_.count(i) && characters_transforms_[i].first->name_ == c->name_)
			{
				//std::cout << "=========================================================================\n";
				c->set_transform(characters_transforms_[i].second);
				break;
			}
		}
	}

	if(!dialogues_.count(current_line_) && current_line_ <= (--dialogues_.end())->first) //ne pas incrémenter au-delà la clef max
	{
		current_line_ += 1;
		//std::cout << "CURRENT_LINE: " << current_line_ << std::endl;
		//std::cout << std::boolalpha << textbox_.text_.is_finished_ << std::endl;
	}

	if(!hide_ui_textbox_)
	{
		textbox_.update();
		for(const std::unique_ptr<Ui>& ui : ui_elements_)
		{
			ui->update();
		}

		for(std::unique_ptr<Character> const& c : characters_)
		{
			c->t_.show_transform(c->t_.transform_name_, c->character_);
		}
	}
}