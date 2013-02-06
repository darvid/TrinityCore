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

// TODO: move to world config/db
enum TransmogCurrency
{
    OLD_TRANSMOG_TOKEN = 38644,
    TRANSMOG_TOKEN = 24368
};

enum TransmogSetFields
{
    TRANSMOG_FIELD_ENTRY,
    TRANSMOG_FIELD_ACCOUNTID,
    TRANSMOG_FIELD_EMPTY,
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

        typedef std::pair<Classes, EquipmentSlots> ClassSlot;
        typedef std::map<ClassSlot, uint32> TransmogVendorContainer;

        uint32 GetVendorForSlotAndClass(int8 playerClass, EquipmentSlots slot);

        void LoadTransmog();
        void LoadTransmogVendorEntries();
        void LoadCustomSets(uint32 accountId);
        const int GetNumTransmogSets() { return _transmogSetStore.size(); }
        const int GetNumCustomTransmogSets() {
            return _transmogCustomSetStore.size();
        }
        const int GetTotalTransmogSets() {
            return GetNumTransmogSets() + GetNumCustomTransmogSets();
        }

        static bool CanAfford(Player* player, const std::vector<uint32>* setInfo);
        TransmogSets GetAllSets(RaceClass raceClass) {
            TransmogSetsContainer::const_iterator it;
            return _transmogSetStore.find(raceClass)->second;
        };
        const TransmogSets* GetAllSets(uint32 accountId) {
            TransmogCustomSetsContainer::const_iterator it;
            it = _transmogCustomSetStore.find(accountId);
            if (it != _transmogCustomSetStore.end())
                return &it->second;
        }

        TransmogResult TransmogItem(Player* player, uint32 newEntry,
                                    int8 slot, bool silent = false);
        TransmogResult TransmogSet(Player* player, RaceClass raceClass,
                                   uint32 setId);
        TransmogResult TransmogSet(Player* player, uint32 accountId,
                                   uint32 setId);
        TransmogResult TransmogSet(Player* player, const std::vector<uint32>* setinfo);
        bool SaveCustomSet(Player* player, uint32 accountId, uint32 setId);

        uint32 GetTransmogrifiedItemEntry(uint32 itemGuid);
        int8 RemoveAllTransmogrifiedItems(Player* player);
        void RemoveTransmogrifiedItem(Player* player, Item* item);
        void RemoveTransmogrifiedItem(uint32 itemGuid);
        void SetTransmogrifiedItem(uint32 itemGuid, uint32 newEntry);

    private:
        typedef std::map<RaceClass, TransmogSets> TransmogSetsContainer;
        TransmogSetsContainer _transmogSetStore;

        typedef std::map<uint32, TransmogSets> TransmogCustomSetsContainer;
        TransmogCustomSetsContainer _transmogCustomSetStore;

        typedef std::map<uint32, uint32> TransmogrifiedItemsContainer;
        TransmogrifiedItemsContainer _transmogrifiedItemsStore;

        TransmogVendorContainer _transmogVendorStore;

        int8 TransmogFieldForSlot(int8 slot);
        void LoadTransmogrifiedItems();
        void LoadPredefinedSets();
};


#define sTransmogMgr ACE_Singleton<TransmogMgr, ACE_Thread_Mutex>::instance()
#endif