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


#define WIDTH 320
#define HEIGHT 240
void Drawanother(CanvasPoint from, CanvasPoint to, DrawingWindow &window){
    float xDiff = to.x - from.x;
    float yDiff = to.y - from.y;
    float numberOfSteps = fmax(abs(xDiff), abs(yDiff));
    float xStepSize =  xDiff/numberOfSteps;
    float yStepSize =  yDiff/numberOfSteps;
    for (float i = 0.0; i <= numberOfSteps; i++) {
        float x = from.x + (xStepSize*i);
        float y = from.y + (yStepSize*i);
        uint32_t colour = (255 << 24) + (255 << 16) + (255 << 8) + 255;

        window.setPixelColour(round(x), round(y), colour);
    }
}

//for grey spectrum

void drawTriangle(CanvasPoint v0, CanvasPoint v1, CanvasPoint v2, DrawingWindow &window) {
    Drawanother(v0, v1, window);
    Drawanother(v1, v2, window);
    Drawanother(v2, v0, window);
}

void drawTriangles(DrawingWindow &window, const std::vector<CanvasTriangle> &triangles) {
    window.clearPixels();
    for (const auto &triangle: triangles) {
        drawTriangle(const_cast<CanvasTriangle &>(triangle).v0(),
                     const_cast<CanvasTriangle &>(triangle).v1(),
                     const_cast<CanvasTriangle &>(triangle).v2(), window);
    }
}




std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues){
    std::vector<float> result;
    float difference = (to - from) / (numberOfValues - 1);
    for (int a = 0; a < numberOfValues; a++){
        result.push_back(from + a * difference);
    }
    return result;
}


float interpolate(float y0, float x0, float y1, float x1, float y) {
    return x0 + (x1 - x0) * (y - y0) / (y1 - y0);
}

