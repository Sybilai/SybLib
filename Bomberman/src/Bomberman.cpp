#include "../include/Bomberman.hpp"
#include "../include/sybil/utils/Vec2.hpp"
#include <queue>
#include <string>
#include <iostream>

using namespace rapidjson;


namespace boom
{
	// --------------------------------------------------------------------
	BombermanGame::BombermanGame()
	{ }

	// --------------------------------------------------------------------
	BombermanGame::~BombermanGame()
	{ }

	// --------------------------------------------------------------------
	void BombermanGame::HandleEvents()
	{
		std::queue<std::string> msgs = m_IOManager.GetMsgQueue();
		while (!msgs.empty())
		{
			std::string msg = msgs.front();

			Document d;
			d.Parse<0>(msg.c_str());
			
			std::string msg_type = d["event"].GetString();

			// Kind and heartfelt note: Cezar Andrici is responsible for the absolute clusterf*** that follows
			if (msg_type == "login")
				HandleELogin(d);
			else if (msg_type == "game_rules")
				HandleEGameRules(d);
			else if (msg_type == "frame")
				HandleEFrame(d);
			else if (msg_type == "game_state")
				HandleEState(d);

			msgs.pop();
		} 
	} 

	// --------------------------------------------------------------------
	void BombermanGame::HandleELogin(const rapidjson::Document& d)
	{
		m_pBots[0]->Identify(d["your_id"].GetUint());
	}

	// --------------------------------------------------------------------
	void BombermanGame::HandleEGameRules(const Document& d)
	{
		for (Value::ConstMemberIterator it = d["data"].MemberBegin(); it != d["data"].MemberEnd(); ++it)
		{
			std::string member = it->name.GetString();
			if (member == "sizeN")
			{
				m_Rules.m_MapWidth = it->value.GetUint();
			}
			else if (member == "sizeM")
			{
				m_Rules.m_MapHeight = it->value.GetUint();
			}
			else if (member == "bombs")
			{
				for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
				{
					std::string mem2 = it2->name.GetString();
					unsigned int val = it2->value.GetUint();
					if (mem2 == "life")
						m_Rules.m_BombLife = val;
					else if (mem2 == "range")
						m_Rules.m_BombRange = val;
					else if (mem2 == "speed")
						m_Rules.m_BombSpeed = val;
				}
			}
			else if (member == "flames")
			{
				for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
				{
					std::string mem2 = it2->name.GetString();
					unsigned int val = it2->value.GetUint();
					if (mem2 == "life")
						m_Rules.m_FlameLife = val;
				}
			}
			else if (member == "players")
			{
				for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
				{
					std::string mem2 = it2->name.GetString();
					unsigned int val = it2->value.GetUint();
					if (mem2 == "speed")
						m_Rules.m_PlayerSpeed = val;
				}
			}
			else if (member == "currentFrame")
			{
				m_RuleBuffer.start_frame = it->value.GetUint();
				if (m_RuleBuffer.update_rate)
					m_RuleBuffer.ready_for_lunch = true;
			}
			else if (member == "framesPerSecond")
			{
				m_RuleBuffer.update_rate = it->value.GetUint();
				if (m_RuleBuffer.start_frame)
					m_RuleBuffer.ready_for_lunch = true;
			}
		}
	} // HandleEGameRules()

