#include "chat.h"


wxBEGIN_EVENT_TABLE(ChatFrame, wxFrame)
    EVT_CLOSE(ChatFrame::OnClose)
    EVT_TIMER(wxID_ANY, ChatFrame::OnTimer)  // Bind timer event
wxEND_EVENT_TABLE()

ChatFrame::ChatFrame(communication *server,int id,char username[32]) : wxFrame(NULL, wxID_ANY, username),start(false)
{
    l_server = server;
    user_id = id;
    l_server->try_to_get_msgs(user_id);

    strcpy(this->username,username);



    sendButton = new wxButton(this, wxID_ANY, "Posalji", wxPoint(640, 720), wxSize(150, 50));
    sendButton->Bind(wxEVT_BUTTON, &ChatFrame::SendFunction, this);

    msgTextInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 720), wxSize(620, 50));

    chatView = new wxListView(this, wxID_ANY, wxPoint(10, 10), wxSize(780, 700), wxLC_LIST | wxLC_SINGLE_SEL);

    for(int i=0;i<l_server->number_of_msg;i++)
    {
        char text[550] = {0};
        if(l_server->msgs_list[i].to_user_id==user_id)
        {
            strcat(text,"Ja : ");    
        }else
        {
            strcat(text,username);
            strcat(text," : ");
        }
        
        strcat(text,l_server->msgs_list[i].text);
        chatView->InsertItem(i, text);
    }

    SetSize(wxSize(800, 800));

    Bind(wxEVT_CLOSE_WINDOW, &ChatFrame::OnClose, this);
    

     // Initialize and start the timer
    timer = new wxTimer(this);
    timer->Bind(wxEVT_TIMER, &ChatFrame::OnTimer, this);
    timer->Start(500);
}

void ChatFrame::OnTimer(wxTimerEvent& event) 
{
    //std::cout << "Test..." << std::endl;
    if(l_server->try_to_get_msg(user_id))
    {
        char text[550] = {0};

        if(l_server->last_msg.to_user_id==user_id)
        {
            strcat(text,"Ja : ");    
        }else
        {
            strcat(text,username);
            strcat(text," : ");
        }
        
        strcat(text,l_server->last_msg.text);
        chatView->InsertItem(l_server->number_of_msg, text);
        l_server->number_of_msg++;
    }
}

void ChatFrame::AddMessage(wxStyledTextCtrl* chatDisplay, const wxString& sender, const wxString& message, bool rightAlign)
{
    
}

void ChatFrame::SendFunction(wxCommandEvent& event) 
{
    wxString msgText = msgTextInput->GetValue();
    msgTextInput->SetValue("");
    std::string msgStdString = std::string(msgText.mb_str());

    char msg[500] = {0};
    msgStdString.copy(msg, sizeof(msg) - 1);
    msg[sizeof(msg) - 1] = '\0';

    l_server->try_to_send_msg(user_id,msg);
}

void ChatFrame::OnClose(wxCloseEvent& event) 
{
    if (timer)
    {
        timer->Stop();  // Stop the timer
        delete timer;   // Delete the timer
    }

    l_server->number_of_msg = 0;

    perent->Enable(true);
    Destroy();
}

