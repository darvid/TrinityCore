/* ScriptData
SDName: Profession_NPC
SDAuthor: Dave
SD%Complete: 100
SDComment:
SDCategory: passivewow
EndScriptData */

#include "ScriptPCH.h"
#include <cstring>

class profession_vendor : public CreatureScript
{
    public:

        profession_vendor() : CreatureScript("profession_vendor")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Blacksmithing", GOSSIP_SENDER_MAIN,
                SKILL_BLACKSMITHING);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Engineering", GOSSIP_SENDER_MAIN,
                SKILL_ENGINEERING);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Enchanting", GOSSIP_SENDER_MAIN,
                SKILL_ENCHANTING);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Jewelcrafting", GOSSIP_SENDER_MAIN,
                SKILL_JEWELCRAFTING);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Leatherworking", GOSSIP_SENDER_MAIN,
                SKILL_LEATHERWORKING);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Tailoring", GOSSIP_SENDER_MAIN,
                SKILL_TAILORING);

            player->PlayerTalkClass->SendGossipMenu(GOSSIP_ICON_TRAINER, creature->GetGUID());

            return true;
        }

		bool PlayerAlreadyHasTwoProfessions(const Player *player) const
        {
                uint32 skill_count = 0;

                if (player->HasSkill(SKILL_MINING))
                        skill_count++;
                if (player->HasSkill(SKILL_SKINNING))
                        skill_count++;
                if (player->HasSkill(SKILL_HERBALISM))
                        skill_count++;

                if (skill_count >= 2)
                        return true;

                for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
                {
                        SkillLineEntry const *skill_info = sSkillLineStore.LookupEntry(i);
                        if (!skill_info)
                                continue;
                        if (skill_info->categoryId == SKILL_CATEGORY_SECONDARY)
                                continue;
                        if ((skill_info->categoryId != SKILL_CATEGORY_PROFESSION) || !skill_info->canLink)
                                continue;
                        const uint32 skill_id = skill_info->id;
                        if (player->HasSkill(skill_id))
                                skill_count++;
                        if (skill_count >= 2)
                                return true;
                }

                return false;
        }

		bool LearnProfession(Player *player, Creature* creature, SkillType skill)
		{
			if (PlayerAlreadyHasTwoProfessions(player))
				creature->MonsterWhisper("You must unlearn a profession first.", player->GetGUID());
			else {
				switch (skill)
				{
					case SKILL_BLACKSMITHING:
						player->learnSpell(51300, false);
						creature->MonsterWhisper("To socket your belt or bracers, see an Enchanter.", player->GetGUID());
						break;
					case SKILL_ENGINEERING:
						player->learnSpell(51306, false);
						creature->MonsterWhisper("To enhance your cloak or boots, see an Enchanter.", player->GetGUID());
						break;
                    case SKILL_ENCHANTING:
                        player->learnSpell(51313, false);
                        creature->MonsterWhisper("To enchant your rings, see an Enchanter", player->GetGUID());
                        break;
					case SKILL_JEWELCRAFTING:
						player->learnSpell(51311, false);
						creature->MonsterWhisper("Superior gems may be found in the Gems vendor.", player->GetGUID());
						break;
					case SKILL_LEATHERWORKING:
						player->learnSpell(51302, false);
						creature->MonsterWhisper("To enhance your leggings or bracers, see an Enchanter.", player->GetGUID());
						break;
                    case SKILL_TAILORING:
						player->learnSpell(51309, false);
						creature->MonsterWhisper("To enhance your cloak, see an Enchanter.", player->GetGUID());
						break;
				}
				SkillLineEntry const *skill_info = sSkillLineStore.LookupEntry(skill);
				uint16 max_level = player->GetPureMaxSkillValue(skill_info->id);
				player->SetSkill(skill_info->id, player->GetSkillStep(skill_info->id), max_level, max_level);
			}
		}

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            LearnProfession(player, creature, (SkillType)action);
			player->CLOSE_GOSSIP_MENU();
            return true;
        }
};

void AddSC_profession_vendor()
{
    new profession_vendor();
}
