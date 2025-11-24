#include "ingamestate.h"
#include "GUI/textbutton.h"
#include "transform.h"

#include <iostream>

InGameState::InGameState(sdl::Renderer& renderer)
	: renderer_(renderer), textbox_({255, 255, 255, 255}, renderer), background_("img/backgrounds/class.png", 0, 0, renderer), hide_ui_textbox_(false)
{
	textbox_.text_.text_ = "Come on PLAYER! Maybe literature isn\'t that boring.";

	//TODO : beaucoup trop de trucs hardcodés

	int x_textbutton = textbox_.textbox_.position_.x + 283;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h - 24;

	std::vector<std::unique_ptr<Ui>> ui_elements;
	ui_elements.reserve(10);

	ui_elements.push_back(std::make_unique<TextButton>("History", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[0].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Skip", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 8, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[1].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Auto", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 16, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[2].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Save", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 24, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[3].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Load", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 32, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[4].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Settings", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 40, y_textbutton, renderer_, std::bind(&InGameState::temp_function, this, std::placeholders::_1)));

	menu_ = std::make_unique<UiManager>(std::move(ui_elements), ui_elements[0].get());

	characters_.push_back(std::make_unique<Character>("Sayori", "img/characters/sayori.png", "img/gui/textbox.png", renderer));

	//t11, t32, t42
	/*characters_[0]->character_.zoom(0.8f);
	characters_[0]->character_.set_center();
	characters_[0]->character_.set_position(characters_[0]->character_.position_.x, characters_[0]->character_.position_.y - 26);*/

	//t41
	//characters_[0]->character_.zoom(0.8f);
	//characters_[0]->character_.set_position_xcenter(200, characters_[0]->character_.position_.y - 26);

	//t33
	//characters_[0]->character_.zoom(0.8f);
	//characters_[0]->character_.set_position_xcenter(1040, characters_[0]->character_.position_.y - 26);

	//f11
	//characters_[0]->character_.zoom(0.84f);
	//characters_[0]->character_.set_center();
	//characters_[0]->character_.set_position(characters_[0]->character_.position_.x, characters_[0]->character_.position_.y - 65);
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
			if(ui->is_mouse_on_ui() != Ui::MOUSE_NOT_ON_ANY_UI) //si collision avec un textbutton, ne pas gérer les événements de la Textbox (= ne pas passer au prochain dialogue)
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

			//TODO : temporaire
			if(hide_ui_textbox_)
			{
				//f11
				/*characters_[0]->character_.position_ = characters_[0]->initial_rect_;
				characters_[0]->character_.zoom(0.85f);
				characters_[0]->character_.set_center();
				characters_[0]->character_.set_position(characters_[0]->character_.position_.x, characters_[0]->character_.position_.y - 75);*/
			}
			else
			{
				//t11
				/*Transform t11;
				characters_[0]->set_transform(t11);*/
			}
		}
	}
}

void InGameState::draw(sdl::Renderer& renderer)
{
	//background_.draw(renderer);
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

void InGameState::update(Uint64 time_step)
{
	if(!hide_ui_textbox_)
	{
		textbox_.update(time_step);
		menu_->update(time_step);
	}

	characters_[0]->set_transform();
}