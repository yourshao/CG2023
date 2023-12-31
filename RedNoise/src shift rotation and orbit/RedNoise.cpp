#include <iostream>
#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>
#include <CanvasTriangle.h>
#include <ModelTriangle.h>
#include <sstream>
#include <fstream>
#include <math.h>
#define WIDTH 320
#define HEIGHT 240






//
// Created by 邵振群 on 2023/11/20.
//



//void Drawanother(CanvasPoint from, CanvasPoint to, DrawingWindow &window, Colour color){
//    float xDiff = to.x - from.x;
//    float yDiff = to.y - from.y;
//    float numberOfSteps = fmax(abs(xDiff), abs(yDiff));
//    float xStepSize =  xDiff/numberOfSteps;
//    float yStepSize =  yDiff/numberOfSteps;
//    for (float i = 0.0; i <= numberOfSteps; i++) {
//        float x = from.x + (xStepSize*i);
//        float y = from.y + (yStepSize*i);
//        uint32_t colour = (255 << 24) + (color.red << 16) + (color.green << 8) + color.blue;
//
//        window.setPixelColour(round(x), round(y), colour);
//    }
//}

//for grey spectrum

//void drawTriangle(CanvasPoint v0, CanvasPoint v1, CanvasPoint v2, DrawingWindow &window) {
//    Drawline(v0, v1, window);
//    Drawline(v1, v2, window);
//    Drawline(v2, v0, window);
//}
//
//void drawTriangles(DrawingWindow &window, const std::vector<CanvasTriangle> &triangles) {
//    window.clearPixels();
//    for (const auto &triangle: triangles) {
//        drawTriangle(const_cast<CanvasTriangle &>(triangle).v0(),
//                     const_cast<CanvasTriangle &>(triangle).v1(),
//                     const_cast<CanvasTriangle &>(triangle).v2(), window);
//    }
//}

//void drawPoint(const CanvasPoint& point, DrawingWindow& window) {
//    uint32_t colour = (255 << 24) + (255 << 16) + (255 << 8) + 255;
//    window.setPixelColour(round(point.x), round(point.y), colour);
//}
//
//uint32_t generateRandomColor() {
//    uint8_t r = rand() % 256;
//    uint8_t g = rand() % 256;
//    uint8_t b = rand() % 256;
//    return (255 << 24) + (r << 16) + (g << 8) + b;
//}
//
//

//int numberOfValues(CanvasPoint v1, CanvasPoint v2){
//    return std::max({std::abs(v1.x - v2.x), std::abs(v1.y - v2.y)});
//}
//
//
//std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues){
//    std::vector<float> result;
//    float difference = (to - from) / (numberOfValues - 1);
//    for (int a = 0; a < numberOfValues; a++){
//        result.push_back(from + a * difference);
//    }
//    return result;
//}


float interpolate(float y0, float x0, float y1, float x1, float y) {
    if (y0 == y1) {
        // 如果两个y坐标相等，直接返回其中一个x坐标
        return x0;
    }

    return x0 + (x1 - x0) * (y - y0) / (y1 - y0);
}