	// --------------------------------------------------------------------
	void BombermanGame::HandleEFrame(const Document& d)
	{
		// This "frame" event can have multiple sub-events
		// (*it) is a single event
		for (Value::ConstValueIterator it = d["frame"].Begin(); it != d["frame"].End(); ++it)
		{
			// Event descriptor
			std::string event_type = "none";
			syb::Vec2 location;

			// New event descriptor. Entity agnostic. Allocated once and used for every entity that needs the factory's services.
			struct NewEntityBuffer
			{
				NewEntityBuffer() :
					is_blocking(false),
					is_mortal(false),
					type(),
					position(),
					last_update(0),
					direction("none"),
					bombs(0),
					kills(0),
					object_id(0),
					id(0),
					name()
				{ }

				bool is_blocking;
				bool is_mortal;
				std::string type;
				syb::Vec2 position;
				//powerups
				unsigned int last_update;
				std::string direction;
				unsigned int bombs;
				unsigned int kills;
				unsigned int object_id;
				unsigned int id; // ffs dude fo real
				std::string name;
			} new_e;

			for (Value::ConstMemberIterator it2 = it->MemberBegin(); it2 != it->MemberEnd(); ++it2)
			{
				std::string member = it2->name.GetString();

				if (member == "event")
					event_type = it2->value.GetString();
				else if (member == "x")
					location.x = it2->value.GetDouble();
				else if (member == "y")
					location.y = it2->value.GetDouble();
				else if (member == "data")
				{
					for (Value::ConstMemberIterator it3 = it2->value.MemberBegin(); it3 != it2->value.MemberEnd(); ++it3)
					{
						std::string member2 = it3->name.GetString();
						if (member2 == "isBlocking")
							new_e.is_blocking = it3->value.GetBool();
						else if (member2 == "mortal")
							new_e.is_mortal = it3->value.GetBool();
						else if (member2 == "type")
							new_e.type = it3->value.GetString();
						else if (member2 == "pos")
						{
							for (Value::ConstMemberIterator it4 = it3->value.MemberBegin(); it4 != it3->value.MemberEnd(); ++it4)
							{
								std::string member3 = it4->name.GetString();
								if (member3 == "x")
									new_e.position.x = it4->value.GetDouble();
								else if (member3 == "y")
									new_e.position.y = it4->value.GetDouble();
							}
						}
						else if (member2 == "powerups")
						{

						}
						else if (member2 == "lastUpdate")
							new_e.last_update = it3->value.GetUint();
						else if (member2 == "direction")
							new_e.direction = it3->value.GetString();
						else if (member2 == "bombs")
							new_e.bombs = it3->value.GetUint();
						else if (member2 == "kills")
							new_e.kills = it3->value.GetUint();
						else if (member2 == "object_id")
							new_e.object_id = it3->value.GetUint();
						else if (member2 == "id")
							new_e.id = it3->value.GetUint();
						else if (member2 == "name")
							new_e.name = it3->value.GetString();
					}
				}
			}

			// Do something ca$hto with the potentially partially filled buffer.
			if (event_type == "move_entity")
			{
				//m_pWorld->MoveEntity(id, pos);
			}
			else if (event_type == "destroy_entity")
			{
				//m_pWorld->DestroyEntity(id);
			}
			else if (event_type == "new_entity")
			{
				//m_pWorld->CreateEntity(isBlocking, mortal, type, pos, direction, id);
			}
		}
	} // HandleEFrame()

	// --------------------------------------------------------------------
	void BombermanGame::HandleEState(const Document& d)
	{
		for (Value::ConstValueIterator it_w = d["data"].Begin(); it_w != d["data"].End(); ++it_w)
		{
			for (Value::ConstValueIterator it_h = it_w->Begin(); it_h != it_w->End(); ++it_h)
			{
				// at this level, there is an array of stacked items
				// i.e. iterate through each item in the tile stack
				for (Value::ConstMemberIterator it = it_h->MemberBegin(); it != it_h->MemberEnd(); ++it)
				for (Value::ConstValueIterator it_content = it->value.Begin(); it_content != it->value.End(); ++it)
				{
					// iterate through (*it_content)'s members
					for (Value::ConstMemberIterator it_member = it_content->MemberBegin(); it_member != it_content->MemberEnd(); ++it_member)
					{
						std::string member = it_member->name.GetString();

						if (member == "type")
						{

						}
						else if (member == "isBlocking")
						{

						}
						else if (member == "pos")
						{
							for (Value::ConstMemberIterator it_coord = it_member->value.MemberBegin(); it_coord != it_member->value.MemberEnd(); ++it_coord)
							{
								std::string coord = it_coord->name.GetString();
								/*if (coord == "x")
									new_e.position.x = it_coord->value.GetDouble();
								else if (coord == "y")
									new_e.position.y = it_coord->value.GetDouble();*/
							}
						}
						else if (member == "object_id")
						{

						}
					}
				}
			}
		}
	} // HandleEState()
} // namespace boom