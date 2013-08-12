//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Implementation of game protocol
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////
#include "otpch.h"

#include "protocol76.h"
#include "resources.h"

#include "networkmessage.h"
#include "outputmessage.h"

#include "items.h"

#include "tile.h"
#include "player.h"
#include "chat.h"

#include "configmanager.h"
#include "actions.h"
#include "game.h"
#include "iologindata.h"
#include "house.h"
#include "waitlist.h"
#include "quests.h"
#include "ban.h"
#include "connection.h"
#include "creatureevent.h"

#include <string>
#include <iostream>
#include <sstream>
#include <time.h>
#include <list>

#include <boost/function.hpp>

extern Game g_game;
extern ConfigManager g_config;
extern Actions actions;
extern Ban g_bans;
extern CreatureEvents* g_creatureEvents;
Chat g_chat;

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
uint32_t Protocol76::protocol76Count = 0;
#endif

#ifdef __SERVER_PROTECTION__
#error "You should not use __SERVER_PROTECTION__"
#define ADD_TASK_INTERVAL 40
#define CHECK_TASK_INTERVAL 5000
#else
#define ADD_TASK_INTERVAL -1
#endif

// Helping templates to add dispatcher tasks
template<class T1, class f1, class r>
void Protocol76::addGameTask(r (Game::*f)(f1), T1 p1)
{
	if(m_now > m_nextTask || m_messageCount < 5)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else
	{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class f1, class f2, class r>
void Protocol76::addGameTask(r (Game::*f)(f1, f2), T1 p1, T2 p2)
{
	if(m_now > m_nextTask || m_messageCount < 5)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else
	{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3,
class f1, class f2, class f3,
class r>
void Protocol76::addGameTask(r (Game::*f)(f1, f2, f3), T1 p1, T2 p2, T3 p3)
{
	if(m_now > m_nextTask || m_messageCount < 5)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else
	{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4,
class f1, class f2, class f3, class f4,
class r>
void Protocol76::addGameTask(r (Game::*f)(f1, f2, f3, f4), T1 p1, T2 p2, T3 p3, T4 p4)
{
	if(m_now > m_nextTask || m_messageCount < 5)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else
	{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4, class T5,
class f1, class f2, class f3, class f4, class f5,
class r>
void Protocol76::addGameTask(r (Game::*f)(f1, f2, f3, f4, f5), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
{
	if(m_now > m_nextTask || m_messageCount < 5)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4, p5)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else
	{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4, class T5, class T6,
class f1, class f2, class f3, class f4, class f5, class f6,
class r>
void Protocol76::addGameTask(r (Game::*f)(f1, f2, f3, f4, f5, f6), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
{
	if(m_now > m_nextTask || m_messageCount < 5)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4, p5, p6)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else
	{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7,
class f1, class f2, class f3, class f4, class f5, class f6, class f7,
class r>
void Protocol76::addGameTask(r (Game::*f)(f1, f2, f3, f4, f5, f6, f7), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
{
	if(m_now > m_nextTask || m_messageCount < 5)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4, p5, p6, p7)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else
	{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
class f1, class f2, class f3, class f4, class f5, class f6, class f7, class f8,
class r>
void Protocol76::addGameTask(r (Game::*f)(f1, f2, f3, f4, f5, f6, f7, f8), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
{
	if(m_now > m_nextTask || m_messageCount < 5)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4, p5, p6, p7, p8)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else
	{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

Protocol76::Protocol76(Connection* connection) :
	Protocol(connection)
{
	player = NULL;
	m_nextTask = 0;
	m_nextPing = 0;
	m_lastTaskCheck = 0;
	m_messageCount = 0;
	m_rejectCount = 0;
	m_debugAssertSent = false;
	m_acceptPackets = false;
	eventConnect = 0;
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	protocol76Count++;
#endif
}

Protocol76::~Protocol76()
{
	player = NULL;
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	protocol76Count--;
#endif
}

void Protocol76::setPlayer(Player* p)
{
	player = p;
}

void Protocol76::releaseProtocol()
{
	//dispatcher thread
	if(player)
	{
		if(player->client == this)
			player->client = NULL;
	}

	Protocol::releaseProtocol();
}

void Protocol76::deleteProtocolTask()
{
	//dispatcher thread
	if(player)
    {
		#ifdef __DEBUG_NET_DETAIL__
		std::cout << "Deleting Protocol76 - Protocol:" << this << ", Player: " << player << std::endl;
		#endif

		g_game.FreeThing(player);
		player = NULL;
	}

	Protocol::deleteProtocolTask();
}

bool Protocol76::login(const std::string& name, uint32_t accnumber, const std::string& password, uint8_t gamemasterLogin)
{
	//dispatcher thread
	Player* _player = g_game.getPlayerByName(name);
	if(!_player || g_config.getNumber(ConfigManager::ALLOW_CLONES) || name == "Account Manager")
	{
		player = new Player(name, this);

		player->useThing2();
		player->setID();

		if(!IOLoginData::getInstance()->loadPlayer(player, name, true))
		{
#ifdef __DEBUG__
			std::cout << "Protocol76::login - loadPlayer failed - " << name << std::endl;
#endif
			disconnectClient(0x14, "Your character could not be loaded.");
			return false;
		}

		bool letNamelockedLogin = true;
		if(g_bans.isPlayerNamelocked(name) && accnumber > 1)
		{
			if(g_config.getString(ConfigManager::ACCOUNT_MANAGER) == "yes")
			{
				std::string realPassword = player->password;
				player = NULL;
				player = new Player("Account Manager", this);
				player->useThing2();
				player->setID();
				IOLoginData::getInstance()->loadPlayer(player, "Account Manager");
				player->password = realPassword;
				player->realAccount = accnumber;
				player->namelockedPlayer = name;
			}
			else
				letNamelockedLogin = false;
		}

		if(player->getName() == "Account Manager" && accnumber > 1 && !player->accountManager && g_config.getString(ConfigManager::ACCOUNT_MANAGER) == "yes")
		{
			player->accountManager = true;
			player->realAccount = accnumber;
		}

		if(gamemasterLogin == 1 && player->getAccountType() < ACCOUNT_TYPE_GAMEMASTER && player->getName() != "Account Manager")
		{
			disconnectClient(0x14, "You are not a gamemaster!");
			return false;
		}

		if(!player->hasFlag(PlayerFlag_CannotBeBanned))
		{
			uint32_t bannedBy = 0, banTime = 0;
			int32_t reason = 0, action = 0;
			std::string comment = "";
			bool deletion = false;
			if(g_bans.getBanInformation(accnumber, bannedBy, banTime, reason, action, comment, deletion))
			{
				uint64_t timeNow = time(NULL);
				if((deletion && banTime != 0) || banTime > timeNow)
				{
					std::string name_;
					if(bannedBy == 0)
						name_ = (deletion ? "Automatic deletion" : "Automatic banishment");
					else
						IOLoginData::getInstance()->getNameByGuid(bannedBy, name_);

					char date[16];
					formatDate2(banTime, date);
					char buffer[500];
					sprintf(buffer, "Your account has been %s by:\n%s, for the following reason:\n%s.\nThe action taken was:\n%s.\nThe comment given was:\n%s\nYour %s:\n%s.",
						(deletion ? "deleted" : "banished") ,name_.c_str(),
						getReason(reason).c_str(), getAction(action, false).c_str(),
						comment.c_str(),
						(deletion ? "account was deleted on" : "banishment will be lifted at"),
						date);
					disconnectClient(0x14, buffer);
					return false;
				}
			}
		}

		if(!letNamelockedLogin || (player->getName() == "Account Manager" && g_config.getString(ConfigManager::ACCOUNT_MANAGER) != "yes"))
		{
			disconnectClient(0x14, "Your character has been namelocked.");
			return false;
		}

		if(g_game.getGameState() == GAME_STATE_CLOSING && !player->hasFlag(PlayerFlag_CanAlwaysLogin))
		{
			disconnectClient(0x14, "The game is just going down.\nPlease try again later.");
			return false;
		}

		if(g_game.getGameState() == GAME_STATE_CLOSED && !player->hasFlag(PlayerFlag_CanAlwaysLogin))
		{
			disconnectClient(0x14, "Server is currently closed. Please try again later.");
			return false;
		}

		if(g_config.getString(ConfigManager::ONE_PLAYER_ON_ACCOUNT) == "yes" && player->getAccountType() < ACCOUNT_TYPE_GAMEMASTER && player->getName() != "Account Manager" && g_game.getPlayerByAccount(player->getAccount()))
		{
			disconnectClient(0x14, "You may only login with one character\nof your account at the same time.");
			return false;
		}

		if(!WaitingList::getInstance()->clientLogin(player))
		{
			int32_t currentSlot = WaitingList::getInstance()->getClientSlot(player);
			int32_t retryTime = WaitingList::getTime(currentSlot);
			std::stringstream ss;

			ss << "Too many players online.\n" << "You are at place "
				<< currentSlot << " on the waiting list.";

			OutputMessage* output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
			if(output)
            {
				TRACK_MESSAGE(output);
			    output->AddByte(0x16);
			    output->AddString(ss.str());
			    output->AddByte(retryTime);
			    OutputMessagePool::getInstance()->send(output);
			}
			getConnection()->closeConnection();
			return false;
		}

		if(!IOLoginData::getInstance()->loadPlayer(player, name))
		{
			disconnectClient(0x14, "Your character could not be loaded.");
			return false;
		}

		if(!g_game.placeCreature(player, player->getLoginPosition()))
		{
			if(!g_game.placeCreature(player, player->getTemplePosition(), true))
			{
				disconnectClient(0x14, "Temple position is wrong. Contact the administrator.");
				return false;
			}
		}

		player->lastIP = player->getIP();
		player->lastLoginSaved = std::max(time(NULL), player->lastLoginSaved + 1);
		m_acceptPackets = true;
		return true;
	}
	else
	{
		if(eventConnect != 0 || g_config.getString(ConfigManager::REPLACE_KICK_ON_LOGIN) != "yes")
		{
			//Already trying to connect
			disconnectClient(0x14, "Your already logged in.");
			return false;
		}

		if(_player->isOnline())
		{
			g_chat.removeUserFromAllChannels(_player);
			_player->disconnect();
			_player->isConnecting = true;
			addRef();
			eventConnect = Scheduler::getScheduler().addEvent(
				createSchedulerTask(1000, boost::bind(&Protocol76::connect, this, _player->getID())));

			return true;
		}

		addRef();
		return connect(_player->getID());
	}
	return false;
}

bool Protocol76::connect(uint32_t playerId)
{
	unRef();
	eventConnect = 0;
	Player* _player = g_game.getPlayerByID(playerId);
	if(!_player || _player->isRemoved() || _player->isOnline())
	{
		disconnectClient(0x14, "Your already logged in.");
		return false;
	}

	player = _player;
	player->useThing2();
	player->isConnecting = false;
	player->client = this;
	player->client->sendAddCreature(player, false);
	player->sendIcons();
	player->lastIP = player->getIP();
	player->lastLoginSaved = std::max(time(NULL), player->lastLoginSaved + 1);
	m_acceptPackets = true;
	return true;
}

bool Protocol76::logout(bool displayEffect, bool forced)
{
	//dispatcher thread
	if(!player)
		return false;

	if(!player->isRemoved())
	{
		if(forced)
			g_creatureEvents->playerLogout(player);
		else
		{
			bool flag = (player->getAccountType() == ACCOUNT_TYPE_GOD);
			if(player->getTile()->hasFlag(TILESTATE_NOLOGOUT) && !flag)
			{
				player->sendCancelMessage(RET_YOUCANNOTLOGOUTHERE);
				return false;
			}

			if(player->hasCondition(CONDITION_INFIGHT) && !flag)
			{
				player->sendCancelMessage(RET_YOUMAYNOTLOGOUTDURINGAFIGHT);
				return false;
			}

			//scripting event - onLogout
			if(!g_creatureEvents->playerLogout(player) && !flag)
			{
				//Let the script handle the error message
				return false;
			}
		}
	}

	if(player->isRemoved() || player->getHealth() <= 0)
		displayEffect = false;

	if(displayEffect)
		g_game.addMagicEffect(player->getPosition(), NM_ME_POFF);

	if(Connection* connection = getConnection())
		connection->closeConnection();

	return g_game.removeCreature(player);
}

bool Protocol76::parseFirstPacket(NetworkMessage& msg)
{
	if(g_game.getGameState() == GAME_STATE_SHUTDOWN)
	{
		getConnection()->closeConnection();
		return false;
	}

	/*uint16_t clientos =*/ msg.GetU16();
	uint16_t version = msg.GetU16();

	uint8_t isSetGM = msg.GetByte();
	uint32_t accnumber = msg.GetU32();
	const std::string name = msg.GetString();
	std::string password = msg.GetString();

	if(version < CLIENT_VERSION_MIN || version > CLIENT_VERSION_MAX)
	{
		disconnectClient(0x14, CLIENT_VERSION_STRING);
		return false;
	}

	if(!accnumber)
	{
		if(g_config.getString(ConfigManager::ACCOUNT_MANAGER) == "yes")
		{
			accnumber = 1;
			password = "1";
		}
		else
		{
			disconnectClient(0x14, "You must enter your account number.");
			return false;
		}
	}

	if(g_game.getGameState() == GAME_STATE_STARTUP || g_game.getServerSaveMessage(0))
	{
		disconnectClient(0x14, "Gameworld is starting up. Please wait.");
		return false;
	}

	if(g_bans.isIpDisabled(getIP()))
	{
		disconnectClient(0x14, "Too many connections attempts from this IP. Try again later.");
		return false;
	}

	if(g_bans.isIpBanished(getIP()))
	{
		disconnectClient(0x14, "Your IP is banished!");
		return false;
	}

	std::string acc_pass;
	if(!(IOLoginData::getInstance()->getPassword(accnumber, name, acc_pass) && passwordTest(password, acc_pass)) && name != "Account Manager")
	{
		g_bans.addLoginAttempt(getIP(), false);
		getConnection()->closeConnection();
		return false;
	}

	g_bans.addLoginAttempt(getIP(), true);

	Dispatcher::getDispatcher().addTask(
		createTask(boost::bind(&Protocol76::login, this, name, accnumber, password, isSetGM)));

	return true;
}

void Protocol76::onRecvFirstMessage(NetworkMessage& msg)
{
	parseFirstPacket(msg);
}

void Protocol76::disconnectClient(uint8_t error, const char* message)
{
	OutputMessage* output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	TRACK_MESSAGE(output);
	output->AddByte(error);
	output->AddString(message);
	OutputMessagePool::getInstance()->send(output);
	disconnect();
}

void Protocol76::disconnect()
{
	if(getConnection())
		getConnection()->closeConnection();
}

void Protocol76::parsePacket(NetworkMessage &msg)
{
	if(!m_acceptPackets || msg.getMessageLength() <= 0 || !player)
		return;

	m_now = OTSYS_TIME();
	#ifdef __SERVER_PROTECTION__
	int64_t interval = m_now - m_lastTaskCheck;
	if(interval > CHECK_TASK_INTERVAL)
	{
		interval = 0;
		m_lastTaskCheck = m_now;
		m_messageCount = 1;
		m_rejectCount = 0;
	}
	else
	{
		m_messageCount++;
		if((interval > 800 && interval / m_messageCount < 25))
			getConnection()->closeConnection();
	}
	#endif

	uint8_t recvbyte = msg.GetByte();
	//a dead player can not performs actions
	if((player->isRemoved() || player->getHealth() <= 0) && recvbyte != 0x14)
		return;

	if(player->getName() == "Account Manager")
	{
		switch(recvbyte)
		{
			case 0x14:
				parseLogout(msg);
				break;

			case 0x96:
				parseSay(msg);
				break;

			default:
				sendCancelWalk();
				break; 
		}
	}
	else
	{
		switch(recvbyte)
		{
			case 0x14: // logout
				parseLogout(msg);
				break;

			case 0x1E: // keep alive / ping response
				parseReceivePing(msg);
				break;

			case 0x64: // move with steps
				parseAutoWalk(msg);
				break;

			case 0x65: // move north
			case 0x66: // move east
			case 0x67: // move south
			case 0x68: // move west
				parseMove(msg, (Direction)(recvbyte - 0x65));
				break;

			case 0x69: // stop-autowalk
				addGameTask(&Game::playerStopAutoWalk, player->getID());
				break;

			case 0x6A:
				parseMove(msg, NORTHEAST);
				break;

			case 0x6B:
				parseMove(msg, SOUTHEAST);
				break;

			case 0x6C:
				parseMove(msg, SOUTHWEST);
				break;

			case 0x6D:
				parseMove(msg, NORTHWEST);
				break;

			case 0x6F: // turn north
			case 0x70: // turn east
			case 0x71: // turn south
			case 0x72: // turn west
				parseTurn(msg, (Direction)(recvbyte - 0x6F));
				break;

			case 0x78: // throw item
				parseThrow(msg);
				break;

			case 0x79: // description in shop window
				parseLookInShop(msg);
				break;

			case 0x7A: // player bought from shop
				parsePlayerPurchase(msg);
				break;

			case 0x7B: // player sold to shop
				parsePlayerSale(msg);
				break;

			case 0x7C: // player closed shop window
				parseCloseShop(msg);
				break;

			case 0x7D: // Request trade
				parseRequestTrade(msg);
				break;

			case 0x7E: // Look at an item in trade
				parseLookInTrade(msg);
				break;

			case 0x7F: // Accept trade
				parseAcceptTrade(msg);
				break;

			case 0x80: // close/cancel trade
				parseCloseTrade();
				break;

			case 0x82: // use item
				parseUseItem(msg);
				break;

			case 0x83: // use item
				parseUseItemEx(msg);
				break;

			case 0x84: // battle window
				parseBattleWindow(msg);
				break;

			case 0x85: //rotate item
				parseRotateItem(msg);
				break;

			case 0x87: // close container
				parseCloseContainer(msg);
				break;

			case 0x88: //"up-arrow" - container
				parseUpArrowContainer(msg);
				break;

			case 0x89:
				parseTextWindow(msg);
				break;

			case 0x8A:
				parseHouseWindow(msg);
				break;

			case 0x8C: // throw item
				parseLookAt(msg);
				break;

			case 0x96: // say something
				parseSay(msg);
				break;

			case 0x97: // request channels
				parseGetChannels(msg);
				break;

			case 0x98: // open channel
				parseOpenChannel(msg);
				break;

			case 0x99: // close channel
				parseCloseChannel(msg);
				break;

			case 0x9A: // open priv
				parseOpenPriv(msg);
				break;

			case 0x9B: //process report
				parseProcessRuleViolation(msg);
				break;

			case 0x9C: //gm closes report
				parseCloseRuleViolation(msg);
				break;

			case 0x9D: //player cancels report
				parseCancelRuleViolation(msg);
				break;

			case 0x9E: // close NPC
				parseCloseNpc(msg);
				break;

			case 0xA0: // set attack and follow mode
				parseFightModes(msg);
				break;

			case 0xA1: // attack
				parseAttack(msg);
				break;

			case 0xA2: //follow
				parseFollow(msg);
				break;

			case 0xA3: // invite party
				parseInviteToParty(msg);
				break;

			case 0xA4: // join party
				parseJoinParty(msg);
				break;

			case 0xA5: // revoke party
				parseRevokePartyInvite(msg);
				break;

			case 0xA6: // pass leadership
				parsePassPartyLeadership(msg);
				break;

			case 0xA7: // leave party
				parseLeaveParty(msg);
				break;

			case 0xA8: // share exp
				parseEnableSharedPartyExperience(msg);
				break;

			case 0xAA:
				parseCreatePrivateChannel(msg);
				break;

			case 0xAB:
				parseChannelInvite(msg);
				break;

			case 0xAC:
				parseChannelExclude(msg);
				break;

			case 0xBE: // cancel move
				parseCancelMove(msg);
				break;

			case 0xC9: //client request to resend the tile
				parseUpdateTile(msg);
				break;

			case 0xCA: //client request to resend the container (happens when you store more than container maxsize)
				parseUpdateContainer(msg);
				break;

			case 0xD2: // request outfit
				if(g_config.getString(ConfigManager::ALLOW_CHANGEOUTFIT) == "yes")
					parseRequestOutfit(msg);
				break;

			case 0xD3: // set outfit
				if(g_config.getString(ConfigManager::ALLOW_CHANGEOUTFIT) == "yes")
					parseSetOutfit(msg);
				break;

			case 0xDC:
				parseAddVip(msg);
				break;

			case 0xDD:
				parseRemoveVip(msg);
				break;

			case 0xE6:
				parseBugReport(msg);
				break;

			case 0xE7:
				parseViolationWindow(msg);
				break;

			case 0xE8:
				parseDebugAssert(msg);
				break;

			case 0xF0:
				parseQuestLog(msg);
				break;

			case 0xF1:
				parseQuestLine(msg);
				break;

			default:
				std::cout << "Unknown byte received: 0x" << std::hex << (int16_t)recvbyte << std::dec << ", from player: " << player->getName() << "." << std::endl;
				break;
		}
	}
}

void Protocol76::GetTileDescription(const Tile* tile, NetworkMessage* msg)
{
	if(tile)
	{
		int32_t count = 0;
		if(tile->ground)
		{
			msg->AddItem(tile->ground);
			count++;
		}

		ItemVector::const_iterator it;
		for(it = tile->topItems.begin(); ((it != tile->topItems.end()) && (count < 10)); ++it)
		{
			msg->AddItem(*it);
			count++;
		}

		CreatureVector::const_iterator itc;
		for(itc = tile->creatures.begin(); ((itc != tile->creatures.end()) && (count < 10)); ++itc)
		{
			if((*itc)->isInGhostMode() && !player->isAccessPlayer())
				continue;

			bool known;
			uint32_t removedKnown;
			checkCreatureAsKnown((*itc)->getID(), known, removedKnown);
			AddCreature(msg,*itc, known, removedKnown);
			count++;
		}

		for(it = tile->downItems.begin(); ((it != tile->downItems.end()) && (count < 10)); ++it)
		{
			msg->AddItem(*it);
			count++;
		}
	}
}

void Protocol76::GetMapDescription(uint16_t x, uint16_t y, unsigned char z,
	uint16_t width, uint16_t height, NetworkMessage* msg)
{
	int32_t skip = -1;
	int32_t startz, endz, zstep = 0;
	if(z > 7)
	{
		startz = z - 2;
		endz = std::min(MAP_MAX_LAYERS - 1, z + 2);
		zstep = 1;
	}
	else
	{
		startz = 7;
		endz = 0;
		zstep = -1;
	}

	for(int32_t nz = startz; nz != endz + zstep; nz += zstep)
		GetFloorDescription(msg, x, y, nz, width, height, z - nz, skip);

	if(skip >= 0)
	{
		msg->AddByte(skip);
		msg->AddByte(0xFF);
		//cc += skip;
	}

	#ifdef __DEBUG__
	//printf("tiles in total: %d \n", cc);
	#endif
}

void Protocol76::GetFloorDescription(NetworkMessage* msg, int32_t x, int32_t y, int32_t z, int32_t width, int32_t height, int32_t offset, int& skip)
{
	Tile* tile;
	for(int32_t nx = 0; nx < width; nx++)
	{
		for(int32_t ny = 0; ny < height; ny++)
		{
			tile = g_game.getTile(x + nx + offset, y + ny + offset, z);
			if(tile)
			{
				if(skip >= 0)
				{
					msg->AddByte(skip);
					msg->AddByte(0xFF);
				}
				skip = 0;
				GetTileDescription(tile, msg);
			}
			else
			{
				skip++;
				if(skip == 0xFF)
				{
					msg->AddByte(0xFF);
					msg->AddByte(0xFF);
					skip = -1;
				}
			}
		}
	}
}

void Protocol76::checkCreatureAsKnown(uint32_t id, bool &known, uint32_t &removedKnown)
{
	// loop through the known creature list and check if the given creature is in
	std::list<uint32_t>::iterator i;
	for(i = knownCreatureList.begin(); i != knownCreatureList.end(); ++i)
	{
		if((*i) == id)
		{
			// know... make the creature even more known...
			knownCreatureList.erase(i);
			knownCreatureList.push_back(id);

			known = true;
			return;
		}
	}

	// ok, he is unknown...
	known = false;

	// ... but not in future
	knownCreatureList.push_back(id);

	// too many known creatures?
	if(knownCreatureList.size() > 150)
	{
		// lets try to remove one from the end of the list
		for(int32_t n = 0; n < 150; n++)
		{
			removedKnown = knownCreatureList.front();

			Creature* c = g_game.getCreatureByID(removedKnown);
			if((!c) || (!canSee(c)))
				break;

			// this creature we can't remove, still in sight, so back to the end
			knownCreatureList.pop_front();
			knownCreatureList.push_back(removedKnown);
		}

		// hopefully we found someone to remove :S, we got only 150 tries
		// if not... lets kick some players with debug errors :)
		knownCreatureList.pop_front();
	}
	else
	{
		// we can cache without problems :)
		removedKnown = 0;
	}
}

bool Protocol76::canSee(const Creature* c) const
{
	if(c->isRemoved())
		return false;

	if(c->isInGhostMode() && !player->isAccessPlayer())
		return false;

	return canSee(c->getPosition());
}

bool Protocol76::canSee(const Position& pos) const
{
	return canSee(pos.x, pos.y, pos.z);
}

bool Protocol76::canSee(int32_t x, int32_t y, int32_t z) const
{
#ifdef __DEBUG__
	if(z < 0 || z >= MAP_MAX_LAYERS)
		std::cout << "WARNING! Protocol76::canSee() Z-value is out of range!" << std::endl;
#endif

	const Position& myPos = player->getPosition();
	if(myPos.z <= 7)
	{
		//we are on ground level or above (7 -> 0)
		//view is from 7 -> 0
		if(z > 7)
			return false;
	}
	else if(myPos.z >= 8)
	{
		//we are underground (8 -> 15)
		//view is +/- 2 from the floor we stand on
		if(std::abs(myPos.z - z) > 2)
			return false;
	}

	//negative offset means that the action taken place is on a lower floor than ourself
	int32_t offsetz = myPos.z - z;

	if ((x >= myPos.x - 8 + offsetz) && (x <= myPos.x + 9 + offsetz) &&
		(y >= myPos.y - 6 + offsetz) && (y <= myPos.y + 7 + offsetz))
		return true;

	return false;
}

//********************** Parse methods *******************************//
void Protocol76::parseLogout(NetworkMessage& msg)
{
	Dispatcher::getDispatcher().addTask(
		createTask(boost::bind(&Protocol76::logout, this, true, false)));
}

void Protocol76::parseCreatePrivateChannel(NetworkMessage& msg)
{
	addGameTask(&Game::playerCreatePrivateChannel, player->getID());
}

void Protocol76::parseChannelInvite(NetworkMessage& msg)
{
	const std::string name = msg.GetString();
	addGameTask(&Game::playerChannelInvite, player->getID(), name);
}

void Protocol76::parseChannelExclude(NetworkMessage& msg)
{
	const std::string name = msg.GetString();
	addGameTask(&Game::playerChannelExclude, player->getID(), name);
}

void Protocol76::parseGetChannels(NetworkMessage& msg)
{
	addGameTask(&Game::playerRequestChannels, player->getID());
}

void Protocol76::parseOpenChannel(NetworkMessage& msg)
{
	unsigned short channelId = msg.GetU16();
	addGameTask(&Game::playerOpenChannel, player->getID(), channelId);
}

void Protocol76::parseCloseChannel(NetworkMessage& msg)
{
	unsigned short channelId = msg.GetU16();
	addGameTask(&Game::playerCloseChannel, player->getID(), channelId);
}

void Protocol76::parseOpenPriv(NetworkMessage& msg)
{
	const std::string receiver = msg.GetString();
	addGameTask(&Game::playerOpenPrivateChannel, player->getID(), receiver);
}

void Protocol76::parseProcessRuleViolation(NetworkMessage& msg)
{
	const std::string reporter = msg.GetString();
	addGameTask(&Game::playerProcessRuleViolation, player->getID(), reporter);
}

void Protocol76::parseCloseRuleViolation(NetworkMessage& msg)
{
	const std::string reporter = msg.GetString();
	addGameTask(&Game::playerCloseRuleViolation, player->getID(), reporter);
}

void Protocol76::parseCancelRuleViolation(NetworkMessage& msg)
{
	addGameTask(&Game::playerCancelRuleViolation, player->getID());
}

void Protocol76::parseCloseNpc(NetworkMessage& msg)
{
	addGameTask(&Game::playerCloseNpcChannel, player->getID());
}

void Protocol76::parseCancelMove(NetworkMessage& msg)
{
	addGameTask(&Game::playerCancelAttackAndFollow, player->getID());
}

void Protocol76::parseReceivePing(NetworkMessage& msg)
{
	if(m_now > m_nextPing)
	{
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(&Game::playerReceivePing, &g_game, player->getID())));
		m_nextPing = m_now + 2000;
	}
}

void Protocol76::parseAutoWalk(NetworkMessage& msg)
{
	// first we get all directions...
	std::list<Direction> path;
	size_t numdirs = msg.GetByte();
	for(size_t i = 0; i < numdirs; ++i)
	{
		uint8_t rawdir = msg.GetByte();
		Direction dir = SOUTH;
		switch(rawdir)
		{
			case 1:
				dir = EAST;
				break;
			case 2:
				dir = NORTHEAST;
				break;
			case 3:
				dir = NORTH;
				break;
			case 4:
				dir = NORTHWEST;
				break;
			case 5:
				dir = WEST;
				break;
			case 6:
				dir = SOUTHWEST;
				break;
			case 7:
				dir = SOUTH;
				break;
			case 8:
				dir = SOUTHEAST;
				break;
			default:
				continue;
		}
		path.push_back(dir);
	}
	addGameTask(&Game::playerAutoWalk, player->getID(), path);
}

void Protocol76::parseMove(NetworkMessage& msg, Direction dir)
{
	addGameTask(&Game::playerMove, player->getID(), dir);
}

void Protocol76::parseTurn(NetworkMessage& msg, Direction dir)
{
	addGameTask(&Game::playerTurn, player->getID(), dir);
}

void Protocol76::parseRequestOutfit(NetworkMessage& msg)
{
	addGameTask(&Game::playerRequestOutfit, player->getID());
}

void Protocol76::parseSetOutfit(NetworkMessage& msg)
{
	uint16_t looktype = msg.GetU16();
	uint8_t lookhead = msg.GetByte();
	uint8_t lookbody = msg.GetByte();
	uint8_t looklegs = msg.GetByte();
	uint8_t lookfeet = msg.GetByte();
	uint8_t lookaddons = msg.GetByte();

	Outfit_t newOutfit;
	newOutfit.lookType = looktype;
	newOutfit.lookHead = lookhead;
	newOutfit.lookBody = lookbody;
	newOutfit.lookLegs = looklegs;
	newOutfit.lookFeet = lookfeet;
	newOutfit.lookAddons = lookaddons;
	addGameTask(&Game::playerChangeOutfit, player->getID(), newOutfit);
}

void Protocol76::parseUseItem(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t stackpos = msg.GetByte();
	uint8_t index = msg.GetByte();
	bool isHotkey = (pos.x == 0xFFFF && pos.y == 0 && pos.z == 0);
	addGameTask(&Game::playerUseItem, player->getID(), pos, stackpos, index, spriteId, isHotkey);
}

void Protocol76::parseUseItemEx(NetworkMessage& msg)
{
	Position fromPos = msg.GetPosition();
	uint16_t fromSpriteId = msg.GetSpriteId();
	uint8_t fromStackPos = msg.GetByte();
	Position toPos = msg.GetPosition();
	uint16_t toSpriteId = msg.GetU16();
	uint8_t toStackPos = msg.GetByte();
	bool isHotkey = (fromPos.x == 0xFFFF && fromPos.y == 0 && fromPos.z == 0);
	addGameTask(&Game::playerUseItemEx, player->getID(), fromPos, fromStackPos, fromSpriteId, toPos, toStackPos, toSpriteId, isHotkey);
}

void Protocol76::parseBattleWindow(NetworkMessage& msg)
{
	Position fromPos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t fromStackPos = msg.GetByte();
	uint32_t creatureId = msg.GetU32();
	bool isHotkey = (fromPos.x == 0xFFFF && fromPos.y == 0 && fromPos.z == 0);
	addGameTask(&Game::playerUseBattleWindow, player->getID(), fromPos, fromStackPos, creatureId, spriteId, isHotkey);
}

void Protocol76::parseCloseContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.GetByte();
	addGameTask(&Game::playerCloseContainer, player->getID(), cid);
}

void Protocol76::parseUpArrowContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.GetByte();
	addGameTask(&Game::playerMoveUpContainer, player->getID(), cid);
}

void Protocol76::parseUpdateTile(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	//addGameTask(&Game::playerUpdateTile, player->getID(), pos);
}

void Protocol76::parseUpdateContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.GetByte();
	addGameTask(&Game::playerUpdateContainer, player->getID(), cid);
}

void Protocol76::parseThrow(NetworkMessage& msg)
{
	Position fromPos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t fromStackpos = msg.GetByte();
	Position toPos = msg.GetPosition();
	uint8_t count = msg.GetByte();
	if(toPos != fromPos)
		addGameTask(&Game::playerMoveThing, player->getID(), fromPos, spriteId,
			fromStackpos, toPos, count);
}

void Protocol76::parseLookAt(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t stackpos = msg.GetByte();
	addGameTask(&Game::playerLookAt, player->getID(), pos, spriteId, stackpos);
}

void Protocol76::parseSay(NetworkMessage& msg)
{
	SpeakClasses type = (SpeakClasses)msg.GetByte();
	std::string receiver;
	uint16_t channelId = 0;
	switch(type)
	{
		case SPEAK_PRIVATE:
		case SPEAK_PRIVATE_RED:
		case SPEAK_RVR_ANSWER:
			receiver = msg.GetString();
			break;

		case SPEAK_CHANNEL_Y:
		case SPEAK_CHANNEL_R1:
		case SPEAK_CHANNEL_R2:
			channelId = msg.GetU16();
			break;

		default:
			break;
	}

	const std::string text = msg.GetString();
	addGameTask(&Game::playerSay, player->getID(), channelId, type, receiver, text);
}

void Protocol76::parseFightModes(NetworkMessage& msg)
{
	uint8_t rawFightMode = msg.GetByte(); //1 - offensive, 2 - balanced, 3 - defensive
	uint8_t rawChaseMode = msg.GetByte(); // 0 - stand while fightning, 1 - chase opponent
	uint8_t rawSecureMode = msg.GetByte(); // 0 - can't attack unmarked, 1 - can attack unmarked

	chaseMode_t chaseMode = CHASEMODE_STANDSTILL;
	if(rawChaseMode == 1)
		chaseMode = CHASEMODE_FOLLOW;

	fightMode_t fightMode = FIGHTMODE_ATTACK;
	if(rawFightMode == 2)
		fightMode = FIGHTMODE_BALANCED;
	else if(rawFightMode == 3)
		fightMode = FIGHTMODE_DEFENSE;

	secureMode_t secureMode = SECUREMODE_OFF;
	if(rawSecureMode == 1)
		secureMode = SECUREMODE_ON;

	addGameTask(&Game::playerSetFightModes, player->getID(), fightMode, chaseMode, secureMode);
}

void Protocol76::parseAttack(NetworkMessage& msg)
{
	uint32_t creatureId = msg.GetU32();
	addGameTask(&Game::playerSetAttackedCreature, player->getID(), creatureId);
}

void Protocol76::parseFollow(NetworkMessage& msg)
{
	uint32_t creatureId = msg.GetU32();
	addGameTask(&Game::playerFollowCreature, player->getID(), creatureId);
}

void Protocol76::parseTextWindow(NetworkMessage& msg)
{
	uint32_t windowTextId = msg.GetU32();
	const std::string newText = msg.GetString();
	addGameTask(&Game::playerWriteItem, player->getID(), windowTextId, newText);
}

void Protocol76::parseHouseWindow(NetworkMessage &msg)
{
	uint8_t doorId = msg.GetByte();
	uint32_t id = msg.GetU32();
	const std::string text = msg.GetString();
	addGameTask(&Game::playerUpdateHouseWindow, player->getID(), doorId, id, text);
}

void Protocol76::parseLookInShop(NetworkMessage &msg)
{
	uint16_t id = msg.GetU16();
	uint16_t count = msg.GetByte();
	addGameTask(&Game::playerLookInShop, player->getID(), id, count);
}

void Protocol76::parsePlayerPurchase(NetworkMessage &msg)
{
	uint16_t id = msg.GetU16();
	uint16_t count = msg.GetByte();
	uint16_t amount = msg.GetByte();
	addGameTask(&Game::playerPurchaseItem, player->getID(), id, count, amount);
}

void Protocol76::parsePlayerSale(NetworkMessage &msg)
{
	uint16_t id = msg.GetU16();
	uint16_t count = msg.GetByte();
	uint16_t amount = msg.GetByte();
	addGameTask(&Game::playerSellItem, player->getID(), id, count, amount);
}

void Protocol76::parseCloseShop(NetworkMessage &msg)
{
	addGameTask(&Game::playerCloseShop, player->getID());
}

void Protocol76::parseRequestTrade(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t stackpos = msg.GetByte();
	uint32_t playerId = msg.GetU32();
	addGameTask(&Game::playerRequestTrade, player->getID(), pos, stackpos, playerId, spriteId);
}

void Protocol76::parseAcceptTrade(NetworkMessage& msg)
{
	addGameTask(&Game::playerAcceptTrade, player->getID());
}

void Protocol76::parseLookInTrade(NetworkMessage& msg)
{
	bool counterOffer = (msg.GetByte() == 0x01);
	int32_t index = msg.GetByte();
	addGameTask(&Game::playerLookInTrade, player->getID(), counterOffer, index);
}

void Protocol76::parseCloseTrade()
{
	addGameTask(&Game::playerCloseTrade, player->getID());
}

void Protocol76::parseAddVip(NetworkMessage& msg)
{
	const std::string name = msg.GetString();
	if(name.size() > 32)
		return;

	addGameTask(&Game::playerRequestAddVip, player->getID(), name);
}

void Protocol76::parseRemoveVip(NetworkMessage& msg)
{
	uint32_t guid = msg.GetU32();
	addGameTask(&Game::playerRequestRemoveVip, player->getID(), guid);
}

void Protocol76::parseRotateItem(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t stackpos = msg.GetByte();
	addGameTask(&Game::playerRotateItem, player->getID(), pos, stackpos, spriteId);
}

void Protocol76::parseDebugAssert(NetworkMessage& msg)
{
	if(m_debugAssertSent)
		return;
	m_debugAssertSent = true;

	std::string assertLine = msg.GetString();
	std::string date = msg.GetString();
	std::string description = msg.GetString();
	std::string comment = msg.GetString();

	FILE* file = fopen("client_assertions.txt", "a");
	if(file)
	{
		char bufferDate[32], bufferIp[32];
		uint64_t tmp = time(NULL);
		formatIP(getIP(), bufferIp);
		formatDate(tmp, bufferDate);
		fprintf(file, "----- %s - %s (%s) -----\n", bufferDate, player->getName().c_str(), bufferIp);
		fprintf(file, "%s\n%s\n%s\n%s\n", assertLine.c_str(), date.c_str(), description.c_str(), comment.c_str());
		fclose(file);
	}
}

void Protocol76::parseBugReport(NetworkMessage& msg)
{
	std::string bug = msg.GetString();
	addGameTask(&Game::playerReportBug, player->getID(), bug);
}

void Protocol76::parseInviteToParty(NetworkMessage& msg)
{
	uint32_t targetId = msg.GetU32();
	addGameTask(&Game::playerInviteToParty, player->getID(), targetId);
}

void Protocol76::parseJoinParty(NetworkMessage& msg)
{
	uint32_t targetId = msg.GetU32();
	addGameTask(&Game::playerJoinParty, player->getID(), targetId);
}

void Protocol76::parseRevokePartyInvite(NetworkMessage& msg)
{
	uint32_t targetId = msg.GetU32();
	addGameTask(&Game::playerRevokePartyInvitation, player->getID(), targetId);
}

void Protocol76::parsePassPartyLeadership(NetworkMessage& msg)
{
	uint32_t targetId = msg.GetU32();
	addGameTask(&Game::playerPassPartyLeadership, player->getID(), targetId);
}

void Protocol76::parseLeaveParty(NetworkMessage& msg)
{
	addGameTask(&Game::playerLeaveParty, player->getID());
}

void Protocol76::parseEnableSharedPartyExperience(NetworkMessage& msg)
{
	uint8_t sharedExpActive = msg.GetByte();
	uint8_t unknown = msg.GetByte();
	addGameTask(&Game::playerEnableSharedPartyExperience, player->getID(), sharedExpActive, unknown);
}

void Protocol76::parseQuestLog(NetworkMessage& msg)
{
	NetworkMessage* _msg = getOutputBuffer();
	if(_msg)
	{
		TRACK_MESSAGE(_msg);
		Quests::getInstance()->getQuestsList(player, _msg);
	}
}

void Protocol76::parseQuestLine(NetworkMessage& msg)
{
	uint16_t quid = msg.GetU16();
	NetworkMessage* _msg = getOutputBuffer();
	if(_msg)
	{
		TRACK_MESSAGE(_msg);
		Quest* quest = Quests::getInstance()->getQuestByID(quid);
		if(quest)
			quest->getMissionList(player, _msg);
	}
}

void Protocol76::parseViolationWindow(NetworkMessage& msg)
{
	std::string targetPlayerName = msg.GetString();
	uint8_t reasonId = msg.GetByte();
	uint8_t actionId = msg.GetByte();
	std::string comment = msg.GetString();
	std::string statement = msg.GetString();
	msg.GetU16();
	bool ipBanishment = msg.GetByte();
	addGameTask(&Game::violationWindow, player->getID(), targetPlayerName, reasonId, actionId, comment, statement, ipBanishment);
}

//********************** Send methods *******************************//
void Protocol76::sendOpenPrivateChannel(const std::string& receiver)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xAD);
		msg->AddString(receiver);
	}
}

void Protocol76::sendCreatureOutfit(const Creature* creature, const Outfit_t& outfit)
{
	if(canSee(creature))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			msg->AddByte(0x8E);
			msg->AddU32(creature->getID());
			if(creature->isInGhostMode())
				AddCreatureInvisible(msg, creature);
			else
				AddCreatureOutfit(msg, creature, outfit);
		}
	}
}

void Protocol76::sendCreatureInvisible(const Creature* creature)
{
	if(canSee(creature))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			msg->AddByte(0x8E);
			msg->AddU32(creature->getID());
			AddCreatureInvisible(msg, creature);
		}
	}
}

void Protocol76::sendCreatureLight(const Creature* creature)
{
	if(canSee(creature))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			AddCreatureLight(msg, creature);
		}
	}
}

void Protocol76::sendWorldLight(const LightInfo& lightInfo)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddWorldLight(msg, lightInfo);
	}
}