// 检查点是否在三角形内
bool isPointInTriangle(int x, int y, float x0, float y0, float x1, float y1, float x2, float y2) {
    float d0 = (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
    float d1 = (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);
    float d2 = (x - x2) * (y0 - y2) - (y - y2) * (x0 - x2);

    return (d0 >= 0 && d1 >= 0 && d2 >= 0) || (d0 <= 0 && d1 <= 0 && d2 <= 0);
}


glm::vec3 barycentric(CanvasTriangle triangle, int x, int y) {
    glm::vec3 u = cross(glm::vec3(triangle.v2().x-triangle.v0().x, triangle.v1().x-triangle.v0().x, triangle.v0().x- x),
                        glm::vec3(triangle.v2().y-triangle.v0().y, triangle.v1().y-triangle.v0().y, triangle.v0().y- y));
    if (std::abs(u.z)<1) return glm::vec3({-1,1,1}); // 避免除以0的错误
    return glm::vec3(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
}



void drawTriangle(CanvasTriangle triangle, DrawingWindow &window, Colour color, float (*zBuffer)[WIDTH][HEIGHT]) {

    // 找到三角形的最小和最大y坐标
    int minY = std::min({triangle.v0().y, triangle.v1().y, triangle.v2().y});
    int maxY = std::max({triangle.v0().y, triangle.v1().y, triangle.v2().y});


    // 对每一行进行扫描
    for (int y = minY; y <= maxY; ++y) {
        // 找到当前行与三角形边的交点
        float x01 = interpolate(triangle.v0().y, triangle.v0().x, triangle.v1().y, triangle.v1().x, y);
        float x12 = interpolate(triangle.v1().y, triangle.v1().x, triangle.v2().y, triangle.v2().x, y);
        float x20 = interpolate(triangle.v2().y, triangle.v2().x, triangle.v0().y, triangle.v0().x, y);

        // 找到当前行的最小和最大x坐标
        int minX = std::min({floor(x01), floor(x12), floor(x20)});
        int maxX = std::max({floor(x01), floor(x12), floor(x20)});

        // 对当前行的每个像素进行遍历
        for (int x = minX; x <= maxX; ++x) {
            // 检查像素是否在三角形内
//            int firstX  = -1;
            if (isPointInTriangle(x, y, triangle.v0().x, triangle.v0().y, triangle.v1().x, triangle.v1().y, triangle.v2().x, triangle.v2().y)) {
//                if (firstX == -1 ) firstX = x ;
//===================
                float interpolatedDepth;
//                int numberOfStepsv0v2 =numberOfValues(triangle.v0(), triangle.v2());
//                int numberOfStepsv1v2 =numberOfValues(triangle.v1(), triangle.v2());
//                int numberOfStepsv0Now = numberOfValues(triangle.v0(), CanvasPoint(firstX, y));
//                int numberOfStepsv1Now = numberOfValues(triangle.v1(), CanvasPoint(x, y));
//                std::vector<float> deepthOfv0v2 = interpolateSingleFloats(triangle.v0().depth, triangle.v2().depth, numberOfStepsv0v2);
//                std::vector<float> deepthOfv1v2 = interpolateSingleFloats(triangle.v1().depth, triangle.v2().depth, numberOfStepsv1v2);
//                deepthOfv0v2(numberOfStepsv0Now), numberOfStepsv1v2(numberOfStepsv1Now),
//
//
    // 计算当前像素的深度
                if (triangle.v0().y != triangle.v2().y && triangle.v1().y != triangle.v2().y) {
//                    std::cout << "aaa" << std::endl;

                    float interpolatedDepthof02 = interpolate(triangle.v0().y, triangle.v0().depth, triangle.v2().y, triangle.v2().depth, y);
                    float interpolatedDepthof12 = interpolate(triangle.v1().y, triangle.v1().depth, triangle.v2().y, triangle.v2().depth, y);
                    float interpolatedx02 = interpolate(triangle.v0().y, triangle.v0().x, triangle.v2().y, triangle.v2().x, y);
                    float interpolatedx12 = interpolate(triangle.v1().y, triangle.v1().x, triangle.v2().y, triangle.v2().x, y);
                    interpolatedDepth = interpolate(interpolatedx02,interpolatedDepthof02,interpolatedx12,interpolatedDepthof12, x);
//                    std::cout << "bbb" << std::endl;

                } else{

                    float interpolatedDepthof02 = interpolate(triangle.v0().x, triangle.v0().depth, triangle.v2().x, triangle.v2().depth, x);
                    float interpolatedDepthof12 = interpolate(triangle.v1().x, triangle.v1().depth, triangle.v2().x, triangle.v2().depth, x);
                    float interpolatedy02 = interpolate(triangle.v0().x, triangle.v0().y, triangle.v2().x, triangle.v2().y, x);
                    float interpolatedy12 = interpolate(triangle.v1().x, triangle.v1().y, triangle.v2().x, triangle.v2().y, x);
                    interpolatedDepth = interpolate(interpolatedy02,interpolatedDepthof02,interpolatedy12,interpolatedDepthof12, y);
                }

//                glm::vec3 weight =  barycentric(triangle, x, y);
//                interpolatedDepth = weight[1]*triangle.v1().depth + weight[0] * triangle.v0().depth + weight[2] * triangle.v2().depth;
//                if(weight[0] < 0 || weight [1] < 0 || weight[2] < 0 ) continue;

                if (x<=0 || x>= WIDTH) return;
                if ((*zBuffer)[x][y] >interpolatedDepth) {
                    // 更新深度信息
                    (*zBuffer)[x][y] = interpolatedDepth;
                    uint32_t thisColor = (255 << 24) + (color.red << 16) + (color.green << 8) + color.blue;
                    window.setPixelColour(x, y, thisColor);
                }
            }
        }
    }
}


void drawTriangles(DrawingWindow &window, std::vector<CanvasTriangle> &triangles, std::vector<Colour> &colors) {
    // Initialize zBuffer
    static float zBuffer[WIDTH][HEIGHT]; // Make zBuffer static if you don't want to pass it around

    // Reset zBuffer for the new frame
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            zBuffer[i][j] = std::numeric_limits<float>::infinity();
        }
    }
    for (size_t i = 0; i < triangles.size(); ++i) {
        drawTriangle(triangles[i], window, colors[i], &zBuffer);

    }

}

