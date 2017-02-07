#include "svip.hpp"
#include "StructuredPointCloud.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <GL/glew.h>

std::vector <std::shared_ptr<ModulesHotel::StructuredPointCloud> > g_structured_point_cloud_list;

void readData()
{
    for (size_t cam = 0; cam<CAMERAS_TO_USE; cam++) {

#ifdef COMPUTER_fangorn2
        const std::string absolute_data_path("/home/jnygaard/SVIPExportToJens/161114b");
#else
        const std::string absolute_data_path("/windows/U-osl/prosjekt/9011-Math/HETEROCOMP/SVIP/161114b");
#endif

        std::stringstream ss;
        ss << absolute_data_path << "/pcloud_camera_" << cam + 1 << ".bin";

        std::stringstream ss2;
        ss2 << absolute_data_path << "/T_matrix_" << cam + 1 << ".bin";

        std::stringstream ss3;
        ss3 << absolute_data_path << "/RGB_texture_" << cam + 1 << ".bin";

        std::stringstream ss4;
        ss4 << absolute_data_path << "/depth_matrix_" << cam + 1 << ".bin";

        std::stringstream ss5;
        ss5 << absolute_data_path << "/C_matrix_" << cam + 1 << ".bin";

        g_structured_point_cloud_list.push_back(
            std::make_shared<ModulesHotel::StructuredPointCloud>(
                ss.str(),    // File name for point cloud data
                ss2.str(),   // File name for the transformation T
                ss3.str(),   // File name for the RGB texture
                ss4.str(),   // File name for the depth matrix
                ss5.str(),   // File name for the 3x3 intrinsics C
                cam)
            );
    }
}


void initSVIP()
{
    glewInit();
    readData();

}

void renderSVIP(const float *p, const float *mv)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //        std::cout << "PM: " << glm::to_string( global_PM ) << std::endl;
    //        std::cout << "MV: " << glm::to_string( global_MV ) << std::endl;
    //        std::cout << std::endl;
    for (size_t cam = 0; cam<g_structured_point_cloud_list.size(); cam++) {
        if (g_structured_point_cloud_list[cam]->m_enabled) {

            g_structured_point_cloud_list[cam]->set_uniforms(); // Shouldn't have to do this for every frame...

                                                                // Same applies here, this should be set some other place. The reason why we do not pass these params through the render call is
                                                                // that we later on want to make use of the scene graph stuff, so we must probably conform to the render call signature given by that
//            glm::mat4 glm_p = glm::make_mat4(p);
            g_structured_point_cloud_list[cam]->render(glm::make_mat4(p), glm::make_mat4(mv)); // * glm::scale( glm::dmat4(1), glm::dvec3(1.0) ) );

        }
    }
}
