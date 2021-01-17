#include "event.h"


bool Event::AddTag(QString TagName, QString& info)
{

    info = "Tag added succesfully";
    if(std::find(_ownedTags.begin(), _ownedTags.end(), TagName) != _ownedTags.end()) {
        info = "Event alredy has this tag";
        return false;
    }
    if(_Tags!=nullptr)
    {
        int succ = _Tags->RegisterTagOwner(TagName,&id);
        if (succ==-1)
        {
            _Tags->RegisterTag(TagName);
            _Tags->RegisterTagOwner(TagName,&id);
        }
    }
    _ownedTags.push_back(TagName);
    std::sort(_ownedTags.begin(), _ownedTags.end());
    return true;
}

QStringList Event::GetTags()
{
    return  _ownedTags;
}

bool Event::RemoveTag(QString TagName)
{
    auto iter = std::find(_ownedTags.begin(), _ownedTags.end(), TagName);
    if (_Tags!=nullptr)_Tags->UnregisterTagOwner(TagName,&id);
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

bool Event::ForgetTag(QString tag)
{
    return _ownedTags.removeOne(tag);
}

bool Event::RenameTag(QString oldName, QString newName)
{
    auto index = _ownedTags.indexOf(oldName);
    if (index==-1) return  false;
    _ownedTags[index] = newName;
    return true;
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

QStringList Event::AllTags()
{
    if (_Tags==nullptr) return QStringList();
    return _Tags->ListActiveTag();
}

int Event::reincarnate(QDate date1)
{
    QDate OldDate = _startDate;
    _startDate = date1;
    bool succes;
    emit DateChange(id, &succes);
    if (succes==true)return 0;
    _startDate = OldDate;
    return -1;
}

int Event::reincarnate(QDate date1,QDate date2)
{

    QDate OldDate = _startDate, OldDate2 = _endDate;
    _startDate = date1;
    _endDate = date2;
    bool succes;
    emit DateChange(id, &succes);
    if (succes==true)return 0;
    _startDate = OldDate;
    _endDate = OldDate2;
    return -1;
}

void Event::Save(QTextStream &out)
{
    out<<"<Event>\n";
    out<< "\t<StartDate> "<< QString::number(_startDate.day()) +"." + QString::number(_startDate.month()) +"." +QString::number(_startDate.year()) <<" </StartDate>\n";
    if (isDual) out << "\t<EndDate> " << QString::number(_endDate.day()) +"." + QString::number(_endDate.month()) +"." +QString::number(_endDate.year()) << " </EndDate>\n";
    out<< "\t<Title> " << name << " </Title>\n";
    out<< "\t<Text> " << text << " </Text>\n";
    out<< "\t<Color> " << color.name() << " </Color>\n";
    if (!_ownedTags.empty())
    {
        out<<"\t<Tags>\n";
        foreach(auto tag, _ownedTags) out << "\t\t<Tag> "<<tag<<" </Tag>\n";
        out<<"\t</Tags>\n";
    }
    out<<"</Event>\n\n";
}

void Event::Unregister()
{
    emit SignOut(id);
    deleteLater();
}

void Event::ReregisterTags()
{
    if (_Tags==nullptr) return;
    foreach(auto tag, _ownedTags)
    {
        if(-1==_Tags->RegisterTagOwner(tag,&id))
        {
            _Tags->RegisterTag(tag);
            _Tags->RegisterTagOwner(tag,&id);
        }
    }
}

void Event::ProvideBoss(Tags *tags)
{
    _Tags = tags;
    ReregisterTags();
}

unsigned Event::TranslateId()
{
    unsigned toRet;
    emit GetRelId(&toRet);
    return toRet;
}

Event::Event(QObject *parent) : QObject(parent)
{
    throw "not enough data";
}

Event::Event(Tags* boss,QDate start, QObject *parent): QObject(parent),_startDate(start),_Tags(boss), _x(-1),_y(-1), isDual(0)
{
    color = Qt::lightGray;
}

Event::Event(Tags* boss,QDate start, QDate end, QObject *parent): QObject(parent), _startDate(start),_endDate(end),_Tags(boss), _x(-1),_y(-1),isDual(1)
{
    color = Qt::lightGray;
}

Event::~Event()
{
    if (_Tags!=nullptr) foreach (auto TagName, _ownedTags) {
        _Tags->UnregisterTagOwner(TagName,&id);
    }
}
