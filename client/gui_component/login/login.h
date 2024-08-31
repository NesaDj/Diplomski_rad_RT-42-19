#ifndef LOGIN_H
#define LOGIN_H

#include <wx/wx.h>

#include "../../communication_component/communication_object.h"
#include "../register/register.h"
#include "../main/mainFrame.h"

class LogInFrame : public wxFrame
{
    public:
        LogInFrame(communication *server);
        
    private:
        void OnClose(wxCloseEvent& event);
        void LoginFunction(wxCommandEvent& event);
        void RegisterFunction(wxCommandEvent& event);

        wxButton* loginButton;
        wxButton* registerButton;

        wxTextCtrl* usernameTextInput;
        wxTextCtrl* passwordTextInput;

        wxStaticText* usernameLabel;
        wxStaticText* passwordLabel;

        RegisterFrame *register_frame;
        MainFrame *main_frame;

        communication *l_server;
};


#endif