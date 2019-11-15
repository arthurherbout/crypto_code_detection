#include <iostream>
#include <algorithm>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::max;

const int BLUE = 0;
const int ORANGE = 1;

void Input(vector<int>* buttons, vector<int>* robot_color) { 
  buttons->clear();
  robot_color->clear();

  int size;
  cin >> size;
  for (int i = 0; i < size; ++i) {
    char robot;
    cin >> robot;
    robot_color->push_back( (robot == 'B') ? BLUE : ORANGE );

    int button;
    cin >> button;
    buttons->push_back(button);
  }
}

int Answer(const vector<int>& buttons, const vector<int>& robot_color) {
  int last_robot_step[] = {1, 1};
  //int last_orange_step = 1;

  //int last_blue_time = 0;
  int last_robot_time[] = {0, 0};

  int current_time = 0;

  for (int i = 0; i < buttons.size(); ++i) { 
    int robot = robot_color[i];
    current_time = max(current_time, last_robot_time[robot] + abs(buttons[i] - last_robot_step[robot])) + 1;
    last_robot_time[robot] = current_time;
    last_robot_step[robot] = buttons[i];
  }
  return current_time;
}

int main() {
  int T;
  cin >> T;
  vector<int> answers;
  for (int i = 0; i < T; ++i) {
    vector<int> buttons;
    vector<int> robot_color;
    Input(&buttons, &robot_color);
    answers.push_back(Answer(buttons, robot_color));
  }

  for (int i = 0; i < T; ++i) {
    cout << "Case #" << i + 1 << ": " << answers[i] << endl;
  }
  return 0;
}