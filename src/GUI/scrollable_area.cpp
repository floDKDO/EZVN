#include "GUI/scrollable_area.h"
#include "constants.h"

#include <iostream>

ScrollableArea::ScrollableArea(int x, int y, int w, int h, sdl::Renderer& renderer)
	: UiWidget(renderer), frame_({x, y, w, h}), scrollbar_(x + w - constants::slider_handle_size_, y + (constants::slider_handle_size_ / 2), h - constants::slider_handle_size_, renderer, std::bind(&ScrollableArea::callback_function, this, std::placeholders::_1)), max_y_(0), renderer_(renderer)
{
	rect_ = frame_;
}

void ScrollableArea::callback_function([[maybe_unused]] Ui* ui)
{
	//std::cout << "APPEL!\n";
	for(auto& pair : ui_widgets_)
	{
		if(UiWidget* widget = dynamic_cast<UiWidget*>(pair.first); widget != nullptr)
		{
			int scroll_offset = get_scroll_offset(widget->rect_.h);
			//std::cout << scroll_offset << std::endl;

			SDL_Rect rect = widget->rect_;
			rect.y = pair.second + scroll_offset; // + pour monter, - pour descendre
			widget->change_position(rect.x, rect.y);
		}
		/*else if(UiGroup* ui_group = dynamic_cast<UiGroup*>(pair.first.get()); ui_group != nullptr)
		{
			std::cout << "LALALALA\n";
			std::vector<UiWidget*> widgets = ui_group->get_navigation_nodes();
			for(UiWidget* widget : widgets)
			{
				int scroll_offset = get_scroll_offset(widget->rect_.h);

				SDL_Rect rect = widget->rect_;
				rect.y = pair.second + scroll_offset; // + pour monter, - pour descendre

				std::cout << scroll_offset << ", " << rect.y << std::endl;

				widget->change_position(rect.x, rect.y);
			}
		}*/
	}
}

void ScrollableArea::draw(sdl::Renderer& renderer)
{
	renderer.set_clip_rect(&frame_);
	renderer.set_draw_color(200, 200, 200, 255); //TODO : hardcodé
	renderer.fill_rect(&frame_);
	for(auto& pair : ui_widgets_)
	{
		if(UiWidget* widget = dynamic_cast<UiWidget*>(pair.first); widget != nullptr)
		{
			//if(SDL_HasIntersection(&pair.first->rect_, &frame_))
			widget->draw(renderer);
		}
		/*else if(UiGroup* ui_group = dynamic_cast<UiGroup*>(pair.first.get()); ui_group != nullptr)
		{
			ui_group->draw(renderer);
		}*/
	}
	scrollbar_.draw(renderer);
	renderer.set_clip_rect(nullptr);
}

void ScrollableArea::update()
{
	for(auto& pair : ui_widgets_)
	{
		if(UiWidget* widget = dynamic_cast<UiWidget*>(pair.first); widget != nullptr)
		{
			//if(SDL_HasIntersection(&pair.first->rect_, &frame_))
			widget->update();
		}
		/*else if(UiGroup* ui_group = dynamic_cast<UiGroup*>(pair.first.get()); ui_group != nullptr)
		{
			ui_group->update();
		}*/
	}
	scrollbar_.update();
}

std::vector<UiWidget*> ScrollableArea::get_navigation_nodes()
{
	std::vector<UiWidget*> vector;
	for(auto& pair : ui_widgets_)
	{
		if(UiWidget* widget = dynamic_cast<UiWidget*>(pair.first); widget != nullptr)
		{
			vector.push_back(widget);
		}
		/*else if(UiGroup* ui_group = dynamic_cast<UiGroup*>(pair.first.get()); ui_group != nullptr)
		{
			std::vector<UiWidget*> widgets = ui_group->get_navigation_nodes();
			for(UiWidget* widget : widgets)
			{
				vector.push_back(widget);
			}
		}*/
	}
	vector.push_back(&scrollbar_);
	return vector;
}

int ScrollableArea::get_scroll_offset(int ui_height)
{
	float t = float((scrollbar_.current_value_ - scrollbar_.min_value_) / (scrollbar_.max_value_ - scrollbar_.min_value_)); //TODO : <=> juste marquer current_value 
	//std::cout << "t: " << t << ", value: " << scrollbar_.current_value_ << std::endl;
	int max_scroll = max_y_ - (frame_.y + frame_.h); //si on a le curseur de la scrollbar tout en haut et qu'on le descend au max, ceci est la valeur (max) en y dont on a monté les éléments de la ScrollableArea
	int max_offset = 0;
	if(max_scroll > 0) //on peut scroll uniquement si la zone à scroller est plus grande que frame_.h 
	{
		max_offset = -max_scroll - ui_height;
	}
	//std::cout << "max_scroll: " << max_scroll << ", " << "max_offset: " << max_offset << std::endl;
	return int(t * max_offset);
}

void ScrollableArea::add_ui_element(std::unique_ptr<UiWidget> widget)
{
	ui_elements_.push_back(std::move(widget));
	UiWidget* w = dynamic_cast<UiWidget*>(ui_elements_.back().get());
	ui_widgets_.push_back({w, w->rect_.y});
	max_y_ = get_max_y();
}

void ScrollableArea::add_ui_element(std::unique_ptr<UiGroup> ui_group)
{
	/*UiGroup* ui_groupp = ui_group.release(); //fonctionne mais fuite de mémoire
	ui_elements_.push_back({ui_groupp, 0});*/

	std::vector<UiWidget*> widgets = ui_group->get_navigation_nodes();
	bool first = false;
	for(UiWidget* widget : widgets)
	{
		if(!first)
		{
			//std::cout << "FIRST Y : " << widget->rect_.y << std::endl;
			first = true;
		}
		ui_widgets_.push_back({widget, widget->rect_.y});
	}
	ui_elements_.push_back(std::move(ui_group)); 

	max_y_ = get_max_y();
}

int ScrollableArea::get_max_y() const
{
	int max_y = 0;
	for(auto& pair : ui_widgets_)
	{
		if(UiWidget* widget = dynamic_cast<UiWidget*>(pair.first); widget != nullptr)
		{
			if(widget->rect_.y > max_y_)
			{
				max_y = widget->rect_.y;
			}
		}
		/*else if(UiGroup* ui_group = dynamic_cast<UiGroup*>(pair.first.get()); ui_group != nullptr)
		{
			std::vector<UiWidget*> widgets = ui_group->get_navigation_nodes();
			for(UiWidget* widget : widgets)
			{
				if(widget->rect_.y > max_y_)
				{
					max_y = widget->rect_.y;
				}
			}
		}*/
	}
	return max_y;
}

void ScrollableArea::change_position(int x, int y)
{
	//TODO : ne fait rien ??
}