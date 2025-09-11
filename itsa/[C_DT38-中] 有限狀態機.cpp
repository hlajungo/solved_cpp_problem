#include <bits/stdc++.h>
#include <functional>
#include <iostream>
#include <map>

// identifiler 和 number 這兩 state 被省去，由於沒有離開的方法

using namespace std;
using std::cin;
using std::endl;
// 狀態定義
enum class State
{
  start,
  build_num,
  build_id,
  stop
};

// 事件定義
enum class Event
{
  Letter,
  Digit,
  Blank,
  Underscore,
  Period,
};

// 將 enum 轉成字串方便顯示
std::string
state_to_string (State s)
{
  switch (s)
  {
  case State::build_id:
    return "build_id";
  case State::build_num:
    return "build_num";
  case State::start:
    return "start";
  case State::stop:
    return "stop";
  default:
    return "Unknown";
  }
}

std::string
event_to_string (Event e)
{
  switch (e)
  {
  case Event::Blank:
    return "Blank";
  case Event::Digit:
    return "Digit";
  case Event::Letter:
    return "Letter";
  case Event::Period:
    return "Period";
  case Event::Underscore:
    return "Underscore";
  default:
    return "Unknown";
  }
}

class FSM
{
private:
  State current_state;

  // 轉移表
  std::map<std::pair<State, Event>, State> transition_table;

  // 轉移時執行的動作
  std::map<std::pair<State, Event>, std::function<void (std::any)> >
      action_table;

  string buf;

public:
  FSM () : current_state (State::start)
  {
    // Start, Event, End
    transition_table = {
      // 4
      { { State::start, Event::Blank }, State::start },
      { { State::start, Event::Period }, State::stop },
      { { State::start, Event::Letter }, State::build_id },
      { { State::start, Event::Digit }, State::build_num },
      // 2
      { { State::build_num, Event::Blank }, State::start },
      { { State::build_num, Event::Digit }, State::build_num },
      // 4
      { { State::build_id, Event::Letter }, State::build_id },
      { { State::build_id, Event::Digit }, State::build_id },
      { { State::build_id, Event::Underscore }, State::build_id },
      { { State::build_id, Event::Blank }, State::start },
    };

    // Start, Event, Func
    action_table = { // start
                     { { State::start, Event::Blank },
                       [&] (std::any c_any)
                       {
                         char c = std::any_cast<char> (c_any);
                         buf = c;
                       } },
                     { { State::start, Event::Digit },
                       [&] (std::any c_any)
                       {
                         char c = std::any_cast<char> (c_any);
                         buf = c;
                       } },
                     { { State::start, Event::Letter },
                       [&] (std::any c_any)
                       {
                         char c = std::any_cast<char> (c_any);
                         buf = c;
                       } },
                     { { State::start, Event::Period },
                       [] (std::any)
                       {

                       } },
                     // build_id
                     { { State::build_id, Event::Digit },
                       [&] (std::any c_any)
                       {
                         char c = std::any_cast<char> (c_any);
                         buf += c;
                       } },
                     { { State::build_id, Event::Letter },
                       [&] (std::any c_any)
                       {
                         char c = std::any_cast<char> (c_any);
                         buf += c;
                       } },
                     { { State::build_id, Event::Underscore },
                       [&] (std::any c_any)
                       {
                         char c = std::any_cast<char> (c_any);
                         buf += c;
                       } },
                     { { State::build_id, Event::Blank },
                       [&] (std::any)
                       {
                         std::cout << buf << " - identifier\n";
                       } },
                     // build_num
                     { { State::build_num, Event::Digit },
                       [&] (std::any c_any)
                       {
                         char c = std::any_cast<char> (c_any);
                         buf += c;
                       } },
                     { { State::build_num, Event::Blank },
                       [&] (std::any)
                       {
                         std::cout << buf << " - number\n";
                       } }
    };
  }

  void
  handle_event (Event e, char c)
  {
    auto key = std::make_pair (current_state, e);

    //std::cout << "事件 " << event_to_string (e) << " 於 "
              //<< state_to_string (current_state) << "\n";

    if (transition_table.find (key) != transition_table.end ())
    { // transition exist
      auto it = action_table.find ({ current_state, e });
      if (it != action_table.end ())
      { // action exist
        it->second (c);
      }

      // transition state
      current_state = transition_table[key];
    }
    else
    {
      std::cout << "Invaild transition, state = "
                << state_to_string (current_state) << "\n";
    }
  }

  State
  get_state () const
  {
    return current_state;
  }
};

int
main ()
{
  string str;
  FSM fsm;
  for (; getline (cin, str);)
  {
    for (auto& c : str)
    {
      if (isdigit (c))
      {
        fsm.handle_event (Event::Digit, c);
      }
      else if (isalpha (c))
      {
        fsm.handle_event (Event::Letter, c);
      }
      else if (c == ' ')
      {
        fsm.handle_event (Event::Blank, c);
      }
      else if (c == '.')
      {
        // fsm.handle_event (Event::Period);
        break;
      }
      else if (c == '_')
      {
        fsm.handle_event (Event::Underscore, c);
      }
      else
      {
        cout << "error in string\n";
      }
    }
  }

  return 0;
}
