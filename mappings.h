#define PIN_ENCODER_A 7
#define PIN_ENCODER_B 8

#define NUM_KEYS 9

// direction w.r.t pins on left side
#define B1 6 	// center
#define B2 5 	// right
#define B3 4 	// up
#define B4 3	// left
#define B5 2	// down
#define MODE_SWITCH_PIN 11
#define MOUSE LOW
#define KB HIGH

#define B6 10
#define B7 9
#define B8 1
#define B9 0

#define SDA_PIN D12
#define SCL_PIN D13

#define MAX_MOUSE_MOVE	7
#define MIN_MOUSE_MOVE	0

#define pin_x A0
#define pin_y A1
#define mouse_btn_right_pin 14
#define mouse_btn_left_pin 15

#define COUNT_TIME 10

// --------- menu items display stuff --------- //
#define L 0
#define R 1
#define COL_L_START 16
#define COL_R_START 40

#define ROW0_START 0
#define ROW1_START 20
#define ROW2_START 40
#define ROW3_START 60
#define ROW4_START 80
#define ROW5_START 100

#define NUM_ITEMS 12

int16_t x_val = 0;
int16_t y_val = 0;

int sleep_timer=0;
int sleep_timeout=10000000;
bool sleep_mode=false;

uint16_t delay_val=250;	// keyboard key delay

bool mode_switching_state=false;


// ------------------ menu layout stuff ------------------ //
uint8_t curr_col=0;
uint8_t curr_row=0;
uint8_t curr_selection=0;
static uint8_t menu_pos[ NUM_ITEMS ][ 2 ] = { { COL_L_START,  ROW0_START}, { COL_R_START, ROW0_START },
											  { COL_L_START,  ROW1_START}, { COL_R_START, ROW1_START },
											  { COL_L_START,  ROW2_START}, { COL_R_START, ROW2_START },
											  { COL_L_START,  ROW3_START}, { COL_R_START, ROW3_START },
											  { COL_L_START,  ROW4_START}, { COL_R_START, ROW4_START },
											  { COL_L_START,  ROW5_START}, { COL_R_START, ROW5_START }
									};

String menu_items[]={"A","B","C","D","E","F","G","H","I","J","K","L"};

// String lcase_alphabets[]={};


static String ucase_alphabets[3][16]={
	{"A","B","C","D","E","F","G","H","I","J","K","L"},
	{"M","N","O","P","Q","R","S","T","U","V","W","X"},
	{"Y","Z","0","1","2","3","4","5","6","7","8","9"}

};
// ------------------------------------------------------------- //




struct Button {
	int pin_num;
	String key_name;
	uint8_t action;
};

Button BTN1 = {B1, "ENTER",  KEY_RETURN};
Button BTN2 = {B2, "RIGHT", KEY_RIGHT_ARROW };
Button BTN3 = {B3, "UP", KEY_UP_ARROW };
Button BTN4 = {B4, "LEFT", KEY_LEFT_ARROW };
Button BTN5 = {B5, "DOWN", KEY_DOWN_ARROW };

Button BTN6 = {B6, "ESC", KEY_ESC};
Button BTN7 = {B7, "TAB",  KEY_TAB };
Button BTN8 = {B8, "SPACE",  (char) 0x20}; //space bar
Button BTN9 = {B9, "BSPACE",  KEY_BACKSPACE};



Button buttons[]={BTN1,BTN2,BTN3,BTN4,BTN5,BTN6,BTN7,BTN8,BTN9};

bool oled;

int last_rotary = 0;
int curr_mode = KB; //default mode is keyboard