void Protocol76::sendCreatureShield(const Creature* creature)
{
	if(canSee(creature))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			msg->AddByte(0x91);
			msg->AddU32(creature->getID());
			msg->AddByte(player->getPartyShield(creature->getPlayer()));
		}
	}
}

void Protocol76::sendCreatureSkull(const Creature* creature)
{
	if(g_game.getWorldType() == WORLD_TYPE_PVP)
	{
		if(canSee(creature))
		{
			NetworkMessage* msg = getOutputBuffer();
			if(msg)
			{
				TRACK_MESSAGE(msg);
				msg->AddByte(0x90);
				msg->AddU32(creature->getID());
				msg->AddByte(player->getSkullClient(creature->getPlayer()));
			}
		}
	}
}

void Protocol76::sendCreatureSquare(const Creature* creature, SquareColor_t color)
{
	if(canSee(creature))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			msg->AddByte(0x86);
			msg->AddU32(creature->getID());
			msg->AddByte((uint8_t)color);
		}
	}
}

void Protocol76::sendTutorial(uint8_t tutorialId)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xDC);
		msg->AddByte(tutorialId);
	}
}

void Protocol76::sendAddMarker(const Position& pos, uint8_t markType, const std::string& desc)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xDD);
		msg->AddPosition(pos);
		msg->AddByte(markType);
		msg->AddString(desc);
	}
}

