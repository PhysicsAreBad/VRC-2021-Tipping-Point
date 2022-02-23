#include "api.h"
#include "ports.h"

/**
 * @brief Represents all the actions/movements of the robot
 * 
 */
class Robot {
    public:
    Robot();
    ~Robot();

    //Drivetrain

    /**
    * @brief Move the robot based off of percentage in 3-axies
    * @param x How much to move the robot backwards/forwards (-100 -> 100)
    * @param y How much to move the robot left/right (-100 -> 100)
    * @param rotate How much to rotate the robot left/right (-100 -> 100)
    */
    void drive(int x, int y, int rotate);

    /**
     * @brief Drive the robot using tank movement
     * @param left Percent left-side
     * @param right Percent right-side
     */
    void driveTank(int left, int right);

    /**
     * @brief Stop the robot's wheels
     */
    void stopDrive();

    //Arm

    /**
     * @brief Move the arm
     * @param amount How much to move the arm Up/Down (100 -> -100)
     */
    void moveArm(int amount);

    /**
     * @brief Move the arm into the preset 'up' position
     */
    void armUp();

    /**
     * @brief Move the arm into the preset 'down' position
     */
    void armDown();

    private:
    //Drivetrain
    pros::Motor* frontLeft;
    pros::Motor* frontRight;
    pros::Motor* backLeft;
    pros::Motor* backRight;

    //Arm
    pros::Motor* arm;
    pros::ADIDigitalIn* limitSwitch;
};