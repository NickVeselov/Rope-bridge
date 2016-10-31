#pragma once
namespace octet
{
	class Bridge
	{
		//scene
		ref<visual_scene> app_scene;
		mat4t mat;

		float distance_between;
		vec3 cliff_size;
		

		//brick model
		vec3 brick_size;

		btRigidBody *create_hinged_body(btRigidBody *previous_rb, vec3 location, int id)
		{
			material *brick_material = new material(vec4(1, 0, 0, 1));

			//create mesh
			mat.loadIdentity();
			mat.translate(location);
			mesh_instance *mesh;
			if (id != -1)
				mesh = app_scene->add_shape(mat, new mesh_box(vec3(brick_size)), brick_material, true);
			else
				mesh = app_scene->add_shape(mat, new mesh_box(vec3(cliff_size)), new material(vec4(0, 0, 1, 1)), true, 100.f);
			//rigid body
			btRigidBody *current_rb = mesh->get_node()->get_rigid_body();

			btVector3 axisA(0.f, 1.f, 0.f);
			btVector3 axisB(0.f, 1.f, 0.f);
			btVector3 pivotA;
			btVector3 pivotB;
			if (id == 0)
				pivotA = btVector3((cliff_size.x() + distance_between) / 2.f, cliff_size.y() - brick_size.y(), 0.f);
			else
				pivotA = btVector3((brick_size.x() + distance_between) / 2.f, 0.f, 0.f);

			if (id != -1)
				pivotB = btVector3(-(brick_size.x() + distance_between) / 2.f, 0.f, 0.f);
			else
				pivotB = btVector3(-(cliff_size.x() + distance_between) / 2.f, cliff_size.y() - brick_size.y(), 0.f);


			btHingeConstraint *hinge = new btHingeConstraint(*previous_rb, *current_rb, pivotA, pivotB, axisA, axisB);
			hinge->setDbgDrawSize(btScalar(5.f));
			app_scene->add_hinge_constraint(hinge, true);

			return current_rb;
		}

	public:

		Bridge(ref<visual_scene> App_scene, mat4t Mat)
		{
			app_scene = App_scene;
			mat = Mat;
		}

		void create_bridge(int bricks_number)
		{
			material *cliff = new material(vec4(0, 0, 1, 1));
			distance_between = 4.f;
			vec3 left_cliff_location = vec3(-10, 5, -5);
			cliff_size = vec3(5, 10, 10);

			mat.loadIdentity();
			mat.translate(left_cliff_location);
			mesh_instance *left_cliff_mesh = app_scene->add_shape(mat, new mesh_box(cliff_size), cliff, true, 100.f);
			btRigidBody *left_cliff_rb = left_cliff_mesh->get_node()->get_rigid_body();

			vec3 brick_size = vec3(1, 1, 6);
			//bank-bridge hinge
			vec3 location = vec3(left_cliff_location.x() + distance_between, left_cliff_location.y() + cliff_size.y() - brick_size.y(), left_cliff_location.z());
			btRigidBody *previous_body = create_hinged_body(left_cliff_rb, location, 0);


			//for (int i = 1; i < bricks_number; i++)
			//{
			//	location = vec3(location.x() + distance_between, location.y(), location.z());
			//	previous_body = create_hinged_body(previous_body, location, i);
			//}

			////bridge-bank hinge
			//location = vec3(location.x() + distance_between, location.y() - cliff_size.y() + brick_size.y(), location.z());
			//previous_body = create_hinged_body(previous_body, location, -1);

			//delete previous_body;
		}


	};
}