void Protocol76::sendReLoginWindow()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x28);
	}
}

void Protocol76::sendStats()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddPlayerStats(msg);
	}
}

void Protocol76::sendTextMessage(MessageClasses mclass, const std::string& message)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddTextMessage(msg, mclass, message);
	}
}

void Protocol76::sendClosePrivate(uint16_t channelId)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		if(channelId == 0x00)
			g_chat.removeUserFromChannel(player, channelId);
		msg->AddByte(0xB3);
		msg->AddU16(channelId);
	}
}

void Protocol76::sendCreatePrivateChannel(uint16_t channelId, const std::string& channelName)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xB2);
		msg->AddU16(channelId);
		msg->AddString(channelName);
	}
}

void Protocol76::sendChannelsDialog()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		ChannelList list;
		list = g_chat.getChannelList(player);
		msg->AddByte(0xAB);
		msg->AddByte(list.size()); //how many
		while(list.size())
		{
			ChatChannel* channel;
			channel = list.front();
			list.pop_front();
			msg->AddU16(channel->getId());
			msg->AddString(channel->getName());
		}
	}
}

void Protocol76::sendChannel(uint16_t channelId, const std::string& channelName)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xAC);
		msg->AddU16(channelId);
		msg->AddString(channelName);
	}
}

