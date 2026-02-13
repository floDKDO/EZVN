#include "GUI/scrollable_area.h"
#include "GUI/text_button.h"

#include <iostream>

ScrollableArea::ScrollableArea(int x, int y, int w, int h, sdl::Renderer& renderer)
	: Ui(renderer), frame_({x, y, w, h}), scrollbar_(std::make_unique<Scrollbar>(x + 150, y, renderer, std::bind(&ScrollableArea::callback_function, this, std::placeholders::_1))), renderer_(renderer) //TODO : hardcodé
{}

void ScrollableArea::callback_function([[maybe_unused]] Ui* ui)
{
	for(auto& pair : ui_elements_)
	{
		Ui* ui_element = pair.first;
		int max_offset = ui_element->get_rect().h - frame_.h;
		float t = float((scrollbar_->current_value_ - scrollbar_->min_value_) / (scrollbar_->max_value_ - scrollbar_->min_value_));
		int scroll_offset = int(t * max_offset);
		SDL_Rect rect = ui_element->get_rect();
		rect.y = pair.second - scroll_offset;
		dynamic_cast<TextButton*>(ui_element)->set_rect(rect);
		//ui->get_rect().y = frame_initial_y_ - scroll_offset;
	}
}

void ScrollableArea::draw(sdl::Renderer& renderer)
{
	renderer.set_clip_rect(&frame_);
	renderer.set_draw_color(255, 255, 255, 255);
	renderer.fill_rect(&frame_);
	for(auto& pair : ui_elements_)
	{
		pair.first->draw(renderer);
	}
	scrollbar_->draw(renderer);
	renderer.set_clip_rect(nullptr);
}

void ScrollableArea::update()
{
	for(auto& pair : ui_elements_)
	{
		pair.first->update();
	}
	scrollbar_->update();
}

SDL_Rect ScrollableArea::get_rect() const
{
	return frame_;
}

std::vector<Ui*> ScrollableArea::get_navigation_nodes()
{
	std::vector<Ui*> vector;
	for(auto& pair : ui_elements_)
	{
		vector.push_back(pair.first);
	}
	vector.push_back(scrollbar_.get());
	return vector;
}

void ScrollableArea::add_ui_element(Ui* ui)
{
	ui_elements_.push_back({ui, ui->get_rect().y});
}