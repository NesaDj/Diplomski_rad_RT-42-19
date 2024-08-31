Razvio sam C++ aplikaciju za razmenu poruka kao diplomski rad.

Link do videa u kome je prikazan rad aplikacije:

https://www.youtube.com/watch?v=Hx2xzN3Rgcg

Komanda za kompailiranje servera:
g++ main.cpp -o tcp_server db_function/db_function.cpp handle_client_components/handle_client_components.cpp -lsqlite3

Komanda za kompailiranje klijenta:
g++ -o tcp_client_v3.out main.cpp `wx-config --cxxflags --libs` gui_component/chat/chat.cpp gui_component/main/mainFrame.cpp gui_component/login/login.cpp gui_component/register/register.cpp communication_component/communication_object.cpp
