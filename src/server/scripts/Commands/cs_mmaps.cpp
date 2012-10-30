/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: mmaps_commandscript
%Complete: 20
Comment: All mmaps related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "PointMovementGenerator.h"
//#include "PathFinder.h"
#include "MMapFactory.h"
#include "Map.h"
#include "TargetedMovementGenerator.h"


class mmaps_commandscript : public CommandScript
{
public:
    mmaps_commandscript() : CommandScript("mmaps_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand mmapCommandTable[] =
        {
            { "path",           SEC_GAMEMASTER,     false, &HandleMmapPathCommand,            "", NULL },
/*          { "loc",            SEC_GAMEMASTER,     false, &HandleMmapLocCommand,             "", NULL },
            { "loadedtiles",    SEC_GAMEMASTER,     false, &HandleMmapLoadedTilesCommand,     "", NULL },
            { "stats",          SEC_GAMEMASTER,     false, &HandleMmapStatsCommand,           "", NULL },
            { "testarea",       SEC_GAMEMASTER,     false, &HandleMmapTestArea,               "", NULL },*/
            { NULL,             0,                  false, NULL,                              "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "mmap",           SEC_GAMEMASTER,         true,  NULL,     "", mmapCommandTable  },
            { NULL,             SEC_GAMEMASTER,         false, NULL,                     "", NULL }
        };
        return commandTable;
    }

    static bool HandleMmapPathCommand(ChatHandler* handler, char const* args)
    {
       if (!MMAP::MMapFactory::createOrGetMMapManager()->GetNavMesh(handler->GetSession()->GetPlayer()->GetMapId()))
       {
           handler->PSendSysMessage("NavMesh not loaded for current map.");
           return true;
       }

       handler->PSendSysMessage("mmap path:");

       // units
       Player* player = handler->GetSession()->GetPlayer();
       Unit* target = handler->getSelectedUnit();
       if (!player || !target)
       {
           handler->PSendSysMessage("Invalid target/source selection.");
           return true;
       }

       char* para = strtok((char*)args, " ");

       bool useStraightPath = false;
       if (para && strcmp(para, "true") == 0)
           useStraightPath = true;

       // unit locations
       float x, y, z;
       player->GetPosition(x, y, z);

       // path
       PathFinderMovementGenerator path(target);
       path.SetUseStrightPath(useStraightPath);
       path.Calculate(x, y, z);

       PointsArray pointPath = path.GetPath();
       handler->PSendSysMessage("%s's path to %s:", target->GetName(), player->GetName());
       handler->PSendSysMessage("Building %s", useStraightPath ? "StraightPath" : "SmoothPath");
       handler->PSendSysMessage("length %i type %u", pointPath.size(), path.GetPathType());

       Vector3 start = path.GetStartPosition();
       Vector3 end = path.GetEndPosition();
       Vector3 actualEnd = path.GetActualEndPosition();

       handler->PSendSysMessage("start      (%.3f, %.3f, %.3f)", start.x, start.y, start.z);
       handler->PSendSysMessage("end        (%.3f, %.3f, %.3f)", end.x, end.y, end.z);
       handler->PSendSysMessage("actual end (%.3f, %.3f, %.3f)", actualEnd.x, actualEnd.y, actualEnd.z);

       if (!player->isGameMaster())
          handler->PSendSysMessage("Enable GM mode to see the path points.");

       // this entry visible only to GM's with "gm on"
       static const uint32 WAYPOINT_NPC_ENTRY = 1;
       Creature* wp = NULL;
       for (uint32 i = 0; i < pointPath.size(); ++i)
       {
          wp = player->SummonCreature(WAYPOINT_NPC_ENTRY, pointPath[i].x, pointPath[i].y, pointPath[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 9000);
       }

       return true;
   }

};


void AddSC_mmaps_commandscript()
{
    new mmaps_commandscript();
}
