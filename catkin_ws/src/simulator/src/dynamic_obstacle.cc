#include <functional> 
#include <random> 
#include <gazebo/gazebo.hh> 
#include <gazebo/physics/physics.hh> 
#include <gazebo/common/common.hh> 
#include <ignition/math/Vector3.hh>

namespace gazebo {
  class DynamicObstacle : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
            {
              this->model = _parent;

              if (_sdf->HasElement("altitude")) {
                altitude = _sdf->GetElement("altitude")->Get<float>();
              }

              if (_sdf->HasElement("speed")) {
                speed = _sdf->GetElement("speed")->Get<float>();
              }

              std::random_device rd;
              this->generator = std::mt19937(rd());
              this->distribution = std::uniform_real_distribution<float>(-1.0, 1.0);
              const auto x = this->distribution(this->generator);
              const auto y = this->distribution(this->generator);
              vel = ignition::math::Vector3d{x, y, 0.0};

              this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                  std::bind(&DynamicObstacle::OnUpdate, this));
            }
    public: void OnUpdate()
            {
              const auto curr_vel = this->model->WorldLinearVel();
              if (curr_vel.Distance(vel) > speed/10.f) {
                const auto rand_x = distribution(generator)*speed*3;
                const auto rand_y = distribution(generator)*speed*3;
                auto rand_vel = ignition::math::Vector3d{rand_x, rand_y, 0.0};
                this->vel = vel + rand_vel;
                this->vel = -speed*vel.Normalized();
              }

              this->model->SetLinearVel(vel);
            }

    private: ignition::math::Vector3d vel;
    private: physics::ModelPtr model;
    private: event::ConnectionPtr updateConnection;
    private: float speed=1.f, altitude=2.5f;
    private: std::mt19937 generator;
    private: std::uniform_real_distribution<float> distribution;
  };


  GZ_REGISTER_MODEL_PLUGIN(DynamicObstacle);
}
