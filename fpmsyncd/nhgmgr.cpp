#include "nhgmgr.h"


NHGMgr::getInstance()
{
    static NHGMgr instance;
    return &instance;
}
NHGMgr::~NHGMgr()
{
}
int NHGMgr::reviceNewNHGFromRIB(const RibNHGContext* ribCtx)
{
    int ret=0;
    // add nhg to rib table
    // add ngh to sonic table
    // update rib_2_sonic map
    return ret;
}
int NHGMgr::reviceDelNHGFromRIB(const RIBNHGContext* ribCtx)
{
    int ret=0;
    // del nhg from sonic table
    // del ngh from rib table
    // update rib_2_sonic map
    return ret;
}

RIBNHGContext* NHGMgr::getNhgContextByKey(string key)
{
   return m_rib_nhg_table->getNhgContextByKey(key);
}

RIBNHGContext* NHGMgr::getNhgContextByRIBID(uint32_t id)
{
   return m_rib_nhg_table->getNhgContextByRIBID(id);
}

RIBNHGContext* RIBNhgTable::getNhgContextByKey(string key)
{
   if m_key_2_id_map.find(key) != m_key_2_id_map.end()
   {
       uint32_t id = m_key_2_id_map[key];
       return getNhgContextByRIBID(id);
   }else{
       return nullptr;
   }
}
