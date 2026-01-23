#include "text.h"
#include "constants.h"

#include <iostream>

int Text::global_text_divisor_ = 45;
int Text::initial_text_speed_ = 500;

Text::Text(std::string_view text, Color color, std::string_view font_path, int font_size, int x, int y, sdl::Renderer& renderer)
	: Text(text, color, font_path, font_size, x, y, renderer, false, 0)
{}

Text::Text(std::string_view text, Color color, std::string_view font_path, int font_size, int x, int y, sdl::Renderer& renderer, bool is_animated, Uint32 wrap_length)
	: Drawable(renderer, color), text_(text), text_dialogue_(""), index_dialogue_(0), is_finished_(false), wrap_length_(wrap_length),
	font_size_(font_size), font_style_(0), previous_font_style_(0), font_path_(font_path), font_(font_path_, font_size_),
	outline_size_(constants::text_outline_size_), font_outline_(create_outline()),
	previous_text_(""), is_animated_(is_animated),
	surface_(create_surface(font_, color_)), surface_outline_(create_surface(font_outline_, constants::text_outline_color_))
	/*,local_text_speed_(global_text_divisor_),*/
{
	int w, h;
	font_.size_UTF8(text_, &w, &h);
	position_ = {x, y, w, h};

	texture_ = std::make_unique<sdl::Texture>(renderer_, surface_);
	texture_->set_blend_mode(SDL_BLENDMODE_BLEND);
}

sdl::Font Text::create_outline()
{
	sdl::Font font_outline = sdl::Font(font_path_, font_size_);
	font_outline.set_outline(outline_size_);
	return font_outline;
}

sdl::Surface Text::create_surface(sdl::Font& font, Color color)
{
	std::string text;
	if(text_.empty() || (is_animated_ && text_dialogue_.empty()))
	{
		text = " ";
	}
	else
	{
		if(is_animated_)
		{
			text = text_dialogue_;
		}
		else
		{
			text = text_;
		}
	}

	sdl::Surface surface = sdl::Surface(font, text, color.to_SDL_Color(), wrap_length_);
	surface.set_blend_mode(SDL_BLENDMODE_BLEND);

	position_.w = surface.fetch()->w;
	position_.h = surface.fetch()->h;

	if(&font == &font_outline_)
	{
		SDL_Rect rect = {-outline_size_, -outline_size_, position_.w, position_.h};
		surface.blit(nullptr, surface_, &rect);
	}

	return surface;
}

void Text::recreate_surfaces_texture()
{
	surface_ = create_surface(font_, color_);
	surface_outline_ = create_surface(font_outline_, constants::text_outline_color_);

	texture_ = std::make_unique<sdl::Texture>(renderer_, surface_);
	texture_->set_blend_mode(SDL_BLENDMODE_BLEND);
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

int Text::get_width_one_char(char c) const
{
	std::string s(1, c);
	int w;
	font_.size_UTF8(s, &w, nullptr);
	return w;
}

int Text::get_width_text() const
{
	int w;
	font_.size_UTF8(text_, &w, nullptr);
	return w;
}

int Text::get_height_one_char(char c) const
{
	std::string s(1, c);
	int h;
	font_.size_UTF8(s, nullptr, &h);
	return h;
}

int Text::get_height_text() const
{
	int h;
	font_.size_UTF8(text_, nullptr, &h);
	return h;
}

void Text::draw(sdl::Renderer& renderer)
{
	renderer.copy(*texture_, nullptr, &(position_), angle_, nullptr, flip_);
	//SDL_RenderCopy(renderer.fetch(), texture_->fetch(), nullptr, &position_);
}

//TODO : écrire le code spécifique aux dialogues dans la classe TextBox ??
//TODO : écrire le code spécifique aux inputfields dans la classe Inputfield ??
void Text::update() 
{
	if(is_animated_)
	{		
		Uint64 now = SDL_GetTicks64();

		if(now - last_time_ > (int(float(initial_text_speed_) / float(global_text_divisor_))) && index_dialogue_ < text_.length())
		{
			text_dialogue_.push_back(text_[index_dialogue_]);
			index_dialogue_ += 1;
			last_time_ = now;
		}

		if(text_.length() == 0 || index_dialogue_ == text_.length() - 1) //TODO : text_.length() == 0 obligatoire ??
		{
			is_finished_ = true;
		}
	}
	else
	{
		is_finished_ = true; //pour bouton Skip
	}

	if(previous_font_style_ != font_style_)
	{
		font_.set_style(font_style_);
		font_outline_.set_style(font_style_);
		recreate_surfaces_texture();
		previous_font_style_ = font_style_;
	}

	if(previous_text_ != text_ || (is_animated_ && previous_text_ != text_dialogue_)) //the text has been modified (in a inputfield for example)
	{
		recreate_surfaces_texture();

		if(is_animated_)
		{
			previous_text_ = text_dialogue_;
		}
		else
		{
			previous_text_ = text_;
		}
	}
}