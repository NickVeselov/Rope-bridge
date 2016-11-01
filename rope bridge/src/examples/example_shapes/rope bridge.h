////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#include "bridge.h"

namespace octet {
  /// Scene containing a box with octet.
  class example_shapes : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
	Bridge *bridge;
  public:
    example_shapes(int argc, char **argv) : app(argc, argv) {
    }

    ~example_shapes() {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
	  //app_scene->get_camera_instance(0)->
      app_scene->get_camera_instance(0)->get_node()->translate(vec3(10, 25, 35));
	 // app_scene
      material *red = new material(vec4(1, 0, 0, 1));
      material *water = new material(vec4(0, 0, 1, 1));
      
      mat4t mat;
      //mat.translate(-10, -5, 0);
      //app_scene->add_shape(mat, new mesh_box(vec3(2, 3, 2)), blue, true);
	  //mat.loadIdentity();
	  //mat.translate(10, -5, 0);
	  //mesh_instance *right_cliff_mesh = app_scene->add_shape(mat, new mesh_box(vec3(2, 3, 2)), blue, true);
	  //btRigidBody *right_cliff_rb = right_cliff_mesh->get_node()->get_rigid_body();	  
	  //mat.loadIdentity();
	  //mat.translate( 3, 6, 0);
	  //app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 2, 4)), blue, true);
	  // ground

	  mat.loadIdentity();
	  mat.translate(0, 0, 0);
	  app_scene->add_shape(mat, new mesh_box(vec3(100, 1, 200)), water, false);
	  bridge = new Bridge(app_scene, mat, 1);
	  bridge->create_bridge(20);

	  mat.loadIdentity();
	  mat.translate(-10, 30, 0);
	  app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2),2), red, true);
	  mat.loadIdentity();
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy, vec4(0.53f, 0.8f, 0.976f, 0));

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);

	  //app_scene->get_camera_instance(0)->get_node()->rotate(0.1f, vec3(0, 0, 1));

    }
  };
}
