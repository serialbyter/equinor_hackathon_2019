#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

#include "simulator/csv_parser.h"
#include "simulator/wall_merger.h"

struct Wall {
    int x,y,x_size,y_size; // (x,y) is the lower point of the box
    Wall(int x, int y, int x_size, int y_size)
        : x(x), y(y), x_size(x_size), y_size(y_size)
    {}
};

std::string WorldTagsBefore() {
    return "\
<sdf version='1.6'>\n\
  <world name='default'>\n\
    <light name='sun' type='directional'>\n\
      <cast_shadows>1</cast_shadows>\n\
      <pose frame=''>0 0 10 0 -0 0</pose>\n\
      <diffuse>0.8 0.8 0.8 1</diffuse>\n\
      <specular>0.1 0.1 0.1 1</specular>\n\
      <attenuation>\n\
        <range>1000</range>\n\
        <constant>0.9</constant>\n\
        <linear>0.01</linear>\n\
        <quadratic>0.001</quadratic>\n\
      </attenuation>\n\
      <direction>-0.5 0.5 -1</direction>\n\
    </light>\n\
    <model name='ground_plane'>\n\
      <static>1</static>\n\
      <link name='link'>\n\
        <collision name='collision'>\n\
          <geometry>\n\
            <plane>\n\
              <normal>0 0 1</normal>\n\
              <size>100 100</size>\n\
            </plane>\n\
          </geometry>\n\
          <surface>\n\
            <friction>\n\
              <ode>\n\
                <mu>100</mu>\n\
                <mu2>50</mu2>\n\
              </ode>\n\
              <torsional>\n\
                <ode/>\n\
              </torsional>\n\
            </friction>\n\
            <contact>\n\
              <ode/>\n\
            </contact>\n\
            <bounce/>\n\
          </surface>\n\
          <max_contacts>10</max_contacts>\n\
        </collision>\n\
        <visual name='visual'>\n\
          <cast_shadows>0</cast_shadows>\n\
          <geometry>\n\
            <plane>\n\
              <normal>0 0 1</normal>\n\
              <size>100 100</size>\n\
            </plane>\n\
          </geometry>\n\
          <material>\n\
            <script>\n\
              <uri>file://media/materials/scripts/gazebo.material</uri>\n\
              <name>Gazebo/Grass</name>\n\
            </script>\n\
          </material>\n\
        </visual>\n\
        <self_collide>0</self_collide>\n\
        <kinematic>0</kinematic>\n\
        <gravity>1</gravity>\n\
      </link>\n\
    </model>\n\
    <gravity>0 0 -9.8</gravity>\n\
    <magnetic_field>6e-06 2.3e-05 -4.2e-05</magnetic_field>\n\
    <atmosphere type='adiabatic'/>\n\
    <physics name='default_physics' default='0' type='ode'>\n\
      <max_step_size>0.004</max_step_size>\n\
      <real_time_factor>1</real_time_factor>\n\
      <real_time_update_rate>250</real_time_update_rate>\n\
    </physics>\n\
    <scene>\n\
      <ambient>0.4 0.4 0.4 1</ambient>\n\
      <background>0.7 0.7 0.7 1</background>\n\
      <shadows>1</shadows>\n\
      <sky>\n\
        <clouds>\n\
          <speed>0.6</speed>\n\
          <direction>0</direction>\n\
          <humidity>0.5</humidity>\n\
          <mean_size>0.5</mean_size>\n\
          <ambient>0.8 0.8 0.8 1</ambient>\n\
        </clouds>\n\
      </sky>\n\
    </scene>\n\
    <spherical_coordinates>\n\
      <surface_model>EARTH_WGS84</surface_model>\n\
      <latitude_deg>0</latitude_deg>\n\
      <longitude_deg>0</longitude_deg>\n\
      <elevation>0</elevation>\n\
      <heading_deg>0</heading_deg>\n\
    </spherical_coordinates>\n\
    \n\
    <include>\n\
        <uri>model://equidrone</uri>\n\
        <pose>0 0 0.05 0 0 0</pose>\n\
    </include>\n";
}

