#include <stdio.h>
void pole();
int logic();
int width = 80;
int height = 25;
int ball_x = 40;
int ball_y = 12;
int ball_x_start = 40;
int ball_y_start = 12;
int ball_x_speed = -1;
int ball_y_speed = 1;
int racket1_x = 1;
int racket1_y = 12;
int racket2_x = 78;
int racket2_y = 12;
int racket_y_start = 12;
int score1 = 0;
int score2 = 0;
int win_score = 21;
char input;
int main() {
  pole();
  logic();
  return 0;
}

void pole() {
  for (int y = 0; y < height + 1; y++) {
    for (int x = 0; x < width + 1; x++) {
      if ((y == 0 && x != width) || (y == height && x != width)) {
        printf("-");  // верхняя граница + нижняя граница
      } else if (x == 0 || x == width - 1) {
        printf("|");  // левая граница + правая граница
      } else if (x == width) {
        printf("\n");  // перенос на следующую строку
      } else if (x == ball_x && y == ball_y) {
        printf("*");
      } else if (x == width / 2) {
        printf("|");  // разделительная черта в центре
      } else if (x == racket1_x &&
                 (y == racket1_y || y == racket1_y + 1 || y == racket1_y - 1)) {
        printf("|");  // левая ракетка
      } else if (x == racket2_x &&
                 (y == racket2_y || y == racket2_y + 1 || y == racket2_y - 1)) {
        printf("|");  // правая ракетка
      } else if (x == width / 2 - 2 && y == 2) {
        printf("%d", score1);  // счет игрока 1
      } else if (x == width / 2 + 2 && y == 2) {
        printf("%d", score2);  // счет игрока 2
      } else
        printf(" ");
    }
  }
}

int logic() {  // логика
  while (score1 < win_score &&
         score2 < win_score) {  // while пока счет одной из команд не будет 21
    input = getchar();  // ввод символа
    if (input == '\n') {
      printf("\33[0d\33[2J");
      pole();
    }  // enter в input = продвижение шарика по x и y от скорости x и y
    ball_x += ball_x_speed;
    ball_y += ball_y_speed;
    if (ball_y == 1 || ball_y == height) {
      ball_y_speed = -ball_y_speed;
    }  // изменение вектора при столкновении с верхней или нижней стенкой
    if (ball_x == 2 && (ball_y == racket1_y - 1 || ball_y == racket1_y ||
                        ball_y == racket1_y + 1)) {
      ball_x_speed = -ball_x_speed;
    }  // изменение вектора по x при столкновении с левой ракеткой
    if (ball_x == 2 && (ball_y == racket1_y - 2 || ball_y == racket1_y + 2)) {
      ball_y_speed = -ball_y_speed;
    }  // изменение вектора по y при столкновении с углом левой ракетки
    if (ball_x == width - 3 &&
        (ball_y == racket2_y - 1 || ball_y == racket2_y ||
         ball_y == racket2_y + 1)) {
      ball_x_speed = -ball_x_speed;
    }  // изменение вектора по x при столкновении с правой ракеткой
    if (ball_x == width - 3 &&
        (ball_y == racket2_y - 2 || ball_y == racket2_y + 2)) {
      ball_y_speed = -ball_y_speed;
    }  // изменение вектора по y при столкновении с углом правой ракетки
    if (ball_x == 1) {
      ball_x = ball_x_start;
      ball_y = ball_y_start;
      score2 = score2 + 1;
      racket1_y = racket_y_start;
      racket2_y = racket_y_start;
    }
    if (ball_x == width - 2) {
      ball_x = ball_x_start;
      ball_y = ball_y_start;
      score1 = score1 + 1;
      racket1_y = racket_y_start;
      racket2_y = racket_y_start;
    }
    switch (input % 32) {
      case 1:
        if (racket1_y - 2 != 0) {
          racket1_y--;
        }
        break;
      case 26:
        if (racket1_y + 2 != height) {
          racket1_y++;
        }
        break;
      case 11:
        if (racket2_y - 2 != 0) {
          racket2_y--;
        }
        break;
      case 13:
        if (racket2_y + 2 != height) {
          racket2_y++;
        }
      default:
        break;
    }  // смещение ракеток по y

    if (score1 == win_score) {
      printf("\33[0d\33[2J");
      printf("\n ПОБЕДА ИГРОКА СЛЕВА СО СЧЕТОМ %d:%d", score1, score2);
    }
    if (score2 == win_score) {
      printf("\33[0d\33[2J");
      printf("\n ПОБЕДА ИГРОКА СЛЕВА СО СЧЕТОМ: %d:%d", score2, score1);
    }
  }
}