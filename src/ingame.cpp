#include "ingame.h"
#include "GUI/textbutton.h"

#include <iostream>

InGame::InGame(sdl::Renderer& renderer)
	: renderer_(renderer), textbox_({255, 255, 255, 255}, renderer), background_("img/backgrounds/class.png", 0, 0, renderer)
{
	textbox_.text_.text_ = "Come on PLAYER! Maybe literature isn\'t that boring.";

	//TODO : beaucoup trop de trucs hardcodés

	int x_textbutton = textbox_.textbox_.position_.x + 283;
	int y_textbutton = textbox_.textbox_.position_.y + textbox_.textbox_.position_.h - 24;

	std::vector<std::unique_ptr<Ui>> ui_elements;
	ui_elements.reserve(10);

	ui_elements.push_back(std::make_unique<TextButton>("History", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton, y_textbutton, renderer_, std::bind(&InGame::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[0].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Skip", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 8, y_textbutton, renderer_, std::bind(&InGame::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[1].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Auto", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 16, y_textbutton, renderer_, std::bind(&InGame::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[2].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Save", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 24, y_textbutton, renderer_, std::bind(&InGame::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[3].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Load", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 32, y_textbutton, renderer_, std::bind(&InGame::temp_function, this, std::placeholders::_1)));
	x_textbutton += dynamic_cast<TextButton*>(ui_elements[4].get())->text_.get_width_text();
	ui_elements.push_back(std::make_unique<TextButton>("Settings", SDL_Color{255, 255, 255, 255}, SDL_Color{255, 0, 0, 255}, SDL_Color{255, 0, 0, 255}, x_textbutton + 40, y_textbutton, renderer_, std::bind(&InGame::temp_function, this, std::placeholders::_1)));

	menu_ = std::make_unique<Menu>(std::move(ui_elements), ui_elements[0].get());

	characters_.push_back(std::make_unique<Character>("Sayori", "img/characters/sayori.png", "img/gui/textbox.png", renderer));

	characters_[0]->character_.resize(characters_[0]->character_.position_.w * 0.8, characters_[0]->character_.position_.h * 0.8);
	characters_[0]->character_.set_position(250, characters_[0]->character_.position_.y - 30);
}

void InGame::temp_function(Ui* ui)
{
	std::cout << "Pressed!\n";
}

void InGame::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym)
			{
				case SDLK_SPACE:
					//Prochain dialogue
					textbox_.show_new_dialogue("And then, I would be I good guy because they are a lot of people that like somebody that used to be.", "Sayori");
					break;

				default:
					break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(e.button.button == SDL_BUTTON_LEFT)
			{
				//TODO : si sur la textbox OU sur rien du tout, alors passer au dialogue suivant
			}
			break;

		case SDL_KEYUP:
			break;

		default:
			break;
	}
	menu_->handle_events(e);
}

void InGame::draw(sdl::Renderer& renderer)
{
	background_.draw(renderer);
	for(std::unique_ptr<Character> const& c : characters_)
	{
		c->draw(renderer);
	}
	textbox_.draw(renderer);
	menu_->draw(renderer);
}

void InGame::update(Uint64 time_step)
{
	textbox_.update(time_step);
	menu_->update(time_step);
}