std::string DynamicObstacleTag(int x, int y) {
    std::string tag = "\
    <include>\n\
        <uri>model://dynamic_obstacle</uri>\n\
        <name>dynamic_obstacle" + std::to_string(x) + std::to_string(y) + "</name>\n\
        <pose>";
    tag += std::to_string(x) + ".5 " + std::to_string(y) + ".5 ";
    tag += "2 0 0 1.57</pose>\n\
    </include>";
        
    return tag;
}


std::string WorldTagsAfter() {
    
  return "\
    <gui fullscreen='0'>\n\
      <camera name='user_camera'>\n\
        <pose frame=''>1.45228 -10.4475 11.1878 0 0.455643 1.12819</pose>\n\
        <view_controller>orbit</view_controller>\n\
        <projection_type>perspective</projection_type>\n\
      </camera>\n\
    </gui>\n\
  </world>\n\
</sdf>";
}


std::string WallTag(const Wall& wall) {
    float x_mid = (float)wall.x + (float)wall.x_size/2.f;
    float y_mid = (float)wall.y + (float)wall.y_size/2.f;
    std::string tag = "\
<model name=\"wall\">\n\
    <pose frame=''>" + std::to_string(x_mid) + " " + std::to_string(y_mid) + " 2 0 0 0</pose>\n\
    <static>1</static>\n\
    <self_collide>0</self_collide>\n\
    <link name='wall-link'>\n\
        <collision name='collision'>\n\
            <geometry>\n\
                <box>\n\
                    <size>" + std::to_string(wall.x_size) + " " + std::to_string(wall.y_size) + " 4</size>\n\
                </box>\n\
            </geometry>\n\
        </collision>\n\
        <visual name='visual'>\n\
            <geometry>\n\
                <box>\n\
                    <size>" + std::to_string(wall.x_size) + " " + std::to_string(wall.y_size) + " 4</size>\n\
                </box>\n\
            </geometry>\n\
            <material><script>\n\
                <name>Gazebo/Grey</name>\n\
                <uri>file://media/materials/scripts/gazebo.material</uri>\n\
            </script></material>\n\
        </visual>\
        <gravity>0</gravity>\n\
    </link>\n\
</model>";
    
    return tag;
}

std::string StartTag(int x, int y) {
    
    std::string tag = "\
    <include>\n\
        <uri>model://equidrone</uri>\n\
        <pose>";
    
    tag += std::to_string(x) + ".5 " + std::to_string(y) + ".5 ";
    
    tag += "0.05 0 0 1.57</pose>\n\
    </include>";
        
    return tag;
}

std::string GoalTag(int x, int y) {
    std::string tag = "<model name='goal'>\n\
            <pose frame=''>";
    tag += std::to_string(x) + ".5 ";
    tag += std::to_string(y) + ".5 ";
    tag += "0.05 0 -0 0</pose>\n\
      <static>1</static>\n\
      <link name='link'>\n\
        <collision name='collision'>\n\
          <geometry>\n\
            <box>\n\
              <size>2.0 2.0 0.1</size>\n\
            </box>\n\
          </geometry>\n\
        </collision>\n\
        <visual name='visual'>\n\
          <geometry>\n\
            <box>\n\
              <size>2.0 2.0 0.1</size>\n\
            </box>\n\
          </geometry>\n\
          <material>\n\
            <script>\n\
              <name>Gazebo/Grey</name>\n\
              <uri>file://media/materials/scripts/gazebo.material</uri>\n\
            </script>\n\
          </material>\n\
        </visual>\n\
      </link>\n\
    </model>";
    return tag;
}

std::string NumberTag(int x, int y, int num) {
    std::string tag =  "<include>\n\
        \t<uri>model://numberbox" + std::to_string(num) + "</uri>\n\
        \t<pose frame=''>";
    tag += std::to_string(x) + ".5 ";
    tag += std::to_string(y) + ".5 ";
    tag += "2 0 -0 0</pose>\n\
    </include>\n";
    
    return tag;
}

std::vector<std::pair<int,int>> ExtractWalls(const std::vector<std::vector<MapCell>>& gridmap) {
    std::vector<std::pair<int,int>> walls;
    for (int y = 0; y < gridmap.size(); y++){
        const auto& row = gridmap[y];
        for (int x = 0; x < row.size(); x++){
            if (row[x].isWall()) {
                walls.emplace_back(std::make_pair(x,y));
            }
        }
    }
    
    return walls;
}


