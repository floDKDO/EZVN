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
		Transform(std::string transform_name, int number_of_transform_steps, bool is_character_visible);

		std::string transform_to_focus();
		std::string transform_to_unfocus();

		void recreate_transform(Image& image, std::string transform_name, bool is_visible);
		void show_transform(std::string transform_name, Image& image);

		std::string transform_name_;

	private:
		using Step = std::pair<TransformStep, std::function<void(TransformStep&)>>;
		using LineOfTransformSteps = std::vector<Step>;

		void fill_all_transforms();

		LineOfTransformSteps add_line(std::vector< std::function<void(TransformStep&)>> foncs);

		void tcommon(std::string transform_name, Image& image);
		void focus_common(std::string transform_name, Image& image);
		void autofocus_focus(Image& image);
		void autofocus_unfocus(Image& image);
		void hop(std::string transform_name, Image& image);
		void hop_focus(std::string transform_name, Image& image);
		void sink(std::string transform_name, Image& image);
		void dip(std::string transform_name, Image& image);
		void lhide(Image& image);
		void rhide(Image& image);
		void test(Image& image);
		void hide(Image& image); //TODO : time

		bool is_character_visible_;
		unsigned int current_line_;
		bool is_finished_;

		std::unordered_map<std::string, std::function<void(Image&)>> all_build_methods_;
		std::unordered_map<std::string, std::vector<LineOfTransformSteps>> all_transforms_;
};

