#include "tags.h"

Tags::Tags(QObject *parent) : QObject(parent), _now(0)
{

}

bool Tags::HasTag(QString name)
{
    return _tagsNames.find(name)!=_tagsNames.end();
}

int Tags::RegisterTag(QString tag)
{
    if (HasTag(tag)) return -1;
    _tagsNames[tag] = _now;
    _tagOwners[_now++] = std::vector<const unsigned*>();
    return 0;
}

int Tags::RenameTag(QString oldName, QString newName)
{
    if (!HasTag(oldName)) return -1;
    if (HasTag(newName)) return -1;
    _tagsNames[newName] = _tagsNames[oldName];
    _tagsNames.erase(oldName);
    auto toCall = _tagOwners[_tagsNames[newName]];
    foreach (auto call, toCall) emit NameChange(*call, oldName, newName);
    return 0;
}

QStringList Tags::ListActiveTag()
{
    QStringList toRet;
    for (auto iterator = _tagsNames.begin();iterator!=_tagsNames.end();iterator++) toRet.push_back(iterator->first);
    return toRet;
}

int Tags::RegisterTagOwner(QString tag, const unsigned* id)
{
   if(!HasTag(tag)) return -1;
   auto x = _tagOwners[_tagsNames[tag]];
   if (!x.empty() && std::find(x.begin(),x.end(),id)!=x.end()) return -1;
   _tagOwners[_tagsNames[tag]].push_back(id);
   //emit OnTagModify();
   return 0;
}

int Tags::UnregisterTagOwner(QString tag, const unsigned* owner)
{
    auto vec = _tagOwners[_tagsNames[tag]];
    auto x = std::find (vec.begin(), vec.end(), owner);
    if (x==vec.end()) return -1;
    vec.erase(x);
    //emit OnTagModify();
    return 0;
}

int Tags::DeleteTag(QString tag)
{
    if(!HasTag(tag)) return -1;
    //emit OnDeletedTag(tag);
    unsigned tagId = _tagsNames[tag];
    _tagsNames.erase(tag);
    auto toCall = _tagOwners[_tagsNames[tag]];
    _tagOwners.erase(tagId);
    foreach (auto call, toCall) emit NameChange(*call, tag, "");
    return 0;
}

std::vector<const unsigned *> Tags::ProvideTagged(QString tag)
{
    return _tagOwners[_tagsNames[tag]];
}
