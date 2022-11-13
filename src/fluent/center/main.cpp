#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flow.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

int main()
{
  Fl_Double_Window win(640, 480);
  Fl_Flow flow(0, 0, win.w(), win.h());
  Fl_Button button(0, 0, 200, 200, "Centered");
  Fl_Box side(0, 0, 200, 1);
  side.color(FL_RED);
  side.box(FL_FLAT_BOX);

  flow.left(side).up().expand();
  flow.left(button).center()
    .up().center();

  win.resizable(flow);
  win.show();

  return Fl::run();
}
