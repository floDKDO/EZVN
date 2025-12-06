#include "ingame.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"

#include <iostream>
#include <algorithm>

InGame::InGame(Game& game, sdl::Renderer& renderer)
	: GameState(game), current_dialogue_index_(0), textbox_(renderer), background_("img/backgrounds/class.png", 0, 0, renderer), hide_ui_textbox_(false)
{
	build_ui_elements(renderer);

	textbox_.text_.text_ = "Come on PLAYER! Maybe literature isn\'t that boring.";

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
			if(current_dialogue_index_ < dialogues_.size())
			{
				textbox_.show_new_dialogue(std::get<0>(dialogues_[current_dialogue_index_]), (std::get<1>(dialogues_[current_dialogue_index_])), std::get<2>(dialogues_[current_dialogue_index_]));
				current_dialogue_index_ += 1;
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
	background_.draw(renderer);
	for(std::unique_ptr<Character> const& c : characters_)
	{
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
			/*if(c->name_ == "Sayori")
			{
				c->set_transform(TransformName::t41);
			}
			else if(c->name_ == "Monika")
			{
				c->set_transform(TransformName::t42);
			}
			else if(c->name_ == "Natsuki")
			{
				c->set_transform(TransformName::t43);
			}
			else if(c->name_ == "Yuri")
			{
				c->set_transform(TransformName::t44);
			}*/
		}
	}
}