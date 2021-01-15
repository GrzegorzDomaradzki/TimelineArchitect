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
    emit OnTagModify();
    return 0;
}

int Tags::RenameTag(QString oldName, QString newName)
{
    if (!HasTag(oldName)) return -1;
    if (HasTag(newName)) return -1;
    _tagsNames[newName] = _tagsNames[oldName];
    _tagsNames.erase(oldName);
    emit OnTagModify();
    return 0;
}

int Tags::RegisterTagOwner(QString tag, const unsigned* id)
{
   if(!HasTag(tag)) return -1;
   _tagOwners[_tagsNames[tag]].push_back(id);
   emit OnTagModify();
   return 0;
}

int Tags::UnregisterTagOwner(QString tag, const unsigned* owner)
{
    auto vec = _tagOwners[_tagsNames[tag]];
    vec.erase(std::find (vec.begin(), vec.end(), owner));
    emit OnTagModify();
    return 0;
}

int Tags::DeleteTag(QString tag)
{
    if(!HasTag(tag)) return -1;
    emit OnDeletedTag(tag);
    unsigned tagId = _tagsNames[tag];
    _tagsNames.erase(tag);
    _tagOwners.erase(tagId);
    return 0;
}

std::vector<const unsigned *> Tags::ProvideTagged(QString tag)
{
    return _tagOwners[_tagsNames[tag]];
}