//CanvasPoint creatOnePoint() {
//    float yPoint = rand() % HEIGHT;
//    float xPoint = rand() % WIDTH;
//    return CanvasPoint(xPoint, yPoint);
//}

// 将0到1的数值映射到RGB范围
void mapToRGB(Colour *currentcolor, float r, float g, float b) {
    currentcolor->red = static_cast<int>(r * 255);
    currentcolor->green = static_cast<int>(g * 255);
    currentcolor->blue = static_cast<int>(b * 255);
}

std::vector<ModelTriangle> loadModel(const std::string& objFilePath, const std::string& mtlFilePath) {
    std::ifstream objFile(objFilePath);
    std::ifstream mtlFile(mtlFilePath);

    std::vector<ModelTriangle> triangles;
    std::vector<Colour> colors;

    Colour currentColor;

    std::string line;
    while (std::getline(mtlFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "newmtl"){
            std::string colorName;
            iss >> colorName;
            currentColor.name = colorName;

        }else if (type == "Kd") {
            float r, g, b;
            iss >> r >> g >> b;
//            mapToRGB(&currentColor, r, g, b);
            currentColor.red = static_cast<int>(r * 255);
            currentColor.green = static_cast<int>(g * 255);
            currentColor.blue = static_cast<int>(b * 255);

            colors.push_back(currentColor);
        }
    }

    // 存储顶点的数组
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> vertexIndices;

    while (std::getline(objFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "usemtl") {
            // 设置当前使用的颜色
            std::string colorName;
            iss >> colorName;
            for (const Colour &c: colors) {
                if (c.name == colorName) {
                    currentColor = c;
                    break;
                }
            }
        }else if (type == "f") {
            // 读取面信息
            glm::vec3 f;
            iss >> f.x;
            iss.ignore();
            iss >> f.y;
            iss.ignore();
            iss  >> f.z;
            iss.ignore();

            vertexIndices.push_back(f); // OBJ文件索引从1开始，需要减1

            // 构建三角形并添加到数组中
            for (size_t i = 0; i < vertexIndices.size(); ++i) {
                ModelTriangle triangle(vertices[vertexIndices[i].x - 1 ], vertices[vertexIndices[i].y -1 ], vertices[vertexIndices[i].z - 1], currentColor);
                triangles.push_back(triangle);
            }
        } else if (type == "v") {
            // 读取顶点信息
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
    }
    return triangles;
}

CanvasPoint getCanvasIntersectionPoint(const glm::vec3& cameraPosition, const glm::vec3& vertexPosition, int focalLength, glm::mat3& cameraOrientation){

    glm::vec3 adjustedVector = ( cameraPosition - vertexPosition) * cameraOrientation ;

    float u = focalLength * -adjustedVector.x / adjustedVector.z * 30 + WIDTH/2;
    float v = focalLength * adjustedVector.y / adjustedVector.z * 30 + HEIGHT/2;

//    float u = focalLength* (vertexPosition.x - cameraPosition.x)  / (cameraPosition.z - vertexPosition.z)*30  + WIDTH/2 ;
//    float v = focalLength* (vertexPosition.y - cameraPosition.y) / (cameraPosition.z - vertexPosition.z)*30 + HEIGHT/2 ;

    float scale = 1.0;

    CanvasPoint point( u*scale, v*scale);
    return point;
}

std::vector<CanvasTriangle> projectionTrianglePoint(std::vector<ModelTriangle> threeDtriangle, const glm::vec3& cameraPosition, glm::mat3 &camaraOrientation){
    std::vector<CanvasTriangle> result;
    for (const auto &triangle : threeDtriangle) {

        CanvasPoint point1 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[0], 14, camaraOrientation);//前一位越大越没有深度，后一位越大越近
        point1.depth = std::sqrt(pow(triangle.vertices[0].x - cameraPosition.x, 2) + pow(triangle.vertices[0].z - cameraPosition.z, 2));

        CanvasPoint point2 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[1], 14, camaraOrientation);
        point2.depth = std::sqrt(pow(triangle.vertices[1].x - cameraPosition.x, 2) + pow(triangle.vertices[1].z- cameraPosition.z, 2));

        CanvasPoint point3 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[2], 14, camaraOrientation);
        point3.depth = std::sqrt(pow(triangle.vertices[2].x - cameraPosition.x, 2) + pow(triangle.vertices[2].z - cameraPosition.z, 2));

        CanvasTriangle newTriangle( point1 , point2 , point3 );
        result.push_back(newTriangle);

    }

    return result;
}

