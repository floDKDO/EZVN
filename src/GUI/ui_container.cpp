#include "GUI/ui_container.h"
#include "constants.h"

#include <iostream>

UiContainer::UiContainer(int x, int y)
	: title_(nullptr), frame_({x, y, 0, 0})
{}

UiContainer::UiContainer(std::string_view title, int x, int y, sdl::Renderer& renderer)
	: frame_({x, y, 0, 0}), title_(std::make_unique<Text>(title, constants::textbutton_normal_color_, constants::textbutton_font_, constants::textbutton_text_size_, x, y, renderer))
{}

void UiContainer::draw(sdl::Renderer& renderer)
{
	for(Ui* ui : ui_elements_)
	{
		ui->draw(renderer);
	}

	if(title_ != nullptr)
	{
		title_->draw(renderer);
	}
}

void UiContainer::update()
{
	for(Ui* ui : ui_elements_)
	{
		ui->update();
	}

	if(title_ != nullptr)
	{
		title_->update();
	}
}

std::vector<UiWidget*> UiContainer::get_navigation_nodes()
{
	return ui_elements_;
}

void UiContainer::add_ui_element(UiWidget* widget)
{
	SDL_Rect rect = widget->rect_;
	rect.x = frame_.x;
	if(ui_elements_.size() == 0)
	{
		rect.y = frame_.y + 80; //TODO : hardcodé (80 = espace entre le premier checkable et le titre)
	}
	else
	{
		rect.y = ui_elements_.back()->rect_.y + rect.h + 20; //TODO : hardcodé (20 = espace entre chaque checkable)
	}
	widget->change_position(rect.x, rect.y);

	ui_elements_.push_back(widget);
}

void UiContainer::set_title(std::string_view title)
{
	title_->text_ = title;
}