#pragma once

#include <string>
#include <string_view>

namespace sdl
{

class PrefPath
{
	public:
		PrefPath(); //SDL_GetPrefPath()
		PrefPath(const PrefPath& pref_path) = delete;
		PrefPath(PrefPath&& pref_path) = delete;
		PrefPath& operator=(const PrefPath& pref_path) = delete;
		PrefPath& operator=(PrefPath&& pref_path) = delete;
		~PrefPath(); //SDL_free()

		//pas de méthode "fetch" car inutile
		std::string get_pref_path() const;

	private:
		char* pref_path_;
};

}