void Protocol76::sendRuleViolationsChannel(uint16_t channelId)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xAE);
		msg->AddU16(channelId);
		RuleViolationsMap::const_iterator it = g_game.getRuleViolations().begin();
		for( ; it != g_game.getRuleViolations().end(); ++it)
		{
			RuleViolation& rvr = *it->second;
			if(rvr.isOpen && rvr.reporter)
				AddCreatureSpeak(msg, rvr.reporter, SPEAK_RVR_CHANNEL, rvr.text, channelId, rvr.time);
		}
	}
}

void Protocol76::sendRemoveReport(const std::string& name)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xAF);
		msg->AddString(name);
	}
}

void Protocol76::sendRuleViolationCancel(const std::string& name)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xB0);
		msg->AddString(name);
	}
}

void Protocol76::sendLockRuleViolation()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xB1);
	}
}

void Protocol76::sendIcons(int32_t icons)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xA2);
		msg->AddU16(icons);
	}
}

void Protocol76::sendContainer(uint32_t cid, const Container* container, bool hasParent)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x6E);
		msg->AddByte(cid);

		msg->AddItemId(container);
		msg->AddString(container->getName());
		msg->AddByte(container->capacity());
		msg->AddByte(hasParent ? 0x01 : 0x00);
		if(container->size() > 255)
			msg->AddByte(255);
		else
			msg->AddByte(container->size());

		ItemList::const_iterator cit;
		uint32_t i = 0;
		for(cit = container->getItems(); cit != container->getEnd() && i < 255; ++cit, ++i)
			msg->AddItem(*cit);
	}
}

