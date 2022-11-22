#include <FL/Fl.H>

template <typename T>
struct Fl_Instruction
{
  static const int NONE = 0;
  static const int EXPAND = 50;
  static const int CENTER = 60;

  static const int MOVE_LEFT = 1;
  static const int MOVE_RIGHT = 2;
  static const int MOVE_UP = 3;
  static const int MOVE_DOWN = 4;

  static const int EXPAND_LEFT = 6;
  static const int EXPAND_RIGHT = 7;
  static const int EXPAND_UP = 8;
  static const int EXPAND_DOWN = 9;

  static const int CENTER_LEFT = 11;
  static const int CENTER_RIGHT = 12;
  static const int CENTER_UP = 13;
  static const int CENTER_DOWN = 14;

  static int decode(char c, int _type)
  {
    if(_type == EXPAND)
    {
      if(c == '<') return EXPAND_LEFT;
      else if(c == '>') return EXPAND_RIGHT;
      else if(c == '^') return EXPAND_UP;
      else if(c == 'v') return EXPAND_DOWN;
    }
    else if(_type == CENTER)
    {
      if(c == '<') return CENTER_LEFT;
      else if(c == '>') return CENTER_RIGHT;
      else if(c == '^') return CENTER_UP;
      else if(c == 'v') return CENTER_DOWN;
    }
    else if(_type == NONE)
    {
      if(c == '<') return MOVE_LEFT;
      else if(c == '>') return MOVE_RIGHT;
      else if(c == '^') return MOVE_UP;
      else if(c == 'v') return MOVE_DOWN;
    }

    throw Fl_Exception("Invalid instruction");
  }

  Fl_Instruction() : m_widget(0), m_instruction() { }

  Fl_Instruction(Fl_Widget& widget, int instr)
    : m_widget(&widget), m_instruction(instr) {}

  Fl_Instruction(Fl_Widget_Tracker& widget, int instr)
    : m_widget(widget), m_instruction(instr) {}

  int x_direction()
  {
    if(m_instruction == MOVE_LEFT ||
      m_instruction == EXPAND_LEFT ||
      m_instruction == CENTER_LEFT) { return -1;}
    else if(m_instruction == MOVE_RIGHT ||
      m_instruction == EXPAND_RIGHT ||
      m_instruction == CENTER_RIGHT) { return 1; }

    return 0;
  }

  int y_direction()
  {
    if(m_instruction == MOVE_UP ||
      m_instruction == EXPAND_UP ||
      m_instruction == CENTER_UP) { return -1; }
    else if(m_instruction == MOVE_DOWN ||
      m_instruction == EXPAND_DOWN ||
      m_instruction == CENTER_DOWN) { return 1; }

    return 0;
  }

  Fl_Widget_Tracker m_widget;
  int m_instruction;
};

#define Fl_Instruction Fl_Instruction<char>

