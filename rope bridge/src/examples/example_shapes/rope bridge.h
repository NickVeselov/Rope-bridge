////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#include "bridge.h"
#include "collisions handler.h"
#include "read csv file.h"

namespace octet {
  /// Scene containing a box with octet.
  class example_shapes : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
	//bridge constructor
	Bridge *bridge;

	mouse_look mouse_look_helper;
	helper_fps_controller fps_helper;

	ref<scene_node> player_node;

	ref<camera_instance> the_camera;

	float number = 0;
	btRigidBody *ball;

	CollisionsHandler *ch;

  public:
    example_shapes(int argc, char **argv) : app(argc, argv) {
    }

    ~example_shapes() {
    }

    /// this is called once OpenGL is initialized
	void app_init() {
		mouse_look_helper.init(this, 100.f/360.f, false);
		fps_helper.init(this, vec3(10.f, 4.f, 0.f));
		
		app_scene = new visual_scene();
		app_scene->create_default_camera_and_lights();

		the_camera = app_scene->get_camera_instance(0);
		the_camera->get_node()->translate(vec3(10, 32, 0));
		//how far can you see
		the_camera->set_far_plane(1000);

		the_camera->get_node()->translate(vec3(0, 80, 80));

		// app_scene
		material *red = new material(vec4(1, 0, 0, 1));
		material *water = new material(vec4(0, 0, 1, 1));

		mat4t mat;

		// ground
		mat.loadIdentity();
		mat.translate(0, 0, 0);
		app_scene->add_shape(mat, new mesh_box(vec3(400, 1, 200)), water, false);

		CSVReader r;
		r.read_file("test.csv");

		//bridge
		bridge = new Bridge(app_scene, mat, 1);
		bridge->create_bridge(r.values);

		//ball
		mat.loadIdentity();
		mat.translate(bridge->get_camera_starting_point() + vec3(0, 10,0));
		//mesh_instance *ball_mesh = app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), red, true);
		
		//float player_height = 40.f;//1.83f;
		float player_radius = r.values[11];
		float player_mass = r.values[12];


		mesh_instance *ball_mesh = app_scene->add_shape(
			mat,
			new mesh_sphere(vec3(0), player_radius),
			red, true, player_mass
		);
		ball = ball_mesh->get_node()->get_rigid_body();
		
		ball->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		mat.loadIdentity();
		mat.translate(bridge->get_camera_starting_point() + vec3(0, 40, 0));
		mesh_instance *sun_mesh = app_scene->add_shape(
			mat,
			new mesh_sphere(vec3(0), player_radius),
			new material(vec4(1, 1, 0, 1)), true, player_mass
		);

		set_spring_constraint(sun_mesh->get_node()->get_rigid_body());

		ch = new CollisionsHandler(app_scene->get_world());

	}

	void set_spring_constraint(btRigidBody *sun_rb)
	{

	}

	void move_ball()
	{
		float ball_speed = 5.0f,
			jump_force = 20.f;
		if (is_key_down(key_up))
		{
			ball->applyForce(btVector3(ball_speed, 0.f, 0.f), btVector3(0, 0, 0));
		}
		else if (is_key_down(key_down))
		{
			ball->applyForce(btVector3(-ball_speed, 0.f, 0.f), btVector3(0, 0, 0));
		}
		else if (is_key_down(key_left))
		{
			ball->applyForce(btVector3(0.f, 0.f, -ball_speed), btVector3(0, 0, 0));
		}
		else if (is_key_down(key_right))
		{
			ball->applyForce(btVector3(0.f, 0.f, ball_speed), btVector3(0, 0, 0));
		}
		else if (is_key_down(key_space))
		{
			ball->applyForce(btVector3(0.f, jump_force, 0.f), btVector3(0, 0, 0));
		}
		

		
	}

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
	  app_scene->begin_render(vx, vy, vec4(0.53f, 0.8f, 0.976f, 0));
	  
	  //scene_node *camera_node = the_camera->get_node();
	 // mat4t &camera_to_world = camera_node->access_nodeToParent();
	  //mouse_look_helper.update(camera_to_world);

	  //fps_helper.update(player_node, camera_node);

	  // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

	  //move_ball();

      // draw the scene
      app_scene->render((float)vx / vy);
	 // number += 0.5f;
	  //app_scene->get_camera_instance(0)->get_node()->rotate(-1.f, vec3(1, 0, 0));
	  //app_scene->get_camera_instance(0)->get_node()->translate(vec3(0.f, 0.f, 0.1f));
	  //std::cout << number << std::endl;

	  //std::cout << app_scene->collisions_callback() << std::endl;
	  //app_scene->check_for_collisions();
	  //if (ch.collision_duration > 0)
		 // ch.collision_duration--;
	  ch->handle();
    }
  };
}
