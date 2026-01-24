#pragma once

#include "GUI/ui.h"
#include "GUI/button.h"
#include "constants.h"

#include <iostream>

//= Choice menu
template<size_t N>
class ButtonGroup : public Ui
{
	public:
		ButtonGroup(std::vector<std::string> texts, int y, sdl::Renderer& renderer);

		void draw(sdl::Renderer& renderer) override;
		void update() override;
		void handle_events(const SDL_Event& e);
		std::vector<Ui*> get_navigation_nodes() override;

		void buttongroup_function(Ui* ui);

	private:
		std::vector<Button> buttons_;
		Button* selected_button_;
};

template<size_t N>
ButtonGroup<N>::ButtonGroup(std::vector<std::string> texts, int y, sdl::Renderer& renderer)
	: Ui(renderer), selected_button_(nullptr)
{
	SDL_assert(texts.size() == N);

	for(unsigned int i = 0; i < N; ++i)
	{
		buttons_.push_back(Button(texts[i], (constants::window_width_ - constants::buttongroup_width_) / 2, y + i * constants::buttongroup_y_spacing_, renderer, std::bind(&ButtonGroup::buttongroup_function, this, std::placeholders::_1))); 
	}
}

template<size_t N>
void ButtonGroup<N>::draw(sdl::Renderer& renderer)
{
	for(const Button& b : buttons_)
	{
		b.draw(renderer);
	}
}

template<size_t N>
void ButtonGroup<N>::update()
{
	for(const Button& b : buttons_)
	{
		b.update();
		if(b.state_ == State::SELECTED)
		{
			if(selected_button_ == nullptr || selected_button_ == &b)
			{
				selected_button_ = &b;
			}
			else
			{
				selected_button_->state_ = State::NORMAL;
			}
		}
	}
	selected_button_ = nullptr;
}

template<size_t N>
void ButtonGroup<N>::handle_events(const SDL_Event& e)
{
	for(const Button& b : buttons_)
	{
		b.handle_events(e);
	}
}

template<size_t N>
std::vector<Ui*> ButtonGroup<N>::get_navigation_nodes()
{
	std::vector<Ui*> vector;
	for(const Button& b : buttons_)
	{
		vector.push_back(&b);
	}
	return vector;
}

template<size_t N>
void ButtonGroup<N>::buttongroup_function(Ui* ui)
{
	Button* button = dynamic_cast<Button*>(ui);
	std::cout << button->text_.text_ << std::endl;
}
