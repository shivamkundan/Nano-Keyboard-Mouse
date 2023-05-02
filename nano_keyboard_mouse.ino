// See: https://w3c.github.io/uievents/tools/key-event-viewer.html

// special: UNDO
// modifiers: ESC, TAB, CAPS, SHIFT, CTRL, ALT, WIN, BSPACE, DEL

// categories: letters, numbers, special chars, modifiers, function keys,

#include <Keyboard.h>                               //Including the keyboard library
#include <Mouse.h>                                  //Including the mouse library

#include <RotaryEncoder.h>
#include <Adafruit_TestBed.h>
#include "oled.h"

extern Adafruit_TestBed TB;
#define DEFAULT_I2C_PORT &Wire

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

RotaryEncoder encoder(PIN_ENCODER_A, PIN_ENCODER_B, RotaryEncoder::LatchMode::TWO03);

void draw_menu_items(){
	// -- menu_item(col,row,selected,text);


	for (int i=0;i<NUM_ITEMS;i++){


		curr_col=menu_pos[i][0];
		curr_row=menu_pos[i][1];

		if (i==curr_selection){
			// display_L.setTextColor(SSD1306_WHITE); // normal text
			// display_L.fillRect(curr_col, curr_row, display_L.width()/2, 10, SSD1306_WHITE);
			display_L.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
		}
		else {
			// display_L.fillRect(curr_col, curr_row, display_L.width()/2, 10, SSD1306_WHITE);
			// display_L.drawRect(curr_col, curr_row, display_L.width()/2, 10, SSD1306_WHITE);
			display_L.setTextColor(SSD1306_WHITE); // normal text
			// display_L.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
		}

		display_L.setCursor(curr_col,curr_row+1);
		display_L.println(menu_items[i]);

	}

}

// -------------------------------------------------------------- //
void checkPosition() {
	encoder.tick(); // just call tick() to check the state.
}

void display_rotary_val(int curr_rotary){
	// display management
	// display_L.clearDisplay();
	// if (curr_rotary<0)
	// 	display_L.setCursor(4,0);
	// else
	// 	display_L.setCursor(16,0);

	display_L.setCursor(0,0);
	display_L.println(curr_selection);
	display_L.println(curr_rotary);
	// display_L.display();
}

void mouse_scroll(int direction){
	Mouse.move(0, 0, -1*direction);
}

int read_rotary_encoder(int last_rotary){
	int curr_rotary = encoder.getPosition();
	RotaryEncoder::Direction direction = encoder.getDirection();

	if (curr_rotary != last_rotary) {
		sleep_timer=0;
		display_rotary_val(curr_rotary);

		if (curr_selection>=0 && curr_selection<8)
			curr_selection=curr_rotary;

		mouse_scroll((int)direction);
	}
	else
		sleep_timer+=1;

	return curr_rotary;
}


void read_rotary_only(int last_rotary){
	int curr_rotary = encoder.getPosition();
	RotaryEncoder::Direction direction = encoder.getDirection();

	if (curr_rotary!=last_rotary){
			last_rotary=curr_rotary;

			if (curr_rotary<0){
							curr_selection=0;
							curr_rotary=curr_selection;}
			else if (curr_rotary>NUM_ITEMS-1){
							curr_selection=NUM_ITEMS-1;
							curr_rotary=curr_selection;}
			else
				curr_selection=curr_rotary;
	}
	display_rotary_val(curr_rotary);


	// if (curr_rotary >last_rotary){
	// 	last_rotary = curr_rotary;
	// 	if (curr_selection<NUM_ITEMS-1)
	// 		curr_selection=curr_rotary;
	// }
	// else if (curr_rotary < last_rotary){
	// 	last_rotary = curr_rotary;
	// 	if (curr_selection>0)
	// 		curr_selection=curr_rotary;
}


		// if (curr_selection>1 && curr_selection<NUM_ITEMS)
		// 	curr_selection=curr_rotary;


void switch_mode(){

	int curr_mode=digitalRead(MODE_SWITCH_PIN);
	// display_L.clearDisplay();
	display_L.setCursor(0,80);
	display_L.println("   ");
	display_L.setCursor(0,80);
	display_L.println(curr_mode);
	display_L.display();
	// curr_selection=0;
}