void Protocol76::sendShop(const std::list<ShopInfo>& shop)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x7A);
		if(shop.size() > 255)
			msg->AddByte(255);
		else
			msg->AddByte(shop.size());

		std::list<ShopInfo>::const_iterator it;
		uint32_t i = 0;
		for(it = shop.begin(); it != shop.end() && i < 255; ++it, ++i)
			AddShopItem(msg, (*it));
	}
}

void Protocol76::sendCloseShop()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x7C);
	}
}

void Protocol76::sendPlayerCash(uint32_t amount)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x7B);
		msg->AddU32(amount);
	}
}

void Protocol76::sendTradeItemRequest(const Player* player, const Item* item, bool ack)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		if(ack)
			msg->AddByte(0x7D);
		else
			msg->AddByte(0x7E);

		msg->AddString(player->getName());

		if(const Container* tradeContainer = item->getContainer())
		{
			std::list<const Container*> listContainer;
			ItemList::const_iterator it;
			Container* tmpContainer = NULL;

			listContainer.push_back(tradeContainer);

			std::list<const Item*> listItem;
			listItem.push_back(tradeContainer);
			while(listContainer.size() > 0)
			{
				const Container* container = listContainer.front();
				listContainer.pop_front();
				for(it = container->getItems(); it != container->getEnd(); ++it)
				{
					if((tmpContainer = (*it)->getContainer()))
						listContainer.push_back(tmpContainer);
					listItem.push_back(*it);
				}
			}

			msg->AddByte(listItem.size());
			while(listItem.size() > 0)
			{
				const Item* item = listItem.front();
				listItem.pop_front();
				msg->AddItem(item);
			}
		}
		else
		{
			msg->AddByte(1);
			msg->AddItem(item);
		}
	}
}

