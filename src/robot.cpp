#include "robot.h"

//Init sensors & motors
Robot::Robot() {
    frontLeft = new pros::Motor(FRONT_LEFT_MOTOR);
	frontRight = new pros::Motor(FRONT_RIGHT_MOTOR, true);
	backLeft = new pros::Motor(BACK_LEFT_MOTOR);
	backRight = new pros::Motor(BACK_RIGHT_MOTOR, true);

    arm = new pros::Motor(ARM);
    arm->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); //Configure the arm motor to hold it's position when stopped
    arm->set_zero_position(arm->get_position()); //Reset zero position
    limitSwitch = new pros::ADIDigitalIn(ARM_MAX_LIMIT_SWITCH);
}

void Robot::drive(int x, int y, int rotate) {
    //Find the largest motor value denominator
  	//Used to prevent clipping due to scale issues
  	double denominator = fmax(fabs(y) + fabs(x) + fabs(rotate), 100);

  	//Assign power according to mechanum control logic
  	*frontLeft = (y + x + rotate) / denominator * 100;
 	*backLeft = (y - x + rotate) / denominator * 100;
 	*frontRight = (y - x - rotate) / denominator * 100;
  	*backRight = (y + x - rotate) / denominator * 100;
}

void Robot::driveTank(int left, int right) {
    *frontLeft = left;
    *backLeft = left;
    *frontRight = right;
    *backRight = right;
}

void Robot::stopDrive() {
    driveTank(0, 0);
}

void Robot::moveArm(int amount) {
    if (limitSwitch->get_value() == 0 || amount < 0) {
        arm->move_velocity(amount*0.75);
    }
}

void Robot::armUp() {
    arm->move_absolute(10, 0.5);
}

void Robot::armDown() {
    arm->move_absolute(0, 0.5);
}

//Cleanup pointers
Robot::~Robot() {
    stopDrive();
    armDown();

    //Drivetrain
    delete frontLeft;
    delete frontRight;
    delete backLeft;
    delete backRight;

    //Arm
    delete arm;
    delete limitSwitch;
}

