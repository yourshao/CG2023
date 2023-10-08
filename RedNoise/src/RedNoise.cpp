#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#define WIDTH 320
#define HEIGHT 240

//for grey spectrum

std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues){
    std::vector<float> result;
    float difference = ( to - from ) /( numberOfValues-1 )  ;
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



void draw(DrawingWindow &window) {
	window.clearPixels();

    glm::vec3 topLeft = {255, 0, 0};        // red
    glm::vec3 topRight = {0, 0, 255};       // blue
    glm::vec3 bottomRight = {0, 255, 0};    // green
    glm::vec3 bottomLeft = {255, 255, 0};  //yellow

    std::vector<glm::vec3> left = interpolateThreeElementValues(topLeft, bottomLeft, HEIGHT);
    std::vector<glm::vec3> right = interpolateThreeElementValues(topRight, bottomRight, HEIGHT);

    for (size_t y = 0; y < window.height; y++) {
        // for grey spectrum
//        std::vector<float> result;
//        result = interpolateSingleFloats(0, 255, WIDTH);

//for colour
        glm::vec3 firstOne = left[y];
        glm::vec3 lastOne = right[y];


        std::vector<glm::vec3> resultHorizontal = interpolateThreeElementValues(firstOne, lastOne, WIDTH);


//
       for (size_t x = 0; x < window.width; x++) {
//          //this is original red noise
//			float red = rand() % 256;
//			float green = 0.0;
//			float blue = 0.0;
//			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
// make grey spectrum
//            float red = 255-result.front();
//            float green = 255-result.front();
//            float blue = 255-result.front();
//            result.erase(result.begin());
//            uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);

            float red = resultHorizontal[x].x;
            float green = resultHorizontal[x].y;
            float blue = resultHorizontal[x].z;

            uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);




			window.setPixelColour(x, y, colour);
		}
	}
}






void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}




int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
    // test for the interpolateSingleFloats() function
//    std::vector<float> result;
//    result = interpolateSingleFloats(2.2, 8.5, 7);
//    for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
//    std::cout << std::endl;
//

//for four colour
//    std::vector<glm::vec3> result;
//    glm::vec3 from = {1.0, 4.0, 9.2};
//    glm::vec3 to = {4.0, 1.0, 9.8};
//    result = interpolateThreeElementValues(from,to,4);
//    for(size_t i=0; i< result.size(); i++) std::cout << result[i].x << result[i].y<<result[i].z;
//    std::cout << std::endl;







    while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}



}
