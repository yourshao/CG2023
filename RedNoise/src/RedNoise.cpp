#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>
#include <CanvasTriangle.h>


#define WIDTH 320
#define HEIGHT 240

//for grey spectrum

std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues){
    std::vector<float> result;
    float difference = ( to - from ) /(numberOfValues-1)  ;
    for (int a = 0 ;a < numberOfValues; a++){
        result.push_back(from + a * difference);
    }
    return result;
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues){
std::vector<glm::vec3> result (numberOfValues)  ;
    float oneFrom;
    float oneTo;
    for(int a = 0; a<3;a++) {
        oneFrom = from[a];
        oneTo = to[a];
        std::vector<float> oneVectorResult = interpolateSingleFloats(oneFrom, oneTo, numberOfValues);

        if (a == 0 ){
            for(int x = 0; x< numberOfValues; x++){
                result[x].x =oneVectorResult[x];
            }
        }else if (a == 1 ){
            for(int x = 0; x< numberOfValues; x++){
                result[x].y =oneVectorResult[x];
            }
        } else if (a == 2 ){
            for(int x = 0; x< numberOfValues; x++){
                result[x].z =oneVectorResult[x];
            }
        }

    }
    return result;

}

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
void drawTriangle(CanvasPoint v0, CanvasPoint v1, CanvasPoint v2, DrawingWindow &window) {
    Drawanother(v0, v1, window);
    Drawanother(v1, v2, window);
    Drawanother(v2, v0, window);
}


void drawTriangles(DrawingWindow &window, const std::vector<CanvasTriangle> &triangles) {
    window.clearPixels();
    for (const auto &triangle : triangles) {
        drawTriangle(const_cast<CanvasTriangle&>(triangle).v0(),
                     const_cast<CanvasTriangle&>(triangle).v1(),
                     const_cast<CanvasTriangle&>(triangle).v2(), window);
    }
}
CanvasPoint creatOnePoint() {
    float yPoint = rand() % HEIGHT;
    float xPoint = rand() % WIDTH;
    return CanvasPoint(xPoint, yPoint);
}

void handleEvent(SDL_Event event, DrawingWindow &window, std::vector<CanvasTriangle> &triangles) {
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
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}



int main(int argc, char *argv[]) {
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;

    std::vector<CanvasTriangle> triangles; // 存储生成的三角形

    while (true) {
        // We MUST poll for events - otherwise the window will freeze!
        if (window.pollForInputEvents(event)) handleEvent(event, window, triangles);
        drawTriangles(window, triangles);
        // Need to render the frame at the end, or nothing actually gets shown on the screen!
        window.renderFrame();
    }

    return 0;
}
