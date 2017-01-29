#include <LiquidCrystal.h>

#define LCD_04  12
#define LCD_06  11

#define LCD_11  5
#define LCD_12  4
#define LCD_13  3
#define LCD_14  2

#define ANALOG_00 A0
#define ANALOG_01 A1
#define ANALOG_02 A2
#define ANALOG_03 A3
#define ANALOG_04 A4
#define ANALOG_05 A5

#define KEEP_ALIVE_PIN 8
#define KEEP_ALIVE_MILLIS 15000

#define LCD_COLS 8
#define LCD_ROWS 2

#define ANSWER_COUNT 20

const char* ANSWERS[] = {
  "It is certain",
  "It is decidedly so",
  "Without a doubt",
  "Yes, definitely",
  "You may rely on it",
  "As I see it, yes",
  "Most likely",
  "Outlook good",
  "Yes",
  "Signs point to yes",
  "Reply hazy try again",
  "Ask again later",
  "Better not tell you now",
  "Cannot predict now",
  "Concentrate and ask again",
  "Don't count on it",
  "My reply is no",
  "My sources say no",
  "Outlook not so good",
  "Very doubtful"
};

#define LONG_WAIT_MILLIS 1100
#define SHORT_WAIT_MILLIS 650

LiquidCrystal lcd(LCD_04, LCD_06, LCD_11, LCD_12, LCD_13, LCD_14);

void setup()
{
  initialize_keep_alive();
  initialize_random();
  initialize_lcd();
}

// keep alive pin is 
void initialize_keep_alive()
{
  pinMode(KEEP_ALIVE_PIN, OUTPUT);
  digitalWrite(KEEP_ALIVE_PIN, HIGH);
}

// Seems to need multiple floating
// analog reads to gaurantee a randomish
// number to see random.
void initialize_random()
{
  int a0 = analogRead(ANALOG_00);
  int a1 = analogRead(ANALOG_01);
  int a2 = analogRead(ANALOG_02);
  int a3 = analogRead(ANALOG_03);
  int a4 = analogRead(ANALOG_04);
  int seed = (a0 * a1) + (a1 * a2) + (a2 * a3) + (a3 * a4);
  randomSeed(seed);
}

void initialize_lcd()
{
  lcd.begin(LCD_COLS, LCD_ROWS);
}

void loop()
{
  display_answer(generate_answer());

  if (keep_alive_timeout()) {
    end_keep_alive();
  }
}

char* generate_answer()
{
  return (char*) ANSWERS[answer_index()];
}

int answer_index()
{
  static int i = random(ANSWER_COUNT);
  return i;
}

void display_answer(char* answer)
{
  display_rows(answer);
  if (strlen(answer) > LCD_ROWS * LCD_COLS) {
    delay(SHORT_WAIT_MILLIS);
    for (int i = 0; strlen(&answer[i]) >= LCD_ROWS * LCD_COLS; i++) {
      display_rows(&answer[i]);
      delay(SHORT_WAIT_MILLIS);
    }
    delay(SHORT_WAIT_MILLIS);
  }
  delay(LONG_WAIT_MILLIS);
}

void display_rows(char *value)
{
  char lines[LCD_ROWS][LCD_COLS + 1];
  memset(lines, 0, LCD_ROWS * (LCD_COLS + 1));

  for (int row = 0; row < LCD_ROWS; row++) {
    int offset = row * LCD_ROWS;
    int s = min(row * LCD_COLS, strlen(value));
    int e = min((row + 1) * LCD_COLS, strlen(value));

    if (s >= offset && s < e) {
      strncpy(lines[row], &value[s], e - s);
    }

    display_row(row, lines[row]);
  }
}

void display_row(int row, char *value)
{
  lcd.setCursor(0, row);
  lcd.print(value);  
}

bool keep_alive_timeout()
{
  return millis() > KEEP_ALIVE_MILLIS;
}

void end_keep_alive()
{
  digitalWrite(KEEP_ALIVE_PIN, LOW);
}

