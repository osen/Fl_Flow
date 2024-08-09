# Fl_Flow

![](doc/tutorial/logo.png)

## A fast and fun layout manager for FLTK

Fl_Flow is a layout manager for FLTK which works in a largely
different manner to existing systems you may have encountered so
far. However it is intended to simplify the UI development process
and provide a much smoother workflow for both initial development
as well as future maintenance.

## Simple tutorial

Lets have a look at how Fl_Flow works and what kinds of functionality
it provides. The first step is to create a minimal program displaying
a single window with a button. This can be done using the following
code:

```cpp
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Flow.H>

int main()
{
  Fl_Double_Window win(640, 480);
  Fl_Flow flow(0, 0, win.w(), win.h());
  Fl_Button button(0, 0, 100, 30, "Button");

  win.resizable(flow);
  win.show();

  return Fl::run();
}
```
The program can be compiled as a normal C++ FLTK program using a
command similar to:

    $ c++ main.cpp -lfltk

This would result in a window appearing with the following layout.

![](doc/tutorial/1_button_initial.png)

You will notice that by default the button will appear at the bottom
right corner of the window if no positional instructions are given.
This may seem strange at first (i.e other systems default to the
top left) however after you start to use the system you may start
to see that it offers a few conveniences.

Our next step is to move the button to the top left. Rather than
set absolute coordinates (such as 0, 0), Flow works using instructions.
So as the window is resized, these same instructions get rerun and
the window layout is regenerated (albeit at a different size). For
now we just move up via the following code:

    flow.rule(button, "^");

As you may have guessed, directions are given within the rule()
function via < > ^ v symbols. As the following image demonstrates, the
button will keep moving upwards until it hits either the edge of the
window or another positioned Widget.

![](doc/tutorial/2_button_up.png)

With that in place, we can then instruct the Widget to move to the
left. Rather than writing multiple statements in C, the command can
be collapsed as seen in the following code:

    flow.rule(button, "^<");

So with this, the button will first move all the way to the top
edge, and then all the way to the left edge as shown in the following:

![](doc/tutorial/3_button_up_left.png)

With this in place, we will now create a new Text Box and provide
it the same instructions to move up and left. This will place it
neatly beside the original button. You will see that if we swap the
instructions and move it left first and then up, it will result in
being placed under the original button. This provides useful
flexibility. The code for the Text Box should be as follows.

```cpp
Fl_Input text(0, 0, 150, 30);
flow.rule(text, "^<");
```

The following image shows the route that the Text Box will take.
This should also be fairly easy to visualize compared to other
approaches using many nested grid containers.

![](doc/tutorial/4_text_up_left.png)

Next we are going to add a separator widget to close off the top
row. For this we are going to do something slightly different.
Instead of moving to the left, we provide an instruction for the
Widget to expand left. this means it will keep growing towards the
left until it touches the left edge. After this our usual instruction
to move upwards is given.

```cpp
Fl_Box sep(0, 0, 1, 1);
sep.color(FL_BLACK);
sep.box(FL_FLAT_BOX);
flow.rule(sep, "=<^");
```

So again, you may notice that any directional instruction with a =
symbol before it will expand in that direction rather than simply
move. This process is demonstrated by the following image.

![](doc/tutorial/5_separator_expand_left_up.png)

Note that the Separator Widget is by default 1x1 pixels in size.
This allows us to expand it in the vertical direction as well as
horizontal. The final result of this can be seen in the following
image.

![](doc/tutorial/6_separator_result.png)

At this point, our code should be similar to the following:

```cpp
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Flow.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>

int main()
{
  Fl_Double_Window win(640, 480);
  Fl_Flow flow(0, 0, win.w(), win.h());
  Fl_Button button(0, 0, 100, 30, "Button");
  Fl_Input text(0, 0, 150, 30);
  Fl_Box sep(0, 0, 10, 1);
  sep.color(FL_BLACK);
  sep.box(FL_FLAT_BOX);
 
  flow.rule(button, "^<");
  flow.rule(text, "^<");
  flow.rule(sep, "=<^");

  win.resizable(flow);
  win.show();

  return Fl::run();
}
```
## Advanced ordering

Next in this tutorial we will look further into the ways that
ordering of the flow instructions can affect the layout and also
provide useful results. We will add two new Widgets; a Text Area
and another Separator. Ultimately we want all the remaining space
to be taken up by the Text Area but for now we will only move it
into the top left and not expand it just yet. As for the separator,
we will use this to section off some button widgets but for now
lets expand it horizontally and move it upwards. These tasks can
be seen in the following code and subsequent image:

```cpp
Fl_Multiline_Input area(0, 0, 10, 10);
Fl_Box sep2(0, 0, 10, 1)
sep2.color(FL_BLACK);
sep2.box(FL_FLAT_BOX);
flow.rule(area, "<^");
flow.rule(sep2, "=<^");
```

![](doc/tutorial/7_textarea_expand_1.png)

Now that we have the Separator and Text Area out of the way
temporarily, we simply add a Button. Next we move the Separator
down to rest ontop of the newly placed button. Finally with everything
else in the correct positions, we expand the Text Area in both
dimensions. These steps are detailed below:

```cpp
Fl_Button button2(0, 0, 100, 30, "Button");
flow.rule(button2, "v");
flow.rule(sep2, "v");
flow.rule(area, "=>=v");
```

![](doc/tutorial/8_textarea_expand_2.png)

As you may have noticed, the widget flow instructions can be specified
multiple times and not only will they append to the current list
but they will also be in sequence of any instructions on sibling
Widgets. The final output can be seen in the following.

![](doc/tutorial/9_textarea_expand_result.png)

Importantly, all of the Widgets are placed using instructions rather
than absolute coordinates which means if we later resize the top-level
window, these instructions are repeated and the UI layout effectively
scales. For example the layout we have created should scale as
shown:

![](doc/tutorial/10_resize.png)

Finally a complete listing of the code required to create a program
that provides this layout. This should be considerably simpler when
compared with other UI systems.

```cpp
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
```

## Centering widgets

Flow also provides an instruction to center a component within
available space. For this, you can use the '/' character. For
example, the following will horizontally and vertically center a
button in the empty space rather than expanding into it.

    Fl_Button button(0, 0, 200, 200, "Button");
    flow.rule(button, "/</^");

This will result in the following:

![](doc/tutorial/11_center.png)

If you only wanted to center it horizontally but make it expand
vertically instead, then you would need a rule such as this.

    flow.rule(button, "/<=^");

This is very powerful because by only centering in the available
space, it will take into consideration other components such as
side bars. Considering that you can also put Fl_Flow widgets into
other Fl_Flow widgets, you can achieve any layout possible.

Hopefully this tutorial has given you some insight into how Flow
works. You may have even noticed that we did not actually need the
Separator Widgets in order to achieve the layout we created. Your
next step could be to have a look at some of the examples and finally
to download the library and have a play.

