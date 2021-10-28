#include <FL/Fl.H>

template <typename T>
struct Fl_Transform
{
  Fl_Widget_Tracker m_widget;
  int m_padding;
  int m_x;
  int m_y;
  int m_w;
  int m_h;
  int m_cx;
  int m_cy;
  int m_cw;
  int m_ch;

  Fl_Transform(Fl_Widget_Tracker _widget, int _padding)
    : m_widget(_widget), m_padding(_padding), m_x(), m_y(), m_w(), m_h(),
    m_cx(), m_cy(), m_cw(), m_ch()
  {
    m_x = _widget.widget()->x() - m_padding;
    m_y = _widget.widget()->y() - m_padding;
    m_w = _widget.widget()->w() + m_padding * 2;
    m_h = _widget.widget()->h() + m_padding * 2;

    m_cx = m_x;
    m_cy = m_y;
    m_cw = m_w;
    m_ch = m_h;
  }

  bool contains(Fl_Transform& _other)
  {
    if(_other.m_x < m_x) return false;
    if(_other.m_y < m_y) return false;
    if(_other.m_x + _other.m_w > m_x + m_w) return false;
    if(_other.m_y + _other.m_h > m_y + m_h) return false;
    return true;
  }

  bool colliding(Fl_Transform& _other)
  {
    if(m_x < _other.m_x)
    {
      if(m_x + m_w < _other.m_x) return false;
    }
    else
    {
      if(_other.m_x + _other.m_w < m_x) return false;
    }

    if(m_y < _other.m_y)
    {
      if(m_y + m_h < _other.m_y) return false;
    }
    else
    {
      if(_other.m_y + _other.m_h < m_y) return false;
    }

    return true;
  }

  void apply()
  {
    m_widget.widget()->resize(m_cx + m_padding, m_cy + m_padding,
      m_cw - m_padding * 2, m_ch - m_padding * 2);
  }

  void debug_output()
  {
    printf("Committed: %i %i %i %i\n", m_cx, m_cy, m_cw, m_ch);
    printf("Staging: %i %i %i %i\n", m_x, m_y, m_w, m_h);
  }

  void commit()
  {
    m_cx = m_x;
    m_cy = m_y;
    m_cw = m_w;
    m_ch = m_h;
  }

  void rollback()
  {
    m_x = m_cx;
    m_y = m_cy;
    m_w = m_cw;
    m_h = m_ch;
  }

  void contract(int _w, int _h)
  {
    m_x += m_w / 2 - _w / 2;
    m_y += m_h / 2 - _h / 2;
    m_w = _w;
    m_h = _h;
  }

  void translate(int _x, int _y)
  {
    commit();
    m_x += _x;
    m_y += _y;
  }

  void scale(int _x, int _y)
  {
    commit();
    if(_x < 0)
    {
      m_x += _x;
      m_w -= _x;
    }
    else
    {
      m_w += _x;
    }

    if(_y < 0)
    {
      m_y += _y;
      m_h -= _y;
    }
    else
    {
      m_h += _y;
    }
  }
};

#define Fl_Transform Fl_Transform<char>

