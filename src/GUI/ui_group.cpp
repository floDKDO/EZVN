#include "GUI/ui_group.h"
#include "constants.h"

#include <iostream>

UiGroup::UiGroup(int x, int y)
	: title_(nullptr), frame_({x, y, 0, 0})
{
	is_visible_ = true;
}

UiGroup::UiGroup(std::string_view title, int x, int y, sdl::Renderer& renderer)
	: frame_({x, y, 0, 0}), title_(std::make_unique<Text>(title, constants::textbutton_normal_color_, constants::textbutton_font_, constants::textbutton_text_size_, x, y, renderer))
{
	is_visible_ = true;
}

void UiGroup::draw(sdl::Renderer& renderer)
{
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		ui_widget->draw(renderer);
	}

	if(title_ != nullptr)
	{
		title_->draw(renderer);
	}
}

void UiGroup::update()
{
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		ui_widget->update();
	}

	if(title_ != nullptr)
	{
		title_->update();
	}
}

std::vector<UiWidget*> UiGroup::get_navigation_nodes()
{
	std::vector<UiWidget*> widgets;
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		widgets.push_back(ui_widget.get());
	}
	return widgets;
}

UiWidget* UiGroup::add_ui_element(std::unique_ptr<UiWidget> widget)
{
	SDL_Rect rect = widget->rect_;

	int x_offset = 0;
	if(title_ != nullptr)
	{
		x_offset = std::abs(widget->rect_.w - title_->position_.w) / 2; //centrer les ui par rapport au titre
	}
	rect.x = frame_.x - x_offset;

	if(ui_elements_.size() == 0)
	{
		if(title_ == nullptr)
		{
			rect.y = frame_.y;
		}
		else
		{
			rect.y = frame_.y + constants::ui_group_y_spacing_at_top_; // + espace entre le premier ui et le titre
		}
	}
	else
	{
		rect.y = ui_elements_.back()->rect_.y + rect.h + constants::ui_group_y_spacing_between_ui_; // + espace entre chaque ui
	}
	widget->change_position(rect.x, rect.y);

	UiWidget* widget_ptr = widget.get();
	ui_elements_.push_back(std::move(widget));
	return widget_ptr;
}

void UiGroup::set_title(std::string_view title)
{
	title_->text_ = title;
}

void UiGroup::set_center()
{
	for(std::unique_ptr<UiWidget>& ui_widget : ui_elements_)
	{
		ui_widget->change_position((constants::window_width_ / 2) - (ui_widget->rect_.w / 2), ui_widget->rect_.y);
	}
}