#pragma once

#include "gamestate.h"
#include "textbox.h"
#include "character.h"
#include "backgroundmanager.h"
#include "musicmanager.h"
#include "GUI/texttoggle.h"
#include "sound.h"
#include <vector>
#include <memory>
#include <map>

class InGame : public GameState
{
	public:
	InGame(Game& game, sdl::Renderer& renderer);

	void build_ui_elements(sdl::Renderer& renderer) override;
	void handle_events(const SDL_Event& e) override;
	void draw(sdl::Renderer& renderer) override;
	void update() override;

	void set_initial_dialogue();

	TextToggle* get_texttoggle(const std::string_view text);

	void add_character(const std::string_view character_variable, const std::string_view character_name, const std::string_view character_path, sdl::Renderer& renderer, const std::string_view textbox_path = "", const std::string_view namebox_path = "");
	Character::Editableproperties get_last_character_properties(const std::string_view character_variable);
	std::string get_last_character_name(const std::string_view character_variable);

	void insert_dialogue(const std::string_view character_variable, const std::string_view dialogue);

	void insert_character(const std::string_view character_variable, const std::optional<std::string> new_character_name = std::nullopt, const std::optional<std::string> transform_name = std::nullopt, const std::optional<int> zorder = std::nullopt);

	void insert_sound(const std::string_view sound_path, int fadein_length, int fadeout_length, int volume, int channel, bool loop);

	void insert_autofocus(bool autofocus);

	void show_next_dialogue();

	enum class WhichDialogue
	{
		next,
		previous
	};
	void show_dialogue_mouse_wheel(WhichDialogue which_dialogue);

	void auto_function(Ui* ui);
	void skip_function(Ui* ui);
	void settings_function(Ui* ui);
	void temp_function(Ui* ui);

	void draw_characters(sdl::Renderer& renderer);

	void update_characters();
	void update_autofocus();
	void update_skip_auto_modes();
	void update_sounds();
	void update_textbox();

	BackgroundManager background_manager_;
	MusicManager music_manager_;

	template <typename T>
	struct MyPair
	{
		std::string character_variable_;
		T t_;
	};

	std::vector<MyPair<std::unique_ptr<Character>>> characters_; //unique_ptr pour que stable_sort fonctionne

	struct AudioProperties
	{
		int fadein_length;
		int fadeout_length;
		int volume;
		bool loop;
		int channel; //not used for musics
	};

	std::map<unsigned int, MyPair<Character::Editableproperties>> characters_transforms_;
	std::map<unsigned int, MyPair<const std::string_view>> dialogues_;
	std::map<unsigned int, std::pair<AudioProperties, std::optional<Sound>>> sounds_;
	std::map<unsigned int, bool> autofocus_;

	unsigned int unique_id_;
	unsigned int current_unique_id_;

	unsigned int current_unique_id_when_previous_;
	bool is_current_unique_id_saved_;

	Uint64 last_time_;

	Textbox textbox_;

	private:
		bool skip_mode_;
		bool auto_mode_;

		struct CurrentSound
		{
			unsigned int key_;
			bool played_;
			Sound* sound_;
		};
		CurrentSound currently_playing_sound_;

		bool hide_ui_textbox_;
		sdl::Renderer& renderer_;
};