std::vector<Wall> ReduceWalls(std::vector<std::vector<MapCell>> gridwalls) {
    std::vector<Wall> reduced;
    for (int y = 0; y < gridwalls.size(); y++) {
        for (int x = 0; x < gridwalls[y].size(); x++) {
            if (gridwalls[y][x].isWall()) {
                const int stepX = expandX(gridwalls, x,y);
                const int stepY = expandY(gridwalls, x,y);
                const int stepDiag = expandDiag(gridwalls, x,y);
                
                if (stepX > stepY && stepX > stepDiag*stepDiag) {
                    reduced.emplace_back(x,y,1 + stepX,1);
                    
                    // remove walls from gridwalls to avoid duplicates
                    for (int i = 0; i <= stepX; i++) {
                        gridwalls[y][x + i] = MapCell(MapCell::Type::EMPTY);
                    }
                }
                else if (stepY > stepX && stepY > stepDiag*stepDiag) {
                    reduced.emplace_back(x,y,1,1+stepY);
                    
                    // remove walls from gridwalls to avoid duplicates
                    for (int i = 0; i <= stepY; i++) {
                        gridwalls[y + i][x] = MapCell(MapCell::Type::EMPTY);
                    }
                }
                else if (stepDiag*stepDiag > 0) {
                    reduced.emplace_back(x,y,1 + stepDiag,1 + stepDiag);
                    
                    // remove walls from gridwalls to avoid duplicates
                    for (int i = 0; i <= stepDiag; i++) {
                        for (int j = 0; j <= stepDiag; j++) {
                            gridwalls[y + j][x + i] = MapCell(MapCell::Type::EMPTY);
                        }
                    }
                }
                else {
                    reduced.emplace_back(x,y,1,1);
                }
            }
        }
    }
    
    return reduced;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " inputfile.csv" << " outputfile.world" << std::endl;
        return 1;
    }
    const auto infile = argv[1];
    const auto outfile = argv[2];

    const auto gridmap = ParseCSVMap(infile);
    const auto gridwalls = ExtractWalls(gridmap); // only for debug
    const auto walls = ReduceWalls(gridmap);
    
    std::cout << "Reduced " << gridwalls.size() << " 1x1 walls to " << walls.size() << " walls" << std::endl;
    
    std::ofstream output;
    output.open(outfile);

    output << WorldTagsBefore() << '\n';

    for (const auto wall : walls) {
        output << WallTag(wall) << '\n';
    }
    
    bool print_warning = false;
    for (int y = 0; y < gridmap.size(); y++){
        const auto& row = gridmap[y];
        std::cout << "\n";
        for (int x = 0; x < row.size(); x++){
            switch (row[x].getType()){
            case MapCell::Type::WALL:
                std::cout << "w";
                break;
            case MapCell::Type::START:
                std::cout << "s";
                //output << StartTag(x, y) << '\n';
                throw std::invalid_argument("start tag is depracated");
                break;
            case MapCell::Type::GOAL:
                std::cout << "g";
                output << GoalTag(x, y) << '\n';
                break;
            case MapCell::Type::OBJECT:
                if (1 <= row.at(x).getNum() && row.at(x).getNum() <= 9) {
                    std::cout << row.at(x).getNum();
                    output << NumberTag(x,y,row.at(x).getNum()) << '\n';
                }
                else {
                    std::cout << '?';
                    print_warning = true;
                }
                break;
            case MapCell::Type::DYNAMIC_OBSTACLE:
                std::cout << 'd';
                output << DynamicObstacleTag(x,y) << '\n';
                break;
            case MapCell::Type::EMPTY:
                std::cout << " ";
                break;
            default:
                throw std::invalid_argument("invalid enum case");
            }
        }
    }

    
    output << WorldTagsAfter() << std::endl;
    
    output.close();
    
    if (print_warning)
        std::cout << "\nWARNING: some object not drawn";
    
    std::cout << std::endl;
    

    return 0;
}

