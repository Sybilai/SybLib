#ifndef BOMB_GAME_HPP
#define BOMB_GAME_HPP

#include "ai/IBot.hpp"
#include "core/Game.hpp"
#include "rapidjson/document.h"
#include "BombEntities.hpp"
#include "utils/Vec2.hpp"

using namespace rapidjson;


namespace syb
{
	class BombGame : public Game
	{
	public:
		BombGame(IBot *bot) :
			Game(bot)
		{
		}

	private:
		void HandleEvents()
		{
			std::queue<std::string> msgs = m_pIO->GetMessageQueue();
			while (!msgs.empty())
			{
				const char *msg = msgs.front().c_str();

				Document d;
				d.Parse<0>(msg);
				std::string msgType = d["event"].GetString();

				// String-man's switch statement
				if
					(msgType == "login")
				{
					m_pBot->SetId(d["your_id"].GetInt());
					m_pBot->AttachIOChannel(m_pIO);
				}
				else if
					(msgType == "game_state")
				{
					m_pWorld->FlattenMap();
					int x = 0, y = 0;

					for (Value::ConstValueIterator it = d["data"].Begin(); it != d["data"].End(); ++it, ++x)
					{
						// Lines(left to right)
						y = 0;
						for (Value::ConstValueIterator it2 = it->Begin(); it2 != it->End(); ++it2, ++y)
						{
							for (Value::ConstMemberIterator it3 = it2->MemberBegin(); it3 != it2->MemberEnd(); ++it3)
							{
								// it3->value is the "content" array
								if (it3->value.Size())
									m_pWorld->m_FlatMap[x][y] = 1;
								for (Value::ConstValueIterator it4 = it3->value.Begin(); it4 != it3->value.End(); ++it4)
								{
									for (Value::ConstMemberIterator it5 = it4->MemberBegin(); it5 != it4->MemberEnd(); ++it5)
									{
										std::string member = it5->name.GetString();
										if (member == "type")
										{
											std::string ty = it5->value.GetString();
											if (ty == "fixblock")
											{
												m_pWorld->m_Map[x][y].m_Blocks.push_back(BombWorld::BL_FIXBLOCK);
											}
											else if (ty == "bot")
											{
												m_pWorld->m_Map[x][y].m_Blocks.push_back(BombWorld::BL_BOT);
											}
											else if (ty == "flame")
											{
												m_pWorld->m_Map[x][y].m_Blocks.push_back(BombWorld::BL_FLAME);
											}
											else if (ty == "bomb")
											{
												m_pWorld->m_Map[x][y].m_Blocks.push_back(BombWorld::BL_BOMB);
											}
										}
									}
								}
							}
						}
					}
				}
				else if
					(msgType == "game_rules")
				{
					for (Value::ConstMemberIterator it = d["data"].MemberBegin(); it != d["data"].MemberEnd(); ++it)
					{
						std::string mem = it->name.GetString();
						if (mem == "sizeN")
						{
							m_pWorld->m_Width = it->value.GetInt();
						}
						else if (mem == "sizeM")
						{
							m_pWorld->m_Height = it->value.GetInt();
						}
						else if (mem == "bombs")
						{
							for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
							{
								std::string mem2 = it2->name.GetString();
								unsigned int val = it2->value.GetUint();
								if (mem2 == "life")
									Bomb::LIFE = val;
								else if (mem2 == "range")
									Bomb::RANGE = val;
								else if (mem2 == "speed")
									Bomb::SPEED = val;
							}
						}
						else if (mem == "flames")
						{
							for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
							{
								std::string mem2 = it2->name.GetString();
								unsigned int val = it2->value.GetUint();
								if (mem2 == "life")
									Flame::LIFE = val;
							}
						}
						else if (mem == "players")
						{
							for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); ++it2)
							{
								std::string mem2 = it2->name.GetString();
								unsigned int val = it2->value.GetUint();
								if (mem2 == "speed")
									Bomberman::SPEED = val;
							}
						}
						else if (mem == "framesPerSecond")
						{
							m_FixedFPS = it->value.GetUint();
						}
					}
				}
				else if
					(msgType == "frame")
				{
					for (Value::ConstValueIterator it = d["frame"].Begin(); it != d["frame"].End(); ++it)
					{
						std::string eventType = "none";
						Vec2 pos;
						int id;
						int aiId = -1;
						std::string direction;
						std::string type;
						bool mortal;
						bool isBlocking;

						for (Value::ConstMemberIterator it2 = it->MemberBegin(); it2 != it->MemberEnd(); ++it2)
						{
							std::string mem = it2->name.GetString();
							
							if (mem == "event")
								eventType = it2->value.GetString();
							else if (mem == "object_id")
								id = it2->value.GetInt();
							else if (mem == "pos")
							{
								for (Value::ConstMemberIterator it3 = it2->value.MemberBegin(); it3 != it2->value.MemberEnd(); ++it3)
								{
									std::string mem2 = it3->name.GetString();
									if (mem2 == "x")
										pos.x = it3->value.GetInt();
									else if (mem2 == "y")
										pos.y = it3->value.GetInt();
								}
							}
							else if (mem == "data")
							{
								for (Value::ConstMemberIterator it3 = it2->value.MemberBegin(); it3 != it2->value.MemberEnd(); ++it3)
								{
									std::string mem2 = it3->name.GetString();
									if (mem2 == "isBlocking")
										isBlocking = it3->value.GetBool();
									else if (mem2 == "mortal")
										mortal = it3->value.GetBool();
									else if (mem2 == "type")
										type = it3->value.GetString();
									else if (mem2 == "pos")
									{
										for (Value::ConstMemberIterator it4 = it3->value.MemberBegin(); it4 != it3->value.MemberEnd(); ++it4)
										{
											std::string mem3 = it4->name.GetString();
											if (mem3 == "x")
												pos.x = it4->value.GetInt();
											else if (mem3 == "y")
												pos.y = it4->value.GetInt();
										}
									}
									else if (mem2 == "direction")
										direction = it3->value.GetString();
									else if (mem2 == "object_id")
										id = it3->value.GetInt();
									else if (mem2 == "id")
										aiId = it3->value.GetInt();
								}
							}
						}

						if (eventType == "move_entity")
						{
							m_pWorld->MoveEntity(id, pos);
						}
						else if (eventType == "destroy_entity")
						{
							m_pWorld->DestroyEntity(id);
						}
						else if (eventType == "new_entity")
						{
							m_pWorld->CreateEntity(isBlocking, mortal, type, pos, direction, id);
							if (!m_pBot->m_pMe)
							{
								if (aiId == m_pBot->GetId())
								{
									m_pBot->m_pPos = &m_pWorld->m_vBombermen[m_pWorld->m_vBombermen.size() - 1].m_Pos;
									m_pBot->m_pMe = &m_pWorld->m_vBombermen[m_pWorld->m_vBombermen.size() - 1];
									//m_pBot->m_pPos = &m_pWorld->m_vBombermen[m_pWorld->m_vBombermen.size() - 1].m_Pos;
								}
							}
						}
					}

					if (!m_pBot)
						m_pBot->AttachWorld(m_pWorld);
					m_State = G_STATE_RUNNING;
				}

				msgs.pop();
			}
		}
	};
} // namespace syb

#endif // BOMB_GAME_HPP