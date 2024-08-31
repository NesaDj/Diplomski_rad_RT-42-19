#include "register.h"

RegisterFrame::RegisterFrame(communication *server) : wxFrame(NULL, wxID_ANY, "Registracija") 
{
    l_server = server;

    wxPanel* panel = new wxPanel(this);

    usernameLabel = new wxStaticText(panel, wxID_ANY, "Username:", wxPoint(10, 10), wxDefaultSize, wxALIGN_LEFT);
    
    usernameTextInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10, 30), wxSize(250, 30));
    
    passwordLabel = new wxStaticText(panel, wxID_ANY, "Password:", wxPoint(10, 70), wxDefaultSize, wxALIGN_LEFT);
    passwordTextInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10, 90), wxSize(250, 30));

    confirmPasswordLabel = new wxStaticText(panel, wxID_ANY, "Confirm password:", wxPoint(10, 130), wxDefaultSize, wxALIGN_LEFT);
    confirmPasswordTextInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10, 150), wxSize(250, 30));

    registerButton = new wxButton(panel, wxID_ANY, "Registruj se", wxPoint(10, 190), wxSize(250, 50));
    registerButton->Bind(wxEVT_BUTTON, &RegisterFrame::RegisterFunction, this);

    SetSize(wxSize(270, 270));

    Bind(wxEVT_CLOSE_WINDOW, &RegisterFrame::OnClose, this);
};

void RegisterFrame::OnClose(wxCloseEvent& event) 
{
    perent->Enable(true);
    Destroy();
}

void RegisterFrame::RegisterFunction(wxCommandEvent& event) 
{
    char username[32] = {0};
    char password[32] = {0};
    char confirm_password[32] = {0};

    wxString usernameText = usernameTextInput->GetValue();
    wxString passwordText = passwordTextInput->GetValue();
    wxString confirm_passwordText = confirmPasswordTextInput->GetValue();

    std::string usernameStdString = std::string(usernameText.mb_str());
    std::string passwordStdString = std::string(passwordText.mb_str());
    std::string confirm_passwordStdString = std::string(confirm_passwordText.mb_str());


    usernameStdString.copy(username, sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0';

    passwordStdString.copy(password, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';

    confirm_passwordStdString.copy(confirm_password, sizeof(confirm_password) - 1);
    confirm_password[sizeof(confirm_password) - 1] = '\0';

    if(strcmp(password,confirm_password)!=0)
    {
        wxLogMessage("You didnt confirm password right...");
    }else
    {
        if(l_server->try_registration(username,password)==true)
        {
            wxLogMessage("You registred sucesfully...");
            perent->Enable(true);
            Destroy();
        }else
        {
            wxLogMessage("You didnt sucesfully registred. Try again.");
        }
    }
}