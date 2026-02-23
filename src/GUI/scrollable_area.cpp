#include "GUI/scrollable_area.h"
#include "constants.h"

#include <iostream>

ScrollableArea::ScrollableArea(int x, int y, int w, int h, sdl::Renderer& renderer)
	: ScrollableArea("", x, y, w, h, renderer)
{}

ScrollableArea::ScrollableArea(std::string_view title, int x, int y, int w, int h, sdl::Renderer& renderer)
	: UiWidget(renderer), frame_({x, y, w, h}), 
	scrollbar_(x + w - constants::slider_handle_size_, y + (constants::slider_handle_size_ / 2), h - constants::slider_handle_size_, h / constants::slider_step_count_, renderer, std::bind(&ScrollableArea::callback_function, this, std::placeholders::_1)), 
	title_(!title.empty() ? std::make_unique<Text>(title, constants::textbutton_normal_color_, constants::textbutton_font_, constants::textbutton_text_size_, x, y, renderer) : nullptr),
	renderer_(renderer)
{
	if(title_ != nullptr)
	{
		title_->position_.x += (frame_.w - title_->get_width_text()) / 2;
		title_->position_.y -= title_->get_height_text();
	}

	rect_ = frame_;
}

void ScrollableArea::scroll_elements()
{
	for(auto& pair : elements_value_)
	{
		if(std::holds_alternative<UiWidget*>(pair.first))
		{
			UiWidget* widget = std::get<UiWidget*>(pair.first);

			int scroll_offset = get_scroll_offset();
			//std::cout << scroll_offset << std::endl;

			SDL_Rect rect = widget->rect_;
			rect.y = pair.second + scroll_offset; // + pour monter, - pour descendre
			widget->change_position(rect.x, rect.y);
		}
		else if(std::holds_alternative<Dialogue>(pair.first))
		{
			Text* character_name = std::get<Dialogue>(pair.first).first;
			Text* dialogue = std::get<Dialogue>(pair.first).second;

			int scroll_offset_character_name = get_scroll_offset();
			int scroll_offset_dialogue = get_scroll_offset();

			//std::cout << "scroll_offset_character_name: " << scroll_offset_character_name << ", scroll_offset_dialogue: " << scroll_offset_dialogue << std::endl;

			SDL_Rect rect_character_name = character_name->position_;
			SDL_Rect rect_dialogue = dialogue->position_;

			rect_character_name.y = pair.second + scroll_offset_character_name; // + pour monter, - pour descendre
			rect_dialogue.y = pair.second + scroll_offset_dialogue; // + pour monter, - pour descendre

			character_name->position_.y = rect_character_name.y;
			dialogue->position_.y = rect_dialogue.y;
		}
	}
}

void ScrollableArea::callback_function([[maybe_unused]] Ui* ui)
{
	scroll_elements();
}

void ScrollableArea::draw(sdl::Renderer& renderer)
{
	renderer.set_clip_rect(&frame_);
	renderer.set_draw_color(200, 200, 200, 255); //TODO : hardcodé
	renderer.fill_rect(&frame_);
	for(auto& pair : elements_value_)
	{
		if(std::holds_alternative<UiWidget*>(pair.first))
		{
			//if(SDL_HasIntersection(&pair.first->rect_, &frame_))
			UiWidget* widget = std::get<UiWidget*>(pair.first);
			widget->draw(renderer);
		}
		else if(std::holds_alternative<Dialogue>(pair.first))
		{
			Text* character_name = std::get<Dialogue>(pair.first).first;
			Text* dialogue = std::get<Dialogue>(pair.first).second;
			character_name->draw(renderer);
			dialogue->draw(renderer);
		}
	}
	scrollbar_.draw(renderer);
	renderer.set_clip_rect(nullptr);

	if(title_ != nullptr)
	{
		title_->draw(renderer);
	}
}

void ScrollableArea::update()
{
	for(auto& pair : elements_value_)
	{
		if(std::holds_alternative<UiWidget*>(pair.first))
		{
			//if(SDL_HasIntersection(&pair.first->rect_, &frame_))
			UiWidget* widget = std::get<UiWidget*>(pair.first);
			widget->update();
		}
		else if(std::holds_alternative<Dialogue>(pair.first))
		{
			Text* character_name = std::get<Dialogue>(pair.first).first;
			Text* dialogue = std::get<Dialogue>(pair.first).second;
			character_name->update();
			dialogue->update();
		}
	}
	scrollbar_.update();

	if(title_ != nullptr)
	{
		title_->update();
	}

	//std::cout << max_y_ << ", " << get_max_y() << std::endl;
}

std::vector<UiWidget*> ScrollableArea::get_navigation_nodes()
{
	std::vector<UiWidget*> vector;
	for(auto& pair : elements_value_)
	{
		if(std::holds_alternative<UiWidget*>(pair.first))
		{
			UiWidget* widget = std::get<UiWidget*>(pair.first);
			vector.push_back(widget);
		}
	}
	vector.push_back(&scrollbar_);
	return vector;
}

