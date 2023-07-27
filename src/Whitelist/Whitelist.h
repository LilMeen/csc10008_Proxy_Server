#ifndef WHITELIST_H_INCLUDED
#define WHITELIST_H_INCLUDED

#include "../ProxyServer.h"

#endif // WHITELIST_H_INCLUDED

vector <string> loadWhiteList(){
    ifstream ifs;
    ifs.open("whilelist.conf");
    if (!ifs.is_open()){
        cout << "Cannot load file!";
    }
    else{
    vector <string> webName;
    string s;
    while(getline(ifs, s)){
        webName.push_back(s);
        s = "";
        }
    ifs.close();
    return webName;
    }
}

bool isAllowed(vector <string> webName, string host){
    string web = host.substr(4);
    for (int i = 0; i < (int)webName.size(); ++i)
        if (web == webName[i])
            return true;
    return false;
}


