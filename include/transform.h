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
		//std::string previous_transform_name_;

	private:
		/*struct TransformAllSteps
		{
			TransformAllSteps(int number_of_transform_steps)
				: current_step_number_(0), transform_finished_(false), number_of_transform_steps_(number_of_transform_steps), transform_steps_(number_of_transform_steps)
			{}

			unsigned int current_step_number_;
			bool transform_finished_; //TODO : membre inutilisé
			unsigned int number_of_transform_steps_;
			std::vector<TransformStep> transform_steps_;
		};*/

		using Step = std::pair<TransformStep, std::function<void(TransformStep&)>>;
		using LineOfTransformSteps = std::vector<Step>;

		void fill_all_transforms(/*Image& image*/);

		//Step create_step(Image& image, std::function<void(TransformStep&)> fonc);
		void add_step(Image& image, std::vector<LineOfTransformSteps>& transform_steps, std::function<void(TransformStep&)> fonc);
		void go_to_next_line_of_steps(std::vector<LineOfTransformSteps>& transform_steps);

		std::vector<LineOfTransformSteps> tcommon(int xpos, Image& image);
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

		std::vector<LineOfTransformSteps> focus_common(int xpos, Image& image);
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

		void autofocus_focus(Image& image);
		void autofocus_unfocus(Image& image);

		std::vector<LineOfTransformSteps> hop(int xpos, Image& image);
		void h11(Image& image);
		void h21(Image& image);
		void h22(Image& image);
		void h31(Image& image);
		void h32(Image& image);
		void h33(Image& image);
		void h41(Image& image);
		void h42(Image& image);
		void h43(Image& image);
		void h44(Image& image);
		void h51(Image& image);
		void h52(Image& image);
		void h53(Image& image);
		void h54(Image& image);
		void h55(Image& image);

		std::vector<LineOfTransformSteps> hop_focus(int xpos, Image& image);
		void hf11(Image& image);
		void hf21(Image& image);
		void hf22(Image& image);
		void hf31(Image& image);
		void hf32(Image& image);
		void hf33(Image& image);
		void hf41(Image& image);
		void hf42(Image& image);
		void hf43(Image& image);
		void hf44(Image& image);
		void hf51(Image& image);
		void hf52(Image& image);
		void hf53(Image& image);
		void hf54(Image& image);
		void hf55(Image& image);

		std::vector<LineOfTransformSteps> sink(int xpos, Image& image);
		void s11(Image& image);
		void s21(Image& image);
		void s22(Image& image);
		void s31(Image& image);
		void s32(Image& image);
		void s33(Image& image);
		void s41(Image& image);
		void s42(Image& image);
		void s43(Image& image);
		void s44(Image& image);
		void s51(Image& image);
		void s52(Image& image);
		void s53(Image& image);
		void s54(Image& image);
		void s55(Image& image);

		std::vector<LineOfTransformSteps> dip(int xpos, Image& image);
		void d11(Image& image);
		void d21(Image& image);
		void d22(Image& image);
		void d31(Image& image);
		void d32(Image& image);
		void d33(Image& image);
		void d41(Image& image);
		void d42(Image& image);
		void d43(Image& image);
		void d44(Image& image);
		void d51(Image& image);
		void d52(Image& image);
		void d53(Image& image);
		void d54(Image& image);
		void d55(Image& image);

		void lhide(Image& image);
		void rhide(Image& image);

		void test(Image& image);
		void hide(Image& image); //TODO : time

		bool is_character_visible_;
		unsigned int current_line_;
		bool is_finished_;

		std::unordered_map<std::string, std::function<void(Image&)>> all_build_methods_;
		std::unordered_map<std::string, std::vector<LineOfTransformSteps>> all_transforms_;
		//std::vector<LineOfTransformSteps> transform_steps_;

		//std::unordered_map<TransformName, TransformAllSteps> transforms_;

		//std::pair<std::string, TransformAllSteps> transform_;
};

