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
		void recreate_transform(std::string transform_name, bool is_visible);
		void show_transform(Image& image, std::string_view transform_name);

		std::string transform_name_;
		std::string previous_transform_name_;

	private:
		struct Step
		{
			TransformStep transform_step_;
			std::function<void(TransformStep&, Image&)> fonc_;
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

			void add_line(std::vector<std::function<void(TransformStep&, Image&)>> foncs)
			{
				LineOfSteps line;
				for(const std::function<void(TransformStep&, Image&)>& fonc : foncs)
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

		enum class TransformKind
		{
			NORMAL,
			FOCUSED
		};

		void init_transform(std::string transform_name);

		void tcommon(std::string transform_name);
		void focus_common(std::string transform_name);
		void hop(std::string transform_name);
		void hop_focus(std::string transform_name);
		void sink(std::string transform_name);
		void dip(std::string transform_name);
		void lhide();
		void rhide();
		void test();
		void hide(); //TODO : time => pas un paramètre mais une variable globale / membre

		//void focus(Image& image);
		//void unfocus(Image& image);

		bool is_character_visible_;
		std::unordered_map<std::string, std::function<void()>> all_build_methods_;
		std::unordered_map<std::string, std::pair<AllLinesOfSteps, TransformKind>> all_transforms_;
};

