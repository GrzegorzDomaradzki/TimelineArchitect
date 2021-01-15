#include "event.h"


bool Event::AddTag(QString TagName, QString& info)
{

    info = "Tag added succesfully";
    if (TagName.isEmpty() || TagName[0]=='_')
    {
        info = "TagName can't be empty or starts from '_'";
        return false;
    }
    if(std::find(_ownedTags.begin(), _ownedTags.end(), TagName) != _ownedTags.end()) {
        info = "Event alredy has this tag";
        return false;
    }
    int succ = _Tags->RegisterTagOwner(TagName,id);
    if (succ==-1)
    {
        _Tags->RegisterTag(TagName);
        _Tags->RegisterTagOwner(TagName,id);
    }
    _ownedTags.push_back(TagName);
    std::sort(_ownedTags.begin(), _ownedTags.end());
    return true;
}

bool Event::RemoveTag(QString TagName)
{
    auto iter = std::find(_ownedTags.begin(), _ownedTags.end(), TagName);
    if(*iter==TagName)
    {
        _ownedTags.erase(iter);
        return true;
    }
    return false;
}

bool Event::IsBinary()
{
    return !isDual;
}

QDate Event::GetDateStart()
{
    return _startDate;
}

QDate Event::GetDateEnd()
{
    if (isDual) return _endDate;
    return _startDate;
}

bool Event::reincarnate(QDate date1, QString& info)
{
    QDate OldDate = _startDate;
    _startDate = date1;
    auto to_emit = id;
    emit DateChange(to_emit);
    if (to_emit==id)return 1;
    _startDate = OldDate;
    return 0;
}

bool Event::reincarnate(QDate date1,QDate date2, QString& info)
{

    QDate OldDate = _startDate, OldDate2 = _endDate;
    _startDate = date1;
    _endDate = date2;
    auto to_emit = id;
    emit DateChange(to_emit);
    if (to_emit==id)return 1;
    _startDate = OldDate;
    _endDate = OldDate2;
    return 0;
}

void Event::Save(QTextStream out)
{
    out<<"<Event>\n";
    out<< "\t<StartDate>"<< _startDate.toString() <<"</StartDate>\n";
    if (isDual) out << "\t<EndDate>" << _endDate.toString() << "</EndDate>\n";
    out<< "\t<Title> " << name << " </Title>\n";
    out<< "\t<Text> " << text << " </Text>\n";
    out<< "\t<Multimedia>" << multimedia << "</Multimedia>\n";
    if (!_ownedTags.empty())
    {
        out<<"\t<Tags>\n";
        foreach(auto tag, _ownedTags) out << "\t\t<Tag>"<<tag<<"</Tag>\n";
        out<<"\t</Tags>\n";
    }
    out<<"</Event>\n";
}

void Event::Unregister()
{
    emit SignOut(id);
    deleteLater();
}

Event::Event(QObject *parent) : QObject(parent)
{
    throw "not enough data";
}

Event::Event(Tags* boss,QDate start, QObject *parent): QObject(parent),_startDate(start), isDual(0),_Tags(boss), _x(-1),_y(-1)
{
}

Event::Event(Tags* boss,QDate start, QDate end, QObject *parent): QObject(parent), _startDate(start),_endDate(end),isDual(1),_Tags(boss), _x(-1),_y(-1)
{
}

Event::~Event()
{

    foreach (auto TagName, _ownedTags) {
        _Tags->UnregisterTagOwner(TagName,id);
    }
}