void Protocol76::sendCloseTrade()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x7F);
	}
}

void Protocol76::sendCloseContainer(uint32_t cid)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x6F);
		msg->AddByte(cid);
	}
}

void Protocol76::sendCreatureTurn(const Creature* creature, uint8_t stackPos)
{
	if(stackPos < 10)
	{
		if(canSee(creature))
		{
			NetworkMessage* msg = getOutputBuffer();
			if(msg)
			{
				TRACK_MESSAGE(msg);
				msg->AddByte(0x6B);
				msg->AddPosition(creature->getPosition());
				msg->AddByte(stackPos);
				msg->AddU16(0x63); /*99*/
				msg->AddU32(creature->getID());
				msg->AddByte(creature->getDirection());
			}
		}
	}
}

void Protocol76::sendCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddCreatureSpeak(msg, creature, type, text, 0);
	}
}

void Protocol76::sendToChannel(const Creature* creature, SpeakClasses type, const std::string& text, uint16_t channelId, uint32_t time /*= 0*/)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddCreatureSpeak(msg, creature, type, text, channelId, time);
	}
}

void Protocol76::sendCancel(const std::string& message)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddTextMessage(msg, MSG_STATUS_SMALL, message);
	}
}

void Protocol76::sendCancelTarget()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xA3);
	}
}

void Protocol76::sendChangeSpeed(const Creature* creature, uint32_t speed)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x8F);
		msg->AddU32(creature->getID());
		msg->AddU16(speed);
	}
}

void Protocol76::sendCancelWalk()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xB5);
		msg->AddByte(player->getDirection());
	}
}

void Protocol76::sendSkills()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddPlayerSkills(msg);
	}
}

void Protocol76::sendPing()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x1E);
	}
}

void Protocol76::sendDistanceShoot(const Position& from, const Position& to, uint8_t type)
{
	if((canSee(from) || canSee(to)) && type <= 41)
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			AddDistanceShoot(msg, from, to, type);
		}
	}
}

void Protocol76::sendMagicEffect(const Position& pos, uint8_t type)
{
	if(canSee(pos) && type <= 56)
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			AddMagicEffect(msg, pos, type);
		}
	}
}

void Protocol76::sendAnimatedText(const Position& pos, uint8_t color, std::string text)
{
	if(canSee(pos))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			AddAnimatedText(msg, pos, color, text);
		}
	}
}

void Protocol76::sendCreatureHealth(const Creature* creature)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddCreatureHealth(msg, creature);
	}
}

void Protocol76::sendFYIBox(const std::string& message)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x15);
		msg->AddString(message);
	}
}

//tile
void Protocol76::sendAddTileItem(const Tile* tile, const Position& pos, const Item* item)
{
	if(canSee(pos))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			AddTileItem(msg, pos, item);
		}
	}
}

void Protocol76::sendUpdateTileItem(const Tile* tile, const Position& pos, uint32_t stackpos, const Item* item)
{
	if(canSee(pos))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			UpdateTileItem(msg, pos, stackpos, item);
		}
	}
}

void Protocol76::sendRemoveTileItem(const Tile* tile, const Position& pos, uint32_t stackpos)
{
	if(canSee(pos))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			RemoveTileItem(msg, pos, stackpos);
		}
	}
}

void Protocol76::sendUpdateTile(const Tile* tile, const Position& pos)
{
	if(canSee(pos))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			msg->AddByte(0x69);
			msg->AddPosition(pos);
			if(tile)
			{
				GetTileDescription(tile, msg);
				msg->AddByte(0x00);
				msg->AddByte(0xFF);
			}
			else
			{
				msg->AddByte(0x01);
				msg->AddByte(0xFF);
			}
		}
	}
}

void Protocol76::sendAddCreature(const Creature* creature, bool isLogin)
{
	if(canSee(creature->getPosition()))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			if(creature == player)
			{
				msg->AddByte(0x0A);
				msg->AddU32(player->getID());

				msg->AddByte(0x32);
				msg->AddByte(0x00);

				if(player->getAccountType() >= ACCOUNT_TYPE_TUTOR)
					msg->AddByte(0x01);
				else
					msg->AddByte(0x00);

				if(violationReasons[player->getAccountType()] > 0)
				{
					msg->AddByte(0x0B);
					for(int32_t i = 0; i <= 22; i++)
					{
						if(i <= violationReasons[player->getAccountType()])
							msg->AddByte(violationActions[player->getAccountType()]);
						else
							msg->AddByte(Action_None);
					}
				}

				AddMapDescription(msg, player->getPosition());

				if(isLogin)
					AddMagicEffect(msg, player->getPosition(), NM_ME_TELEPORT);

				AddInventoryItem(msg, SLOT_HEAD, player->getInventoryItem(SLOT_HEAD));
				AddInventoryItem(msg, SLOT_NECKLACE, player->getInventoryItem(SLOT_NECKLACE));
				AddInventoryItem(msg, SLOT_BACKPACK, player->getInventoryItem(SLOT_BACKPACK));
				AddInventoryItem(msg, SLOT_ARMOR, player->getInventoryItem(SLOT_ARMOR));
				AddInventoryItem(msg, SLOT_RIGHT, player->getInventoryItem(SLOT_RIGHT));
				AddInventoryItem(msg, SLOT_LEFT, player->getInventoryItem(SLOT_LEFT));
				AddInventoryItem(msg, SLOT_LEGS, player->getInventoryItem(SLOT_LEGS));
				AddInventoryItem(msg, SLOT_FEET, player->getInventoryItem(SLOT_FEET));
				AddInventoryItem(msg, SLOT_RING, player->getInventoryItem(SLOT_RING));
				AddInventoryItem(msg, SLOT_AMMO, player->getInventoryItem(SLOT_AMMO));

				AddPlayerStats(msg);
				AddPlayerSkills(msg);

				//gameworld light-settings
				LightInfo lightInfo;
				g_game.getWorldLightInfo(lightInfo);
				AddWorldLight(msg, lightInfo);

				//player light level
				AddCreatureLight(msg, creature);

				if(isLogin)
				{
					std::string tempstring = g_config.getString(ConfigManager::LOGIN_MSG);
					if(player->getName() != "Account Manager")
					{
						if(!player->getLastLoginSaved() > 0)
						{
							tempstring += " Please choose your outfit.";
							sendOutfitWindow();
						}
						else
						{
							if(tempstring.size())
								AddTextMessage(msg, MSG_STATUS_DEFAULT, tempstring.c_str());
							tempstring = "Your last visit was on ";
							time_t lastLogin = player->getLastLoginSaved();
							tempstring += ctime(&lastLogin);
							tempstring.erase(tempstring.length() -1);
							tempstring += ".";
						}
						AddTextMessage(msg, MSG_STATUS_DEFAULT, tempstring);
					}
					else if(player->getName() == "Account Manager")
					{
						if(player->getNamelockedPlayer() != "")
							AddTextMessage(msg, MSG_STATUS_CONSOLE_ORANGE, "Hello, it appears that your character has been namelocked, what would you like as your new name?");
						else if(!player->isAccountManager())
							AddTextMessage(msg, MSG_STATUS_CONSOLE_ORANGE, "Hello, type 'account' to create an account or type 'recover' to recover an account.");
						else
							AddTextMessage(msg, MSG_STATUS_CONSOLE_ORANGE, "Hello, type 'account' to manage your account and if you want to start over then type 'cancel'.");
					}
				}

				for(VIPListSet::iterator it = player->VIPList.begin(); it != player->VIPList.end(); it++)
				{
					std::string vip_name;
					if(IOLoginData::getInstance()->getNameByGuid((*it), vip_name))
					{
						Player* tmpPlayer = g_game.getPlayerByName(vip_name);
						sendVIP((*it), vip_name, (tmpPlayer && (!tmpPlayer->isInGhostMode() || player->isAccessPlayer())));
					}
				}
			}
			else
			{
				AddTileCreature(msg, creature->getPosition(), creature);
				if(isLogin)
					AddMagicEffect(msg, creature->getPosition(), NM_ME_TELEPORT);
			}
		}
	}
}

void Protocol76::sendRemoveCreature(const Creature* creature, const Position& pos, uint32_t stackpos, bool isLogout)
{
	if(!(creature->isInGhostMode() && !player->isAccessPlayer()) && canSee(pos))
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			RemoveTileItem(msg, pos, stackpos);
		}
	}
}

