#pragma once

//based on http://www.bulletphysics.org/mediawiki-1.5.8/index.php?title=Collision_Callbacks_and_Triggers
namespace octet {
	class CollisionsHandler
	{
		class ObjectsPair
		{
			const btCollisionObject *A;
			const btCollisionObject *B;

			bool exist;
		public:
			ObjectsPair(const btCollisionObject *first, const btCollisionObject *second)
			{
				A = first;
				B = second;
			}

			bool operator==(ObjectsPair &p)
			{
				if ((A == p.A && B == p.B) || (A == p.B && B == p.A))
					return true;
				return false;					
			}

			void match_found()
			{
				exist = true;
			}

			void reset_values()
			{
				exist = false;
			}

			bool exists()
			{
				return exist;
			}

			float distance = 0.f;

			const btCollisionObject *getA()
			{
				return A;
			}

			const btCollisionObject *getB()
			{
				return B;
			}
		};

		const float epsilon = 0.1f;

		btDiscreteDynamicsWorld *world;
		std::vector<ObjectsPair> previous_frame;
		std::vector<ObjectsPair> current_frame;

		bool textOutput;

		void call_collision_event(ObjectsPair objects)
		{
			//if ((objects.getA()->getCollisionFlags() == btCollisionObject::CF_CHARACTER_OBJECT) ||
			//	(objects.getB()->getCollisionFlags() == btCollisionObject::CF_CHARACTER_OBJECT))
			if (textOutput)
				std::cout << "Collision! Distance = " << objects.distance << std::endl;
		}

		void call_separation_event(ObjectsPair objects)
		{
			if (textOutput)
				std::cout << "Separation! Distance = " << objects.distance << std::endl;
		}

		void find_collision_pairs()
		{
			int numManifolds = world->getDispatcher()->getNumManifolds();
			for (int i = 0; i < numManifolds; i++)
			{
				btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
				const btCollisionObject* obA = contactManifold->getBody0();
				const btCollisionObject* obB = contactManifold->getBody1();

				int numContacts = contactManifold->getNumContacts();
				for (int j = 0; j < numContacts; j++)
				{
					btManifoldPoint& pt = contactManifold->getContactPoint(j);
					if (pt.getDistance() < epsilon)
					{
						current_frame.push_back(ObjectsPair(obA, obB));
						current_frame[current_frame.size() - 1].distance = pt.getDistance();
						const btVector3& ptA = pt.getPositionWorldOnA();
						const btVector3& ptB = pt.getPositionWorldOnB();
						const btVector3& normalOnB = pt.m_normalWorldOnB;

						int fA = obA->getCollisionFlags(),
							fB = obB->getCollisionFlags();
					}
				}
			}
		}

		void compare_frames()
		{
			for (int i = 0; i < previous_frame.size(); i++)
				previous_frame[i].reset_values();

			for (int i = 0; i < current_frame.size(); i++)
			{
				current_frame[i].reset_values();
				for (int j = 0; j < previous_frame.size(); j++)
				{
					if (current_frame[i] == previous_frame[j])
					{
						current_frame[i].match_found();
						previous_frame[j].match_found();
					}
				}
			}
		}

	public:

		CollisionsHandler(btDiscreteDynamicsWorld *World, bool enableTextOutput = false)
		{
			world = World;
			textOutput = enableTextOutput;
		}

		void handle()
		{
			find_collision_pairs();

			compare_frames();

			for (int i = 0; i < current_frame.size(); i++)
				if (!current_frame[i].exists())
					call_collision_event(current_frame[i]);

			for (int i = 0; i < previous_frame.size(); i++)
				if (!previous_frame[i].exists())
				{
					call_separation_event(previous_frame[i]);
				}
			// 1 2 3 -> 1 3 = 2 separated
			// 1 3 -> 1 2 3 = 2 collided
			previous_frame = current_frame;
			current_frame.clear();
		}
	};
}