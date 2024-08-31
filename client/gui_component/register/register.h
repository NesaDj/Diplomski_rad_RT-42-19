#ifndef REGISTER_H
#define REGISTER_H

#include "../../communication_component/communication_object.h"
#include <wx/wx.h>

class RegisterFrame : public wxFrame
{
    public:
        RegisterFrame(communication *server);

        wxFrame* perent;

    private:
        void OnClose(wxCloseEvent& event);
        void RegisterFunction(wxCommandEvent& event);

        wxButton* registerButton;

        wxTextCtrl* usernameTextInput;
        wxTextCtrl* passwordTextInput;
        wxTextCtrl* confirmPasswordTextInput;

        wxStaticText* usernameLabel;
        wxStaticText* passwordLabel;
        wxStaticText* confirmPasswordLabel;

        communication *l_server;
};




#endif