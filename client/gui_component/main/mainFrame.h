#ifndef MAIN_H
#define MAIN_H

#include <wx/wx.h>
#include "../../communication_component/communication_object.h"
#include "../chat/chat.h"

class MainFrame : public wxFrame
{
    public:
        MainFrame(communication *server);

        wxFrame* perent;
    private:
        void OnClose(wxCloseEvent& event);
        void LogoutFunction(wxCommandEvent& event);
        void OpenChat(wxCommandEvent& event);
        
        wxButton* logoutButton;
        wxListBox* listBox;

        bool start;

        ChatFrame* chat_frame;

        void OnEnable(wxActivateEvent& event);

        communication *l_server;
};




#endif