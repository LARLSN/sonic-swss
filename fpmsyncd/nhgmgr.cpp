#include "nhgmgr.h"
#include <string.h>
#include "logger.h"

using namespace std;

NHGMgr::NHGMgr()
{
}

NHGMgr::~NHGMgr()
{
}

int NHGMgr::addNHG(const NextHopGroupFull nhg)
{
    int ret = 0;
    if (m_rib_nhg_table.isNhgExist(nhg.id))
    {
        RIBNhgEntry *entry = m_rib_nhg_table.getEntry(nhg.id);
        m_rib_nhg_table.updateNhg(nhg.id, nhg);
    }
    else
    {
        ret = m_rib_nhg_table.addNhg(nhg);
    }
    // TODO: srv6 sonic nhg create
    return ret;
}

int NHGMgr::delNHG(uint32_t id)
{
    if (!m_rib_nhg_table.isNhgExist(id))
    {
        SWSS_LOG_ERROR("NextHop group id %d not found.", id);
        return 0;
    }

    // TODO: del nhg from sonic table
    return m_rib_nhg_table.delNhg(id);
}

RIBNhgEntry *NHGMgr::getRIBNhgEntryByKey(string key)
{
    return m_rib_nhg_table.getEntry(key);
}

RIBNhgEntry *NHGMgr::getRIBNhgEntryByRIBID(uint32_t id)
{
    return m_rib_nhg_table.getEntry(id);
}

// TODO: add sonic boject creationuint32_t
SonicNHGObject *NHGMgr::getSonicNHGByKey(std::string key)
{
    return nullptr;
}

SonicNHGObject *NHGMgr::getSonicNHGByRIBID(uint32_t id)
{
    return nullptr;
}

RIBNhgTable::RIBNhgTable()
{
}

RIBNhgEntry *RIBNhgTable::getEntry(string key)
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
        SWSS_LOG_ERROR("NextHop group id %d not found.", id);
        return 0;
    }

    RIBNhgEntry *entry = m_nhg_map[id];
    if (entry->getDependentsID().size() != 0)
    {
        SWSS_LOG_ERROR("NextHop group id %d still has dependents.", id);
        return -1;
    }

    // TODO: update depends and dependents

    m_key_2_id_map.erase(entry->getKey());
    delete entry;
    m_nhg_map.erase(id);
    return 0;
}

int RIBNhgTable::addNhg(NextHopGroupFull nhg)
{
    if (m_nhg_map.find(nhg->getId()) != m_nhg_map.end())
    {
        SWSS_LOG_ERROR("NextHop group id %d key %s already exists.", nhg->id);
        return -1;
    }

    RIBNhgEntry *entry = RIBNhgEntry::create_nhg_entry(nhg);
    if (entry == nullptr)
    {
        SWSS_LOG_ERROR("Failed to create nhg entry for %s", nhg->getKey());
        return -1;
    }

    // TODO: update depends and dependents

    m_nhg_map.insert(std::make_pair(nhg->getId(), entry));
    m_key_2_id_map.insert(std::make_pair(entry->getKey(), entry));
    return 0;
}

int RIBNhgTable::updateNhg(NextHopGroupFull nhg)
{
    if (m_nhg_map.find(nhg.getId()) == m_nhg_map.end())
    {
        SWSS_LOG_ERROR("NextHop group id %d key %s not exists.", nhg.id);
        return -1;
    }

    RIBNhgEntry *entry = m_nhg_map.find(nhg->getId())->second();

    int ret = entry->setEntry(nhg);
    if (ret != 0)
    {
        SWSS_LOG_ERROR("Failed to create nhg entry for %s", nhg->getKey());
        return -1;
    }

    // TODO: update depends and dependents

    m_nhg_map.insert(std::make_pair(nhg.id, entry));
    m_key_2_id_map.insert(std::make_pair(entry->getKey(), entry));
    return 0;
}

