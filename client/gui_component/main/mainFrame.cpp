#include "mainFrame.h"

MainFrame::MainFrame(communication *server) : wxFrame(NULL, wxID_ANY, "C++ aplikacija za razmenu poruka"),start(false)
{
    l_server = server;
    l_server->try_get_users();

    logoutButton = new wxButton(this, wxID_ANY, "Izloguj se", wxPoint(10, 520), wxSize(330, 50));
    logoutButton->Bind(wxEVT_BUTTON, &MainFrame::LogoutFunction, this);

    listBox = new wxListBox(this, wxID_ANY, wxPoint(10, 10), wxSize(330, 500));

    for(int i=0;i<l_server->number_of_user_in_list;i++)
    {
        listBox->Append(l_server->user_list[i].username);
    }

    listBox->Bind(wxEVT_LISTBOX, &MainFrame::OpenChat, this);

    SetSize(wxSize(350, 600));

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

    Bind(wxEVT_ACTIVATE, &MainFrame::OnEnable, this);

}


void MainFrame::LogoutFunction(wxCommandEvent& event) 
{
    free(l_server->user_list);

    perent->Enable(true);
    Destroy();
}

void MainFrame::OnClose(wxCloseEvent& event) 
{
    free(l_server->user_list);

    perent->Enable(true);
    Destroy();
}


void MainFrame::OpenChat(wxCommandEvent& event) 
{
    if(start==false)
    {
        listBox->SetSelection(wxNOT_FOUND);
        start=true;
        return;
    };

    int selection = listBox->GetSelection();  // Get the index of the selected item
    if (selection != wxNOT_FOUND) {
        wxString selectedItem = listBox->GetString(selection);  // Get the text of the selected item
        
        char username[32] = {0};
        std::string usernameStdString = std::string(selectedItem.mb_str());
        usernameStdString.copy(username, sizeof(username) - 1);
        username[sizeof(username) - 1] = '\0';

        int id = l_server->get_id_from_username(username);

        chat_frame = new ChatFrame(l_server,id,username);
        chat_frame->perent = this;
        chat_frame->Show(true);

        this->Enable(false);
    }
}


void MainFrame::OnEnable(wxActivateEvent& event)
{
    if (IsEnabled())
    {
        listBox->SetSelection(wxNOT_FOUND);
    }
}