#include "ingamestate.h"
#include "GUI/textbutton.h"
#include "transform.h"
#include "constants.h"

#include <iostream>
#include <algorithm>

InGameState::InGameState(sdl::Renderer& renderer)
	: textbox_(renderer), background_("img/backgrounds/class.png", 0, 0, renderer), hide_ui_textbox_(false), renderer_(renderer)
{
	textbox_.text_.text_ = "Come on PLAYER! Maybe literature isn\'t that boring.";

	int x_textbutton = textbox_.textbox_.position_.x + constants::textbox_textbuttons_x_delta_;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h + constants::textbox_textbuttons_y_delta_;

	std::vector<std::unique_ptr<Ui>> ui_elements;
	ui_elements.reserve(10);

	ui_elements.push_back(std::make_unique<TextButton>("History", x_textbutton, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1), true));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[0].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements.push_back(std::make_unique<TextButton>("Skip", x_textbutton, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1), true));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[1].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements.push_back(std::make_unique<TextButton>("Auto", x_textbutton, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1), true));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[2].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements.push_back(std::make_unique<TextButton>("Save", x_textbutton, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1), true));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[3].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements.push_back(std::make_unique<TextButton>("Load", x_textbutton, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1), true));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[4].get())->text_.get_width_text() + constants::textbox_textbuttons_x_spacing_;
	ui_elements.push_back(std::make_unique<TextButton>("Settings", x_textbutton, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1), true));

	menu_ = std::make_unique<UiManager>(std::move(ui_elements));

	characters_.push_back(std::make_unique<Character>("Sayori", "img/characters/sayori.png", renderer, 1));
	characters_.push_back(std::make_unique<Character>("Monika", "img/characters/monika.png", renderer, 4));
	characters_.push_back(std::make_unique<Character>("Natsuki", "img/characters/natsuki.png", renderer, 3));
	characters_.push_back(std::make_unique<Character>("Yuri", "img/characters/yuri.png", renderer, 2));

	std::stable_sort(characters_.begin(), characters_.end(), [&](const std::unique_ptr<Character>& a, const std::unique_ptr<Character>& b) -> bool { return a->character_.zorder_ < b->character_.zorder_; });
}

void InGameState::temp_function(Ui* ui)
{
	(void)ui;
	std::cout << "Pressed!\n";
}

void InGameState::handle_events(const SDL_Event& e)
{
	if(!hide_ui_textbox_)
	{
		menu_->handle_events(e);
		for(Ui* ui : menu_->navigation_list_)
		{
			if(ui->is_mouse_on_ui() != Ui::MOUSE_NOT_ON_ANY_UI_) //si collision avec un textbutton, ne pas gérer les événements de la Textbox (= ne pas passer au prochain dialogue)
			{
				return;
			}
		}
		textbox_.handle_events(e);
	}

	if(e.type == SDL_MOUSEBUTTONDOWN)
	{
		if(e.button.button == SDL_BUTTON_RIGHT)
		{
			hide_ui_textbox_ = !hide_ui_textbox_;
		}
	}
}

void InGameState::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	for(std::unique_ptr<Character> const& c : characters_)
	{
		c->draw(renderer);
	}

	if(!hide_ui_textbox_)
	{
		textbox_.draw(renderer);
		menu_->draw(renderer);
	}
}

void InGameState::update()
{
	if(!hide_ui_textbox_)
	{
		textbox_.update();
		menu_->update();

		for(std::unique_ptr<Character> const& c : characters_)
		{
			if(c->name_ == "Sayori")
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
			}
		}
	}
}