int ScrollableArea::get_scroll_offset()
{
	float t = float((scrollbar_.current_value_ - scrollbar_.min_value_) / (scrollbar_.max_value_ - scrollbar_.min_value_)); 
	//std::cout << "t: " << t << ", value: " << scrollbar_.current_value_ << std::endl;
	int max_scroll = get_max_y() - (frame_.y + frame_.h); //si on a le curseur de la scrollbar tout en haut et qu'on le descend au max, ceci est la valeur (max) en y dont on a monté les éléments de la ScrollableArea
	int max_offset = 0;
	if(max_scroll > 0) //on peut scroll uniquement si la zone à scroller est plus grande que frame_.h 
	{
		max_offset = -max_scroll; 

		for(auto it = elements_value_.crbegin(); it != elements_value_.crend(); ++it)
		{
			if(std::holds_alternative<Dialogue>(it->first))
			{
				//Text* character_name = std::get<Dialogue>(it->first).first;
				Text* dialogue = std::get<Dialogue>(it->first).second;
				int last_dialogue_height;
				dialogue->texture_->query(nullptr, nullptr, nullptr, &last_dialogue_height);
				max_offset -= last_dialogue_height;
				break;
			}
		}
	}
	//std::cout << "get_max_y(): " << get_max_y() << ", max_scroll: " << max_scroll << ", " << "max_offset : " << max_offset << ", res : " << t * max_offset << std::endl;
	return int(t * max_offset);
}

void ScrollableArea::add_ui_element(std::unique_ptr<UiWidget> widget)
{
	elements_.push_back(std::move(widget));
	UiWidget* w = dynamic_cast<UiWidget*>(std::get<std::unique_ptr<Ui>>(elements_.back()).get());
	elements_value_.push_back({w, w->rect_.y});
}

void ScrollableArea::add_ui_element(std::unique_ptr<UiGroup> ui_group)
{
	std::vector<UiWidget*> widgets = ui_group->get_navigation_nodes();
	bool first = false;
	for(UiWidget* widget : widgets)
	{
		if(!first)
		{
			//std::cout << "FIRST Y : " << widget->rect_.y << std::endl;
			first = true;
		}
		elements_value_.push_back({widget, widget->rect_.y});
	}
	elements_.push_back(std::move(ui_group)); 
}

void ScrollableArea::add_text(DialoguePtr text)
{
	std::cout << "RESULTAT: " << text.first->text_ << ", " << text.second->text_ << std::endl;

	int last_text_y = 0;
	int last_height = 0;
	for(auto it = elements_value_.crbegin(); it != elements_value_.crend(); ++it)
	{
		if(std::holds_alternative<Dialogue>(it->first))
		{
			Text* previous_dialogue = std::get<Dialogue>(it->first).second;
			last_text_y = it->second;
			last_text_y += 20; //TODO : hardcodé => espace entre chaque dialogue
			previous_dialogue->texture_->query(nullptr, nullptr, nullptr, &last_height); //height du dialogue précédent
			break;
		}
	}

	for(auto it = elements_value_.crbegin(); it != elements_value_.crend(); ++it)
	{
		if(std::holds_alternative<Dialogue>(it->first))
		{
			std::cout << "Y: " << std::get<Dialogue>(it->first).second->position_.y << std::endl;
		}
	}

	if(last_text_y == 0)
	{
		last_text_y += frame_.y; //pour le premier dialogue, le placer correctement dans frame_ si frame_.y est différent de 0
		last_text_y += 20; //TODO : hardcodé => espace entre le premier dialogue et le haut de la ScrollableArea
	}

	std::cout << "LAST TEXT Y: " << last_text_y << std::endl;

	elements_.push_back(std::move(text));
	Text* character_name = std::get<DialoguePtr>(elements_.back()).first.get();
	Text* dialogue = std::get<DialoguePtr>(elements_.back()).second.get();

	character_name->position_.y = last_text_y + last_height; 
	dialogue->position_.y = last_text_y + last_height; 
	std::cout << "FINAL POSITION Y: " << character_name->position_.y << ", " << dialogue->position_.y << std::endl;

	elements_value_.push_back(std::make_pair(std::make_pair(character_name, dialogue), dialogue->position_.y)); //character_name et dialogue ont la même valeur pour y
}

void ScrollableArea::remove_last_text()
{
	//supprimer le dialogue courant et le dialogue précédent dans le cas d'un scrollback
	std::get<DialoguePtr>(elements_.back()).first.reset();
	std::get<DialoguePtr>(elements_.back()).second.reset();
	elements_.pop_back();
	elements_value_.pop_back();

	std::get<DialoguePtr>(elements_.back()).first.reset();
	std::get<DialoguePtr>(elements_.back()).second.reset();
	elements_.pop_back();
	elements_value_.pop_back();
}

int ScrollableArea::get_max_y() const
{
	int max_y = 0;
	for(auto& pair : elements_value_)
	{
		if(pair.second > max_y)
		{
			max_y = pair.second;
		}
	}
	return max_y;
}

void ScrollableArea::change_position([[maybe_unused]] int x, [[maybe_unused]] int y)
{
	//TODO : ne fait rien ??
}