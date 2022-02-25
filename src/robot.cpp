#include "robot.h"

//Init sensors & motors
Robot::Robot() {
    frontLeft = new pros::Motor(FRONT_LEFT_MOTOR);
	frontRight = new pros::Motor(FRONT_RIGHT_MOTOR, true);
	backLeft = new pros::Motor(BACK_LEFT_MOTOR);
	backRight = new pros::Motor(BACK_RIGHT_MOTOR, true);

    arm = new pros::Motor(ARM);
}

void Robot::init() {
    arm->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); //Configure the arm motor to hold it's position when stopped
    arm->move_velocity(100);
    pros::delay(100);
    while(arm->get_actual_velocity() > 2)
        arm->move_velocity(100);
    arm->move_velocity(0);
    pros::delay(100);
    arm->set_zero_position(arm->get_position()); //Reset zero position
}

void Robot::drive(int x, int y, int rotate) {
    //Find the largest motor value
  	//Used to prevent clipping due to scale issues (ie a motor being assigned over 100%)
  	double denominator = fmax(fabs(y) + fabs(x) + fabs(rotate), 128);

  	//Assign power according to mechanum control logic
  	*frontLeft = (y + x + rotate) / denominator * 128;
 	*backLeft = (y - x + rotate) / denominator * 128;
 	*frontRight = (y - x - rotate) / denominator * 128;
  	*backRight = (y + x - rotate) / denominator * 128;
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
    *arm = -amount*0.75;
}

void Robot::armUp() {
    arm->move_absolute(-500, 50);
    pros::delay(50);
    while(std::abs(arm->get_actual_velocity()) > 5)
        arm->move_absolute(-500, 50);
}

void Robot::armDown() {
    arm->move_absolute(0, 100);
    pros::delay(50);
    while(std::abs(arm->get_actual_velocity()) > 5)
        arm->move_absolute(0, 100);
}

double Robot::getArmPosition() {
    return arm->get_position();
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
}

