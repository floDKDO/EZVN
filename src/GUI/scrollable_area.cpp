#include "GUI/scrollable_area.h"

#include <iostream>

ScrollableArea::ScrollableArea(int x, int y, int w, int h, sdl::Renderer& renderer)
	: UiWidget(renderer), frame_({x, y, w, h}), scrollbar_(x + 150, y + 20, renderer, std::bind(&ScrollableArea::callback_function, this, std::placeholders::_1)), max_y_(0), renderer_(renderer) //TODO : hardcodé
{
	rect_ = frame_;
}

void ScrollableArea::callback_function([[maybe_unused]] Ui* ui)
{
	for(auto& pair : ui_elements_)
	{
		UiWidget* widget = pair.first;

		int scroll_offset = get_scroll_offset(widget->rect_.h);

		SDL_Rect rect = widget->rect_;
		rect.y = pair.second + scroll_offset; // + pour monter, - pour descendre
		widget->change_position(rect.x, rect.y);
	}
}

void ScrollableArea::draw(sdl::Renderer& renderer)
{
	renderer.set_clip_rect(&frame_);
	//renderer.set_draw_color(255, 255, 255, 255); //TODO : hardcodé
	//renderer.fill_rect(&frame_);
	for(auto& pair : ui_elements_)
	{
		if(SDL_HasIntersection(&pair.first->rect_, &frame_))
		{
			pair.first->draw(renderer);
		}
	}
	scrollbar_.draw(renderer);
	renderer.set_clip_rect(nullptr);
}

void ScrollableArea::update()
{
	for(auto& pair : ui_elements_)
	{
		if(SDL_HasIntersection(&pair.first->rect_, &frame_))
		{
			pair.first->update();
		}
	}
	scrollbar_.update();
}

std::vector<UiWidget*> ScrollableArea::get_navigation_nodes()
{
	std::vector<UiWidget*> vector;
	for(auto& pair : ui_elements_)
	{
		vector.push_back(pair.first);
	}
	vector.push_back(&scrollbar_);
	return vector;
}

int ScrollableArea::get_scroll_offset(int ui_height)
{
	float t = float((scrollbar_.current_value_ - scrollbar_.min_value_) / (scrollbar_.max_value_ - scrollbar_.min_value_));
	int max_offset = ui_height - max_y_; 
	return int(t * max_offset);
}

void ScrollableArea::add_ui_element(UiWidget* widget)
{
	ui_elements_.push_back({widget, widget->rect_.y});
	max_y_ = get_max_y();
}

int ScrollableArea::get_max_y() const
{
	int max_y = 0;
	for(auto& pair : ui_elements_)
	{
		if(pair.first->rect_.y > max_y_)
		{
			max_y = pair.first->rect_.y;
		}
	}
	return max_y;
}

void ScrollableArea::change_position(int x, int y)
{
	//TODO : ne fait rien ??
}