#pragma once

#include "transform_step.h"

#include <vector>
#include <unordered_map>
#include <functional>

class Transform
{
	//TODO : on show/replace/hide => https://www.renpy.org/doc/html/transforms.html#external-atl-events
	public:
		Transform();
		Transform(std::string_view transform_name, bool is_character_visible);

		std::string transform_to_focus();
		std::string transform_to_unfocus();
		void recreate_transform(Image& image, std::string transform_name, bool is_visible);
		void show_transform(std::string_view transform_name);

		std::string transform_name_;

	private:
		struct Step
		{
			TransformStep transform_step_;
			std::function<void(TransformStep&)> fonc_;
		};

		struct LineOfSteps
		{
			bool is_line_finished()
			{
				for(const Step& step : steps_)
				{
					if(!step.transform_step_.is_finished_)
					{
						return false;
					}
				}
				return true;
			}

			std::vector<Step> steps_;
		};

		struct AllLinesOfSteps
		{
			AllLinesOfSteps()
				: current_line_(0), is_finished_(false)
			{}

			void add_line(std::vector<std::function<void(TransformStep&)>> foncs)
			{
				LineOfSteps line;
				for(const std::function<void(TransformStep&)>& fonc : foncs)
				{
					line.steps_.push_back({TransformStep(), fonc});
				}
				lines_.push_back(line);
			}

			LineOfSteps& get_current_line()
			{
				return lines_[current_line_];
			}

			void go_to_next_line()
			{
				if(lines_.size() > current_line_ + 1)
				{
					current_line_ += 1;
				}
				else
				{
					is_finished_ = true;
				}
			}

			unsigned int current_line_;
			bool is_finished_;
			std::vector<LineOfSteps> lines_;
		};

		void init_transform(Image& image, std::string transform_name);

		void tcommon(std::string transform_name, Image& image);
		void focus_common(std::string transform_name, Image& image);
		void hop(std::string transform_name, Image& image);
		void hop_focus(std::string transform_name, Image& image);
		void sink(std::string transform_name, Image& image);
		void dip(std::string transform_name, Image& image);
		void lhide(Image& image);
		void rhide(Image& image);
		void test(Image& image);
		void hide(Image& image); //TODO : time => pas un paramètre mais une variable globale / membre

		bool is_character_visible_;
		std::unordered_map<std::string, std::function<void(Image&)>> all_build_methods_;
		std::unordered_map<std::string, AllLinesOfSteps> all_transforms_;
};

