#include "text.h"
#include "constants.h"

#include <iostream>

int Text::global_text_divisor_ = 45;

Text::Text(const std::string text, const SDL_Color color, const std::string font_path, const int font_size, const int x, const int y, sdl::Renderer& renderer, const Uint32 wrap_length)
	:text_(text), color_(color), font_size_(font_size), font_style_(0), renderer_(renderer), wrap_length_(wrap_length), text_dialogue_(""), index_dialogue_(0), 
	previous_text_(""), previous_font_style_(0), font_path_(font_path), is_finished_(false), /*local_text_speed_(global_text_divisor_),*/ last_time_(0), 
	font_(font_path_, font_size_), font_outline_(font_path_, font_size_), outline_size_(constants::text_outline_size_)
{
	int w, h;
	font_.size_UTF8(text_, &w, &h);
	position_ = {x, y, w, h};

	font_outline_.set_outline(outline_size_);

	if(wrap_length_ != 0)
	{
		is_dialogue_ = true;
	}
	else
	{
		is_dialogue_ = false;
	}

	create_surface_texture();
}

void Text::create_surface_texture()
{
	if(text_.empty() || (is_dialogue_ && text_dialogue_.empty()))
	{
		surface_outline_ = std::make_unique<sdl::Surface>(font_outline_, " ", constants::text_outline_color_, wrap_length_);
		surface_ = std::make_unique<sdl::Surface>(font_, " ", color_, wrap_length_);
	}
	else
	{
		if(is_dialogue_)
		{
			surface_outline_ = std::make_unique<sdl::Surface>(font_outline_, text_dialogue_, constants::text_outline_color_, wrap_length_);
			surface_ = std::make_unique<sdl::Surface>(font_, text_dialogue_, color_, wrap_length_);
		}
		else
		{
			surface_outline_ = std::make_unique<sdl::Surface>(font_outline_, text_, constants::text_outline_color_, wrap_length_);
			surface_ = std::make_unique<sdl::Surface>(font_, text_, color_, wrap_length_);
		}
	}
	position_.w = surface_->fetch()->w;
	position_.h = surface_->fetch()->h;

	SDL_Rect rect = {-outline_size_, -outline_size_, position_.w, position_.h};

	surface_outline_->set_blend_mode(SDL_BLENDMODE_BLEND);
	surface_outline_->blit(nullptr, *surface_, &rect);

	texture_ = std::make_unique<sdl::Texture>(renderer_, *surface_);
	texture_->set_blend_mode(SDL_BLENDMODE_BLEND);
}

void Text::show()
{
	color_.a = 255;
	texture_->set_alpha_mod(color_.a);
}

void Text::hide()
{
	color_.a = 0;
	texture_->set_alpha_mod(color_.a);
}

void Text::change_color(const SDL_Color color)
{
	color_ = color;
	texture_->set_color_mod(color_.r, color_.g, color_.b);
}

void Text::set_italic()
{
	font_style_ |= TTF_STYLE_ITALIC;
}

void Text::set_bold()
{
	font_style_ |= TTF_STYLE_BOLD;
}

void Text::set_underline()
{
	font_style_ |= TTF_STYLE_UNDERLINE;
}

void Text::set_strike_through()
{
	font_style_ |= TTF_STYLE_STRIKETHROUGH;
}

void Text::unset_italic()
{
	font_style_ &= ~TTF_STYLE_ITALIC;
}

void Text::unset_bold()
{
	font_style_ &= ~TTF_STYLE_BOLD;
}

void Text::unset_underline()
{
	font_style_ &= ~TTF_STYLE_UNDERLINE;
}

void Text::unset_strike_through()
{
	font_style_ &= ~TTF_STYLE_STRIKETHROUGH;
}

void Text::unset_all()
{
	font_style_ = TTF_STYLE_NORMAL;
}

int Text::get_width_one_char(const char c)
{
	std::string s(1, c);
	int w;
	font_.size_UTF8(s, &w, nullptr);
	return w;
}

int Text::get_width_text() 
{
	int w;
	font_.size_UTF8(text_, &w, nullptr);
	return w;
}

int Text::get_height_one_char(const char c)
{
	std::string s(1, c);
	int h;
	font_.size_UTF8(s, nullptr, &h);
	return h;
}

int Text::get_height_text()
{
	int h;
	font_.size_UTF8(text_, nullptr, &h);
	return h;
}

void Text::draw(sdl::Renderer& renderer)
{
	SDL_RenderCopy(renderer.fetch(), texture_->fetch(), nullptr, &position_);
}

//TODO : écrire le code spécifique aux dialogues dans la classe TextBox ??
//TODO : écrire le code spécifique aux inputfields dans la classe Inputfield ??
void Text::update() 
{
	if(is_dialogue_) 
	{
		Uint64 now = SDL_GetTicks64();
		if(now - last_time_ > (int(float(initial_text_speed_) / float(global_text_divisor_))) && index_dialogue_ < text_.length())
		{
			text_dialogue_.push_back(text_[index_dialogue_]);
			index_dialogue_ += 1;
			last_time_ = now;
		}

		if(index_dialogue_ == text_.length() - 1)
		{
			is_finished_ = true;
		}
	}

	if(previous_font_style_ != font_style_)
	{
		font_.set_style(font_style_);
		font_outline_.set_style(font_style_);
		create_surface_texture();
		previous_font_style_ = font_style_;
	}

	if(previous_text_ != text_ || (is_dialogue_ && previous_text_ != text_dialogue_)) //the text has been modified (in a inputfield for example)
	{
		create_surface_texture();

		if(is_dialogue_)
		{
			previous_text_ = text_dialogue_;
		}
		else
		{
			previous_text_ = text_;
		}
	}
}