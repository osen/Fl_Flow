#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>

template <typename T>
struct Fl_Helper
{
  static void position(Fl_Widget_Tracker _w, int _x, int _y)
  {
    Fl_Widget_Tracker parent = _w.widget()->parent();
    int x = 0;
    int y = 0;

    if(parent.exists())
    {
      x = parent.widget()->x();
      y = parent.widget()->y();
    }

    _w.widget()->position(x + _x, y + _y);
  }
};

#define Fl_Helper Fl_Helper<char>

