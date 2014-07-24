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
				m_World.m_pRules->m_MapWidth = it->value.GetUint();
			else if (member == "sizeM")
				m_World.m_pRules->m_MapHeight = it->value.GetUint();
			else if (member == "bombs")
			{
				for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
				{
					std::string mem2 = it2->name.GetString();
					unsigned int val = it2->value.GetUint();
					if (mem2 == "life")
						m_World.m_pRules->m_BombLife = val;
					else if (mem2 == "range")
						m_World.m_pRules->m_BombRange = val;
					else if (mem2 == "speed")
						m_World.m_pRules->m_BombSpeed = val;
				}
			}
			else if (member == "flames")
			{
				for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
				{
					std::string mem2 = it2->name.GetString();
					unsigned int val = it2->value.GetUint();
					if (mem2 == "life")
						m_World.m_pRules->m_FlameLife = val;
				}
			}
			else if (member == "players")
			{
				for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
				{
					std::string mem2 = it2->name.GetString();
					unsigned int val = it2->value.GetUint();
					if (mem2 == "speed")
						m_World.m_pRules->m_PlayerSpeed = val;
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
		
		unsigned int width = m_World.m_pRules->MapWidth();
		unsigned int height = m_World.m_pRules->MapHeight();

		m_World.m_Map.resize(height);
		for (unsigned int h_begin = 0; h_begin != height; ++h_begin)
			m_World.m_Map[h_begin].resize(width);
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
							Factory::buffer.is_blocking = it3->value.GetBool();
						else if (member2 == "mortal")
							Factory::buffer.is_mortal = it3->value.GetBool();
						else if (member2 == "type")
							Factory::buffer.type = it3->value.GetString();
						else if (member2 == "pos")
						{
							for (Value::ConstMemberIterator it4 = it3->value.MemberBegin(); it4 != it3->value.MemberEnd(); ++it4)
							{
								std::string member3 = it4->name.GetString();
								if (member3 == "x")
									Factory::buffer.position.x = it4->value.GetDouble();
								else if (member3 == "y")
									Factory::buffer.position.y = it4->value.GetDouble();
							}
						}
						else if (member2 == "powerups")
						{

						}
						else if (member2 == "lastUpdate")
							Factory::buffer.last_update = it3->value.GetUint();
						else if (member2 == "direction")
							Factory::buffer.direction = it3->value.GetString();
						else if (member2 == "bombs")
							Factory::buffer.bombs = it3->value.GetUint();
						else if (member2 == "kills")
							Factory::buffer.kills = it3->value.GetUint();
						else if (member2 == "object_id")
							Factory::buffer.object_id = it3->value.GetUint();
						else if (member2 == "id")
							Factory::buffer.id = it3->value.GetUint();
						else if (member2 == "name")
							Factory::buffer.name = it3->value.GetString();
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
				if (!m_World.HasEntity(Factory::buffer.object_id))
					Factory::CreateEntity(m_World);
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
				for (Value::ConstValueIterator it_content = it->value.Begin(); it_content != it->value.End(); ++it_content)
				{
					// iterate through (*it_content)'s members, i.e. each stack member's members // dang
					for (Value::ConstMemberIterator it_member = it_content->MemberBegin(); it_member != it_content->MemberEnd(); ++it_member)
					{
						std::string member = it_member->name.GetString();

						if (member == "type")
							Factory::buffer.type = it_member->value.GetString();
						else if (member == "isBlocking")
							Factory::buffer.is_blocking = it_member->value.GetBool();
						else if (member == "pos")
						{
							for (Value::ConstMemberIterator it_coord = it_member->value.MemberBegin(); it_coord != it_member->value.MemberEnd(); ++it_coord)
							{
								std::string coord = it_coord->name.GetString();
								if (coord == "x")
									Factory::buffer.position.x = it_coord->value.GetDouble();
								else if (coord == "y")
									Factory::buffer.position.y = it_coord->value.GetDouble();
							}
						}
						else if (member == "object_id")
							Factory::buffer.object_id = it_member->value.GetUint();
						else if (member == "mortal")
							Factory::buffer.is_mortal = it_member->value.GetBool();
						else if (member == "powerups")
						{

						}
						else if (member == "lastUpdate")
							Factory::buffer.last_update = it_member->value.GetUint();
						else if (member == "direction")
							Factory::buffer.direction = it_member->value.GetString();
						else if (member == "bombs")
							Factory::buffer.bombs = it_member->value.GetUint();
						else if (member == "kills")
							Factory::buffer.kills = it_member->value.GetUint();
						else if (member == "id")
							Factory::buffer.id = it_member->value.GetUint();
						else if (member == "name")
							Factory::buffer.name = it_member->value.GetString();
					}

					if (!m_World.HasEntity(Factory::buffer.object_id))
						Factory::CreateEntity(m_World);
				}
			}
		}
	} // HandleEState()
} // namespace boom