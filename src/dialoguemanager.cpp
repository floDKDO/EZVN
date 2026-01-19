#include "dialoguemanager.h"

DialogueManager::DialogueManager(sdl::Renderer& renderer)
	: last_time_(0), which_dialogue_from_where_({Where::none, false, false}), skip_mode_(false), auto_mode_(false), textbox_(renderer), renderer_(renderer)
{}

void DialogueManager::handle_events_mouse_wheel(const SDL_Event& e)
{
	//Dialogues
	if(e.type == SDL_MOUSEWHEEL) //condition placée en premier pour que le scroll de la mouse wheel sur un textbutton fonctionne
	{
		if(e.wheel.y > 0) //scroll vers l'avant => reculer d'un dialogue
		{
			//annuler le mode skip
			//skip_toggle_->change_checked(false); //TODO
			skip_mode_ = false;

			which_dialogue_from_where_ = {Where::prev, true, false};
		}
		else //scroll vers l'arrière => avancer d'un dialogue
		{
			which_dialogue_from_where_ = {Where::next, true, false};
		}
	}
}

void DialogueManager::handle_events_keyboard_mouse(const SDL_Event& e)
{
	//Dialogues
	if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
	|| (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT))
	{
		//annuler le mode auto
		//auto_toggle_->change_checked(false); //TODO
		auto_mode_ = false;

		if(textbox_.text_.is_finished_)
		{
			which_dialogue_from_where_ = {Where::next, false, true};
		}
		else
		{
			textbox_.text_.is_animated_ = false; //afficher le dialogue en entier après un clic / touche espace sur un dialogue en train de s'afficher
		}
	}
}

void DialogueManager::draw(sdl::Renderer& renderer)
{
	textbox_.draw(renderer);
}

void DialogueManager::update_skip_auto_modes()
{
	if(skip_mode_)
	{
		which_dialogue_from_where_ = {Where::next, false, false};
		//show_next_dialogue(); 
	}

	//Dialogues
	if(auto_mode_)
	{
		if(textbox_.text_.is_finished_)
		{
			Uint64 now = SDL_GetTicks64();
			if(last_time_ == 0)
			{
				last_time_ = now;
			}

			if(now > last_time_ + textbox_.get_text_delay())
			{
				which_dialogue_from_where_ = {Where::next, false, true};
				//show_next_dialogue();

				last_time_ = SDL_GetTicks64();
			}
		}
	}
}

void DialogueManager::update(Script::InfoDialogue& info_dialogue, const Character& character)
{
	update_skip_auto_modes();

	textbox_.update();

	if(which_dialogue_from_where_.which_dialogue_ == Where::none && !textbox_.is_first_dialogue_)
	{
		return;
	}

	std::cout << "*************************PERSO: " << info_dialogue.character_variable_ << ", texte: " << info_dialogue.t_ << std::endl;
	textbox_.show_new_dialogue(info_dialogue.t_, character.properties_.name_, skip_mode_, which_dialogue_from_where_.wait_for_end_of_dialogue_);
	textbox_.change_textbox(character.properties_.textbox_path_, renderer_);
	textbox_.change_namebox(character.properties_.namebox_path_, renderer_);
	textbox_.change_namebox_text_color(character.properties_.namebox_text_color_);
	//which_dialogue_from_where_ = {Where::none, false, false};
}

void DialogueManager::reset()
{
	textbox_.text_.text_ = "";
	textbox_.is_first_dialogue_ = true;
}