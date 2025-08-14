//
// Created by lsn on 25-8-11.
//

#ifndef NHGMGR_H
#define NHGMGR_H

#include "dbconnector.h"
#include<string.h>

// For now
#define DEFAULT_RIB_TABLE_SIZE 1000
#define DEFAULT_SONIC_TABLE_SIZE 1000

using namespace std;

class RIBNHGContext{
};
class SonicNHGObject{
};

class NHGMgr {

public:
    NHGMgr();
    ~NHGMgr();
    static NHGMgr* getInstance();
    int reviceNewNHGFromRIB(const RIBNHGContext*);
    int reviceDelNHGFromRIB(const RIBNHGContext*);
    RIBNHGContext* getNhgContextByKey(std::string key);
    RIBNHGContext* getNhgContextByRIBID(uint32_t id);
    void getSonicNhgBySonicID(uint32_t);
    SonicNHGObject* getSonicNhgByRIBID(string key);
    SonicNHGObject* getSonicNhgByRIBID(string key);
    SonicNHGObject* getSonicNhgByKey(string key);

private:
    DBConnector* m_db;
    RIBNhgTable m_rib_nhg_table;
    SonicNhgTable m_sonic_nhg_table;
    map<uint32_t, uint32_t> m_sonic_2_rib_map;
    map<uint32_t, uint32_t> m_rib_2_sonic_map;
};

class RIBNhgTable {

public:
    RIBNhgTable(DBConnector& db);
    ~RIBNhgTable();
    int addNhg();
    int delNhg();
    vector<RIBNHGContext*> getDepends(std::string key);
    vector<RIBNHGContext*> getDependents(uint64_t id);
    RIBNHGContext* getNhgContext(uint64_t id);

    bool isNhgExist(string key);
    bool isNhgExist(uint64_t id);
    vector<RIBNHGContext*> getNhgByID(std::string key);

private:
    DBConnector* m_db;
    map<uint64_t, RIBNhgEntry*> m_nhg_map;
    map<string, uint64_t> m_key_2_id_map;
}

class NexthopKey {

public:
    NexthopKey();
    string nexthop;
    string intf;
    vector<pair<uint32_t,uint32_t>> group;
    string getNhgKey() {
        if length(group) <= 0 {
            return nexthop + "|" + intf;
        }else{
            string key = "";
            for (auto it = group.begin(); it != group.end(); ++it) {
                key += it->first + "|" + it->second + "|";
            }
            return key;
        }
    };
    NexthopKey createNexthopKeyFromRIBCtx(const RIBNHGContext*) {
        return "";
    }
}


class RIBNhgEntry {

public:
    RIBNhgEntry();
    vector<RIBNhgEntry*> getDependsID();
    vector<RIBNhgEntry*> getDependentsID();

private:
    RIBNHGContext m_nhg_ctx;
    vector<pair<uint32_t,uint32_t>> m_depends;
    vector<pair<uint32_t,uint32_t>> m_dependents;
    void create_nhg_entry(string key, void* nhg);
}

class SonicNhgEntry {

public:
    SonicNhgEntry();
    void create_nhg_entry(string key, void* nhg);

private:
    vector<pair<uint32_t,uint32_t>> m_depends;
    vector<pair<uint32_t,uint32_t>> m_dependents;
}

class SonicNhgTable {

public:
    SonicNhgTable(DBConnector& db);
    ~SonicNhgTable();
    void addNhg();
    void delNhg();
    void getNhgByKey(std::string key, void* nhg);
    void getNhgByID(string key, void* nhg);

private:
    DBConnector* m_db;
    void write_to_db();
}

class SonicNhgEntry {

public:
    SonicNhgEntry();
    void create_nhg_entry();
    vector<SonicNhgEntry*> getDependsID();
    vector<SonicNhgEntry*> getDependentsID();

private:
    vector<pair<uint32_t,uint32_t>> m_depends;
    vector<pair<uint32_t,uint32_t>> m_dependents;
}
#endif //NHGMGR_H
