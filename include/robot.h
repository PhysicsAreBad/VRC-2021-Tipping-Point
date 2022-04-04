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

    void init();

    //Drivetrain

    /**
    * @brief Move the robot based off of percentage in 3-axies
    * @param x How much to move the robot backwards/forwards (-128 -> 127)
    * @param y How much to move the robot left/right (-128 -> 127)
    * @param rotate How much to rotate the robot left/right (-128 -> 127)
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

    /**
     * @brief Toggle if the robot drive relative to the ring fork or the goal clamp
     */
    void toggleInverseControls();

    /**
     * @brief Get the direction the robot is driving relative to.
     * 
     * @return true - Movement relative to the goal end
     * @return false - Movement relative to the ring end
     */
    bool getInversed();

    //Arm

    /**
     * @brief Move the arm
     * @param amount How much to move the arm Up/Down (127 -> -128)
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

    /**
     * @brief Get the Arm Position
     * @return the current position of the arm in encoder units (0 should be completely down)
     */
    double getArmPosition();
        
    private:
    //Drivetrain
    pros::Motor* frontLeft;
    pros::Motor* frontRight;
    pros::Motor* backLeft;
    pros::Motor* backRight;
    bool isInverse;

    //Arm
    pros::Motor* arm;
};