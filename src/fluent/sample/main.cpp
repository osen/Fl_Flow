#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flow.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Input.H>

struct HelloWindow : Fl_Double_Window
{
  HelloWindow() : Fl_Double_Window(640, 480),
    m_flow(0, 0, w(), h()),
    //m_flow(100, 100, w() - 200, h() - 200),
    m_load(0, 0, 100, 30, "Load"),
    m_save(0, 0, 100, 30, "Save"),
    m_close(0, 0, 100, 30, "Close"),
    m_text(0, 0, 120, 30)
  {
    add_resizable(m_flow);
    m_flow.set_padding(10);

    m_flow(m_load).up().left();
    m_flow(m_save).up().left();
    m_flow(m_close).up();
    m_flow(m_text).left().expand()
      .up().expand();

    end();
  }

  Fl_Flow m_flow;
  Fl_Button m_load;
  Fl_Button m_save;
  Fl_Button m_close;
  Fl_Multiline_Input m_text;
};

int main(int argc, char* argv[])
{
  HelloWindow win;
  win.show(argc, argv);

  Fl::run();

  return 0;
}
