#include "main.h"

#include <memory>

#include "robot.h"
#include "utils.cpp"

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

void renderControllerScreen(pros::Controller controller, bool isInversed, bool isSlowed) {
		controller.clear_line(0);
		pros::delay(200);
		controller.set_text(0, 2, "I: " + BoolToString(isInversed) + " S: " + BoolToString(isSlowed));
}

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

	bool isSlowed = false;

	renderControllerScreen(master, robot->getInversed(), isSlowed);

	while (true) {
		int arm = deadzone(master.get_analog(ANALOG_LEFT_Y), 48);

		if (master.get_digital(DIGITAL_L2)) {
			robot->toggleInverseControls();
			renderControllerScreen(master, robot->getInversed(), isSlowed);
		}
			
		if (master.get_digital(DIGITAL_R2)) {
			isSlowed = !isSlowed;
			renderControllerScreen(master, robot->getInversed(), isSlowed);
		}
			
		pros::lcd::set_text(1, "Inversed: " + BoolToString(robot->getInversed()) + "  Slowed: " + BoolToString(isSlowed));
		
		
		double multiplier = isSlowed ? 0.25 : 1;


		int x = deadzone(master.get_analog(ANALOG_RIGHT_X), 48) * multiplier ; 
		int y = deadzone(master.get_analog(ANALOG_RIGHT_Y), 48) * multiplier;
		int rotate = deadzone(master.get_analog(ANALOG_LEFT_X), 48) * multiplier * (robot->getInversed() ? -1 : 1);

		robot->drive(x, y, rotate);

		if (master.get_digital(DIGITAL_L1)) {
			robot->armUp();
		} else if (master.get_digital(DIGITAL_R1)) {
			robot->armDown();
		} else {
			robot->moveArm(arm * (isSlowed ? 0.50 : 1));
		}

		pros::delay(10);
	}
}
