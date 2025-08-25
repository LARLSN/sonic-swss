#include "nhgmgr.h"

NHGMgr::getInstance()
{
    static NHGMgr instance;
    return &instance;
}

NHGMgr::~NHGMgr()
{
}

int NHGMgr::addNHG(const NextHopGroupFull nhg)
{
    int ret = 0;
    string key = NexthopKey::getKeyFromNextHopGroupFull(nhg);
    if (m_rib_nhg_table.isNhgExist(key))
    {
        RIBNhgEntry *entry = m_rib_nhg_table->getEntry(key);
        m_rib_nhg_table.update(entry)
    }
    else
    {
        RIBNhgEntry *new_entry = new RIBNhgEntry(nhg);
        ret = m_rib_nhg_table.insert(new_entry)
    }
    // srv6 sonic nhg create
    return ret;
}

int NHGMgr::delNHG(uint32_t id)
{
    if (!m_rib_nhg_table.isNhgExist(key))
    {
        SWSS_LOG_ERROR("NextHop group id %d not found. Dropping the route %s", nhg_id, destipprefix);
        return 0;
    }

    // del nhg from sonic table
    return m_rib_nhg_table.delNhg(id);
}

RIBNhgEntry *NHGMgr::getRIBNhgEntryByKey(std::string key)
{
    return m_rib_nhg_table->getEntry(key);
}

RIBNhgEntry *NHGMgr::getRIBNhgEntryByRIBID(uint32_t id)
{
    return m_rib_nhg_table->getEntry(id);
}

// TODO: add sonic boject creationuint32_t

SonicNHGObject *NHGMgr::getSonicNhgByRIBID(string key)
{
    return nullptr;
}

SonicNHGObject *getSonicNhgByKey(string key)
{
    return nullptr;
}

RIBNhgEntry *RIBNhgTable::getEntry(std::string key)
{
    auto it = m_key_2_id_map.find(key);
    if (it == m_key_2_id_map.end())
    {
        return nullptr;
    }
    it = m_nhg_map.find(it->second);
    if (it == m_nhg_map.end())
    {
        return nullptr;
    }
    return it->second;
}

RIBNhgEntry *RIBNhgTable::getEntry(uint32_t id)
{
    auto it = m_nhg_map.find(id);
    if (it == m_key_2_id_map.end())
    {
        return nullptr;
    }
    return it->second;
}

int RIBNhgTable::delNhg(uint32_t id)
{
    if (m_nhg_map.find(id) == m_nhg_map.end())
    {
        SWSS_LOG_ERROR("NextHop group id %d not found.", nhg_id);
        return 0;
    }

    RIBEntry *entry = m_nhg_map[id];
    if (entry->getDependentsID().size() != 0)
    {
        SWSS_LOG_ERROR("NextHop group id %d still has dependents.", nhg_id);
        return -1;
    }

    m_key_2_id_map.erase(entry->getKey());
    delete entry;
    m_nhg_map.erase(id);
    return 0;
}

int RIBNhgTable::addNhg(NextHopGroupFull nhg)
{
    if (nhg == nullptr)
    {
        SWSS_LOG_ERROR("nhg is null");
        return -1;
    }
    if (m_nhg_map
            .find(nhg->getId()) != m_nhg_map.end())
    {
        SWSS_LOG_ERROR("NextHop group id %d key %s already exists.", nhg->id);
        return -1;
    }

    RIBEntry *entry = RIBEntry::create_nhg_entry(nhg);
    if (entry == nullptr)
    {
        SWSS_LOG_ERROR("Failed to create nhg entry for %s", nhg->getKey());
        return -1;
    }

    update_nhg_dependents(entry)

        m_nhg_map.insert(std::make_pair(nhg->getId(), entry));
    m_key_2_id_map.insert(std::make_pair(entry->getKey(), entry));
    return 0;
}

void RIBNhgTable::add_nhg_dependents(RIBNhgEntry *entry)
{
    for (int i = 0; i < entry->nhg.group.size(); i++)
    {
        m_nhg_map[entry->nhg.group[i].first]->m_depends.push_back(entry->nhg.getId());
    }
}

void RIBNhgTable::remove_nhg_dependents(RIBNhgEntry *entry)
{
    for (int i = 0; i < entry->nhg.group.size(); i++)
    {
        m_nhg_map[entry->nhg.group[i].first]->m_depends.push_back(entry->nhg.getId());
    }
}

RIBNhgEntry *RIBNhgEntry::create_nhg_entry(const NextHopGroupFull nhg)
{

    RIBNhgEntry *entry = new RIBNhgEntry();
    int ret = entry->setEntry(nhg) if ret != 0
    {
        delete entry;
        return nullptr;
    }
    return entry;
}

vector<pair<uint32_t, uint8_t>> RIBNhgEntry::getGroup()
{
    return m_group;
}

vector<pair<uint32_t, uint8_t>> RIBNhgEntry::getResolvedGroup()
{
    return m_resolved_group;
}

vector<RIBNhgEntry *> RIBNhgEntry::~getDependsID()
{
    return m_depends;
}

vector<RIBNhgEntry *> RIBNhgEntry::getDependentsID()
{
    return m_dependents;
}

int setEntry(NexthopGroupFull)
{
    entry->key = NexthopKey(&nhg) for (int i = 0; i < nhg.group.size(); i++)
    {

        // validate group member
        if (m_map.find(nhg.group[i].first) == m_map.end())
        {
            SWSS_LOG_ERROR("NextHop id %d in group not found.", nhg_id);
            return -1;
        }

        // update resolved group
        if (nhg
                .group[i]
                .type != nexthop_types_t::NEXTHOP_TYPE_RECURSIVED)
        {
            entry->resolved_group.push_back(nhg.group[i]);
        }

        // TODO: add dependent and depends
    }
    return 0;
}

SonicNHGTable::SonicNHGTable(DBConnector &db)
{
    m_db = db;
}

SonicNHGTable::~SonicNHGTable()
{
    return;
}

int SonicNHGTable::addNhg()
{
    return 0;
}

int SonicNHGTable::delNhg()
{
    return 0;
}

SonicNHGEntry *SonicNHGTable::getEntry(std::string key)
{
    return nullptr;
}

SonicNHGEntry *SonicNHGTable::getEntry(uint32_t id)
{
    return nullptr;
}

SonicNHGEntry::SonicNHGEntry()
{
    return;
}

SonicNHGEntry::~SonicNHGEntry()
{
    return;
}

void SonicNHGEntry::create_nhg_entry()
{
    return;
}