std::vector<Colour> projectionTriangleColour(std::vector<ModelTriangle> threeDtriangle){
    std::vector<Colour> result;
    for(const auto &triangle : threeDtriangle){
        Colour color = triangle.colour;
        result.push_back(color);

    }
    return result;
}


void lookAt(glm::mat3& cameraOrientation, glm::vec3& cameraPosition, float angleIncrement){
// 更新角度
glm::mat3 rotationMatrix ( glm::vec3(cos(-angleIncrement), 0.0, sin(-angleIncrement)),
                           glm::vec3(0.0, 1.0, 0.0),
                           glm::vec3(-sin(-angleIncrement), 0.0, cos(-angleIncrement))
);

cameraOrientation = rotationMatrix * cameraOrientation;
}


std::vector<CanvasTriangle> cameraOrbit(std::vector<CanvasTriangle> &triangles ,std::vector<ModelTriangle> &TDtriangles, glm::vec3& cameraPosition, glm::mat3 &cameraOrientation,DrawingWindow &window, std::vector<Colour> &colors ){

    float currentAngle = glm::radians(270.0f);
    // 定义圆的半径和摄像机移动的角度增量
    float radius = cameraPosition.z;
    float angleIncrement = -glm::radians(1.0f); // 每次移动5度

    while(true){
        currentAngle += angleIncrement;
        if (currentAngle <= 0.0f) {
            currentAngle = glm::radians(360.0f);
        }
// 计算新的摄像机位置
        cameraPosition.x = radius * cos(currentAngle);
        cameraPosition.z = -radius * sin(currentAngle);
//
        // 更新角度
//
//        glm::mat3 rotationMatrix ( glm::vec3(cos(-angleIncrement), 0.0, sin(-angleIncrement)),
//                                   glm::vec3(0.0, 1.0, 0.0),
//                                   glm::vec3(-sin(-angleIncrement), 0.0, cos(-angleIncrement))
//        );
//
//        cameraOrientation = rotationMatrix * cameraOrientation;

        lookAt(cameraOrientation, cameraPosition, angleIncrement);

        triangles =  projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
        window.clearPixels();
        drawTriangles(window,triangles, colors);
        window.renderFrame();

    }
}


