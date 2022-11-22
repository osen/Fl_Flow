#include <FL/Fl_Group.H>

#include <vector>
#include <string>

template <typename T>
struct Fl_Flow : Fl_Group
{
  Fl_Flow(int _x = 0, int _y = 0, int _w = 128, int _h = 128, const char *_label = 0)
    : Fl_Group(_x, _y, _w, _h, _label), m_padding(5), m_drawn()
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

  void rule(Fl_Widget& _widget, const std::string& _instructions)
  {
    rule(&_widget, _instructions);
  }

  void rule(Fl_Widget_Tracker _widget, const std::string& _instructions)
  {
    int type = Fl_Instruction::NONE;

    add(_widget.widget());

    for(size_t ci = 0; ci < _instructions.length(); ++ci)
    {
      char c = _instructions.at(ci);

      if(c == '=')
      {
        type = Fl_Instruction::EXPAND;
        continue;
      }
      else if(c == '/')
      {
        type = Fl_Instruction::CENTER;
        continue;
      }

      m_instructions.push_back(Fl_Instruction(_widget, Fl_Instruction::decode(c, type)));
      type = Fl_Instruction::NONE;
    }
  }

  Fl_Flow& layout(Fl_Widget& _widget)
  {
    add(&_widget);
    m_instructions.push_back(Fl_Instruction(_widget, Fl_Instruction::NONE));
    return *this;
  }

  Fl_Flow& operator()(Fl_Widget& _widget)
  {
    return layout(_widget);
  }

  Fl_Flow& center()
  {
    Fl_Instruction& last = m_instructions.back();

    switch (last.m_instruction) {
    case Fl_Instruction::NONE:
      throw Fl_Exception("No direction for centering");
    case Fl_Instruction::MOVE_LEFT:
      last.m_instruction = Fl_Instruction::CENTER_LEFT;
      break;
    case Fl_Instruction::MOVE_RIGHT:
      last.m_instruction = Fl_Instruction::CENTER_RIGHT;
      break;
    case Fl_Instruction::MOVE_UP:
      last.m_instruction = Fl_Instruction::CENTER_UP;
      break;
    case Fl_Instruction::MOVE_DOWN:
      last.m_instruction = Fl_Instruction::CENTER_DOWN;
      break;
    default:
      throw Fl_Exception("Tried to center a widget that has already been centered or expanded");
    }

    return *this;
  }

  Fl_Flow& expand()
  {
    Fl_Instruction& last = m_instructions.back();

    switch (last.m_instruction) {
    case Fl_Instruction::NONE:
      throw Fl_Exception("No direction for expanding");
    case Fl_Instruction::MOVE_LEFT:
      last.m_instruction = Fl_Instruction::EXPAND_LEFT;
      break;
    case Fl_Instruction::MOVE_RIGHT:
      last.m_instruction = Fl_Instruction::EXPAND_RIGHT;
      break;
    case Fl_Instruction::MOVE_UP:
      last.m_instruction = Fl_Instruction::EXPAND_UP;
      break;
    case Fl_Instruction::MOVE_DOWN:
      last.m_instruction = Fl_Instruction::EXPAND_DOWN;
      break;
    default:
      throw Fl_Exception("Tried to expand a widget that has already been expanded or centered");
    }

    return *this;
  }

  Fl_Flow& left()
  {
    Fl_Instruction& last = m_instructions.back();
    m_instructions.push_back(Fl_Instruction(last.m_widget, Fl_Instruction::MOVE_LEFT));
    return *this;
  }

  Fl_Flow& right()
  {
    Fl_Instruction& last = m_instructions.back();
    m_instructions.push_back(Fl_Instruction(last.m_widget, Fl_Instruction::MOVE_RIGHT));
    return *this;
  }

  Fl_Flow& up()
  {
    Fl_Instruction& last = m_instructions.back();
    m_instructions.push_back(Fl_Instruction(last.m_widget, Fl_Instruction::MOVE_UP));
    return *this;
  }

  Fl_Flow& down()
  {
    Fl_Instruction& last = m_instructions.back();
    m_instructions.push_back(Fl_Instruction(last.m_widget, Fl_Instruction::MOVE_DOWN));
    return *this;
  }

  /*
   * Ensure that widget layout has occurred at least once
   * before initial draw
   */
  virtual void draw()
  {
    if(!m_drawn)
    {
      m_drawn = true;
      //resize(x(), y(), w(), h());
      prepare();
      process();
    }

    Fl_Group::draw();
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
  bool m_drawn;

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
        i.m_instruction == Fl_Instruction::EXPAND_DOWN ||
        i.m_instruction == Fl_Instruction::CENTER_LEFT ||
        i.m_instruction == Fl_Instruction::CENTER_RIGHT ||
        i.m_instruction == Fl_Instruction::CENTER_UP ||
        i.m_instruction == Fl_Instruction::CENTER_DOWN)
      {
        int xDir = i.x_direction();
        int yDir = i.y_direction();

        Fl_Transform wt(i.m_widget, m_padding);

        int origWidth = wt.m_w;
        int origHeight = wt.m_h;

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
            i.m_instruction == Fl_Instruction::EXPAND_DOWN ||
            i.m_instruction == Fl_Instruction::CENTER_LEFT ||
            i.m_instruction == Fl_Instruction::CENTER_RIGHT ||
            i.m_instruction == Fl_Instruction::CENTER_UP ||
            i.m_instruction == Fl_Instruction::CENTER_DOWN)
          {
            wt.scale(xDir, yDir);
          }
          else
          {
            throw Fl_Exception("Invalid instruction");
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

        /*
         * Transformed *just* too far, so rollback
         */
        wt.rollback();
        //wt.debug_output();

        if(i.m_instruction == Fl_Instruction::CENTER_LEFT ||
          i.m_instruction == Fl_Instruction::CENTER_RIGHT ||
          i.m_instruction == Fl_Instruction::CENTER_UP ||
          i.m_instruction == Fl_Instruction::CENTER_DOWN)
        {
          wt.contract(origWidth, origHeight);
          wt.commit();
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
      for(int ci = 0; ci < children(); ++ci)
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
      for(int ci = 0; ci < children(); ++ci)
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
    for(int ci = 0; ci < children(); ++ci)
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

