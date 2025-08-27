
#ifndef NHGMGR_H
#define NHGMGR_H

#include <string.h>
#include "dbconnector.h"

using namespace std;
using namespace swss;

class NHGMgr
{

public:
    NHGMgr();
    ~NHGMgr();
    static NHGMgr getInstance()
    {
        static NHGMgr instance;
        return instance;
    };
    int addNHG(NextHopGroupFull nhg);
    int delNHG(uint32_t id);
    RIBNhgEntry *getRIBNhgEntryByKey(string key);
    RIBNhgEntry *getRIBNhgEntryByRIBID(uint32_t id);

    // Not implemented
    SonicNHGObject *getSonicNHGByID(uint32_t id);
    SonicNHGObject *getSonicNHGByKey(std::string key);

private:
    DBConnector *m_db;
    RIBNhgTable m_rib_nhg_table;
    SonicNHGTable m_sonic_nhg_table;
};

class RIBNhgTable
{

public:
    RIBNhgTable();
    ~RIBNhgTable();
    int addNhg(NextHopGroupFull nhg);
    int delNhg(uint32_t id);
    int updateNhg(uint32_t, nhg);
    RIBNhgEntry *getEntry(uint32_t id);
    RIBNhgEntry *getEntry(std::string key);
    void add_nhg_dependents(RIBNhgEntry *entry);
    void remove_nhg_dependents(RIBNhgEntry *entry);
    bool isNhgExist(string key);
    bool isNhgExist(uint32_t id);

    // Not implemented
    vector<RIBNhgEntry *> getDepends(std::string key);
    vector<RIBNhgEntry *> getDependents(uint32_t id);

private:
    NexthopKey m_key;
    map<uint32_t, RIBNhgEntry *> m_nhg_map;
    map<string, uint32_t> m_key_2_id_map;
};

class RIBNhgEntry
{
public:
    RIBNhgEntry();
    vector<RIBNhgEntry *> getDependsID();
    vector<RIBNhgEntry *> getDependentsID();
    vector<pair<uint32_t, uint8_t>> getGroup();
    vector<pair<uint32_t, uint8_t>> getResolvedGroup();
    NexthopKey getKey();
    NextHopGroupFull getNhg();
    int setEntry(NexthopGroupFull);
    static RIBNhgEntry *create_nhg_entry(NextHopGroupFull);

private:
    NexthopKey m_key;
    NextHopGroupFull m_nhg;
    vector<pair<uint32_t, uint8_t>> m_group;
    vector<pair<uint32_t, uint8_t>> m_resolved_group;
    vector<uint32_t> m_depends;
    vector<uint32_t> m_dependents;
    // TODO: sonic related object;
    // vector<uint32_t> m_sonic_object_id;
};

class NexthopKey
{

public:
    NexthopKey();
    NexthopKey(const NextHopGroupFull *nhg);
    std::string key;
    string getNhgKey()
    {
        return key;
    }
};

// for pic. Not implemented
class SonicNHGTable
{

public:
    SonicNHGTable();
    ~SonicNHGTable();
    int addNhg();
    int delNhg();
    SonicNHGEntry *getEntry(std::string key);
    SonicNHGEntry *getEntry(uint32_t id);

private:
    DBConnector *m_db;
};

class SonicNHGEntry
{

public:
    SonicNHGEntry();
    void create_nhg_entry();

private:
    NexthopKey m_key;
};

// TODO
class SonicNHGObject
{
};

#endif // NHGMGR_H