void Protocol76::sendMoveCreature(const Creature* creature, const Tile* newTile, const Position& newPos,
	const Tile* oldTile, const Position& oldPos, uint32_t oldStackPos, bool teleport)
{
	if(creature == player)
	{
		NetworkMessage* msg = getOutputBuffer();
		if(msg)
		{
			TRACK_MESSAGE(msg);
			if(teleport || oldStackPos >= 10)
			{
				RemoveTileItem(msg, oldPos, oldStackPos);
				AddMapDescription(msg, newPos);
			}
			else
			{
				if(oldPos.z == 7 && newPos.z >= 8)
					RemoveTileItem(msg, oldPos, oldStackPos);
				else
				{
					msg->AddByte(0x6D);
					msg->AddPosition(oldPos);
					msg->AddByte(oldStackPos);
					msg->AddPosition(newPos);
				}

				if(newPos.z > oldPos.z)
					MoveDownCreature(msg, creature, newPos, oldPos, oldStackPos);
				else if(newPos.z < oldPos.z)
					MoveUpCreature(msg, creature, newPos, oldPos, oldStackPos);

				if(oldPos.y > newPos.y) // north, for old x
				{
					msg->AddByte(0x65);
					GetMapDescription(oldPos.x - 8, newPos.y - 6, newPos.z, 18, 1, msg);
				}
				else if(oldPos.y < newPos.y) // south, for old x
				{
					msg->AddByte(0x67);
					GetMapDescription(oldPos.x - 8, newPos.y + 7, newPos.z, 18, 1, msg);
				}

				if(oldPos.x < newPos.x) // east, [with new y]
				{
					msg->AddByte(0x66);
					GetMapDescription(newPos.x + 9, newPos.y - 6, newPos.z, 1, 14, msg);
				}
				else if(oldPos.x > newPos.x) // west, [with new y]
				{
					msg->AddByte(0x68);
					GetMapDescription(newPos.x - 8, newPos.y - 6, newPos.z, 1, 14, msg);
				}
			}
		}
	}
	else if(canSee(oldPos) && canSee(creature->getPosition()))
	{
		if(teleport || (oldPos.z == 7 && newPos.z >= 8) || oldStackPos >= 10)
		{
			sendRemoveCreature(creature, oldPos, oldStackPos, false);
			sendAddCreature(creature, false);
		}
		else
		{
			NetworkMessage* msg = getOutputBuffer();
			if(msg)
			{
				TRACK_MESSAGE(msg);
				msg->AddByte(0x6D);
				msg->AddPosition(oldPos);
				msg->AddByte(oldStackPos);
				msg->AddPosition(creature->getPosition());
			}
		}
	}
	else if(canSee(oldPos))
		sendRemoveCreature(creature, oldPos, oldStackPos, false);
	else if(canSee(creature->getPosition()))
		sendAddCreature(creature, false);
}

//inventory
void Protocol76::sendAddInventoryItem(slots_t slot, const Item* item)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddInventoryItem(msg, slot, item);
	}
}

void Protocol76::sendUpdateInventoryItem(slots_t slot, const Item* item)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		UpdateInventoryItem(msg, slot, item);
	}
}

void Protocol76::sendRemoveInventoryItem(slots_t slot)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		RemoveInventoryItem(msg, slot);
	}
}

//containers
void Protocol76::sendAddContainerItem(uint8_t cid, const Item* item)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		AddContainerItem(msg, cid, item);
	}
}

void Protocol76::sendUpdateContainerItem(uint8_t cid, uint8_t slot, const Item* item)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		UpdateContainerItem(msg, cid, slot, item);
	}
}

void Protocol76::sendRemoveContainerItem(uint8_t cid, uint8_t slot)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		RemoveContainerItem(msg, cid, slot);
	}
}

void Protocol76::sendTextWindow(uint32_t windowTextId, Item* item, uint16_t maxlen, bool canWrite)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x96);
		msg->AddU32(windowTextId);
		msg->AddItemId(item);
		if(canWrite)
		{
			msg->AddU16(maxlen);
			msg->AddString(item->getText());
		}
		else
		{
			msg->AddU16(item->getText().size());
			msg->AddString(item->getText());
		}

		const std::string& writer = item->getWriter();
		if(writer.size())
			msg->AddString(writer);
		else
			msg->AddString("");

		time_t writtenDate = item->getDate();
		if(writtenDate > 0)
		{
			char date[16];
			formatDate2(writtenDate, date);
			msg->AddString(date);
		}
		else
			msg->AddString("");
	}
}

void Protocol76::sendTextWindow(uint32_t windowTextId, uint32_t itemId, const std::string& text)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x96);
		msg->AddU32(windowTextId);
		msg->AddItemId(itemId);

		msg->AddU16(text.size());
		msg->AddString(text);

		msg->AddString("");
		msg->AddString("");
	}
}

void Protocol76::sendHouseWindow(uint32_t windowTextId, House* _house,
	uint32_t listId, const std::string& text)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0x97);
		msg->AddByte(0x00);
		msg->AddU32(windowTextId);
		msg->AddString(text);
	}
}

void Protocol76::sendOutfitWindow()
{
	#define MAX_NUMBER_OF_OUTFITS 25
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xC8);
		AddCreatureOutfit(msg, player, player->getDefaultOutfit());

		const OutfitListType& global_outfits = Outfits::getInstance()->getOutfits(player->getSex());
		int32_t count_outfits = global_outfits.size();

		if(count_outfits > MAX_NUMBER_OF_OUTFITS)
			count_outfits = MAX_NUMBER_OF_OUTFITS;
		else if(count_outfits == 0)
			return;

		OutfitListType::const_iterator it, it_;
		for(it = global_outfits.begin(); it != global_outfits.end(); ++it)
		{
			if((*it)->premium && !player->isPremium())
				count_outfits--;
		}

		msg->AddByte(count_outfits);

		bool addedAddon;
		const OutfitListType& player_outfits = player->getPlayerOutfits();
		for(it = global_outfits.begin(); it != global_outfits.end() && (count_outfits > 0); ++it)
		{
			if(((*it)->premium && player->isPremium()) || !(*it)->premium)
			{
				addedAddon = false;
				msg->AddU16((*it)->looktype);
				msg->AddString(Outfits::getInstance()->getOutfitName((*it)->looktype));
				//TODO: Try to avoid using loop to get addons
				for(it_ = player_outfits.begin(); it_ != player_outfits.end(); ++it_)
				{
					if((*it_)->looktype == (*it)->looktype)
					{
						msg->AddByte((*it_)->addons);
						addedAddon = true;
						break;
					}
				}
				if(!addedAddon)
					msg->AddByte(0x00);
				count_outfits--;
			}
		}
		player->hasRequestedOutfit(true);
	}
}

void Protocol76::sendVIPLogIn(uint32_t guid)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xD3);
		msg->AddU32(guid);
	}
}

void Protocol76::sendVIPLogOut(uint32_t guid)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xD4);
		msg->AddU32(guid);
	}
}

void Protocol76::sendVIP(uint32_t guid, const std::string& name, bool isOnline)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xD2);
		msg->AddU32(guid);
		msg->AddString(name);
		msg->AddByte(isOnline == true ? 1 : 0);
	}
}

////////////// Add common messages
void Protocol76::AddMapDescription(NetworkMessage* msg, const Position& pos)
{
	msg->AddByte(0x64);
	msg->AddPosition(player->getPosition());
	GetMapDescription(pos.x - 8, pos.y - 6, pos.z, 18, 14, msg);
}

void Protocol76::AddTextMessage(NetworkMessage* msg, MessageClasses mclass, const std::string& message)
{
	msg->AddByte(0xB4);
	msg->AddByte(mclass);
	msg->AddString(message);
}

void Protocol76::AddAnimatedText(NetworkMessage* msg, const Position& pos,
	uint8_t color, const std::string& text)
{
	msg->AddByte(0x84);
	msg->AddPosition(pos);
	msg->AddByte(color);
	msg->AddString(text);
}

void Protocol76::AddMagicEffect(NetworkMessage* msg,const Position& pos, uint8_t type)
{
	msg->AddByte(0x83);
	msg->AddPosition(pos);
	msg->AddByte(type + 1);
}

void Protocol76::AddDistanceShoot(NetworkMessage* msg, const Position& from, const Position& to,
	uint8_t type)
{
	msg->AddByte(0x85);
	msg->AddPosition(from);
	msg->AddPosition(to);
	msg->AddByte(type + 1);
}

void Protocol76::AddCreature(NetworkMessage* msg, const Creature* creature, bool known, uint32_t remove)
{
	if(known)
	{
		msg->AddU16(0x62);
		msg->AddU32(creature->getID());
	}
	else
	{
		msg->AddU16(0x61);
		msg->AddU32(remove);
		msg->AddU32(creature->getID());
		msg->AddString(creature->getName());
	}

	msg->AddByte((int32_t)std::ceil(((float)creature->getHealth()) * 100 / std::max(creature->getMaxHealth(), (int32_t)1)));
	msg->AddByte((uint8_t)creature->getDirection());

	if(!creature->isInvisible() && !creature->isInGhostMode())
		AddCreatureOutfit(msg, creature, creature->getCurrentOutfit());
	else
		AddCreatureInvisible(msg, creature);

	LightInfo lightInfo;
	creature->getCreatureLight(lightInfo);
	msg->AddByte((player->isAccessPlayer() ? 0xFF : lightInfo.level));
	msg->AddByte(lightInfo.color);

	msg->AddU16(creature->getStepSpeed());

	msg->AddByte(player->getSkullClient(creature->getPlayer()));
	msg->AddByte(player->getPartyShield(creature->getPlayer()));
}

void Protocol76::AddPlayerStats(NetworkMessage* msg)
{
	msg->AddByte(0xA0);

	msg->AddU16(player->getHealth());
	msg->AddU16(player->getPlayerInfo(PLAYERINFO_MAXHEALTH));
	msg->AddU16((int32_t)player->getFreeCapacity());
	uint64_t experience = player->getExperience();
	if(experience > 0x7FFFFFFF) //Windows client debugs after 2,147,483,647 exp
		msg->AddU32(0x7FFFFFFF);
	else
		msg->AddU32(experience);
	msg->AddU16(player->getPlayerInfo(PLAYERINFO_LEVEL));
	msg->AddByte(player->getPlayerInfo(PLAYERINFO_LEVELPERCENT));
	msg->AddU16(player->getMana());
	msg->AddU16(player->getPlayerInfo(PLAYERINFO_MAXMANA));
	msg->AddByte(player->getMagicLevel());
	msg->AddByte(player->getPlayerInfo(PLAYERINFO_MAGICLEVELPERCENT));
	msg->AddByte(player->getPlayerInfo(PLAYERINFO_SOUL));
}

void Protocol76::AddPlayerSkills(NetworkMessage* msg)
{
	msg->AddByte(0xA1);

	msg->AddByte(player->getSkill(SKILL_FIST, SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_FIST, SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_CLUB, SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_CLUB, SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_SWORD, SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_SWORD, SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_AXE, SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_AXE, SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_DIST, SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_DIST, SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_SHIELD, SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_SHIELD, SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_FISH, SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_FISH, SKILL_PERCENT));
}