void mouse_move(int x_val, int y_val){

	int x_move=0;
	int y_move=0;

	if (x_val<500)
		x_move=-1*map(x_val, 500, 3, MIN_MOUSE_MOVE, MAX_MOUSE_MOVE);
	else if (x_val>=600)
		x_move=map(x_val, 600, 1023, MIN_MOUSE_MOVE, MAX_MOUSE_MOVE);


	if (y_val<480)
		y_move=-1*map(y_val, 480, 2, MIN_MOUSE_MOVE, MAX_MOUSE_MOVE);
	else if (y_val>=600)
		y_move=map(y_val, 600, 1023, MIN_MOUSE_MOVE, MAX_MOUSE_MOVE);

	Mouse.move(x_move, y_move, 0);
}

void disp_menu(){
	display_L.clearDisplay();
	display_L.setCursor(17,0);
	display_L.println("MENU");
	display_L.display();
}

void normal_keyboard(){
	if (digitalRead(mouse_btn_left_pin)==LOW){
		Mouse.click();
		delay(250);
	}
	else if (digitalRead(mouse_btn_right_pin)==LOW){
		Mouse.click(MOUSE_RIGHT);
		delay(250);
	}

	else {
		// read encoder
		int curr_rotary=read_rotary_encoder(last_rotary);
		last_rotary = curr_rotary;

		x_val = analogRead(pin_x);
		y_val = analogRead(pin_y);
		mouse_move(x_val,y_val);

		for (uint8_t i=0;i<NUM_KEYS;i++){
			if (digitalRead(buttons[i].pin_num) == LOW) {
				sleep_timer=0;
				display_L.clearDisplay();



				// disp rotary value
				display_L.setCursor(17,0);
				display_L.println(curr_rotary);

				// disp current pin value
				display_L.setCursor(0,30);
				display_L.println(String(buttons[i].pin_num));
				display_L.println(buttons[i].key_name);
				display_L.display();


				Keyboard.write(buttons[i].action);
				delay(delay_val);
			}
			else
				sleep_timer+=1;
		}


		// display_L.display();

		if (sleep_timer>sleep_timeout){
			// sleep_mode=true;
			// sleep_timer=0;
			display_L.clearDisplay();
			display_L.display();
		}

	}
}

// -------------------------------------------------------------- //
void setup(void) {
	// Serial.begin(9600);

	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), checkPosition, CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), checkPosition, CHANGE);


	for (uint8_t i=0;i<NUM_KEYS;i++)
		pinMode(buttons[i].pin_num, INPUT_PULLUP);

	pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);

	// attachInterrupt(MODE_SWITCH_PIN, switch_mode, CHANGE);

	pinMode(pin_x,INPUT);
  	pinMode(pin_y,INPUT);
  	pinMode(mouse_btn_left_pin,INPUT_PULLUP);
  	pinMode(mouse_btn_right_pin,INPUT_PULLUP);

  	// attachInterrupt(mouse_btn_left_pin, mouse_click_isr, FALLING);
  	// attachInterrupt(mouse_btn_left_pin, mouse_release_isr, RISING);

	Wire.setSDA(SDA_PIN);
	Wire.setSCL(SCL_PIN);

	oled=init_oled();
	display_L.setRotation(1);
	display_L.setTextColor(SSD1306_WHITE);        // Draw white text
	display_L.setTextSize(2);

	display_L.clearDisplay();

	display_L.setCursor(0,10);             // Start at top-left corner
	display_L.println("HELLO");
	display_L.display();

	Mouse.begin();
}

void loop(void) {

	int curr_mode=digitalRead(MODE_SWITCH_PIN);

	if (curr_mode==HIGH){
			normal_keyboard();
		}
	else if (curr_mode==LOW){
			// disp_menu();
		display_L.clearDisplay();
		int curr_rotary=read_rotary_encoder(last_rotary);
		read_rotary_only(last_rotary);
		draw_menu_items();

		display_L.display();
	}


	// delay(20);
}
