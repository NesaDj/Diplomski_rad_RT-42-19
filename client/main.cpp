#include <wx/wx.h>
#include "gui_component/login/login.h"

class Aplikacija : public wxApp
{
    public:
        virtual bool OnInit();
};

wxIMPLEMENT_APP(Aplikacija);

bool Aplikacija::OnInit()
{
    communication* server;
    server = new communication();

    LogInFrame *frame = new LogInFrame(server);
    frame->Show(true);

    return true;
}