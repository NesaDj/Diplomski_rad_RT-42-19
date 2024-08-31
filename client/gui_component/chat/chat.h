#ifndef CHAT_H
#define CHAT_H

#include "../../communication_component/communication_object.h"

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/listctrl.h>

class ChatFrame : public wxFrame
{
    public:
        ChatFrame(communication *server,int id,char username[32]);

        wxFrame* perent;

    private:
        void OnClose(wxCloseEvent& event);
        void SendFunction(wxCommandEvent& event);

        void AddMessage(wxStyledTextCtrl* chatDisplay, const wxString& sender, const wxString& message, bool rightAlign);

        void OpenChat(wxCommandEvent& event);

        wxButton* sendButton;
        wxTextCtrl* msgTextInput;
        //wxListBox* chatList;
        wxListView* chatView;

        bool start;

        wxTimer* timer; 

        void OnTimer(wxTimerEvent& event);

        wxDECLARE_EVENT_TABLE();

        communication *l_server;

        int user_id;
        char username[32];
};

#endif