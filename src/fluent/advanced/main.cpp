#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flow.H>
#include <FL/Fl_Button.H>
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

  flow(button).up().left();
  flow(text).up().left();
  flow(sep).left().expand()
    .up();

  flow(area).left().up();
  flow(sep2).left().expand()
    .up();

  flow(button2).down();
  flow(sep2).down();
  flow(area).right().expand()
    .down().expand();

  win.resizable(flow);
  win.show();

  return Fl::run();
}
