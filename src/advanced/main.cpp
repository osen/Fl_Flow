#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Flow.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Input.H>

int main()
{
  Fl_Double_Window win(640, 480);
  Fl_Flow flow(0, 0, win.w(), win.h());
  Fl_Button button(0, 0, 100, 30, "Button");
  Fl_Input text(0, 0, 150, 30);
  Fl_Box sep(0, 0, 10, 1);
  Fl_Multiline_Input area(0, 0, 10, 10);
  Fl_Box sep2(0, 0, 10, 1);
  Fl_Button button2(0, 0, 100, 30, "Button");
  sep.color(FL_BLACK);
  sep.box(FL_FLAT_BOX);
  sep2.color(FL_BLACK);
  sep2.box(FL_FLAT_BOX);

  flow.rule(button, "^<");
  flow.rule(text, "^<");
  flow.rule(sep, "=<^");
  flow.rule(area, "<^");
  flow.rule(sep2, "=<^");
  flow.rule(button2, "v");
  flow.rule(sep2, "v");
  flow.rule(area, "=>=v");

  win.resizable(flow);
  win.show();

  return Fl::run();
}
