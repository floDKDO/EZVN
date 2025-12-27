#include "musicmanager.h"
#include "constants.h"

MusicManager::MusicManager(unsigned int& unique_id, unsigned int& current_unique_id)
	: unique_id_(unique_id), current_unique_id_(current_unique_id), currently_playing_music_(nullptr)
{}

void MusicManager::insert_music(const std::string_view music_path, int fadein_length, int fadeout_length, int volume, bool loop)
{
	if(music_path.empty()) //stop music
	{
		musics_.insert({unique_id_, std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop}, std::nullopt)}); //TODO : std::make_pair a l'air obligatoire ici
	}
	else //play music
	{
		musics_.insert({unique_id_, std::make_pair(AudioProperties{fadein_length, fadeout_length, volume, loop}, Music(music_path))});
	}
	unique_id_ += 1;
}

void MusicManager::update()
{
	//TODO : regarder s'il faut modifier des choses par rapport � la boucle des sons en-dessous
	for(unsigned int i = current_unique_id_; i != -1; --i)
	{
		if(musics_.count(i))
		{
			std::pair<AudioProperties, std::optional<Music>>& music_pair = musics_.at(i);
			AudioProperties& music_properties = music_pair.first;

			if(music_pair.second.has_value()) //play music
			{
				Music& music = music_pair.second.value();

				if(currently_playing_music_ == &music)
				{
					if(!sdl::Music::playing())
					{
						music.change_volume(music_properties.volume);
						music.play_music(music_properties.loop, music_properties.fadein_length);
					}
				}
				else //si une musique est d�j� en train de se jouer, il faut stopper (avec ou sans fadeout) la musique courante avant de jouer la nouvelle
				{
					sdl::Music::fade_out(music_properties.fadeout_length);
					currently_playing_music_ = &music;
				}
			}
			else // stop music
			{
				if(sdl::Music::playing())
				{
					sdl::Music::fade_out(music_properties.fadeout_length);
				}
			}
			break;
		}

		//aucune musique trouv�e => stopper l'�ventuelle musique qui �tait en train de se jouer
		if(i == 0)
		{
			//si scroll en arri�re et aucune musique => fadeout de 1.5 secondes (valeur constante)
			if(sdl::Music::playing())
			{
				sdl::Music::fade_out(constants::fadeout_length_scroll_back_);
			}
		}
	}

	if(currently_playing_music_ != nullptr)
	{
		currently_playing_music_->update();
	}
}