#include "login.h"

LogInFrame::LogInFrame(communication *server) : wxFrame(NULL, wxID_ANY, "Prijava") 
{
    l_server = server;

    usernameLabel = new wxStaticText(this, wxID_ANY, "Username:", wxPoint(10, 10), wxDefaultSize, wxALIGN_LEFT);
    
    usernameTextInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 30), wxSize(250, 30));
    
    passwordLabel = new wxStaticText(this, wxID_ANY, "Password:", wxPoint(10, 70), wxDefaultSize, wxALIGN_LEFT);
    
    passwordTextInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 90), wxSize(250, 30));

    loginButton = new wxButton(this, wxID_ANY, "Uloguj se", wxPoint(10, 130), wxSize(250, 50));
    loginButton->Bind(wxEVT_BUTTON, &LogInFrame::LoginFunction, this);

    registerButton = new wxButton(this, wxID_ANY, "Registruj se", wxPoint(10, 190), wxSize(250, 50));
    registerButton->Bind(wxEVT_BUTTON, &LogInFrame::RegisterFunction, this);


    SetSize(wxSize(270, 270));

    Bind(wxEVT_CLOSE_WINDOW, &LogInFrame::OnClose, this);

    if(l_server->start_communication()==false)
    {
        Close(true);
    }
}

void LogInFrame::OnClose(wxCloseEvent& event) 
{
    l_server->end_communication();
    delete[] l_server;

    Destroy();
}

void LogInFrame::LoginFunction(wxCommandEvent& event) 
{
    char username[32] = {0};
    char password[32] = {0};

    wxString usernameText = usernameTextInput->GetValue();
    wxString passwordText = passwordTextInput->GetValue();

    std::string usernameStdString = std::string(usernameText.mb_str());
    std::string passwordStdString = std::string(passwordText.mb_str());

    usernameStdString.copy(username, sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0';

    passwordStdString.copy(password, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';

    if(l_server->try_login(username,password)==true)
    {
        this->main_frame = new MainFrame(l_server);
        main_frame->perent = this;
        main_frame->Show(true);

        this->Enable(false);
    }else
    {
        wxLogMessage("You didnt login try again...");
    }
}

void LogInFrame::RegisterFunction(wxCommandEvent& event) 
{
    this->register_frame = new RegisterFrame(l_server);
    register_frame->perent = this;
    register_frame->Show(true);

    this->Enable(false);
}
