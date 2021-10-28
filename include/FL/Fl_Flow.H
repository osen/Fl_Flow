#include <FL/Fl.H>

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

#include <FL/Fl_Group.H>

#include <exception>
#include <vector>
#include <string>

template <typename T>
struct Fl_Flow : Fl_Group
{
  Fl_Flow(int _x = 0, int _y = 0, int _w = 128, int _h = 128, const char *_label = 0)
    : Fl_Group(_x, _y, _w, _h, _label), m_padding(5)
  {
    //box(FL_FLAT_BOX);
    //color(FL_RED);
    resizable(NULL);
  }

  void set_padding(int _padding)
  {
    m_padding = _padding;
    resize(x(), y(), w(), h());
  }

  void add(Fl_Widget& _widget, const std::string& _instructions)
  {
    add(&_widget, _instructions);
  }

  void add(Fl_Widget_Tracker _widget, const std::string& _instructions)
  {
    bool expand = false;

    for(size_t ci = 0; ci < _instructions.length(); ++ci)
    {
      char c = _instructions.at(ci);

      if(c == '=')
      {
        expand = true;
        continue;
      }

      Fl_Instruction instruction;
      instruction.m_widget = _widget;
      instruction.m_instruction = Fl_Instruction::decode(c, expand);
      expand = false;
      m_instructions.push_back(instruction);
    }
  }

  virtual void resize(int _x, int _y, int _w, int _h)
  {
    Fl_Group::resize(_x, _y, _w, _h);
    prepare();
    process();
  }

private:
  std::vector<Fl_Instruction> m_instructions;
  std::vector<Fl_State> m_states;
  int m_padding;

  void process()
  {
    Fl_Transform pt(this, 0);

    for(size_t ii = 0; ii < m_instructions.size(); ++ii)
    {
      Fl_Instruction& i = m_instructions.at(ii);

      if(i.m_instruction == Fl_Instruction::MOVE_LEFT ||
        i.m_instruction == Fl_Instruction::MOVE_RIGHT ||
        i.m_instruction == Fl_Instruction::MOVE_UP ||
        i.m_instruction == Fl_Instruction::MOVE_DOWN ||
        i.m_instruction == Fl_Instruction::EXPAND_LEFT ||
        i.m_instruction == Fl_Instruction::EXPAND_RIGHT ||
        i.m_instruction == Fl_Instruction::EXPAND_UP ||
        i.m_instruction == Fl_Instruction::EXPAND_DOWN)
      {
        int xDir = i.x_direction();
        int yDir = i.y_direction();

        Fl_Transform wt(i.m_widget, m_padding);

        while(true)
        {
          if(i.m_instruction == Fl_Instruction::MOVE_LEFT ||
            i.m_instruction == Fl_Instruction::MOVE_RIGHT ||
            i.m_instruction == Fl_Instruction::MOVE_UP ||
            i.m_instruction == Fl_Instruction::MOVE_DOWN)
          {
            wt.translate(xDir, yDir);
          }
          else if(i.m_instruction == Fl_Instruction::EXPAND_LEFT ||
            i.m_instruction == Fl_Instruction::EXPAND_RIGHT ||
            i.m_instruction == Fl_Instruction::EXPAND_UP ||
            i.m_instruction == Fl_Instruction::EXPAND_DOWN)
          {
            wt.scale(xDir, yDir);
          }
          else
          {
            throw std::exception();
          }

          /*
           * Collide with parent bounds
           */
          if(!pt.contains(wt))
          {
            break;
          }

          bool colliding = false;

          /*
           * Collide with *positioned* siblings
           */
          for(size_t si = 0; si < m_states.size(); ++si)
          {
            Fl_State& s = m_states.at(si);
            if(!s.m_placed) continue;
            if(s.m_widget.widget() == i.m_widget.widget()) continue;

            Fl_Transform st(s.m_widget, 0);

            if(wt.colliding(st))
            {
              colliding = true;
              break;
            }
          }

          if(colliding) break;
        }

        wt.apply();
      }

      /*
       * Flag widget as placed.
       */
      for(size_t si = 0; si < m_states.size(); ++si)
      {
        Fl_State& s = m_states.at(si);
        if(s.m_widget.widget() != i.m_widget.widget()) continue;
        s.m_placed = true;
        break;
      }
    }
  }

  void prepare()
  {
    /*
     * Remove any states with invalid children
     */
    for(size_t si = 0; si < m_states.size(); ++si)
    {
      if(m_states.at(si).m_widget.deleted())
      {
        m_states.erase(m_states.begin() + si); --si; continue;
      }

      bool found = false;
      for(size_t ci = 0; ci < children(); ++ci)
      {
        if(child(ci) == m_states.at(si).m_widget.widget())
        {
          found = true; break;
        }
      }

      if(!found)
      {
        m_states.erase(m_states.begin() + si); --si; continue;
      }
    }

    /*
     * Remove any instructions with invalid children
     */
    for(size_t ii = 0; ii < m_instructions.size(); ++ii)
    {
      if(m_instructions.at(ii).m_widget.deleted())
      {
        m_instructions.erase(m_instructions.begin() + ii); --ii; continue;
      }

      bool found = false;
      for(size_t ci = 0; ci < children(); ++ci)
      {
        if(child(ci) == m_instructions.at(ii).m_widget.widget())
        {
          found = true; break;
        }
      }

      if(!found)
      {
        m_instructions.erase(m_instructions.begin() + ii); --ii; continue;
      }
    }

    /*
     * Add any missing children into the states
     */
    for(size_t ci = 0; ci < children(); ++ci)
    {
      bool found = false;
      for(size_t si = 0; si < m_states.size(); ++si)
      {
        if(child(ci) == m_states.at(si).m_widget.widget())
        {
          found = true; break;
        }
      }

      if(found == false)
      {
        Fl_State s;
        s.m_widget = child(ci);
        s.m_w = child(ci)->w();
        s.m_h = child(ci)->h();
        m_states.push_back(s);
      }
    }

    /*
     * Reset state for the children
     */
    for(size_t si = 0; si < m_states.size(); ++si)
    {
      m_states.at(si).m_placed = false;
      Fl_Widget_Tracker t = m_states.at(si).m_widget;
      t.widget()->size(m_states.at(si).m_w, m_states.at(si).m_h);
      Fl_Helper::position(t, w() - t.widget()->w() - m_padding, h() - t.widget()->h() - m_padding);
    }
  }
};

#define Fl_Flow Fl_Flow<char>

