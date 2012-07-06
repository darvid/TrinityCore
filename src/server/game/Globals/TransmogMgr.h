#ifndef _TRANSMOGMGR_H
#define _TRANSMOGMGR_H

#include "ArenaTeam.h"
#include "Log.h"
#include "Creature.h"
#include "Player.h"
#include "Item.h"
#include <ace/Singleton.h>
#include <vector>

#define TRANSMOG_ITEMS_TABLE "transmog_items"            // characters DB
#define TRANSMOG_PREDEFINED_SETS_TABLE "transmog_sets"   // world DB

enum TransmogSetFields
{
    TRANSMOG_FIELD_ENTRY,
    TRANSMOG_FIELD_CLASS,
    TRANSMOG_FIELD_RACE,
    TRANSMOG_FIELD_RATING,
    TRANSMOG_FIELD_BRACKET,
    TRANSMOG_FIELD_HEAD,
    TRANSMOG_FIELD_SHOULDERS,
    TRANSMOG_FIELD_CHEST,
    TRANSMOG_FIELD_WRISTS,
    TRANSMOG_FIELD_HANDS,
    TRANSMOG_FIELD_WAIST,
    TRANSMOG_FIELD_LEGS,
    TRANSMOG_FIELD_FEET,
};

class TransmogMgr
{
    friend class ACE_Singleton<TransmogMgr, ACE_Null_Mutex>;
    
    public:
        TransmogMgr();
        ~TransmogMgr();

        typedef std::pair<uint8, uint8> RaceClass;
        typedef std::vector<std::vector<uint32> > TransmogSets;

        void LoadTransmog();

        static bool CanAfford(Player* player, const std::vector<uint32>* setInfo);
        TransmogSets GetAllSets(RaceClass raceClass) {
            TransmogSetsContainer::const_iterator it = _transmogSetStore.find(
                raceClass);
            return it->second;
        };
        TransmogResult TransmogItem(Player* player, uint32 newEntry,
                                    int8 slot, bool silent = false);
        TransmogResult TransmogSet(Player* player, RaceClass raceClass,
                                   uint32 setId);

        uint32 GetTransmogrifiedItemEntry(uint32 itemGuid);
        void RemoveTransmogrifiedItem(uint32 itemGuid);
        void SetTransmogrifiedItem(uint32 itemGuid, uint32 newEntry);

    private:
        typedef std::map<RaceClass, TransmogSets> TransmogSetsContainer;
        TransmogSetsContainer _transmogSetStore;

        typedef std::map<uint32, uint32> TransmogrifiedItemsContainer;
        TransmogrifiedItemsContainer _transmogrifiedItemsStore;

        int8 TransmogFieldForSlot(int8 slot);
        void LoadTransmogrifiedItems();
        void LoadPredefinedSets();
};


#define sTransmogMgr ACE_Singleton<TransmogMgr, ACE_Thread_Mutex>::instance()
#endif