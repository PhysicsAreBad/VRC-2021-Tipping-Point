#include "main.h"

#include <memory>

#include "robot.h"
#include "utils.h"

std::unique_ptr<Robot> robot(new Robot());

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	robot->init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	while (true) {
		pros::lcd::clear_line(0);
		pros::lcd::set_text(0, std::to_string(robot->getArmPosition()));
		int arm = utils::deadzone(master.get_analog(ANALOG_LEFT_Y), 32);

		double multiplier = master.get_digital(DIGITAL_R2) ? 0.25 : 1;

		bool invert = master.get_digital(DIGITAL_L2);

		int x = utils::deadzone(master.get_analog(ANALOG_RIGHT_X), 32) * multiplier * (invert ? -1 : 1); 
		int y = utils::deadzone(master.get_analog(ANALOG_RIGHT_Y), 32) * multiplier * (invert ? -1 : 1);
		int rotate = utils::deadzone(master.get_analog(ANALOG_LEFT_X), 32) * multiplier * (invert ? -1 : 1);

		robot->drive(x, y, rotate);

		if (master.get_digital(DIGITAL_L1)) {
			robot->armUp();
		} else if (master.get_digital(DIGITAL_R1)) {
			robot->armDown();
		} else {
			robot->moveArm(arm * 0.5 * multiplier);
		}

		pros::delay(5);
	}
}
