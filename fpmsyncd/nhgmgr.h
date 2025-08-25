
#ifndef NHGMGR_H
#define NHGMGR_H

#include "dbconnector.h"
#include <string.h>

using namespace std;

class NHGMgr
{

public:
    NHGMgr();
    ~NHGMgr();
    static NHGMgr *getInstance();
    int addNHG(NextHopGroupFull nhg);
    // int addNHGFromRIB(uint32_t id, const string gateway, const int32_t ifindex);
    int delNHG(uint32_t id);
    RIBNhgEntry *getRIBNhgEntryByKey(std::string key);
    RIBNhgEntry *getRIBNhgEntryByRIBID(uint32_t id);
    SonicNHGObject *getSonicNHGByRIBID(string key);
    SonicNHGObject *getSonicNHGByKey(string key);

private:
    DBConnector *m_db;
    RIBNhgTable m_rib_nhg_table;
    SonicNHGTable m_sonic_nhg_table;
};

class RIBNhgTable
{

public:
    RIBNhgTable(DBConnector &db);
    ~RIBNhgTable();
    int addNhg(NextHopGroupFull nhg);
    int delNhg(uint32_t id);
    vector<RIBNhgEntry *> getDepends(std::string key);
    vector<RIBNhgEntry *> getDependents(uint32_t id);
    RIBNhgEntry *getEntry(uint32_t id);
    RIBNhgEntry *getEntry(std::string key);
    void add_nhg_dependents(RIBNhgEntry *entry);
    void remove_nhg_dependents(RIBNhgEntry *entry);
    bool isNhgExist(string key);
    bool isNhgExist(uint32_t id);

private:
    NexthopKey m_key;
    map<uint32_t, RIBNhgEntry *> m_nhg_map;
    map<string, uint32_t> m_key_2_id_map;
}

class RIBNhgEntry
{
public:
    RIBNhgEntry();
    vector<RIBNhgEntry *> getDependsID();
    vector<RIBNhgEntry *> getDependentsID();
    vector<pair<uint32_t, uint8_t>> getGroup();
    vector<pair<uint32_t, uint8_t>> getResolvedGroup();
    int setEntry(NexthopGroupFull);
    static RIBNhgEntry *create_nhg_entry(NextHopGroupFull);
void setKey(NexthopKey) private : NexthopKey m_key;
    NextHopGroupFull m_nhg;
    vector<pair<uint32_t, uint8_t>> m_group;
    vector<pair<uint32_t, uint8_t>> m_resolved_group;
    vector<uint32_t> m_depends;
    vector<uint32_t> m_dependents;
    // vector<uint32_t> m_sonic_object_id;
}

// pic
class SonicNHGTable
{

public:
    SonicNHGTable(DBConnector &db);
    ~SonicNHGTable();
    int addNhg();
    int delNhg();
    SonicNHGEntry *getEntry(std::string key);
    SonicNHGEntry *getEntry(uint32_t id);

private:
    DBConnector *m_db;
}

class SonicNHGEntry
{

public:
    SonicNHGEntry();
    void create_nhg_entry();

private:
    NexthopKey m_key;
}

class NexthopKey
{

public:
    NexthopKey();
    NexthopKey(const NextHopGroupFull *nhg) {

    };
    string nexthop;
    string intf;
    vector<pair<uint32_t, uint32_t>> group;
    vector<pair<uint32_t, uint32_t>> flags;
    string getNhgKey() {

    };
}
#endif // NHGMGR_H
