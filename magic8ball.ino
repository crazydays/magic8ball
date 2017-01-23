#include <LiquidCrystal.h>

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

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  initialize_random();
  initialize_lcd();
}

// Seems to need multiple floating
// analog reads to gaurantee a randomish
// number to see random.
void initialize_random()
{
  int a0 = analogRead(A0);
  int a1 = analogRead(A1);
  int a2 = analogRead(A2);
  int a3 = analogRead(A3);
  int a4 = analogRead(A4);
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
    delay(500);
    for (int i = 0; strlen(&answer[i]) >= LCD_ROWS * LCD_COLS; i++) {
      display_rows(&answer[i]);
      delay(500);
    }
    delay(500);
  }
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

