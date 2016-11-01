#pragma once
namespace octet
{
	enum object_type
	{
		plank,
		cliff,
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

		//cliff
		material *cliff_material;
		vec3 cliff_size;

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
			distance_between = 4.0f;

			plank_size = vec3(1, 0.5f, 6);
			plank_material = new material(vec4(1, 0, 0, 1));

			cliff_material = new material(vec4(0, 0, 1, 1));
			cliff_size = vec3(5, 10, 10);

		}

		void create_cliffs(int size)
		{
			mat.loadIdentity();
			left_cliff_location = vec3(-10, cliff_size.y() + ground_level, 0);
			mat.translate(left_cliff_location);
			left_cliff_mesh = app_scene->add_shape(mat, new mesh_box(cliff_size), cliff_material,false);
			left_cliff_rb = left_cliff_mesh->get_node()->get_rigid_body();

			mat.loadIdentity();
			right_cliff_location = vec3(left_cliff_location.x() + distance_between + plank_size.x()/2.f + cliff_size.x()/2.f + (size - 1)*(distance_between + cliff_size.x()/2.f),
				left_cliff_location.y(), left_cliff_location.z());
			mat.translate(right_cliff_location);
			right_cliff_mesh = app_scene->add_shape(mat, new mesh_box(cliff_size), cliff_material, false);
			right_cliff_rb = right_cliff_mesh->get_node()->get_rigid_body();
		}


		btRigidBody *create_plank(vec3 location)
		{
			mat.loadIdentity();
			mat.translate(location);
			mesh_instance *mesh = app_scene->add_shape(mat, new mesh_box(vec3(plank_size)), plank_material, true);
			return mesh->get_node()->get_rigid_body();
		}

		void bind_bodies(btRigidBody *previous_rb, object_type previos_type, btRigidBody *current_rb, object_type current_type)
		{
			float previous_object_width = 0.f,
				previous_vertical_align = 0.f,
				current_object_width = 0.f,
				current_vertical_align = 0.f;
				
			if (previos_type == plank)
			{
				previous_object_width = plank_size.x();
			}
			else
			{
				previous_object_width = cliff_size.x();
				previous_vertical_align = cliff_size.y() - plank_size.y();

			}

			if (current_type == plank)
			{
				current_object_width = plank_size.x();
			}
			else
			{
				current_object_width = cliff_size.x();
				current_vertical_align = cliff_size.y() - plank_size.y();
			}
			btVector3 pivotA((previous_object_width + distance_between) / 2.f, previous_vertical_align, 0.f);
			btVector3 pivotB(-(current_object_width + distance_between) / 2.f, current_vertical_align, 0.f);
			btVector3 axisA(0.f, 0.f, 1.f);
			btVector3 axisB(0.f, 0.f, 1.f);

			btHingeConstraint *hinge = new btHingeConstraint(*previous_rb, *current_rb, pivotA, pivotB, axisA, axisB);
			hinge->setDbgDrawSize(btScalar(distance_between));
			hinge->setLimit(-0.5f*SIMD_HALF_PI, 0.5f*SIMD_HALF_PI);
			//hinge->set
			app_scene->add_hinge_constraint(hinge, true);
		}

	public:

		Bridge(ref<visual_scene> App_scene, mat4t Mat, float Ground_level)
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
			vec3 location = vec3(left_cliff_location.x() + distance_between + plank_size.x()/2.f + cliff_size.x()/2.f,
				left_cliff_location.y() + cliff_size.y() - plank_size.y(), left_cliff_location.z());
			
			

			btRigidBody *previous_body = create_plank(location);
			bind_bodies(left_cliff_rb, cliff, previous_body, plank);

			btRigidBody *current_body;
			for (int i = 1; i < planks_number; i++)
			{
				location = vec3(location.x() + distance_between, location.y(), location.z());
				current_body = create_plank(location);
				bind_bodies(previous_body, plank, current_body, plank);
				previous_body = current_body;
			}

			location = vec3(location.x() + distance_between, location.y(), location.z());

			btTransform frameA;
			frameA = btTransform::getIdentity();
			//frameA.setOrigin(btVector3(location.x() - cliff_size.x(), right_cliff_location.y() - cliff_size.y(),
			//	right_cliff_location.z() - cliff_size.z()));
			frameA.setOrigin(btVector3(location.x(), location.y(), location.z()));
			//= TransformState.makePosHpr(Point3(0, 0, -5), Vec3(0, 0, -90))

			//bridge-bank hinge
			bind_bodies(previous_body, plank, right_cliff_rb, cliff);
			//btHingeConstraint *h = new btHingeConstraint(*previous_body, frameA);
			//h->setDbgDrawSize(btScalar(distance_between));
			//h->setLimit(-0.5f*SIMD_HALF_PI, 0.5f*SIMD_HALF_PI);
			////h->set
			//app_scene->add_hinge_constraint(h, true);
		}


	};
}
