#include <FL/Fl.H>

#include <exception>

template <typename T>
struct Fl_Instruction
{
  static const int MOVE_LEFT = 1;
  static const int MOVE_RIGHT = 2;
  static const int MOVE_UP = 3;
  static const int MOVE_DOWN = 4;

  static const int EXPAND_LEFT = 6;
  static const int EXPAND_RIGHT = 7;
  static const int EXPAND_UP = 8;
  static const int EXPAND_DOWN = 9;

  static int decode(char c, bool expand)
  {
    if(expand)
    {
      if(c == '<') return EXPAND_LEFT;
      else if(c == '>') return EXPAND_RIGHT;
      else if(c == '^') return EXPAND_UP;
      else if(c == 'v') return EXPAND_DOWN;
    }

    if(c == '<') return MOVE_LEFT;
    else if(c == '>') return MOVE_RIGHT;
    else if(c == '^') return MOVE_UP;
    else if(c == 'v') return MOVE_DOWN;

    throw std::exception();
  }

  Fl_Instruction() : m_widget(0), m_instruction() { }

  int x_direction()
  {
    if(m_instruction == MOVE_LEFT || m_instruction == EXPAND_LEFT) return -1;
    else if(m_instruction == MOVE_RIGHT || m_instruction == EXPAND_RIGHT) return 1;
    return 0;
  }

  int y_direction()
  {
    if(m_instruction == MOVE_UP || m_instruction == EXPAND_UP) return -1;
    else if(m_instruction == MOVE_DOWN || m_instruction == EXPAND_DOWN) return 1;
    return 0;
  }

  Fl_Widget_Tracker m_widget;
  int m_instruction;
};

#define Fl_Instruction Fl_Instruction<char>

