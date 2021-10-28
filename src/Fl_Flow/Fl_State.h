#include <FL/Fl.H>

template <typename T>
struct Fl_State
{
  Fl_State() : m_widget(0), m_w(), m_h(), m_placed() { }

  Fl_Widget_Tracker m_widget;
  int m_w;
  int m_h;
  bool m_placed;
};

#define Fl_State Fl_State<char>