void Protocol76::AddCreatureSpeak(NetworkMessage* msg, const Creature* creature,
	SpeakClasses type, std::string text, uint16_t channelId, uint32_t time /*= 0*/)
{
	msg->AddByte(0xAA);
	msg->AddU32(0x00000000);

	//Do not add name for anonymous channel talk
	if(type != SPEAK_CHANNEL_R2)
	{
		if(type != SPEAK_RVR_ANSWER)
			msg->AddString(creature->getName());
		else
			msg->AddString("Gamemaster");
	}
	else
		msg->AddString("");

	//Add level only for players
	if(const Player* speaker = creature->getPlayer())
	{
		if(type != SPEAK_RVR_ANSWER && speaker->getName() != "Account Manager")
			msg->AddU16(speaker->getPlayerInfo(PLAYERINFO_LEVEL));
		else
			msg->AddU16(0x0000);
	}
	else
		msg->AddU16(0x0000);

	msg->AddByte(type);
	switch(type)
	{
		case SPEAK_SAY:
		case SPEAK_WHISPER:
		case SPEAK_YELL:
		case SPEAK_MONSTER_SAY:
		case SPEAK_MONSTER_YELL:
		case SPEAK_PRIVATE_NP:
			msg->AddPosition(creature->getPosition());
			break;

		case SPEAK_CHANNEL_Y:
		case SPEAK_CHANNEL_R1:
		case SPEAK_CHANNEL_R2:
		case SPEAK_CHANNEL_O:
			msg->AddU16(channelId);
			break;

		case SPEAK_RVR_CHANNEL:
		{
			uint32_t _time = (OTSYS_TIME() / 1000) & 0xFFFFFFFF;
			msg->AddU32(_time - time);
			break;
		}

		default:
			break;
	}
	msg->AddString(text);
}

void Protocol76::AddCreatureHealth(NetworkMessage* msg,const Creature* creature)
{
	msg->AddByte(0x8C);
	msg->AddU32(creature->getID());
	msg->AddByte((int32_t)std::ceil(((float)creature->getHealth()) * 100 / std::max(creature->getMaxHealth(), (int32_t)1)));
}

void Protocol76::AddCreatureInvisible(NetworkMessage* msg, const Creature* creature)
{
	msg->AddU16(0x00);
	msg->AddU16(0x00);
}

void Protocol76::AddCreatureOutfit(NetworkMessage* msg, const Creature* creature, const Outfit_t& outfit)
{
	msg->AddU16(outfit.lookType);

	if(outfit.lookType != 0)
	{
		msg->AddByte(outfit.lookHead);
		msg->AddByte(outfit.lookBody);
		msg->AddByte(outfit.lookLegs);
		msg->AddByte(outfit.lookFeet);
		msg->AddByte(outfit.lookAddons);
	}
	else
		msg->AddItemId(outfit.lookTypeEx);
}

void Protocol76::AddWorldLight(NetworkMessage* msg, const LightInfo& lightInfo)
{
	msg->AddByte(0x82);
	msg->AddByte((player->isAccessPlayer() ? 0xFF : lightInfo.level));
	msg->AddByte(lightInfo.color);
}

void Protocol76::AddCreatureLight(NetworkMessage* msg, const Creature* creature)
{
	LightInfo lightInfo;
	creature->getCreatureLight(lightInfo);
	msg->AddByte(0x8D);
	msg->AddU32(creature->getID());
	msg->AddByte((player->isAccessPlayer() ? 0xFF : lightInfo.level));
	msg->AddByte(lightInfo.color);
}

//tile
void Protocol76::AddTileItem(NetworkMessage* msg, const Position& pos, const Item* item)
{
	msg->AddByte(0x6A);
	msg->AddPosition(pos);
	msg->AddItem(item);
}

void Protocol76::AddTileCreature(NetworkMessage* msg, const Position& pos, const Creature* creature)
{
	msg->AddByte(0x6A);
	msg->AddPosition(pos);

	bool known;
	uint32_t removedKnown;
	checkCreatureAsKnown(creature->getID(), known, removedKnown);
	AddCreature(msg, creature, known, removedKnown);
}

void Protocol76::UpdateTileItem(NetworkMessage* msg, const Position& pos, uint32_t stackpos, const Item* item)
{
	if(stackpos < 10)
	{
		msg->AddByte(0x6B);
		msg->AddPosition(pos);
		msg->AddByte(stackpos);
		msg->AddItem(item);
	}
}

void Protocol76::RemoveTileItem(NetworkMessage* msg, const Position& pos, uint32_t stackpos)
{
	if(stackpos < 10)
	{
		msg->AddByte(0x6C);
		msg->AddPosition(pos);
		msg->AddByte(stackpos);
	}
}

void Protocol76::MoveUpCreature(NetworkMessage* msg, const Creature* creature,
	const Position& newPos, const Position& oldPos, uint32_t oldStackPos)
{
	if(creature == player)
	{
		//floor change up
		msg->AddByte(0xBE);

		//going to surface
		if(newPos.z == 7)
		{
			int32_t skip = -1;
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 5, 18, 14, 3, skip); //(floor 7 and 6 already set)
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 4, 18, 14, 4, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 3, 18, 14, 5, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 2, 18, 14, 6, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 1, 18, 14, 7, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 0, 18, 14, 8, skip);

			if(skip >= 0)
			{
				msg->AddByte(skip);
				msg->AddByte(0xFF);
			}
		}
		//underground, going one floor up (still underground)
		else if(newPos.z > 7)
		{
			int32_t skip = -1;
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, oldPos.z - 3, 18, 14, 3, skip);

			if(skip >= 0)
			{
				msg->AddByte(skip);
				msg->AddByte(0xFF);
			}
		}

		//moving up a floor up makes us out of sync
		//west
		msg->AddByte(0x68);
		GetMapDescription(oldPos.x - 8, oldPos.y + 1 - 6, newPos.z, 1, 14, msg);

		//north
		msg->AddByte(0x65);
		GetMapDescription(oldPos.x - 8, oldPos.y - 6, newPos.z, 18, 1, msg);
	}
}

void Protocol76::MoveDownCreature(NetworkMessage* msg, const Creature* creature,
	const Position& newPos, const Position& oldPos, uint32_t oldStackPos)
{
	if(creature == player)
	{
		//floor change down
		msg->AddByte(0xBF);

		//going from surface to underground
		if(newPos.z == 8)
		{
			int32_t skip = -1;

			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, newPos.z, 18, 14, -1, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, newPos.z + 1, 18, 14, -2, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, newPos.z + 2, 18, 14, -3, skip);

			if(skip >= 0)
			{
				msg->AddByte(skip);
				msg->AddByte(0xFF);
			}
		}
		//going further down
		else if(newPos.z > oldPos.z && newPos.z > 8 && newPos.z < 14)
		{
			int32_t skip = -1;
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, newPos.z + 2, 18, 14, -3, skip);

			if(skip >= 0)
			{
				msg->AddByte(skip);
				msg->AddByte(0xFF);
			}
		}

		//moving down a floor makes us out of sync
		//east
		msg->AddByte(0x66);
		GetMapDescription(oldPos.x + 9, oldPos.y - 1 - 6, newPos.z, 1, 14, msg);

		//south
		msg->AddByte(0x67);
		GetMapDescription(oldPos.x - 8, oldPos.y + 7, newPos.z, 18, 1, msg);
	}
}

//inventory
void Protocol76::AddInventoryItem(NetworkMessage* msg, slots_t slot, const Item* item)
{
	if(item == NULL)
	{
		msg->AddByte(0x79);
		msg->AddByte(slot);
	}
	else
	{
		msg->AddByte(0x78);
		msg->AddByte(slot);
		msg->AddItem(item);
	}
}

void Protocol76::UpdateInventoryItem(NetworkMessage* msg, slots_t slot, const Item* item)
{
	if(item == NULL)
	{
		msg->AddByte(0x79);
		msg->AddByte(slot);
	}
	else
	{
		msg->AddByte(0x78);
		msg->AddByte(slot);
		msg->AddItem(item);
	}
}

void Protocol76::RemoveInventoryItem(NetworkMessage* msg, slots_t slot)
{
	msg->AddByte(0x79);
	msg->AddByte(slot);
}

//containers
void Protocol76::AddContainerItem(NetworkMessage* msg, uint8_t cid, const Item* item)
{
	msg->AddByte(0x70);
	msg->AddByte(cid);
	msg->AddItem(item);
}

void Protocol76::UpdateContainerItem(NetworkMessage* msg, uint8_t cid, uint8_t slot, const Item* item)
{
	msg->AddByte(0x71);
	msg->AddByte(cid);
	msg->AddByte(slot);
	msg->AddItem(item);
}

void Protocol76::RemoveContainerItem(NetworkMessage* msg, uint8_t cid, uint8_t slot)
{
	msg->AddByte(0x72);
	msg->AddByte(cid);
	msg->AddByte(slot);
}

void Protocol76::sendChannelMessage(std::string author, std::string text, SpeakClasses type, uint8_t channel)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg)
	{
		TRACK_MESSAGE(msg);
		msg->AddByte(0xAA);
		msg->AddU32(0x00);
		msg->AddString(author);
		msg->AddU16(0x00);
		msg->AddByte(type);
		msg->AddU16(channel);
		msg->AddString(text);
	}
}

void Protocol76::AddShopItem(NetworkMessage* msg, const ShopInfo item)
{
	const ItemType& it = Item::items[item.itemId];
	msg->AddU16(it.clientId);
	msg->AddByte(item.subType);
	msg->AddString(it.name);
	msg->AddU32(item.buyPrice);
	msg->AddU32(item.sellPrice);
}
