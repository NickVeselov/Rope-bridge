#pragma once
namespace octet
{
	enum object_type
	{
		plank,
		cliff,
		rope,
		column
	};
	
	//ball
	struct Character
	{
		int collision_duration = 0;
	};

	class Bridge
	{
		//scene
		ref<visual_scene> app_scene;
		mat4t mat;

		float distance_between;

		//plank model
		vec3 plank_size;
		material *plank_material;
		vec3 plank_half_size;

		//cliff
		material *cliff_material;
		vec3 cliff_size;
		vec3 cliff_half_size;

		vec3 rope_part_size;

		mesh_instance *left_cliff_mesh;
		vec3 left_cliff_location;
		btRigidBody *left_cliff_rb;

		mesh_instance *right_cliff_mesh;
		vec3 right_cliff_location;
		btRigidBody *right_cliff_rb;

		int size;
		float ground_level;
		
		void set_variables()
		{
			distance_between = 1.0f;

			plank_size = vec3(1, 0.5f, 10);
			plank_material = new material(vec4(0.54f, 0.27f, 0.074f, 1));
			plank_half_size = plank_size / 2.f;

			//cliff_material = new material(vec4(0.586f, 0.55f, 0.598f, 1));
			cliff_material = new material(vec4(0, 0.39f, 0, 1));
			cliff_size = vec3(100, 100, 200);
			cliff_half_size = cliff_size / 2.f;

		}

		void create_cliffs(int size)
		{
			mat.loadIdentity();
			left_cliff_location = vec3(-cliff_half_size.x()*3/2.f, cliff_half_size.y() + ground_level, 0);
			mat.translate(left_cliff_location);
			left_cliff_mesh = app_scene->add_shape(mat, new mesh_box(cliff_half_size), cliff_material,false);
			left_cliff_rb = left_cliff_mesh->get_node()->get_rigid_body();

			mat.loadIdentity();
			right_cliff_location = vec3(left_cliff_location.x() + 2*distance_between + plank_size.x() + cliff_size.x() 
				+ (size - 1)*(distance_between + plank_size.x()), left_cliff_location.y(), left_cliff_location.z());
			mat.translate(right_cliff_location);
			right_cliff_mesh = app_scene->add_shape(mat, new mesh_box(cliff_half_size), cliff_material, false);
			right_cliff_rb = right_cliff_mesh->get_node()->get_rigid_body();
		}


		btRigidBody *create_plank(vec3 &location)
		{
			mat.loadIdentity();
			mat.translate(location);
			mesh_instance *mesh = app_scene->add_shape(mat, new mesh_box(vec3(plank_half_size)), plank_material, true);
			return mesh->get_node()->get_rigid_body();
		}

		void bind_bodies(btRigidBody *previous_rb, object_type previos_type, btRigidBody *current_rb, object_type current_type, float break_limit)
		{
			float pA_x = 0.f,
				pA_y = 0.f,
				pB_x = 0.f,
				pB_y = 0.f;
				
			switch (previos_type)
			{
				case plank:
					pA_x = (plank_size.x() + distance_between) / 2.f;
					break;
				case rope:
					pA_x = rope_part_size.x();
					pA_y = 0;
					break;
				case cliff:
					pA_x = (cliff_size.x() + distance_between) / 2.f;
					pA_y = cliff_half_size.y() - plank_half_size.y();
				default:
					break;
			}

			switch (current_type)
			{
			case octet::plank:
				pB_x = -(plank_size.x() + distance_between) / 2.f;
				break;
			case octet::cliff:
				pB_x = -(cliff_size.x() + distance_between) / 2.f;
				pB_y = cliff_half_size.y() - plank_half_size.y();
				break;
			case octet::rope:
				pB_x = -rope_part_size.x();
				pB_y = 0;
				break;
			case octet::column:
				break;
			default:
				break;
			}

			btVector3 pivotA(pA_x, pA_y, 0.f);
			btVector3 pivotB(pB_x, pB_y, 0.f);
			btVector3 axisA(0.f, 0.f, 1.f);
			btVector3 axisB(0.f, 0.f, 1.f);

			//btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint()
				
			previous_rb->setActivationState(DISABLE_DEACTIVATION);
			current_rb->setActivationState(DISABLE_DEACTIVATION);

			btHingeConstraint *hinge = new btHingeConstraint(*previous_rb, *current_rb, pivotA, pivotB, axisA, axisB);
			hinge->setDbgDrawSize(btScalar(5.f));
			hinge->setLimit(-0.f*SIMD_HALF_PI, 0.f*SIMD_HALF_PI);// , 1.f, 0.3f, 0.f);
			hinge->setBreakingImpulseThreshold(break_limit);
			//hinge->setParam(BT_CONSTRAINT_CFM, 0.f, 0);
			hinge->setParam(BT_CONSTRAINT_STOP_CFM, btScalar(0.f), -1);
			// 0);
			//hinge->setParam(BT_CONSTRAINT_STOP_CFM, 0, 1);
			//hinge->setParam(BT_CONSTRAINT_STOP_CFM, 0, 2);
			//hinge->Limit
			app_scene->add_hinge_constraint(hinge,true);
		}

