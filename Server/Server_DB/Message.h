#pragma once
#include <string>

using namespace std;

class Message
{
public:
    Message(const string& from, const string& to, const string& text) :
        _from(from),
        _to(to),
        _text(text)
    {}

private:
    string _from;
    string _to;
    string _text;
};