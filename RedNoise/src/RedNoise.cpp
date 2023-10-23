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


uint32_t generateRandomColor() {
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    return (255 << 24) + (r << 16) + (g << 8) + b;
}

// 在 drawTriangle 函数中添加颜色参数
void drawTriangle(CanvasTriangle triangle, DrawingWindow &window, uint32_t color) {

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
        int minX = std::min({static_cast<int>(x01), static_cast<int>(x12), static_cast<int>(x20)});
        int maxX = std::max({static_cast<int>(x01), static_cast<int>(x12), static_cast<int>(x20)});

        // 对当前行的每个像素进行遍历
        for (int x = minX; x <= maxX; ++x) {
            // 检查像素是否在三角形内
            if (isPointInTriangle(x, y, triangle.v0().x, triangle.v0().y, triangle.v1().x, triangle.v1().y, triangle.v2().x, triangle.v2().y)) {
                window.setPixelColour(x, y, color);

                if (x == minX || x == maxX || y == minY || y == maxY) {
                    window.setPixelColour(x, y, (255 << 24) + (255 << 16) + (255 << 8) + 255);
                }
            }
        }
    }
}




void drawTriangles(DrawingWindow &window, std::vector<CanvasTriangle> &triangles, std::vector<uint32_t> &colors) {
    window.clearPixels();
    for (size_t i = 0; i < triangles.size(); ++i) {
        drawTriangle(triangles[i], window, colors[i]);
    }
}





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


std::vector<ModelTriangle> loadOBJ(const std::string &filename) {
    std::vector<ModelTriangle> triangles;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return triangles;  // 返回空的三角形向量
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "v") {
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            triangles.emplace_back(ModelTriangle(v, v, v, Colour()));  // 使用相同的颜色，可以根据需求修改
        }
    }

    return triangles;
}













//
//CanvasPoint getCanvasIntersectionPoint(const glm::vec3& cameraPosition, const glm::vec3& vertexPosition, int focalLength){
//
//
//
//
//
//
//}












int main(int argc, char *argv[]) {
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;

    std::vector<CanvasTriangle> triangles; // 存储生成的三角形
    std::vector<uint32_t> colors;          // 存储生成的颜色


    // 将obj打印出来
    std::vector<ModelTriangle> TDtriangles = loadOBJ("/Users/jekwen/Documents/大三上/CG/CG2023/RedNoise/src/cornell-box.obj");
    for (const auto &triangle : TDtriangles) {
        std::cout << triangle << std::endl;
    }



    while (true) {
        if (window.pollForInputEvents(event)) handleEvent(event, window, triangles, colors);
        drawTriangles(window, triangles, colors);
        window.renderFrame();
    }

    return 0;
}