		void create_columns()
		{
			vec3 start = vec3(left_cliff_location.x() + cliff_half_size.x() - 2, left_cliff_location.y() + cliff_half_size.y(), left_cliff_location.z());
			vec3 end = vec3(right_cliff_location.x() - cliff_half_size.x() + 2, right_cliff_location.y() + cliff_half_size.y(), right_cliff_location.z());

			vec3 column_size = vec3(0.5f, 2.f, 0.5f);
			mat.loadIdentity();
			mat.translate(start + vec3(0, column_size.y()/2.f, 0));
			btRigidBody *left_column = app_scene->add_shape(mat, new mesh_box(column_size), new material(vec4(1, 1, 1, 1)), false)->get_node()->get_rigid_body();

			mat.loadIdentity();
			mat.translate(end + vec3(0, column_size.y()/2.f, 0));
			btRigidBody *right_column = app_scene->add_shape(mat, new mesh_box(column_size), new material(vec4(1, 1, 1, 1)), false)->get_node()->get_rigid_body();

			create_rope(left_column,right_column, start,end, column_size);
		}

		btRigidBody *create_rope_part(vec3 &location, material *part_material)
		{
			mat.loadIdentity();
			mat.translate(location);
			mesh_instance *mesh = app_scene->add_shape(mat, new mesh_box(vec3(rope_part_size)), part_material, true);
			return mesh->get_node()->get_rigid_body();
		}

		void create_rope(btRigidBody *left_column, btRigidBody *right_column, vec3 &start, vec3 &end, vec3 &column_size)
		{
			int parts_number = 80;

			rope_part_size = vec3((end - start).x() / 2.f/parts_number, 0.2f, 0.2f);
			material *rope_material = new material(vec4(0.9, 0.9, 0.9, 1));
			//cliff-bridge hinge
			vec3 from = start + vec3(0, 2 * column_size.y() + rope_part_size.y(), 0);
			vec3 to = end + vec3(0, 2 * column_size.y() + rope_part_size.y(), 0);

			vec3 location = vec3(from.x() + rope_part_size.x(), from.y(), from.z());
			btRigidBody *current, *previous = create_rope_part(location, rope_material);

			bind_bodies(left_column, column, previous, rope,10000.f);
			
			for (int i = 1; i < parts_number; i++)
			{
				location += 2 * rope_part_size.x();
				current = create_rope_part(location, rope_material);

				bind_bodies(previous, rope, current, rope, 10000.f);
				previous = current;

			}
			bind_bodies(previous, rope, right_column, column, 10000.f);
		}

	public:

		Bridge(ref<visual_scene> App_scene, mat4t &Mat, float Ground_level)
		{
			app_scene = App_scene;
			mat = Mat;
			ground_level = Ground_level;
			set_variables();
		}


		void create_bridge(int planks_number)
		{
			create_cliffs(planks_number);

			//cliff-bridge hinge
			vec3 location = vec3(left_cliff_location.x() + distance_between + plank_size.x() + cliff_half_size.x(),
				left_cliff_location.y() + cliff_half_size.y() - plank_half_size.y(), left_cliff_location.z());
			
			float break_limit = 20.f;

			btRigidBody *previous_body = create_plank(location);
			bind_bodies(left_cliff_rb, cliff, previous_body, plank, break_limit);

			btRigidBody *current_body;
			for (int i = 1; i < planks_number; i++)
			{
				location = vec3(location.x() + distance_between + plank_size.x(), location.y(), location.z());
				current_body = create_plank(location);
				bind_bodies(previous_body, plank, current_body, plank, break_limit);
				previous_body = current_body;
			}

			location = vec3(location.x() + distance_between + plank_size.x(), location.y(), location.z());

			//btTransform frameA;
			//frameA = btTransform::getIdentity();
			////frameA.setOrigin(btVector3(location.x() - cliff_size.x(), right_cliff_location.y() - cliff_size.y(),
			////	right_cliff_location.z() - cliff_size.z()));
			//frameA.setOrigin(btVector3(location.x(), location.y(), location.z()));
			//btHingeConstraint *h = new btHingeConstraint(*previous_body, frameA);
			//h->setDbgDrawSize(btScalar(distance_between));
			//h->setLimit(-0.5f*SIMD_HALF_PI, 0.5f*SIMD_HALF_PI);
			//app_scene->add_hinge_constraint(h, true);

			//bridge-bank hinge
			bind_bodies(previous_body, plank, right_cliff_rb, cliff, break_limit);

			//create_columns();
		}

		vec3 get_camera_starting_point()
		{
			//return (right_cliff_location);
			return (vec3((right_cliff_location.x() + left_cliff_location.x()) /2.f, right_cliff_location.y() + cliff_half_size.y(), right_cliff_location.z() - 0));
		}

		vec3 get_start()
		{
			return vec3(left_cliff_location.x(), left_cliff_location.y() + cliff_half_size.y(), left_cliff_location.z());
		}

		vec3 get_end()
		{
			return vec3(right_cliff_location.x(), right_cliff_location.y() + cliff_half_size.y(), right_cliff_location.z());
		}
	};
}