// 检查点是否在三角形内
bool isPointInTriangle(int x, int y, float x0, float y0, float x1, float y1, float x2, float y2) {
    float d0 = (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
    float d1 = (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);
    float d2 = (x - x2) * (y0 - y2) - (y - y2) * (x0 - x2);

    return (d0 >= 0 && d1 >= 0 && d2 >= 0) || (d0 <= 0 && d1 <= 0 && d2 <= 0);
}


void drawPoint(const CanvasPoint& point, DrawingWindow& window) {
        uint32_t colour = (255 << 24) + (255 << 16) + (255 << 8) + 255;
        window.setPixelColour(round(point.x), round(point.y), colour);
}





uint32_t generateRandomColor() {
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    return (255 << 24) + (r << 16) + (g << 8) + b;
}

// 在 drawTriangle 函数中添加颜色参数
//void drawTriangle(CanvasTriangle triangle, DrawingWindow &window, uint32_t color) {
//
//    // 找到三角形的最小和最大y坐标
//    int minY = std::min({triangle.v0().y, triangle.v1().y, triangle.v2().y});
//    int maxY = std::max({triangle.v0().y, triangle.v1().y, triangle.v2().y});
//
//    // 对每一行进行扫描
//    for (int y = minY; y <= maxY; ++y) {
//        // 找到当前行与三角形边的交点
//        float x01 = interpolate(triangle.v0().y, triangle.v0().x, triangle.v1().y, triangle.v1().x, y);
//        float x12 = interpolate(triangle.v1().y, triangle.v1().x, triangle.v2().y, triangle.v2().x, y);
//        float x20 = interpolate(triangle.v2().y, triangle.v2().x, triangle.v0().y, triangle.v0().x, y);
//
//        // 找到当前行的最小和最大x坐标
//        int minX = std::min({static_cast<int>(x01), static_cast<int>(x12), static_cast<int>(x20)});
//        int maxX = std::max({static_cast<int>(x01), static_cast<int>(x12), static_cast<int>(x20)});
//
//        // 对当前行的每个像素进行遍历
//        for (int x = minX; x <= maxX; ++x) {
//            // 检查像素是否在三角形内
//            if (isPointInTriangle(x, y, triangle.v0().x, triangle.v0().y, triangle.v1().x, triangle.v1().y, triangle.v2().x, triangle.v2().y)) {
//                window.setPixelColour(x, y, color);
//
//                if (x == minX || x == maxX || y == minY || y == maxY) {
//                    window.setPixelColour(x, y, (255 << 24) + (255 << 16) + (255 << 8) + 255);
//                }
//            }
//        }
//    }
//}
//
//


//void drawTriangles(DrawingWindow &window, std::vector<CanvasTriangle> &triangles, std::vector<uint32_t> &colors) {
//    window.clearPixels();
//    for (size_t i = 0; i < triangles.size(); ++i) {
//        drawTriangle(triangles[i], window, colors[i]);
//    }
//}


CanvasPoint creatOnePoint() {
    float yPoint = rand() % HEIGHT;
    float xPoint = rand() % WIDTH;
    return CanvasPoint(xPoint, yPoint);
}



void handleEvent(SDL_Event event, DrawingWindow &window, std::vector<CanvasTriangle> &triangles, std::vector<uint32_t> &colors)  {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
        else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
        else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
        else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
        else if (event.key.keysym.sym == SDLK_u) {
            // 生成新的三角形并添加到容器
            CanvasPoint v0 = creatOnePoint();
            CanvasPoint v1 = creatOnePoint();
            CanvasPoint v2 = creatOnePoint();
            triangles.push_back(CanvasTriangle(v0, v1, v2));
            uint32_t color = generateRandomColor();
            colors.push_back(color);
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}






// 将0到1的数值映射到RGB范围
Colour mapToRGB(float r, float g, float b) {
    return Colour(static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255));
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

        if (type == "Kd") {
            float r, g, b;
            iss >> r >> g >> b;
            currentColor = mapToRGB(r, g, b);
            currentColor.name = type;
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

        if (type == "v") {
            // 读取顶点信息
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (type == "f") {
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
        } else if (type == "usemtl") {
            // 设置当前使用的颜色
            std::string colorName;
            iss >> colorName;
            for (const Colour& c : colors) {
                if (c.name == colorName) {
                    currentColor = c;
                    break;
                }
            }
        }
    }

    return triangles;
}




CanvasPoint getCanvasIntersectionPoint(const glm::vec3& cameraPosition, const glm::vec3& vertexPosition, int focalLength){
    float u = focalLength* (vertexPosition.x - cameraPosition.x)  / (cameraPosition.z + vertexPosition.z)*30  + WIDTH/2 ;
    float v = focalLength* (vertexPosition.y - cameraPosition.y) / (cameraPosition.z + vertexPosition.z)*30 + HEIGHT/2 ;

    float scale = 1.0;

    CanvasPoint point(u*scale,HEIGHT-v*scale);
    return point;
}

std::vector<CanvasTriangle> projectionTrianglePoint(std::vector<ModelTriangle> threeDtriangle){
    std::vector<CanvasTriangle> result;
    for (const auto &triangle : threeDtriangle) {
        CanvasPoint point1 = getCanvasIntersectionPoint(glm::vec3(0, 0, 16), triangle.vertices[0], 13);
        CanvasPoint point2 = getCanvasIntersectionPoint(glm::vec3(0, 0, 16), triangle.vertices[1], 13);
        CanvasPoint point3 = getCanvasIntersectionPoint(glm::vec3(0, 0, 16), triangle.vertices[2], 13);
        CanvasTriangle newTriangle( point1 , point2 , point3 );

        result.push_back(newTriangle);
    }

    return result;

}
std::vector<uint32_t> projectionTriangleColour(std::vector<ModelTriangle> threeDtriangle){
    std::vector<uint32_t> result;
    for(const auto &triangle : threeDtriangle){
        uint32_t color = (255 << 24) + (0 << 16) + (0 << 8) + 0;
        result.push_back(color);
    }
    return result;
}


int main(int argc, char *argv[]) {
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;

    std::vector<CanvasTriangle> triangles; // 存储生成的三角形
    std::vector<uint32_t> colors;          // 存储生成的颜色


    // 将obj打印出来
    std::vector<ModelTriangle> TDtriangles = loadModel(
            "/Users/jekwen/Documents/大三上/CG/CG2023/RedNoise/src/cornell-box.obj",
            "/Users/jekwen/Documents/大三上/CG/CG2023/RedNoise/src/cornell-box.mtl" );

    triangles =  projectionTrianglePoint(TDtriangles);
    colors = projectionTriangleColour(TDtriangles);

    while (true) {
        if (window.pollForInputEvents(event)) handleEvent(event, window, triangles, colors);
        window.clearPixels();

        drawTriangles(window,triangles);

//
//        for (const auto &triangle : TDtriangles) {
//            std::cout << "============" << std::endl;
//            for (const auto &vertex : triangle.vertices) {
//                CanvasPoint point = getCanvasIntersectionPoint(glm::vec3(0, 0, 16), vertex, 13);
//
//                std::cout << point.x << " " << point.y << std::endl;
//
//                drawPoint(point, window);
//            }
//            //白色
//            CanvasPoint point1 = getCanvasIntersectionPoint(glm::vec3(0, 0, 16), triangle.vertices[0], 13);
//            CanvasPoint point2 = getCanvasIntersectionPoint(glm::vec3(0, 0, 16), triangle.vertices[1], 13);
//            CanvasPoint point3 = getCanvasIntersectionPoint(glm::vec3(0, 0, 16), triangle.vertices[2], 13);
//            CanvasTriangle newTriangle( point1 , point2 , point3);
//
//            uint32_t color = (255 << 24) + (0 << 16) + (0 << 8) + 0;
//
//            drawTriangle(newTriangle, window, color);
//            std::cout << point1.x << " " << point1.y << std::endl;
//        }
        window.renderFrame();
    }

    return 0;
}