void RIBNhgTable::add_nhg_dependents(RIBNhgEntry *entry)
{
    for (int i = 0; i < entry->getNhg().group.size(); i++)
    {
        m_nhg_map[entry->getNhg().group[i].first]->m_depends.push_back(entry->getNhg().getId());
    }
}

void RIBNhgTable::remove_nhg_dependents(RIBNhgEntry *entry)
{
    for (int i = 0; i < entry->getNhg().group.size(); i++)
    {
        m_nhg_map[entry->getNhg().group[i].first]->m_depends.push_back(entry->getNhg().id);
    }
}

RIBNhgEntry *RIBNhgEntry::create_nhg_entry(NextHopGroupFull nhg)
{

    RIBNhgEntry *entry = new RIBNhgEntry();
    int ret = entry->setEntry(nhg);
    if (ret != 0)
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

vector<RIBNhgEntry *> RIBNhgEntry::getDependsID()
{
    return m_depends;
}

vector<RIBNhgEntry *> RIBNhgEntry::getDependentsID()
{
    return m_dependents;
}

NexthopKey RIBNhgEntry::getKey()
{
    return m_key;
}

NextHopGroupFull RIBNhgEntry::getNhg()
{
    return m_nhg;
}
int RIBNhgEntry::setEntry(NexthopGroupFull)
{
    m_key = NexthopKey(&nhg);
    for (int i = 0; i < nhg.group.size(); i++)
    {

        // validate group member
        if (m_map.find(nhg.group[i].first) == m_map.end())
        {
            SWSS_LOG_ERROR("NextHop id %d in group not found.", nhg_id);
            return -1;
        }

        // update resolved group
        if (nhg.group[i].type != NEXTHOP_TYPE_RECURSIVED)
        {
            entry->resolved_group.push_back(nhg.group[i]);
        }

        // TODO: add dependent and depends
    }
    return 0;
}

NexthopKey::NexthopKey(const NextHopGroupFull *nhg)
{
    key = "";
    for (int i = 0; i < nhg->group.size(); i++)
    {
        if (i == 0)
        {
            key = key + "group:";
        }
        key = key + "id" + nhg->group[i].id + "weight" + nhg->group[i].weight;
    }
    switch (nhg->type)
    {
    case NEXTHOP_TYPE_IFINDEX:
    {
        key = key + "type:" + nhg->type;
        key = key + "ifindex:" + nhg->ifindex;
        key = key + "vrf_id" + nhg->vrf_id;
    }
    case NEXTHOP_TYPE_IPV4:
    {
        key = key + "type:" + nhg->type;
        key = key + "ifindex:" + nhg->ifindex;
        key = key + "vrf_id" + nhg->vrf_id;
        key = key + "gate" + nhg->gate.ipv4;
    }
    case NEXTHOP_TYPE_IPV4_IFINDEX:
    {
        key = key + "type:" + nhg->type;
        key = key + "ifindex:" + nhg->ifindex;
        key = key + "vrf_id" + nhg->vrf_id;
        key = key + "gate" + nhg->gate.ipv4;
    }
    case NEXTHOP_TYPE_IPV6:
    {
        key = key + "type:" + nhg->type;
        key = key + "ifindex:" + nhg->ifindex;
        key = key + "vrf_id" + nhg->vrf_id;
        key = key + "gate" + nhg->gate.ipv6;
    }
    case NEXTHOP_TYPE_IPV6_IFINDEX:
    {
        key = key + "type:" + nhg->type;
        key = key + "ifindex:" + nhg->ifindex;
        key = key + "vrf_id" + nhg->vrf_id;
        key = key + "gate" + nhg->gate.ipv6;
    }
    case NEXTHOP_TYPE_BLACKHOLE:
    {
        key = key + "type:" + nhg->type;
        key = key + "blackhole_type:" + nhg->bh_type;
    }
    default:
    }
}

SonicNHGTable::SonicNHGTable()
{
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
