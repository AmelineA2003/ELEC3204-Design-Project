#define outputA 2  //CLK pin
#define outputB 3  //DT pin
#define rstbtn  4  //reset button pin

#define IN3 9
#define IN4 10
#define PWM_B 11

#define GROUND 5
#define LVL1 6
#define LVL2 7
#define E-Brake 8;

//wheel paramters
#define R 7   //radius of the wheel from centre to edge
#define N 40  //no of steps for one rotation
#define pi 3.14

// dynamic variables
int counter = 0;

int pwm;
int duty_cycle;
float displacement;

float ground_disp;
float first_disp;
float second_disp;

bool gound_button;
bool lvl1_button;
bool lvl2_button;

//functions

void readEncoder();
int getCounter();
void resetCounter();
void up_dir();
void change_dir;
void speed_calibrate(float req_speed, float* duty_cycle);
void lvl_calibrate (float* duty_cycle, float* ground_disp, float* first_disp, float* second_disp);

// Get the counter value, disabling interrupts.
// This make sure readEncoder() doesn't change the value
// while we're reading it.