void handleEvent(SDL_Event event, DrawingWindow &window, std::vector<CanvasTriangle> &triangles, glm::vec3& cameraPosition, std::vector<ModelTriangle> &TDtriangles, glm::mat3& cameraOrientation, std::vector<Colour> &colors ) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) {
            cameraPosition.x -= 1;
            triangles =  projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
            std::cout << "LEFT" << std::endl;
        }
        else if (event.key.keysym.sym == SDLK_RIGHT)
        {
            cameraPosition.x += 1;
            triangles =  projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
            std::cout << "RIGHT" << std::endl;
        }
        else if (event.key.keysym.sym == SDLK_UP)
        {
            cameraPosition.y += 1;
            triangles =  projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
            std::cout << "UP" << std::endl;
        }
        else if (event.key.keysym.sym == SDLK_DOWN)
        {
            cameraPosition.y -= 1;
            triangles =  projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
            std::cout << "DOWN" << std::endl;
        }
        else if(event.key.keysym.sym == SDLK_q)
        {
            cameraPosition.z -= 1;
            triangles =  projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
            std::cout << "DOWN" << std::endl;
        } else if(event.key.keysym.sym == SDLK_e)
        {
            cameraPosition.z += 1;
            triangles =  projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
            std::cout << "DOWN" << std::endl;
        }

        else if(event.key.keysym.sym == SDLK_w)
        {
            glm::mat3 rotationMatrix (glm::vec3(1.0, 0.0, 0.0),
                                      glm::vec3(0.0, cos(0.1), sin(0.1)),
                                      glm::vec3(0.0, -sin(0.1), cos(0.1))
            );
            cameraOrientation = rotationMatrix * cameraOrientation;
            triangles = projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
        }
        else if(event.key.keysym.sym == SDLK_s)
        {
            glm::mat3 rotationMatrix (glm::vec3(1.0, 0.0, 0.0),
                                      glm::vec3(0.0, cos(-0.1), sin(-0.1)),
                                      glm::vec3(0.0, -sin(-0.1), cos(-0.1))
            );
            cameraOrientation = rotationMatrix * cameraOrientation;
            triangles = projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
        }
        else if(event.key.keysym.sym == SDLK_d)
        {
            glm::mat3 rotationMatrix (glm::vec3(cos(-0.1), 0.0, sin(-0.1)),
                                      glm::vec3(0.0, 1.0, 0.0),
                                      glm::vec3(-sin(-0.1), 0.0, cos(-0.1))
            );
            cameraOrientation = rotationMatrix * cameraOrientation;
            triangles = projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
        }
        else if(event.key.keysym.sym == SDLK_a)
        {
            glm::mat3 rotationMatrix (glm::vec3(cos(0.1), 0.0, sin(0.1)),
                                      glm::vec3(0.0, 1.0, 0.0),
                                      glm::vec3(-sin(0.1), 0.0, cos(0.1))
            );
            cameraOrientation = rotationMatrix * cameraOrientation;
            triangles = projectionTrianglePoint(TDtriangles, cameraPosition, cameraOrientation);
        }
        else if (event.key.keysym.sym == SDLK_o){
            cameraOrbit(triangles, TDtriangles, cameraPosition, cameraOrientation, window, colors);

        }

        else if (event.key.keysym.sym == SDLK_u) {
//            // 生成新的三角形并添加到容器
//            CanvasPoint v0 = creatOnePoint();
//            CanvasPoint v1 = creatOnePoint();
//            CanvasPoint v2 = creatOnePoint();
//            triangles.push_back(CanvasTriangle(v0, v1, v2));
//            uint32_t color = generateRandomColor();
//            colors.push_back(color);
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}

int main(int argc, char *argv[]) {
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;

    std::vector<CanvasTriangle> triangles;
    std::vector<Colour> colors;
    window.clearPixels();

    // 将obj打印出来
    std::vector<ModelTriangle> TDtriangles = loadModel(
            "/Users/jekwen/Documents/大三上/CG/CG2023/RedNoise/src/cornell-box.obj",
            "/Users/jekwen/Documents/大三上/CG/CG2023/RedNoise/src/cornell-box.mtl" );
    colors = projectionTriangleColour(TDtriangles);

    glm::vec3 cameraPosition = {0, 0, 16};
    glm::mat3 camaraOrientation(glm::vec3(1,0,0),
                              glm::vec3(0,1,0),
                              glm::vec3(0,0,1));
//    glm::mat3 rotationMatrix ( glm::vec3(cos(acos(-1.0)), 0.0, sin(acos(-1.0))),
//                               glm::vec3(0.0, 1.0, 0.0),
//                               glm::vec3(-sin(acos(-1.0)), 0.0, cos(acos(-1.0)))
//    );

    camaraOrientation = rotationMatrix * camaraOrientation;

    triangles =  projectionTrianglePoint(TDtriangles, cameraPosition, camaraOrientation);
    while (true) {
        if (window.pollForInputEvents(event)) handleEvent(event, window, triangles, cameraPosition, TDtriangles, camaraOrientation, colors);
        window.clearPixels();
        drawTriangles(window,triangles, colors);
        window.renderFrame();
    }
    return 0;
}

