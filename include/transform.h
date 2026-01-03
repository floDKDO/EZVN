#pragma once

#include "transformstep.h"

#include <vector>
#include <unordered_map>
#include <functional>

//TODO : ajouter les autres transfos (hop etc.)

class Transform
{
	//TODO : on show/replace/hide => https://www.renpy.org/doc/html/transforms.html#external-atl-events
	public:
		Transform();
		Transform(const std::string transform_name);

		std::string transform_to_focus();
		std::string transform_to_unfocus();

		void create_transform(const std::string transform_name, bool is_visible);
		void show_transform(const std::string transform_name, Image& image);

		bool is_character_visible_;
		std::string transform_name_;
		//std::string previous_transform_name_;

	private:
		struct TransformAllSteps
		{
			TransformAllSteps(const int number_of_transform_steps)
				: current_step_number_(0), transform_finished_(false), number_of_transform_steps_(number_of_transform_steps), transform_steps_(number_of_transform_steps)
			{}

			int current_step_number_;
			bool transform_finished_; //TODO : membre inutilisé
			int number_of_transform_steps_;
			std::vector<TransformStep> transform_steps_;
		};

		void tcommon(const int xpos, Image& image);
		void t11(Image& image);
		void t21(Image& image);
		void t22(Image& image);
		void t31(Image& image);
		void t32(Image& image);
		void t33(Image& image);
		void t41(Image& image);
		void t42(Image& image);
		void t43(Image& image);
		void t44(Image& image);
		void t51(Image& image);
		void t52(Image& image);
		void t53(Image& image);
		void t54(Image& image);
		void t55(Image& image);

		void focus_common(const int xpos, Image& image);
		void f11(Image& image);
		void f21(Image& image);
		void f22(Image& image);
		void f31(Image& image);
		void f32(Image& image);
		void f33(Image& image);
		void f41(Image& image);
		void f42(Image& image);
		void f43(Image& image);
		void f44(Image& image);
		void f51(Image& image);
		void f52(Image& image);
		void f53(Image& image);
		void f54(Image& image);
		void f55(Image& image);

		void test(Image& image);
		void hide(Image& image); //TODO : time

		//std::unordered_map<std::string, std::function<void(Image& image)>> all_transforms_; //ex : {"t11", t11()}

		//std::unordered_map<TransformName, TransformAllSteps> transforms_;

		std::pair<std::string, TransformAllSteps> transform_;
		//std::pair<std::string, std::function<void(Image& image)>>
};

