#include <numbers>
#include <string>
#include <vector>

#include "opengl/GrailGUI.hh"
#include "opengl/CalendarWidget.hh"

using namespace std;
using namespace grail;

class TestWidgets : public GLWin {
 public:
  TestWidgets() : GLWin(0x000000, 0xCCCCCC, "TestWidgets") {}


  void init() {
    const Style *s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();

    //MultiText *guiText = c->addLayer(new MultiText(c, s));
    MultiText *guiText = c->getGuiText();

    const Font* f = FontFace::get("TIMES", 10, FontFace::BOLD);

    CalendarWidget calendar(gui, guiText, 50, 50, 1100, 550, 2021, 8, grail::black, f);

    string event = "this is an event";
    calendar.circleDate(6, 8, grail::green, event);
    calendar.circleDate(27, 8, grail::red, event);
    calendar.circleDate(30, 8, grail::blue, event);
    calendar.circleDate(27, 2, grail::red, event);
    calendar.setViewWeekly();

    calendar.setDayInWeek(30);

    calendar.init();

    
    
  }

};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestWidgets(), 1200, 700);
}
